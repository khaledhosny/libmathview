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

#include <algorithm>

#include "BoundingBox.hh"

void
BoundingBox::append(const BoundingBox& box)
{
  width += box.width;
  height = std::max(height, box.height);
  depth = std::max(depth, box.depth);
}

void
BoundingBox::overlap(const BoundingBox& box)
{
  if (!box.defined())
    return;
  else if (defined())
    {
      width = std::max(width, box.width);
      height = std::max(height, box.height);
      depth = std::max(depth, box.depth);
    }
  else
    *this = box;
}

void
BoundingBox::under(const BoundingBox& box)
{
  if (!box.defined())
    return;
  else if (defined())
    {
      width = std::max(width, box.width);
      height += box.height + box.depth;
    }
  else
    *this = box;
}

void
BoundingBox::over(const BoundingBox& box)
{
  if (!box.defined())
    return;
  else if (defined())
    {
      width = std::max(width, box.width);
      depth += box.height + box.depth;
    }
  else
    *this = box;
}

