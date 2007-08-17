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

#include "AreaId.hh"
#include "SimpleArea.hh"
#include "Rectangle.hh"

AreaRef
SimpleArea::fit(const scaled&, const scaled&, const scaled&) const
{
  return this;
}

void
SimpleArea::strength(int& w, int& h, int& d) const
{ w = h = d = 0; }

void
SimpleArea::render(class RenderingContext&, const scaled&, const scaled&) const
{ }

bool
SimpleArea::searchByArea(AreaId&, const AreaRef& area) const
{ return this == area; }

bool
SimpleArea::searchByCoords(AreaId&, const scaled& x, const scaled& y) const
{ return Rectangle(scaled::zero(), scaled::zero(), box()).isInside(x, y); }

bool
SimpleArea::searchByIndex(AreaId&, CharIndex index) const
{ return false; }

AreaRef
SimpleArea::node(AreaIndex) const
{
  // A leaf area cannot have children
  assert(false);
}

void
SimpleArea::origin(AreaIndex, class Point&) const
{
  // A leaf area cannot have children
  assert(false);
}

CharIndex
SimpleArea::lengthTo(AreaIndex) const
{
  // A leaf area cannot have children
  assert(false);
}
