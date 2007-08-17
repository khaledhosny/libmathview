// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "Configuration.hh"
#include "Gtk_AreaFactory.hh"
#include "Gtk_DefaultPangoShaper.hh"
#include "Gtk_RenderingContext.hh"
#include "MathMLElement.hh"
#include "MathGraphicDevice.hh"
#include "ShapingContext.hh"

static PangoStyle
parsePangoStyle(const String& s, PangoStyle v)
{
  if (s == "italic")
    return PANGO_STYLE_ITALIC;
  else if (s == "oblique")
    return PANGO_STYLE_OBLIQUE;
  else
    return v;
}

static PangoWeight
parsePangoWeight(const String& s, PangoWeight v)
{
  if (s == "light")
    return PANGO_WEIGHT_LIGHT;
  else if (s == "bold")
    return PANGO_WEIGHT_BOLD;
  else
    return v;
}

Gtk_DefaultPangoShaper::Gtk_DefaultPangoShaper(const SmartPtr<AbstractLogger>& logger, const SmartPtr<Configuration>& conf)
{
  static const DefaultPangoTextAttributes defaultVariantDesc[] =
    {
      { "normal", NORMAL_VARIANT, "Serif", "normal", PANGO_STYLE_NORMAL, "normal", PANGO_WEIGHT_NORMAL },
      { "bold", BOLD_VARIANT, "Serif", "normal", PANGO_STYLE_NORMAL, "bold", PANGO_WEIGHT_BOLD },
      { "italic", ITALIC_VARIANT, "Serif", "italic", PANGO_STYLE_ITALIC, "normal", PANGO_WEIGHT_NORMAL },
      { "bold-italic", BOLD_ITALIC_VARIANT, "Serif", "italic", PANGO_STYLE_ITALIC, "bold", PANGO_WEIGHT_BOLD }, 
      { "double-struck", DOUBLE_STRUCK_VARIANT, "Sans", "normal", PANGO_STYLE_NORMAL, "bold", PANGO_WEIGHT_BOLD },
      { "bold-fraktur", BOLD_FRAKTUR_VARIANT, "Serif", "normal", PANGO_STYLE_NORMAL, "bold", PANGO_WEIGHT_BOLD },
      { "script", SCRIPT_VARIANT, "Sans", "normal", PANGO_STYLE_NORMAL, "normal", PANGO_WEIGHT_NORMAL },
      { "bold-script", BOLD_SCRIPT_VARIANT, "Sans", "normal", PANGO_STYLE_NORMAL, "bold", PANGO_WEIGHT_BOLD },
      { "fraktur", FRAKTUR_VARIANT, "Serif", "normal", PANGO_STYLE_NORMAL,"bold",  PANGO_WEIGHT_BOLD },
      { "sans-serif", SANS_SERIF_VARIANT, "Sans", "normal", PANGO_STYLE_NORMAL, "normal", PANGO_WEIGHT_NORMAL },
      { "bold-sans-serif", BOLD_SANS_SERIF_VARIANT, "Sans", "normal", PANGO_STYLE_NORMAL, "bold", PANGO_WEIGHT_BOLD },
      { "sans-serif-italic", SANS_SERIF_ITALIC_VARIANT, "Sans", "italic", PANGO_STYLE_ITALIC, "normal", PANGO_WEIGHT_NORMAL },
      { "sans-serif-bold-italic", SANS_SERIF_BOLD_ITALIC_VARIANT, "Sans", "italic", PANGO_STYLE_ITALIC, "normal", PANGO_WEIGHT_NORMAL },
      { "monospace", MONOSPACE_VARIANT, "Monospace", "normal", PANGO_STYLE_NORMAL, "normal", PANGO_WEIGHT_NORMAL }
    };

  const String baseKey = "gtk-backend/pango-default-shaper/variants/";
  for (unsigned i = 0; i < sizeof(defaultVariantDesc) / sizeof(DefaultPangoTextAttributes); i++)
    {
      const String key = baseKey + defaultVariantDesc[i].variant;
      const String family = conf->getString(logger, key + "/family", defaultVariantDesc[i].family);
      const String style = conf->getString(logger, key + "/style", defaultVariantDesc[i].style);
      const String weight = conf->getString(logger, key + "/weight", defaultVariantDesc[i].weight);
      const PangoStyle pangoStyle = parsePangoStyle(style, PANGO_STYLE_NORMAL);
      const PangoWeight pangoWeight = parsePangoWeight(weight, PANGO_WEIGHT_NORMAL);
      variantDesc[i].variant = defaultVariantDesc[i].variantId;
      variantDesc[i].family = family;
      variantDesc[i].style = pangoStyle;
      variantDesc[i].weight = pangoWeight;
    }
}

