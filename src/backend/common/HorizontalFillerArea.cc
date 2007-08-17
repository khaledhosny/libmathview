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

#include "HorizontalFillerArea.hh"
#include "HorizontalSpaceArea.hh"

SmartPtr<HorizontalFillerArea>
HorizontalFillerArea::create()
{
  return new HorizontalFillerArea();
}

BoundingBox
HorizontalFillerArea::box() const
{
  return BoundingBox();
}

AreaRef
HorizontalFillerArea::fit(const scaled& width, const scaled&, const scaled&) const
{
  //std::cout << "filler becoming space of " << width.toInt() << std::endl;
  return HorizontalSpaceArea::create(width);
}

void
HorizontalFillerArea::strength(int& w, int& h, int& d) const
{
  w = 1;
  h = d = 0;
}

scaled
HorizontalFillerArea::leftEdge() const
{
  return scaled::min();
}

scaled
HorizontalFillerArea::rightEdge() const
{
  return scaled::min();
}
