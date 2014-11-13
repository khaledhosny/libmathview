// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2014, Khaled Hosny <khaledhosny@eglug.org>.
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

#include "Cairo_GlyphArea.hh"
#include "Cairo_RenderingContext.hh"

Cairo_GlyphArea::Cairo_GlyphArea(cairo_scaled_font_t* f, unsigned g)
  : m_font(f), m_glyph(g)
{
  cairo_glyph_t glyphs[1] = { { m_glyph, 0, 0 } };
  cairo_text_extents_t extents;
  cairo_scaled_font_glyph_extents(m_font, glyphs, 1, &extents);

  bbox = BoundingBox(scaled(extents.x_advance), scaled(-extents.y_bearing), scaled(extents.y_bearing + extents.height));

  lbearing = scaled(extents.x_bearing);
  rbearing = scaled(extents.x_bearing + extents.x_advance);
}

Cairo_GlyphArea::~Cairo_GlyphArea()
{
  cairo_scaled_font_destroy(m_font);
}

void
Cairo_GlyphArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Cairo_RenderingContext& context = dynamic_cast<Cairo_RenderingContext&>(c);
  context.draw(x, y, m_font, m_glyph);
}