Gtk_DefaultPangoShaper::~Gtk_DefaultPangoShaper()
{ }

SmartPtr<Gtk_DefaultPangoShaper>
Gtk_DefaultPangoShaper::create(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
{ return new Gtk_DefaultPangoShaper(l, conf); }

void
Gtk_DefaultPangoShaper::registerShaper(const SmartPtr<class ShaperManager>&, unsigned)
{
  // normal characters are not registered because the Pango shaper is supposed to
  // be the default shaper. It will be called anyway as soon as there's a
  // Unicode char that cannot be shaped otherwise
}

void
Gtk_DefaultPangoShaper::unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned)
{
  // nothing to do
}

void
Gtk_DefaultPangoShaper::shape(ShapingContext& context) const
{
  const unsigned n = context.chunkSize();
  assert(n > 0);
  // is it worth specializing this to the case when n == 1????
  gunichar* uni_buffer = new gunichar[n];
  for (unsigned i = 0; i < n; i++) uni_buffer[i] = context.data()[i];
  context.pushArea(n, shapeString(context, uni_buffer, n));
  delete [] uni_buffer;
}

AreaRef
Gtk_DefaultPangoShaper::shapeString(const ShapingContext& context, const gunichar* uni_buffer, unsigned n) const
{
  glong length;
  gchar* buffer = g_ucs4_to_utf8(uni_buffer, n, NULL, &length, NULL);
  PangoLayout* layout = createPangoLayout(buffer, length,
					  context.getSize(),
					  getDefaultTextAttributes());
  g_free(buffer);

  SmartPtr<Gtk_AreaFactory> factory = smart_cast<Gtk_AreaFactory>(context.getFactory());
  assert(factory);
  // LUCA: what is the difference between PangoLayout and PangoLayoutLine?
  return factory->pangoLayoutLine(layout);
}

const Gtk_DefaultPangoShaper::PangoTextAttributes&
Gtk_DefaultPangoShaper::getDefaultTextAttributes()
{
  static const PangoTextAttributes desc = { NORMAL_VARIANT, "serif", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL };
  return desc;
}

const Gtk_DefaultPangoShaper::PangoTextAttributes&
Gtk_DefaultPangoShaper::getTextAttributes(MathVariant variant) const
{
  assert(variant >= NORMAL_VARIANT && variant <= MONOSPACE_VARIANT);
//   static const PangoTextAttributes variantDesc[] =
//     {
//       { NORMAL_VARIANT, "Serif", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL },
//       { BOLD_VARIANT, "Serif", PANGO_STYLE_NORMAL, PANGO_WEIGHT_BOLD },
//       { ITALIC_VARIANT, "Serif", PANGO_STYLE_ITALIC, PANGO_WEIGHT_NORMAL },
//       { BOLD_ITALIC_VARIANT, "Serif", PANGO_STYLE_ITALIC, PANGO_WEIGHT_BOLD }, 
//       { DOUBLE_STRUCK_VARIANT, "Sans", PANGO_STYLE_NORMAL, PANGO_WEIGHT_BOLD },
//       { BOLD_FRAKTUR_VARIANT, "Serif", PANGO_STYLE_NORMAL, PANGO_WEIGHT_BOLD },
//       { SCRIPT_VARIANT, "Sans", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL },
//       { BOLD_SCRIPT_VARIANT, "Sans", PANGO_STYLE_NORMAL, PANGO_WEIGHT_BOLD },
//       { FRAKTUR_VARIANT, "Serif", PANGO_STYLE_NORMAL, PANGO_WEIGHT_BOLD },
//       { SANS_SERIF_VARIANT, "Sans", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL },
//       { BOLD_SANS_SERIF_VARIANT, "Sans", PANGO_STYLE_NORMAL, PANGO_WEIGHT_BOLD },
//       { SANS_SERIF_ITALIC_VARIANT, "Sans", PANGO_STYLE_ITALIC, PANGO_WEIGHT_NORMAL },
//       { SANS_SERIF_BOLD_ITALIC_VARIANT, "Sans", PANGO_STYLE_ITALIC, PANGO_WEIGHT_NORMAL },
//       { MONOSPACE_VARIANT, "Monospace", PANGO_STYLE_NORMAL, PANGO_WEIGHT_NORMAL }
//     };
  return variantDesc[variant - NORMAL_VARIANT];
}

