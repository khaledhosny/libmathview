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
#include "TFMFont.hh"
#include "SVG_RenderingContext.hh"
#include "SVG_TTF_TFMGlyphArea.hh"

SVG_TTF_TFMGlyphArea::SVG_TTF_TFMGlyphArea(const SmartPtr<TFMFont>& f, Char8 i, Char8 ttf_i)
  : SVG_TFMGlyphArea(f, i), ttf_index(ttf_i)
{ }

SVG_TTF_TFMGlyphArea::~SVG_TTF_TFMGlyphArea()
{ }

SmartPtr<SVG_TTF_TFMGlyphArea>
SVG_TTF_TFMGlyphArea::create(const SmartPtr<TFMFont>& font, Char8 index, Char8 ttf_index)
{ return new SVG_TTF_TFMGlyphArea(font, index, ttf_index); }

void
SVG_TTF_TFMGlyphArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  SVG_RenderingContext& context = dynamic_cast<SVG_RenderingContext&>(c);
  context.draw(x, y, getFont(), ttf_index);
}
