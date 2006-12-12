// Copyright (C) 2000-2006, Luca Padovani <padovani@sti.uniurb.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <padovani@sti.uniurb.it>

#include <config.h>

#include <cassert>

#include "Char.hh"
#include "TFMFont.hh"
#include "PS_RenderingContext.hh"
#include "PS_TFMGlyphArea.hh"

PS_TFMGlyphArea::PS_TFMGlyphArea(const SmartPtr<TFMFont>& f, Char8 i)
  : font(f), index(i)
{ }

PS_TFMGlyphArea::~PS_TFMGlyphArea()
{ }

SmartPtr<PS_TFMGlyphArea>
PS_TFMGlyphArea::create(const SmartPtr<TFMFont>& font, Char8 index)
{ return new PS_TFMGlyphArea(font, index); }

SmartPtr<TFMFont>
PS_TFMGlyphArea::getFont() const
{ return font; }

BoundingBox
PS_TFMGlyphArea::box() const
{ return font->getGlyphBoundingBox(index); }

scaled
PS_TFMGlyphArea::leftEdge() const
{ return font->getGlyphLeftEdge(index); }

scaled
PS_TFMGlyphArea::rightEdge() const
{ return font->getGlyphRightEdge(index); }

void
PS_TFMGlyphArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  PS_RenderingContext& context = dynamic_cast<PS_RenderingContext&>(c);
  context.draw(x, y, font, index);
}
