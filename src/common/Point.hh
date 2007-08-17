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

#ifndef __Point_hh__
#define __Point_hh__

#include "scaled.hh"

struct GMV_MathView_EXPORT Point
{
  Point(void) : x(scaled::zero()), y(scaled::zero()) { }
  Point(const scaled& x0, const scaled& y0) : x(x0), y(y0) { }

  void set(const scaled& x0, const scaled& y0) { x = x0; y = y0; }

  scaled x;
  scaled y;
};

#endif // __Point_hh__
