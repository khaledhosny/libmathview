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
{
  PangoRectangle ink_rect;

  PangoLayoutLine* line = pango_layout_get_line(layout, 0);
  pango_layout_line_get_extents(line, &ink_rect, 0);

#if 0
  printf("%s Ink Ascent = %6d Ink Descent = %6d Ink y = %6d Ink height = %6d\n",
	 pango_layout_get_text(layout),
	 PANGO_ASCENT(ink_rect), PANGO_DESCENT(ink_rect),
	 ink_rect.y, ink_rect.height);
#endif
	 
  bbox = BoundingBox(Gtk_RenderingContext::fromPangoPixels(ink_rect.width),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(ink_rect)),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_DESCENT(ink_rect)));
}

Gtk_PangoLayoutArea::~Gtk_PangoLayoutArea()
{ }

BoundingBox
Gtk_PangoLayoutArea::box() const
{ return bbox; }

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
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);
//   printf("before %p %d %d %d %d\n",
// 	 &context,
// 	 x.getValue(), context.getXOrigin().getValue(),
// 	 Gtk_RenderingContext::toGtkPixels(x - context.getXOrigin()),
// 	 Gtk_RenderingContext::toGtkPixels(y - context.getYOrigin()));

  PangoLayoutLine* line = pango_layout_get_line(layout, 0);
  gdk_draw_layout_line(context.getDrawable(),
		       context.getGC(),
		       Gtk_RenderingContext::toGtkX(x),
		       Gtk_RenderingContext::toGtkY(y),
		       line);
}

DOM::Element
Gtk_PangoLayoutArea::dump(const DOM::Document& doc) const
{
  assert(false);
  return DOM::Element(0);
}
