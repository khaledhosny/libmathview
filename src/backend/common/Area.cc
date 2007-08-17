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

#include "Area.hh"
#include "Point.hh"
#include "Element.hh"
#include "Rectangle.hh"
#include "GlyphStringArea.hh"
#include "GlyphArea.hh"

scaled
Area::originX(AreaIndex i) const
{
  Point p;
  origin(i, p);
  return p.x;
}

scaled
Area::originY(AreaIndex i) const
{
  Point p;
  origin(i, p);
  return p.y;
}

bool
Area::indexOfPosition(const scaled&, const scaled&, CharIndex&) const
{ return false; }

bool
Area::positionOfIndex(CharIndex, class Point*, BoundingBox*) const
{ return false; }

SmartPtr<Element>
Area::getElement() const
{ return 0; }

scaled
Area::getStep() const
{ return 0; }

SmartPtr<const GlyphStringArea> 
Area::getGlyphStringArea() const
{ return 0; }

SmartPtr<const GlyphArea>
Area::getGlyphArea() const
{ return 0; }

