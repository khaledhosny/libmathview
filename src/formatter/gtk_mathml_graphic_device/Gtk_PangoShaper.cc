// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
//
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "Gtk_AreaFactory.hh"
#include "Gtk_PangoShaper.hh"
#include "Gtk_RenderingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLElement.hh"

struct HStretchyChar
{
  gunichar ch;
  gunichar left;
  gunichar glue;
  gunichar right;
};

struct VStretchyChar
{
  gunichar ch;
  gunichar top;
  gunichar glue;
  gunichar middle;
  gunichar bottom;
};

static HStretchyChar hMap[] =
  {
    { 0x2190, 0x2190, 0x23af, 0x0000 },
    { 0x2192, 0x0000, 0x23af, 0x2192 },
    { 0x2194, 0x2190, 0x23af, 0x2192 },
    { 0,      0,      0,      0      }
  };

static VStretchyChar vMap[] = 
  {
    { 0x0028, 0x239b, 0x239c, 0x0000, 0x239d }, // LEFT PAREN
    { 0x0029, 0x239e, 0x239f, 0x0000, 0x23a0 }, // RIGHT PAREN
    { 0x005b, 0x23a1, 0x23a2, 0x0000, 0x23a3 }, // LEFT SQUARE BRACKET
    { 0x005d, 0x23a4, 0x23a5, 0x0000, 0x23a6 }, // RIGHT SQUARE BRACKET
    { 0x007b, 0x23a7, 0x23aa, 0x23a8, 0x23a9 }, // LEFT CURLY BRACKET
    { 0x007d, 0x23ab, 0x23aa, 0x23ac, 0x23ad }, // RIGHT CURLY BRACKET
    { 0x2191, 0x2191, 0x23d0, 0x0000, 0x0000 }, // UP ARROW
    { 0x2193, 0x0000, 0x23d0, 0x0000, 0x2193 }, // BOTTOM ARROW
    { 0x222b, 0x2320, 0x23ae, 0x0000, 0x2321 }, // INTEGRAL
    { 0,      0,      0,      0,      0 }
  };

#define NORMAL_INDEX 0
#define H_STRETCHY_INDEX 1
#define V_STRETCHY_INDEX 2

Gtk_PangoShaper::Gtk_PangoShaper()
{ }

Gtk_PangoShaper::~Gtk_PangoShaper()
{ }

void
Gtk_PangoShaper::registerShaper(const SmartPtr<class ShaperManager>& sm, unsigned shaperId)
{
  // normal characters are not registered because the Pango shaper is supposed to
  // be the default shaper. It will be called anyway as soon as there's a
  // Unicode char that cannot be shaped otherwise

  for (unsigned i = 0; hMap[i].ch != 0; i++)
    sm->registerStretchyChar(hMap[i].ch, GlyphSpec(shaperId, H_STRETCHY_INDEX, i));

  for (unsigned i = 0; vMap[i].ch != 0; i++)
    sm->registerStretchyChar(vMap[i].ch, GlyphSpec(shaperId, V_STRETCHY_INDEX, i));
}

void
Gtk_PangoShaper::unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned)
{
  // nothing to do
}

void
Gtk_PangoShaper::shape(const MathFormattingContext& ctxt, ShapingResult& result) const
{
  const GlyphSpec spec = result.getSpec();
  switch (spec.getFontId())
    {
    case NORMAL_INDEX:
      {
	const unsigned n = result.chunkSize();
	gunichar* uni_buffer = new gunichar[n];
	for (unsigned i = 0; i < n; i++) uni_buffer[i] = result.data()[i];
	result.pushArea(n, shapeString(ctxt, uni_buffer, n));
	delete [] uni_buffer;
      }
      break;
    case H_STRETCHY_INDEX:
      result.pushArea(1, shapeStretchyCharH(ctxt, spec, result.getHSpan()));
      break;
    case V_STRETCHY_INDEX:
      result.pushArea(1, shapeStretchyCharV(ctxt, spec, result.getVSpan()));
      break;
    default:
      assert(false);
      break;
    }
}

