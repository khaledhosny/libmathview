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

#ifndef __BoundingBox_hh__
#define __BoundingBox_hh__

#include "gmv_defines.h"
#include "scaled.hh"

struct GMV_MathView_EXPORT BoundingBox
{
  BoundingBox(void) : width(scaled::zero()), height(scaled::min()), depth(scaled::min()) { }
  BoundingBox(const scaled& w, const scaled& h, const scaled& d) : width(w), height(h), depth(d) { }

  bool defined(void) const { return height != scaled::min() && depth != scaled::min(); }
  scaled horizontalExtent(void) const { return width; }
  scaled verticalExtent(void) const { return height + depth; }

  void unset(void) { set(scaled::zero(), scaled::min(), scaled::min()); }
  void set(const scaled& w, const scaled& h, const scaled& d)
  { width = w; height = h; depth = d; }
  void append(const BoundingBox&);
  void overlap(const BoundingBox&);
  void under(const BoundingBox&);
  void over(const BoundingBox&);

  operator bool(void) const { return defined(); }

  scaled width;
  scaled height;
  scaled depth;
};

#endif // __BoundingBox_hh__
