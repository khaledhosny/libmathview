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

#include "GlyphStringArea.hh"
#include "Rectangle.hh"
#include "Point.hh"

CharIndex
GlyphStringArea::length() const
{
  CharIndex l = 0;
  // FIXME: this can be rewritten with an accumulator
  for (const auto & elem : counters)
    l += elem;
  return l;
}

CharIndex
GlyphStringArea::lengthTo(AreaIndex index) const
{
  assert(index >= 0 && index < content.size());
  CharIndex acc = 0;
  for (auto p = counters.begin(); p != counters.begin() + index; p++)
    acc += *p;
  return acc;
}

bool
GlyphStringArea::indexOfPosition(const scaled& x0, const scaled& y, CharIndex& index) const
{
  const BoundingBox bbox = box();
  index = 0;
  scaled x = x0;
  for (auto p = content.begin(); p != content.end(); p++)
    {
      const BoundingBox pbox((*p)->box().width, bbox.height, bbox.depth);
      if (Rectangle(scaled::zero(), scaled::zero(), pbox).isInside(x, y))
	{
	  CharIndex i;
	  if ((*p)->indexOfPosition(x, y, i))
	    {
	      index += i;
	      return true;
	    }
	  else
	    {
	      if (x >= pbox.width / 2) index += counters[p - content.begin()];
	      return true;
	    }
	}
      else
	{
	  index += counters[p - content.begin()];
	  x -= (*p)->box().width;
	}
    }      

  return false;
}

// #include <iostream>

bool
GlyphStringArea::positionOfIndex(CharIndex index, Point* point, BoundingBox* b) const
{
  for (auto p = counters.begin(); p != counters.end(); p++)
    if (index < *p)
      {
	if (content[p - counters.begin()]->positionOfIndex(index, point, b))
	  return true;
	else if (index == *p)
	  {
	    point->x += content[p - counters.begin()]->box().width;
	    if (b) *b = content[p - counters.begin()]->box();
	    return true;
	  }
	else
	  return true;
      }
    else
      {
// 	std::cerr << "GlyphStringArea::positionOfIndex iterating index = " << index << std::endl;
	index -= *p;
	point->x += content[p - counters.begin()]->box().width;
      }

  return false;
}

SmartPtr<const GlyphStringArea>
GlyphStringArea::getGlyphStringArea() const
{ return this; }
