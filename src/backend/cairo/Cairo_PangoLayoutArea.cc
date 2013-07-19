// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "Cairo_PangoLayoutArea.hh"
#include "Cairo_RenderingContext.hh"
#include "Point.hh"

Cairo_PangoLayoutArea::Cairo_PangoLayoutArea(PangoLayout* _layout)
  : layout(_layout)
{
  PangoRectangle rect;

  pango_layout_get_extents(layout, 0, &rect);

  PangoLayoutIter* iter = pango_layout_get_iter(layout);
  const scaled height = Cairo_RenderingContext::fromPangoPixels(pango_layout_iter_get_baseline(iter));
  pango_layout_iter_free(iter);

  bbox = BoundingBox(Cairo_RenderingContext::fromPangoPixels(rect.width),
		     height, Cairo_RenderingContext::fromPangoPixels(rect.height) - height);
}

Cairo_PangoLayoutArea::Cairo_PangoLayoutArea(PangoLayout* _layout, bool)
  : layout(_layout)
{
  // this version of the constructor does not compute the bounding
  // box which is supposed to be initialized by a derived class
  // (see Cairo_PangoLayoutLineArea)
}

Cairo_PangoLayoutArea::~Cairo_PangoLayoutArea()
{ }

BoundingBox
Cairo_PangoLayoutArea::box() const
{ return bbox; }

scaled
Cairo_PangoLayoutArea::leftEdge() const
{
  PangoRectangle rect;
  pango_layout_get_extents(layout, &rect, 0);
  return Cairo_RenderingContext::fromPangoPixels(PANGO_LBEARING(rect));
}

scaled
Cairo_PangoLayoutArea::rightEdge() const
{
  PangoRectangle rect;
  pango_layout_get_extents(layout, &rect, 0);
  return Cairo_RenderingContext::fromPangoPixels(PANGO_RBEARING(rect));
}

void
Cairo_PangoLayoutArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Cairo_RenderingContext& context = dynamic_cast<Cairo_RenderingContext&>(c);
  context.draw(x, y + bbox.height, layout);
}

bool
Cairo_PangoLayoutArea::indexOfPosition(const scaled& x, const scaled& y, CharIndex& index) const
{
  gint utf8_index;
  gint trailing;
  if (pango_layout_xy_to_index(layout,
			       Cairo_RenderingContext::toPangoPixels(x),
			       Cairo_RenderingContext::toPangoPixels(y + bbox.height),
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
Cairo_PangoLayoutArea::positionOfIndex(CharIndex index, Point* p, BoundingBox* b) const
{
  const gchar* buffer = pango_layout_get_text(layout);

  if (index >= 0 && index < g_utf8_strlen(buffer, -1))
    {
      const gchar* ptr = g_utf8_offset_to_pointer(buffer, index);
      PangoRectangle rect;
      pango_layout_index_to_pos(layout, ptr - buffer, &rect);

      if (p)
	{
	  p->x += Cairo_RenderingContext::fromPangoPixels(rect.x);
	  p->y += Cairo_RenderingContext::fromPangoPixels(rect.y);
	}

      if (b) *b = BoundingBox(Cairo_RenderingContext::fromPangoPixels(rect.width),
			      Cairo_RenderingContext::fromPangoPixels(PANGO_ASCENT(rect)),
			      Cairo_RenderingContext::fromPangoPixels(PANGO_DESCENT(rect)));

      return true;
    }
  else
    return false;
}

CharIndex
Cairo_PangoLayoutArea::length() const
{
  return g_utf8_strlen(pango_layout_get_text(layout), -1);
}
