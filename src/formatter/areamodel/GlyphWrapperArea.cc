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
GlyphWrapperArea::origin(AreaIndex, scaled&, scaled&) const
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
GlyphWrapperArea::positionOfIndex(CharIndex index, scaled& dx, scaled&) const
{
  if (index >= 0 && index <= contentLength)
    {
      if (index == contentLength) dx += box().width;
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
{ return index >= 0 && index <= contentLength; }
