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

#include "Gtk_PangoLayoutArea.hh"
#include "Gtk_RenderingContext.hh"

Gtk_PangoLayoutArea::Gtk_PangoLayoutArea(PangoLayout* _layout)
  : layout(_layout)
{ }

Gtk_PangoLayoutArea::~Gtk_PangoLayoutArea()
{ }

BoundingBox
Gtk_PangoLayoutArea::box() const
{
  PangoRectangle rect;
  pango_layout_get_extents(layout, &rect, 0);
  return BoundingBox(Gtk_RenderingContext::fromPangoPixels(rect.width),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(rect)),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_DESCENT(rect)));
}

scaled
Gtk_PangoLayoutArea::leftEdge() const
{
  PangoRectangle rect;
  pango_layout_get_extents(layout, &rect, 0);
  return Gtk_RenderingContext::fromPangoPixels(PANGO_LBEARING(rect));
}

scaled
Gtk_PangoLayoutArea::rightEdge() const
{
  PangoRectangle rect;
  pango_layout_get_extents(layout, &rect, 0);
  return Gtk_RenderingContext::fromPangoPixels(PANGO_RBEARING(rect));
}

void
Gtk_PangoLayoutArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  printf("passo di qui %p\n", &c);
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);
  printf("before %p %d %d\n",
	 &context,
	 Gtk_RenderingContext::toGtkPixels(x - context.getXOrigin()),
	 Gtk_RenderingContext::toGtkPixels(y - context.getYOrigin()));
  gdk_draw_layout(context.getDrawable(),
		  context.getGC(),
		  Gtk_RenderingContext::toGtkPixels(x - context.getXOrigin()),
		  Gtk_RenderingContext::toGtkPixels(y - context.getYOrigin()),
		  layout);
  printf("after!\n");
}

DOM::Element
Gtk_PangoLayoutArea::dump(const DOM::Document& doc) const
{
  assert(false);
  return DOM::Element(0);
}
