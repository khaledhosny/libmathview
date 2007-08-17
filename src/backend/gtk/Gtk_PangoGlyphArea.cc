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
#include <pango/pango.h>

#include "Gtk_PangoGlyphArea.hh"
#include "Gtk_RenderingContext.hh"

Gtk_PangoGlyphArea::Gtk_PangoGlyphArea(PangoFont* f, PangoGlyphString* g)
  : font(f), glyphs(g)
{
  PangoRectangle ink_rect;
  PangoRectangle logical_rect;
  pango_font_get_glyph_extents(font, glyphs->glyphs[0].glyph, &ink_rect, &logical_rect);

#if 0
  printf("%c Ink Ascent = %6d Ink Descent = %6d Ink y = %6d Ink height = %6d\n",
	 glyphs->glyphs[0].glyph,
	 PANGO_ASCENT(ink_rect), PANGO_DESCENT(ink_rect),
	 ink_rect.y, ink_rect.height);
#endif
	 
  bbox = BoundingBox(Gtk_RenderingContext::fromPangoPixels(logical_rect.width),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(ink_rect)),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_DESCENT(ink_rect)));
}

Gtk_PangoGlyphArea::~Gtk_PangoGlyphArea()
{ }

BoundingBox
Gtk_PangoGlyphArea::box() const
{
#if 0
  PangoRectangle ink_rect;
  PangoRectangle logical_rect;
  pango_font_get_glyph_extents(font, glyphs->glyphs[0].glyph, &ink_rect, &logical_rect);
  return BoundingBox(Gtk_RenderingContext::fromPangoPixels(logical_rect.width),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(ink_rect)),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_DESCENT(ink_rect)));
#endif
  return bbox;
}

scaled
Gtk_PangoGlyphArea::leftEdge() const
{
  PangoRectangle rect;
  pango_glyph_string_extents(glyphs, font, &rect, 0);
  return Gtk_RenderingContext::fromPangoPixels(PANGO_LBEARING(rect));
}

scaled
Gtk_PangoGlyphArea::rightEdge() const
{
  PangoRectangle rect;
  pango_glyph_string_extents(glyphs, font, &rect, 0);
  return Gtk_RenderingContext::fromPangoPixels(PANGO_RBEARING(rect));
}

void
Gtk_PangoGlyphArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);
  context.draw(x, y, font, glyphs);
}
