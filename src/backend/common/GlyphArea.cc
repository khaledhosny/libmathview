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

#include "GlyphArea.hh"
#include "Rectangle.hh"

GlyphArea::~GlyphArea()
{ }

bool
GlyphArea::searchByIndex(AreaId&, CharIndex index) const
{
  return (index >= 0 && index < length());
}

bool
GlyphArea::indexOfPosition(const scaled& x, const scaled& y, CharIndex& index) const
{
  const BoundingBox bbox = box();
  if (Rectangle(scaled::zero(), scaled::zero(), bbox).isInside(x, y))
    {
      index = (x < bbox.width / 2) ? 0 : length();
      return true;
    }
  else
    return false;
}

bool
GlyphArea::positionOfIndex(CharIndex index, Point*, BoundingBox* b) const
{
  if (index == 0 && index < length())
    {
      // do nothing about the Point
      if (b) *b = box();
      return true;
    }
  else
    return false;
}

SmartPtr<const GlyphArea>
GlyphArea::getGlyphArea() const
{ return this; }
