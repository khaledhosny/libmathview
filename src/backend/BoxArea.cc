// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

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
