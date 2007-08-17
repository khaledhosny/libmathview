// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "Point.hh"
#include "Gtk_PangoLayoutLineArea.hh"
#include "Gtk_RenderingContext.hh"

Gtk_PangoLayoutLineArea::Gtk_PangoLayoutLineArea(PangoLayout* _layout)
  : Gtk_PangoLayoutArea(_layout, true)
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
  context.draw(x, y, pango_layout_get_line(layout, 0));
}

#if 1

bool
Gtk_PangoLayoutLineArea::indexOfPosition(const scaled& x, const scaled& y, CharIndex& index) const
{
  gint utf8_index;
  gint trailing;
  if (pango_layout_line_x_to_index(pango_layout_get_line(layout, 0),
				   Gtk_RenderingContext::toPangoPixels(x),
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
Gtk_PangoLayoutLineArea::positionOfIndex(CharIndex index, Point* p, BoundingBox* b) const
{
  const gchar* buffer = pango_layout_get_text(layout);

  if (index >= 0 && index <= g_utf8_strlen(buffer, -1))
    {
      const gchar* ptr = g_utf8_offset_to_pointer(buffer, index);
      
      gint xpos;
      pango_layout_line_index_to_x(pango_layout_get_line(layout, 0), ptr - buffer, 0, &xpos);

      if (p)
	{
	  p->x += Gtk_RenderingContext::fromPangoPixels(xpos);
	  p->y += scaled::zero();
	}

      return true;
    }
  else
    return false;
}
#endif
