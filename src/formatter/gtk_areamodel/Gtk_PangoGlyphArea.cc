// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
//
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>
#include <pango/pango.h>

#include "Gtk_PangoGlyphArea.hh"
#include "Gtk_RenderingContext.hh"

Gtk_PangoGlyphArea::Gtk_PangoGlyphArea(PangoFont* f, PangoGlyphString* g)
  : font(f), glyphs(g)
{ }

Gtk_PangoGlyphArea::~Gtk_PangoGlyphArea()
{ }

BoundingBox
Gtk_PangoGlyphArea::box() const
{
  PangoRectangle ink_rect;
  PangoRectangle logical_rect;
  pango_font_get_glyph_extents(font, glyphs->glyphs[0].glyph, &ink_rect, &logical_rect);
  return BoundingBox(Gtk_RenderingContext::fromPangoPixels(logical_rect.width),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(ink_rect)),
		     Gtk_RenderingContext::fromPangoPixels(PANGO_DESCENT(ink_rect)));
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
  gdk_draw_glyphs(context.getDrawable(),
		  context.getGC(),
		  font,
		  Gtk_RenderingContext::toGtkX(x),
		  Gtk_RenderingContext::toGtkY(y),
		  glyphs);
}

DOM::Element
Gtk_PangoGlyphArea::dump(const DOM::Document& doc) const
{
  assert(false);
  return DOM::Element(0);
}
