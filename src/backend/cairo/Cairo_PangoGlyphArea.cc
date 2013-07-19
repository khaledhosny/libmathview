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
#include <pango/pango.h>

#include "Cairo_PangoGlyphArea.hh"
#include "Cairo_RenderingContext.hh"

Cairo_PangoGlyphArea::Cairo_PangoGlyphArea(PangoFont* f, PangoGlyphString* g)
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
	 
  bbox = BoundingBox(Cairo_RenderingContext::fromPangoPixels(logical_rect.width),
		     Cairo_RenderingContext::fromPangoPixels(PANGO_ASCENT(ink_rect)),
		     Cairo_RenderingContext::fromPangoPixels(PANGO_DESCENT(ink_rect)));
}

Cairo_PangoGlyphArea::~Cairo_PangoGlyphArea()
{ }

BoundingBox
Cairo_PangoGlyphArea::box() const
{
#if 0
  PangoRectangle ink_rect;
  PangoRectangle logical_rect;
  pango_font_get_glyph_extents(font, glyphs->glyphs[0].glyph, &ink_rect, &logical_rect);
  return BoundingBox(Cairo_RenderingContext::fromPangoPixels(logical_rect.width),
		     Cairo_RenderingContext::fromPangoPixels(PANGO_ASCENT(ink_rect)),
		     Cairo_RenderingContext::fromPangoPixels(PANGO_DESCENT(ink_rect)));
#endif
  return bbox;
}

scaled
Cairo_PangoGlyphArea::leftEdge() const
{
  PangoRectangle rect;
  pango_glyph_string_extents(glyphs, font, &rect, 0);
  return Cairo_RenderingContext::fromPangoPixels(PANGO_LBEARING(rect));
}

scaled
Cairo_PangoGlyphArea::rightEdge() const
{
  PangoRectangle rect;
  pango_glyph_string_extents(glyphs, font, &rect, 0);
  return Cairo_RenderingContext::fromPangoPixels(PANGO_RBEARING(rect));
}

void
Cairo_PangoGlyphArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Cairo_RenderingContext& context = dynamic_cast<Cairo_RenderingContext&>(c);
  context.draw(x, y, font, glyphs);
}
