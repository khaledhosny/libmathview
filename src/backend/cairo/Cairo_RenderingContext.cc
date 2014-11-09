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

Cairo_RenderingContext::Cairo_RenderingContext(cairo_t* cr)
  : m_cr(cr)
{ }

Cairo_RenderingContext::~Cairo_RenderingContext()
{
  cairo_destroy(m_cr);
}

void
Cairo_RenderingContext::fill(const scaled& x, const scaled& y, const BoundingBox& box) const
{
  cairo_save(m_cr);

  RGBColor fg = getForegroundColor();
  cairo_set_source_rgba(m_cr, fg.red / 255., fg.green / 255., fg.blue / 255., fg.alpha / 255.);
  cairo_rectangle(m_cr, x.toDouble(), -(y + box.height).toDouble(), box.width.toDouble(), (box.height + box.depth).toDouble());
  cairo_fill(m_cr);

  cairo_restore(m_cr);
}

void
Cairo_RenderingContext::draw(const scaled& x, const scaled& y, cairo_scaled_font_t* font,
                             unsigned glyph) const
{
  cairo_save(m_cr);

  RGBColor fg = getForegroundColor();
  cairo_set_scaled_font(m_cr, font);
  cairo_set_source_rgba(m_cr, fg.red / 255., fg.green / 255., fg.blue / 255., fg.alpha / 255.);

  cairo_glyph_t glyphs[1] = { { glyph, x.toDouble(), -y.toDouble() } };
  cairo_show_glyphs(m_cr, glyphs, 1);

  cairo_restore(m_cr);
}
