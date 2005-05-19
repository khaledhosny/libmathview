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

#include "BoxArea.hh"
#include "Rectangle.hh"

BoxArea::BoxArea(const AreaRef& area, const BoundingBox& b)
  : BinContainerArea(area->fit(b.width, b.height, b.depth)), bbox(b)
{ }

AreaRef
BoxArea::fit(const scaled&, const scaled&, const scaled&) const
{
#if 0
  AreaRef newChild = getChild()->fit(bbox.width, bbox.height, bbox.depth);
  if (newChild == getChild())
    return this;
  else
    return clone(newChild);
#endif
  return this;
}

void
BoxArea::strength(int& w, int& h, int& d) const
{
  w = h = d = 0;
}

bool
BoxArea::searchByCoords(AreaId& id, const scaled& x, const scaled& y) const
{
  if (Rectangle(scaled::zero(), scaled::zero(), box()).isInside(x, y))
    {
      if (BinContainerArea::searchByCoords(id, x, y))
	return true;
      return true;
    }
  else
    return false;
}
