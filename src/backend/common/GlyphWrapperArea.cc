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

#include "GlyphWrapperArea.hh"
#include "Rectangle.hh"

AreaRef
GlyphWrapperArea::node(AreaIndex) const
{ assert(false); }

CharIndex
GlyphWrapperArea::lengthTo(AreaIndex) const
{ assert(false); }

void
GlyphWrapperArea::origin(AreaIndex, Point&) const
{ assert(false); }

bool
GlyphWrapperArea::indexOfPosition(const scaled& x, const scaled& y, CharIndex& index) const
{
  const BoundingBox bbox = box();
  if (Rectangle(scaled::zero(), scaled::zero(), bbox).isInside(x, y))
    {
      index = (x < bbox.width / 2) ? 0 : contentLength;
      return true;
    }
  else
    return false;
}

bool 
GlyphWrapperArea::positionOfIndex(CharIndex index, Point* p, BoundingBox* b) const
{
  if (index == 0 && index < contentLength)
    {
      // do nothing about the Point
      if (b) *b = box();
      return true;
    }
  else
    return false;
}

bool
GlyphWrapperArea::searchByArea(AreaId&, const AreaRef& area) const
{ return this == area; }

bool
GlyphWrapperArea::searchByCoords(AreaId&, const scaled& x, const scaled& y) const
{ return Rectangle(scaled::zero(), scaled::zero(), box()).isInside(x, y); }

bool
GlyphWrapperArea::searchByIndex(AreaId&, CharIndex index) const
{ return index >= 0 && index < contentLength; }