PangoLayout*
Gtk_DefaultPangoShaper::createPangoLayout(const gchar* buffer, glong length, const scaled& sp_size,
					  const PangoTextAttributes& attributes) const
{
  // Apparently when setting font sizes the interpretation of PANGO_SCALE is different
  // (see Pango documentation) hence we do NOT use toPangoPixels
  const gint size = Gtk_RenderingContext::toPangoPoints(sp_size);

  // FIXME: I bet there are some leaks here, but using GObjectPtr just
  // gives a segfault!!!
  //GObjectPtr<PangoLayout> layout = pango_layout_new(context);
  PangoLayout* layout = pango_layout_new(context);
  pango_layout_set_text(layout, buffer, length);

  //GObjectPtr<PangoAttrList> attrList = pango_attr_list_new();
  PangoAttrList* attrList = pango_attr_list_new();

#if 1
  PangoFontDescription* fontDesc = pango_font_description_new();
  if (!attributes.family.empty()) pango_font_description_set_family_static(fontDesc, attributes.family.c_str());
  if (attributes.weight != PANGO_WEIGHT_NORMAL) pango_font_description_set_weight(fontDesc, attributes.weight);
  if (attributes.style != PANGO_STYLE_NORMAL) pango_font_description_set_style(fontDesc, attributes.style);
  pango_font_description_set_size(fontDesc, size);
  PangoAttribute* fontDescAttr = pango_attr_font_desc_new(fontDesc);
  fontDescAttr->start_index = 0;
  fontDescAttr->end_index = length;
  pango_attr_list_insert(attrList, fontDescAttr);
  pango_font_description_free(fontDesc);  //???
#else
  // PangoAttribute is not a GObject?
  PangoAttribute* sizeAttr = pango_attr_size_new(size);
  sizeAttr->start_index = 0;
  sizeAttr->end_index = length;
  pango_attr_list_insert(attrList, sizeAttr);
#if 1
  if (attributes.family)
    {
      PangoAttribute* familyAttr = pango_attr_family_new(attributes.family);
      familyAttr->start_index = 0;
      familyAttr->start_index = length;
      pango_attr_list_insert(attrList, familyAttr);
    }

  if (attributes.weight != PANGO_WEIGHT_NORMAL)
    {
      PangoAttribute* weightAttr = pango_attr_weight_new(attributes.weight);
      weightAttr->start_index = 0;
      weightAttr->end_index = length;
      pango_attr_list_insert(attrList, weightAttr);
    }

  if (attributes.style != PANGO_STYLE_NORMAL)
    {
      PangoAttribute* styleAttr = pango_attr_style_new(attributes.style);
      styleAttr->start_index = 0;
      styleAttr->end_index = length;
      pango_attr_list_insert(attrList, styleAttr);
    }
#endif
#endif
  pango_layout_set_attributes(layout, attrList);

  return layout;
}

bool
Gtk_DefaultPangoShaper::isDefaultShaper() const
{ return true; }
