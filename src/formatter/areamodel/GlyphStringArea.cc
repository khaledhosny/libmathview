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

int
GlyphStringArea::length() const
{
  int l = 0;
  // FIXME: this can be rewritten with an accumulator
  for (std::vector<int>::const_iterator p = counters.begin(); p != counters.end(); p++)
    l += *p;
  return l;
}

bool
GlyphStringArea::indexOfPosition(const scaled& x0, const scaled& y, int& index) const
{
  if (Rectangle(scaled::zero(), scaled::zero(), box()).isInside(x0, y))
    {
      index = 0;
      scaled x = x0;
      for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.end(); p++)
	{
	  int i;
	  if ((*p)->indexOfPosition(x, y, i))
	    {
	      index += i;
	      return true;
	    }
	  else
	    {
	      index += counters[p - content.begin()];
	      x -= (*p)->box().width;
	    }
	}
      
      index = 0;
      return true;
    }
  else
    return false;
}

bool
GlyphStringArea::positionOfIndex(int index, scaled& x, scaled& y, BoundingBox& bbox) const
{
  scaled offset = scaled::zero();
  for (std::vector<int>::const_iterator p = counters.begin(); p != counters.end(); p++)
    if (index < *p)
      {
	const bool res = content[p - counters.begin()]->positionOfIndex(index, x, y, bbox);
	assert(res);
	x += offset;
	return true;
      }
    else
      {
	index -= *p;
	offset += content[p - counters.begin()]->box().width;
      }
  return false;
}
