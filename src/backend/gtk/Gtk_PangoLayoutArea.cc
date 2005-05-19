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
#include "Point.hh"

Gtk_PangoLayoutArea::Gtk_PangoLayoutArea(PangoLayout* _layout)
  : layout(_layout)
{
  PangoRectangle rect;

  pango_layout_get_extents(layout, 0, &rect);

  PangoLayoutIter* iter = pango_layout_get_iter(layout);
  const scaled height = Gtk_RenderingContext::fromPangoPixels(pango_layout_iter_get_baseline(iter));
  pango_layout_iter_free(iter);

  bbox = BoundingBox(Gtk_RenderingContext::fromPangoPixels(rect.width),
		     height, Gtk_RenderingContext::fromPangoPixels(rect.height) - height);
}

Gtk_PangoLayoutArea::Gtk_PangoLayoutArea(PangoLayout* _layout, bool)
  : layout(_layout)
{
  // this version of the constructor does not compute the bounding
  // box which is supposed to be initialized by a derived class
  // (see Gtk_PangoLayoutLineArea)
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
  context.draw(x, y + bbox.height, layout);
}

bool
Gtk_PangoLayoutArea::indexOfPosition(const scaled& x, const scaled& y, CharIndex& index) const
{
  gint utf8_index;
  gint trailing;
  if (pango_layout_xy_to_index(layout,
			       Gtk_RenderingContext::toPangoPixels(x),
			       Gtk_RenderingContext::toPangoPixels(y + bbox.height),
			       &utf8_index,
			       &trailing))
    {
      const gchar* buffer = pango_layout_get_text(layout);
      index = g_utf8_pointer_to_offset(buffer, buffer + utf8_index) + trailing;
      return true;
    }
  else
    return false;
}

bool
Gtk_PangoLayoutArea::positionOfIndex(CharIndex index, Point* p, BoundingBox* b) const
{
  const gchar* buffer = pango_layout_get_text(layout);

  if (index >= 0 && index < g_utf8_strlen(buffer, -1))
    {
      const gchar* ptr = g_utf8_offset_to_pointer(buffer, index);
      PangoRectangle rect;
      pango_layout_index_to_pos(layout, ptr - buffer, &rect);

      if (p)
	{
	  p->x += Gtk_RenderingContext::fromPangoPixels(rect.x);
	  p->y += Gtk_RenderingContext::fromPangoPixels(rect.y);
	}

      if (b) *b = BoundingBox(Gtk_RenderingContext::fromPangoPixels(rect.width),
			      Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(rect)),
			      Gtk_RenderingContext::fromPangoPixels(PANGO_DESCENT(rect)));

      return true;
    }
  else
    return false;
}

CharIndex
Gtk_PangoLayoutArea::length() const
{
  return g_utf8_strlen(pango_layout_get_text(layout), -1);
}
