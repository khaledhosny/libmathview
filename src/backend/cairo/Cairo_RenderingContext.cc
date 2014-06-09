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

#include "Cairo_RenderingContext.hh"

Cairo_RenderingContext::Cairo_RenderingContext(void)
  : style(NORMAL_STYLE)
{ }

Cairo_RenderingContext::~Cairo_RenderingContext()
{
  if (cairo_context)
    cairo_destroy(cairo_context);
}

void
Cairo_RenderingContext::fill(const scaled& x, const scaled& y, const BoundingBox& box) const
{
  cairo_t* cr = getCairo();
  cairo_save(cr);

  RGBColor fg = getForegroundColor();
  cairo_set_source_rgba(cr, fg.red / 255., fg.green / 255., fg.blue / 255., fg.alpha / 255.);
  cairo_rectangle(cr,
                  toCairoX(x),
                  toCairoY(y + box.height),
                  toCairoPixels(box.width),
                  toCairoPixels(box.height + box.depth));
  cairo_fill(cr);

  cairo_restore(cr);
}

void
Cairo_RenderingContext::draw(const scaled& x, const scaled& y, cairo_scaled_font_t* font,
                             unsigned glyph) const
{
  cairo_t* cr = getCairo();
  cairo_save(cr);

  RGBColor fg = getForegroundColor();
  cairo_set_scaled_font(cr, font);
  cairo_set_source_rgba(cr, fg.red / 255., fg.green / 255., fg.blue / 255., fg.alpha / 255.);

  cairo_glyph_t glyphs[1] = { { glyph, toCairoX(x), toCairoY(y) } };
  cairo_show_glyphs(cr, glyphs, 1);

  cairo_restore(cr);
}
