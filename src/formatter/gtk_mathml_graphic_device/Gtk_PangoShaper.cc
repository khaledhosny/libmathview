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

#include "Gtk_PangoShaper.hh"
#include "Gtk_LayoutArea.hh"

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

  // PangoAttribute is not a GObject?
  PangoAttribute* sizeAttr = pango_attr_size_new(toPangoPixels(result.getFontSize()));
  GObjectPtr<PangoAttrList> attrList = pango_attr_list_new();
  pango_attr_list_insert(attrList, sizeAttr);

  GObjectPtr<PangoLayout> layout = pango_layout_new(context);
  glong length;
  gchar* buffer = g_ucs4_to_utf8(result.data(), result.chunkSize(), NULL, &length, NULL);
  assert(buffer);
  assert(result.chunkSize() == length);

  pango_layout_set_text(layout, buffer, length);
  pango_layout_set_attributes(layout, attrList);

  SmartPtr<Gtk_AreaFactory> factory = smart_cast<Gtk_AreaFactory>(result.getFactory());
  assert(factory);
  result.pushArea(factory.createPangoLayout(layout));

  g_free(buffer);
  // g_free(sizeAttr); // ????

  return length;
}
