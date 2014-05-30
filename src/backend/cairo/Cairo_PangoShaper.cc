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

#define PANGO_ENABLE_ENGINE // for pango_fc_font_get_glyph()
#include <pango/pangofc-font.h>

#include "Cairo_GlyphArea.hh"
#include "Cairo_PangoShaper.hh"
#include "Cairo_RenderingContext.hh"
#include "ShapingContext.hh"

Cairo_PangoShaper::Cairo_PangoShaper(const GObjectPtr<PangoContext>& c,
                                 const GObjectPtr<PangoFont>& f,
                                 const SmartPtr<MathFont>& mf)
  : MathShaper(mf)
  , context(c)
  , font(f)
{ }

Cairo_PangoShaper::~Cairo_PangoShaper()
{ }

SmartPtr<Cairo_PangoShaper>
Cairo_PangoShaper::create(const GObjectPtr<PangoContext>& c,
                        const GObjectPtr<PangoFont>& f,
                        const SmartPtr<MathFont>& mf)
{
  return new Cairo_PangoShaper(c, f, mf);
}

unsigned
Cairo_PangoShaper::shapeChar(Char32 ch) const
{
  PangoFcFont* fcfont = PANGO_FC_FONT(static_cast<PangoFont*>(font));
  return pango_fc_font_get_glyph(fcfont, ch);
}

AreaRef
Cairo_PangoShaper::getGlyphArea(unsigned glyph, const scaled& sp_size) const
{
  PangoFontDescription* description = pango_font_describe(font);
  const gint size = toPangoPoints(sp_size);
  pango_font_description_set_size(description, size);
  GObjectPtr<PangoCairoFont> newfont = (PangoCairoFont*) pango_context_load_font(context, description);
  cairo_scaled_font_t* cairo_font = pango_cairo_font_get_scaled_font (newfont);

  return Cairo_GlyphArea::create(cairo_font, glyph);
}
