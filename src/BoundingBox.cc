// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <stdio.h>

#include <algorithm>

#include "Rectangle.hh"
#include "BoundingBox.hh"

void
BoundingBox::Null()
{
  null     = true;
  width = ascent = descent = 0;
  lBearing = rBearing = 0;
}

void
BoundingBox::Set(const scaled& w, const scaled& a, const scaled& d, const scaled& l, const scaled& r)
{
  null     = false;
  width    = w;
  ascent   = a;
  descent  = d;
  lBearing = l;
  rBearing = r;
}

void
BoundingBox::Append(const BoundingBox& box)
{  
  if (box.IsNull()) return;

  if (null)
    Set(box.width, box.ascent, box.descent, box.lBearing, box.rBearing);
  else
    {
      ascent   = std::max(ascent, box.ascent);
      descent  = std::max(descent, box.descent);
      rBearing = width + box.rBearing;
      width += box.width;
    }
}

void
BoundingBox::Append(const scaled& w)
{
  width += w;
}

void
BoundingBox::Max(const BoundingBox& box)
{
  if (box.IsNull()) return;

  if (IsNull()) {
    *this = box;
    return;
  }

  ascent = std::max(ascent, box.ascent);
  descent = std::max(descent, box.descent);
  width = std::max(width, box.width);
  lBearing = std::min(lBearing, box.lBearing);
  rBearing = std::max(rBearing, box.rBearing);
}

Rectangle
BoundingBox::GetRectangle(const scaled& x, const scaled& y) const
{
  Rectangle rect;
  rect.x = x;
  rect.y = y - ascent;
  rect.width = width;
  rect.height = GetHeight();
  return rect;
}

void
BoundingBox::Dump() const
{
  if (IsNull()) printf("[null box]");
  else printf("[%d,+%d,-%d,l:%d,r:%d]",
	      sp2ipx(width), sp2ipx(ascent), sp2ipx(descent),
	      sp2ipx(lBearing), sp2ipx(rBearing));
}

