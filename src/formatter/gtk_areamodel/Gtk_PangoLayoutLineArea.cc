// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "Gtk_PangoLayoutLineArea.hh"
#include "Gtk_RenderingContext.hh"

Gtk_PangoLayoutLineArea::Gtk_PangoLayoutLineArea(PangoLayout* _layout)
  : Gtk_PangoLayoutArea(_layout)
{
  PangoRectangle rect;
  PangoLayoutLine* line = pango_layout_get_line(layout, 0);
  pango_layout_line_get_extents(line, &rect, 0);

  bbox = BoundingBox(Gtk_RenderingContext::fromPangoPixels(rect.width),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(rect)),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_DESCENT(rect)));
}

Gtk_PangoLayoutLineArea::~Gtk_PangoLayoutLineArea()
{ }

void
Gtk_PangoLayoutLineArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);

  PangoLayoutLine* line = pango_layout_get_line(layout, 0);
  gdk_draw_layout_line(context.getDrawable(),
		       context.getGC(),
		       Gtk_RenderingContext::toGtkX(x),
		       Gtk_RenderingContext::toGtkY(y),
		       line);
}

