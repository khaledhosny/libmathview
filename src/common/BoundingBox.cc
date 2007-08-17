// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
  width = std::max(width, box.width);
  if (!box.defined())
    return;
  else if (defined())
    {
      height = std::max(height, box.height);
      depth = std::max(depth, box.depth);
    }
  else
    {
      height = box.height;
      depth = box.depth;
    }
}

void
BoundingBox::under(const BoundingBox& box)
{
  width = std::max(width, box.width);
  if (!box.defined())
    return;
  else if (defined())
    height += box.height + box.depth;
  else
    {
      height = box.height + box.depth;
      depth = 0;
    }
}

void
BoundingBox::over(const BoundingBox& box)
{
  width = std::max(width, box.width);
  if (!box.defined())
    return;
  else if (defined())
    depth += box.height + box.depth;
  else
    {
      height = 0;
      depth = box.height + box.depth;
    }
}

