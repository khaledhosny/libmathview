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
SimpleArea::searchByIndex(AreaId&, int index) const
{ return index < length(); }

AreaRef
SimpleArea::node(unsigned) const
{
  // A leaf area cannot have children
  assert(false);
}

void
SimpleArea::origin(unsigned, scaled&, scaled&) const
{
  // A leaf area cannot have children
  assert(false);
}

int
SimpleArea::lengthTo(unsigned) const
{
  // A leaf area cannot haev children
  assert(false);
}
