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

#include "Rectangle.hh"
#include "BoundingBox.hh"

void
BoundingBox::Null()
{
  null     = true;
  width = ascent = descent = 0;
  lBearing = rBearing = tAscent = tDescent = 0;
}

void
BoundingBox::Set(scaled w, scaled a, scaled d, scaled l, scaled r)
{
  null     = false;
  width    = w;
  ascent   = tAscent = a;
  descent  = tDescent = d;
  lBearing = l;
  rBearing = r;
}

void
BoundingBox::Append(const BoundingBox& box)
{  
  if (box.IsNull()) return;

  if (null) {
    Set(box.width, box.ascent, box.descent, box.lBearing, box.rBearing);
    tAscent = box.tAscent;
    tDescent = box.tDescent;
  } else {
    ascent   = scaledMax(ascent, box.ascent);
    descent  = scaledMax(descent, box.descent);
    rBearing = width + box.rBearing;
    width += box.width;
    tAscent  = scaledMax(tAscent, box.tAscent);
    tDescent = scaledMax(tDescent, box.tDescent);
  }
}

void
BoundingBox::Append(scaled w)
{
  width += w;
}

void
BoundingBox::ToRectangle(scaled x, scaled y, Rectangle& rect) const
{
  rect.x = x;
  rect.y = y - ascent;
  rect.width = width;
  rect.height = GetHeight();
}

void
BoundingBox::Dump() const
{
  if (IsNull()) printf("[null box]");
  else printf("[%d,+%d,-%d,l:%d,r:%d,a:%d,d:%d]",
	      sp2ipx(width), sp2ipx(ascent), sp2ipx(descent),
	      sp2ipx(lBearing), sp2ipx(rBearing), sp2ipx(tAscent), sp2ipx(tDescent));
}

