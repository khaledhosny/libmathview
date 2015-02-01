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

#ifndef __SearchingContext_hh__
#define __SearchingContext_hh__

#include "Area.hh"
#include "Rectangle.hh"

class SearchingContext
{
public:
  SearchingContext(const scaled& x, const scaled& y) : x0(x), y0(y) { }
  ~SearchingContext() { }

  bool accept(const Rectangle& rect) const
  { return rect.isInside(x0, y0); }

  bool accept(const BoundingBox& box, const scaled& x, const scaled& y) const
  { return accept(Rectangle(x, y, box));  }

  bool accept(AreaRef area, const scaled& x, const scaled& y) const
  { return accept(area->box(), x, y); }

  scaled getX(void) const { return x0; }
  scaled getY(void) const { return y0; }

  struct Result {
    scaled x;
    scaled y;
    AreaRef area;
  };

  void setResult(AreaRef area, const scaled& x, const scaled& y)
  {
    result.area = area;
    result.x = x;
    result.y = y;
  }
  Result getResult(void) const { return result; }

protected:
  scaled x0;
  scaled y0;

  Result result;
};

#endif // __SearchingContext_hh__
