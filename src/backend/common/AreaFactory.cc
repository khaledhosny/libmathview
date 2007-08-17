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

#include "AreaFactory.hh"

AreaRef
AreaFactory::background(const AreaRef& base, const RGBColor& c) const
{
  std::vector<AreaRef> o;
  o.reserve(2);
  o.push_back(color(ink(base), c));
  o.push_back(base);
  return overlapArray(o);
}

AreaRef
AreaFactory::horizontalLine(const scaled& thickness, const RGBColor& c) const
{
  const scaled halfThickness = thickness / 2;
  std::vector<AreaRef> h;
  h.reserve(2);
  h.push_back(verticalSpace(halfThickness, thickness - halfThickness));
  h.push_back(horizontalFiller());
  return color(ink(horizontalArray(h)), c);
}

AreaRef
AreaFactory::fixedHorizontalLine(const scaled& thickness, const scaled& length, const RGBColor& c) const
{
  const scaled halfThickness = thickness / 2;
  std::vector<AreaRef> h;
  h.reserve(2);
  h.push_back(verticalSpace(halfThickness, thickness - halfThickness));
  h.push_back(horizontalSpace(length));
  return color(ink(horizontalArray(h)), c);
}

AreaRef
AreaFactory::verticalLine(const scaled& thickness, const RGBColor& c) const
{
  std::vector<AreaRef> v;
  v.reserve(3);
  v.push_back(verticalFiller());
  v.push_back(horizontalSpace(thickness));
  v.push_back(verticalFiller());
  return color(ink(verticalArray(v, 1)), c);
}

AreaRef
AreaFactory::fixedVerticalLine(const scaled& thickness, const scaled& height, const scaled& depth, const RGBColor& c) const
{
  std::vector<AreaRef> v;
  v.reserve(2);
  v.push_back(horizontalSpace(thickness));
  v.push_back(verticalSpace(height, depth));
  return color(ink(verticalArray(v, 1)), c);
}

AreaRef
AreaFactory::center(const AreaRef& area) const
{
  std::vector<AreaRef> h;
  h.reserve(3);
  h.push_back(horizontalFiller());
  h.push_back(area);
  h.push_back(horizontalFiller());
  return horizontalArray(h);
}

AreaRef
AreaFactory::left(const AreaRef& area) const
{
  std::vector<AreaRef> h;
  h.reserve(2);
  h.push_back(area);
  h.push_back(horizontalFiller());
  return horizontalArray(h);
}

AreaRef
AreaFactory::right(const AreaRef& area) const
{
  std::vector<AreaRef> h;
  h.reserve(2);
  h.push_back(horizontalFiller());
  h.push_back(area);
  return horizontalArray(h);
}

AreaRef
AreaFactory::middle(const AreaRef& area) const
{
  BoundingBox areaBox = area->box();

  std::vector<AreaRef> v;
  v.reserve(3);
  v.push_back(horizontalSpace(0));
  v.push_back(verticalFiller());
  v.push_back(shift(area, (areaBox.depth - areaBox.depth) / 2));
  v.push_back(verticalFiller());
  return verticalArray(v, 0);
}

AreaRef
AreaFactory::top(const AreaRef& area) const
{
  std::vector<AreaRef> v;
  v.reserve(2);
  v.push_back(verticalFiller());
  v.push_back(area);
  return verticalArray(v, 0);
}

AreaRef
AreaFactory::bottom(const AreaRef& area) const
{
  std::vector<AreaRef> v;
  v.reserve(2);
  v.push_back(area);
  v.push_back(verticalFiller());
  return verticalArray(v, 1);
}

AreaRef
AreaFactory::freeze(const AreaRef& area) const
{
  return box(area, area->box());
}