AreaRef
Gtk_PangoShaper::shapeString(const MathFormattingContext& ctxt, const gunichar* uni_buffer, glong n) const
{
  assert(context);

  glong length;
  gchar* buffer = g_ucs4_to_utf8(uni_buffer, n, NULL, &length, NULL);
  //assert((glong) n == length);

  const MathVariant variant = ctxt.getVariant();

  // FIXME: I bet there are some leaks here, but using GObjectPtr just
  // gives a segfault!!!
  //GObjectPtr<PangoLayout> layout = pango_layout_new(context);
  PangoLayout* layout = pango_layout_new(context);
  pango_layout_set_text(layout, buffer, length);

  //GObjectPtr<PangoAttrList> attrList = pango_attr_list_new();
  PangoAttrList* attrList = pango_attr_list_new();

  // PangoAttribute is not a GObject?
  PangoAttribute* sizeAttr = pango_attr_size_new(Gtk_RenderingContext::toPangoPixels(ctxt.getSize()));
  sizeAttr->start_index = 0;
  sizeAttr->end_index = length;
  pango_attr_list_insert(attrList, sizeAttr);
#if 0
  switch (variant)
    {
    case BOLD_VARIANT:
    case BOLD_ITALIC_VARIANT:
    case BOLD_FRAKTUR_VARIANT:
    case BOLD_SCRIPT_VARIANT:
    case BOLD_SANS_SERIF_VARIANT:
      {
	PangoAttribute* weightAttr = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
	weightAttr->start_index = 0;
	weightAttr->end_index = length;
	pango_attr_list_insert(attrList, weightAttr);
      }
      break;
    default: break;
    }

  switch (variant)
    {
    case ITALIC_VARIANT:
    case BOLD_ITALIC_VARIANT:
    case SANS_SERIF_ITALIC_VARIANT:
      {
	PangoAttribute* styleAttr = pango_attr_style_new(PANGO_STYLE_ITALIC);
	styleAttr->start_index = 0;
	styleAttr->end_index = length;
	pango_attr_list_insert(attrList, styleAttr);
      }
      break;
    default: break;
    }
#endif
  pango_layout_set_attributes(layout, attrList);

  SmartPtr<Gtk_AreaFactory> factory = smart_cast<Gtk_AreaFactory>(ctxt.getDevice()->getFactory());
  assert(factory);
  return factory->pangoLayoutLine(layout);

  //g_free(buffer);
  // g_free(sizeAttr); // ????
}

AreaRef
Gtk_PangoShaper::shapeStretchyCharH(const MathFormattingContext& ctxt, const GlyphSpec& spec, const scaled& span) const
{
  const HStretchyChar* charSpec = &hMap[spec.getGlyphId()];

  const AreaRef normal = (charSpec->ch != 0) ? shapeString(ctxt, &charSpec->ch, 1) : 0;
  const AreaRef left = (charSpec->left != 0) ? shapeString(ctxt, &charSpec->left, 1) : 0;
  const AreaRef glue = (charSpec->glue != 0) ? shapeString(ctxt, &charSpec->glue, 1) : 0;
  const AreaRef right = (charSpec->right != 0) ? shapeString(ctxt, &charSpec->right, 1) : 0;

  return composeStretchyCharH(ctxt.getDevice()->getFactory(), normal, left, glue, right, span);
}

AreaRef
Gtk_PangoShaper::shapeStretchyCharV(const MathFormattingContext& ctxt, const GlyphSpec& spec, const scaled& strictSpan) const
{
  const scaled span = strictSpan - (1 * ctxt.getSize()) / 10;

  const VStretchyChar* charSpec = &vMap[spec.getGlyphId()];

  AreaRef normal = (charSpec->ch != 0) ? shapeString(ctxt, &charSpec->ch, 1) : 0;
  AreaRef top = (charSpec->top != 0) ? shapeString(ctxt, &charSpec->top, 1) : 0;
  AreaRef glue = (charSpec->glue != 0) ? shapeString(ctxt, &charSpec->glue, 1) : 0;
  AreaRef middle = (charSpec->middle != 0) ? shapeString(ctxt, &charSpec->middle, 1) : 0;
  AreaRef bottom = (charSpec->bottom != 0) ? shapeString(ctxt, &charSpec->bottom, 1) : 0;

  return composeStretchyCharV(ctxt.getDevice()->getFactory(), normal, top, glue, middle, bottom, span);
}
