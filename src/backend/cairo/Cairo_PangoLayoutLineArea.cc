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

#include "Point.hh"
#include "Cairo_PangoLayoutLineArea.hh"
#include "Cairo_RenderingContext.hh"

Cairo_PangoLayoutLineArea::Cairo_PangoLayoutLineArea(PangoLayout* _layout)
  : Cairo_PangoLayoutArea(_layout, true)
{
  PangoRectangle rect;
  PangoLayoutLine* line = pango_layout_get_line(layout, 0);
  pango_layout_line_get_extents(line, &rect, 0);

  bbox = BoundingBox(Cairo_RenderingContext::fromPangoPixels(rect.width),
		     Cairo_RenderingContext::fromPangoPixels(PANGO_ASCENT(rect)),
		     Cairo_RenderingContext::fromPangoPixels(PANGO_DESCENT(rect)));
}

Cairo_PangoLayoutLineArea::~Cairo_PangoLayoutLineArea()
{ }

void
Cairo_PangoLayoutLineArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Cairo_RenderingContext& context = dynamic_cast<Cairo_RenderingContext&>(c);
  context.draw(x, y, pango_layout_get_line(layout, 0));
}

#if 1

bool
Cairo_PangoLayoutLineArea::indexOfPosition(const scaled& x, const scaled& y, CharIndex& index) const
{
  gint utf8_index;
  gint trailing;
  if (pango_layout_line_x_to_index(pango_layout_get_line(layout, 0),
				   Cairo_RenderingContext::toPangoPixels(x),
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
Cairo_PangoLayoutLineArea::positionOfIndex(CharIndex index, Point* p, BoundingBox* b) const
{
  const gchar* buffer = pango_layout_get_text(layout);

  if (index >= 0 && index <= g_utf8_strlen(buffer, -1))
    {
      const gchar* ptr = g_utf8_offset_to_pointer(buffer, index);
      
      gint xpos;
      pango_layout_line_index_to_x(pango_layout_get_line(layout, 0), ptr - buffer, 0, &xpos);

      if (p)
	{
	  p->x += Cairo_RenderingContext::fromPangoPixels(xpos);
	  p->y += scaled::zero();
	}

      return true;
    }
  else
    return false;
}
#endif
