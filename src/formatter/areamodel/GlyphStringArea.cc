// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include "GlyphStringArea.hh"
#include "Rectangle.hh"
#include "Point.hh"

CharIndex
GlyphStringArea::length() const
{
  CharIndex l = 0;
  // FIXME: this can be rewritten with an accumulator
  for (std::vector<CharIndex>::const_iterator p = counters.begin(); p != counters.end(); p++)
    l += *p;
  return l;
}

CharIndex
GlyphStringArea::lengthTo(AreaIndex index) const
{
  assert(index >= 0 && index < content.size());
  CharIndex acc = 0;
  for (std::vector<CharIndex>::const_iterator p = counters.begin(); p != counters.begin() + index; p++)
    acc += *p;
  return acc;
}

bool
GlyphStringArea::indexOfPosition(const scaled& x0, const scaled& y, CharIndex& index) const
{
  const BoundingBox bbox = box();
  index = 0;
  scaled x = x0;
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.end(); p++)
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
  for (std::vector<CharIndex>::const_iterator p = counters.begin(); p != counters.end(); p++)
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
