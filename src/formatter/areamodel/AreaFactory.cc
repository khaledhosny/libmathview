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

#include "AreaFactory.hh"

AreaRef
AreaFactory::background(const AreaRef& base, const RGBColor& c) const
{
  assert(false);
  return 0;
}

AreaRef
AreaFactory::horizontalLine(const scaled& thickness, const RGBColor& c) const
{
  scaled halfThickness = thickness / 2;
  std::vector<AreaRef> h;
  h.reserve(2);
  h.push_back(verticalSpace(halfThickness, thickness - halfThickness));
  h.push_back(horizontalFiller());
  return color(ink(horizontalArray(h)), c);
}

AreaRef
AreaFactory::verticalLine(const scaled& thickness, const RGBColor& c) const
{
  std::vector<AreaRef> v;
  v.reserve(2);
  v.push_back(horizontalSpace(thickness));
  v.push_back(verticalFiller());
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
  v.push_back(verticalFiller());
  v.push_back(shift(area, (areaBox.depth - areaBox.depth) / 2));
  v.push_back(verticalFiller());
  return verticalArray(v, 1);
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

