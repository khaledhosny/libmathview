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

#include "Char.hh"
#include "t1lib_T1Font.hh"
#include "Gtk_RenderingContext.hh"
#include "Gtk_t1lib_T1GlyphArea.hh"

Gtk_t1lib_T1GlyphArea::Gtk_t1lib_T1GlyphArea(const SmartPtr<t1lib_T1Font>& f, Char8 i)
  : font(f), index(i)
{ }

Gtk_t1lib_T1GlyphArea::~Gtk_t1lib_T1GlyphArea()
{ }

SmartPtr<Gtk_t1lib_T1GlyphArea>
Gtk_t1lib_T1GlyphArea::create(const SmartPtr<t1lib_T1Font>& font, Char8 index)
{ return new Gtk_t1lib_T1GlyphArea(font, index); }

SmartPtr<t1lib_T1Font>
Gtk_t1lib_T1GlyphArea::getFont() const
{
  return font;
}

BoundingBox
Gtk_t1lib_T1GlyphArea::box() const
{ return font->getGlyphBoundingBox(index); }

scaled
Gtk_t1lib_T1GlyphArea::leftEdge() const
{ return font->getGlyphLeftEdge(index); }

scaled
Gtk_t1lib_T1GlyphArea::rightEdge() const
{ return font->getGlyphRightEdge(index); }

void
Gtk_t1lib_T1GlyphArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);
  context.draw(x, y, font, index);
}
