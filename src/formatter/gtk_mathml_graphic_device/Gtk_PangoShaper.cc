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

struct HStretchyChar
{
  DOM::Char16 ch;
  DOM::Char16 left;
  DOM::Char16 glue;
  DOM::Char16 right;
};

struct VStretchyChar
{
  DOM::Char16 ch;
  DOM::Char16 top;
  DOM::Char16 glue;
  DOM::Char16 middle;
  DOM::Char16 bottom;
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
    { 0x222b, 0x2320, 0x23ae, 0x0000, 0x2321 },
    { 0,      0,      0,      0,      0 }
  };

Gtk_PangoShaper::Gtk_PangoShaper()
{ }

Gtk_PangoShaper::~Gtk_PangoShaper()
{ }

void
Gtk_PangoShaper::registerShaper(const SmartPtr<class ShaperManager>&, unsigned)
{
  // this method is empty because the Pango shaper is supposed to
  // be the default shaper. It will be called anyway as soon as there's a
  // Unicode char that cannot be shaped otherwise
}

void
Gtk_PangoShaper::unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned)
{
  // nothing to do
}

void
Gtk_PangoShaper::shape(const MathFormattingContext& ctxt, ShapingResult& result) const
{
  assert(context);

  unsigned n = result.chunkSize();
  gunichar* uni_buffer = new gunichar[n];
  for (unsigned i = 0; i < n; i++) uni_buffer[i] = result.data()[i];
  glong length;
  gchar* buffer = g_ucs4_to_utf8(uni_buffer, n, NULL, &length, NULL);
  assert(buffer);
  //assert((glong) n == length);
  delete [] uni_buffer;

  MathVariant variant = ctxt.getVariant();

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
  result.pushArea(n, factory->pangoLayout(layout));

  //g_free(buffer);
  // g_free(sizeAttr); // ????
}
