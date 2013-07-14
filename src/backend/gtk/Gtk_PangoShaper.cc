// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
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

#include "AbstractLogger.hh"
#include "Gtk_PangoGlyphArea.hh"
#include "Gtk_PangoShaper.hh"
#include "Gtk_RenderingContext.hh"
#include "ShapingContext.hh"

Gtk_PangoShaper::Gtk_PangoShaper(const GObjectPtr<PangoContext>& c,
                                 const GObjectPtr<PangoFont>& f,
                                 const SmartPtr<MathFont>& mf)
  : MathShaper(mf)
  , context(c)
  , font(f)
{ }

Gtk_PangoShaper::~Gtk_PangoShaper()
{ }

SmartPtr<Gtk_PangoShaper>
Gtk_PangoShaper::create(const GObjectPtr<PangoContext>& c,
                        const GObjectPtr<PangoFont>& f,
                        const SmartPtr<MathFont>& mf)
{
  return new Gtk_PangoShaper(c, f, mf);
}

unsigned
Gtk_PangoShaper::shapeChar(Char32 ch) const
{
  PangoFcFont* fcfont = PANGO_FC_FONT(static_cast<PangoFont*>(font));
  return pango_fc_font_get_glyph(fcfont, ch);
}

AreaRef
Gtk_PangoShaper::getGlyphArea(unsigned glyph, const scaled& sp_size) const
{
  PangoGlyphString* glyphstring = pango_glyph_string_new();
  pango_glyph_string_set_size(glyphstring, 1);

  glyphstring->glyphs[0].glyph = (PangoGlyph) glyph;
  glyphstring->glyphs[0].geometry.x_offset = 0;
  glyphstring->glyphs[0].geometry.y_offset = 0;
  glyphstring->glyphs[0].geometry.width = 0;

  PangoFontDescription* description = pango_font_describe(font);
  const gint size = Gtk_RenderingContext::toPangoPoints(sp_size);
  pango_font_description_set_size(description, size);
  GObjectPtr<PangoFont> newfont = pango_context_load_font(context, description);

  return Gtk_PangoGlyphArea::create(newfont, glyphstring);
}
