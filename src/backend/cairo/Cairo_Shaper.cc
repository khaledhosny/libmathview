// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2013-2014, Khaled Hosny <khaledhosny@eglug.org>.
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

#include "Cairo_GlyphArea.hh"
#include "Cairo_RenderingContext.hh"
#include "Cairo_Shaper.hh"
#include "MathGraphicDevice.hh"
#include "ShapingContext.hh"

Cairo_Shaper::Cairo_Shaper(cairo_scaled_font_t* f,
                           const SmartPtr<MathFont>& mf)
  : MathShaper(mf)
  , font(f)
{ }

Cairo_Shaper::~Cairo_Shaper()
{ }

unsigned
Cairo_Shaper::shapeChar(Char32 ch) const
{
  cairo_glyph_t *glyphs = NULL;
  int num_glyphs;

  String str = StringOfUCS4String(UCS4String(1, ch));

  cairo_scaled_font_text_to_glyphs (font, 0, 0, str.c_str(), -1, &glyphs, &num_glyphs, NULL, NULL, NULL);

  assert (num_glyphs == 1);

  return glyphs[0].index;
}

// FIXME
#define DPI 72.0

AreaRef
Cairo_Shaper::getGlyphArea(unsigned glyph, const scaled& sp_size) const
{
  double size = sp_size.toDouble() * (DPI / 72.0);
  cairo_font_face_t* face = cairo_scaled_font_get_font_face(font);
  cairo_matrix_t matrix, ctm;
  cairo_matrix_init_scale (&matrix, size, size);
  cairo_matrix_init_identity (&ctm);
  cairo_font_options_t* options = cairo_font_options_create();
  cairo_scaled_font_t* new_font = cairo_scaled_font_create(face, &matrix, &ctm, options);

  return Cairo_GlyphArea::create(new_font, glyph);
}
