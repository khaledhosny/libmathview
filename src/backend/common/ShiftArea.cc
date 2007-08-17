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

#include "ShiftArea.hh"
#include "AreaId.hh"
#include "Point.hh"

void
ShiftArea::render(class RenderingContext& context, const scaled& x, const scaled& y) const
{
  getChild()->render(context, x, y + shift);
}

BoundingBox
ShiftArea::box() const
{
  BoundingBox childBox = getChild()->box();
  return
    childBox.defined()
    ? BoundingBox(childBox.width, childBox.height + shift, childBox.depth - shift)
    : childBox;
}

bool
ShiftArea::searchByCoords(AreaId& id, const scaled& x, const scaled& y) const
{
  id.append(0, getChild(), scaled::zero(), shift);
  if (getChild()->searchByCoords(id, x, y - shift)) return true; // ?????
  id.pop_back();
  return false;
}

void
ShiftArea::origin(AreaIndex i, Point& p) const
{
  assert(i == 0);
  p.y += shift;
}
