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

Gtk_PangoShaper::Gtk_PangoShaper()
{ }

Gtk_PangoShaper::~Gtk_PangoShaper()
{ }

void
Gtk_PangoShaper::registerChars(class ShaperManager&, unsigned) const
{
  // this method is empty because the Pango shaper is supposed to
  // be the default shaper. It will be called anyway as soon as there's a
  // Unicode char that cannot be shaped otherwise
}

unsigned
Gtk_PangoShaper::shape(ShapingResult& result) const
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

  // FIXME: I bet there are some leaks here, but using GObjectPtr just
  // gives a segfault!!!
  //GObjectPtr<PangoLayout> layout = pango_layout_new(context);
  PangoLayout* layout = pango_layout_new(context);
  pango_layout_set_text(layout, buffer, length);
  // PangoAttribute is not a GObject?
  PangoAttribute* sizeAttr = pango_attr_size_new(Gtk_RenderingContext::toPangoPixels(result.getFontSize()));
  //GObjectPtr<PangoAttrList> attrList = pango_attr_list_new();
  PangoAttrList* attrList = pango_attr_list_new();
  pango_attr_list_insert(attrList, sizeAttr);
  pango_layout_set_attributes(layout, attrList);

  SmartPtr<Gtk_AreaFactory> factory = smart_cast<Gtk_AreaFactory>(result.getFactory());
  assert(factory);
  result.pushArea(factory->createPangoLayoutArea(layout));

  //g_free(buffer);
  // g_free(sizeAttr); // ????

  result.advance(n);

  return length;
}
