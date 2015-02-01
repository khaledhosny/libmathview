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
