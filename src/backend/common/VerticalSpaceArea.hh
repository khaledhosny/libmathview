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

#ifndef __VerticalSpaceArea_hh__
#define __VerticalSpaceArea_hh__

#include "SimpleArea.hh"

class GMV_MathView_EXPORT VerticalSpaceArea : public SimpleArea
{
protected:
  VerticalSpaceArea(scaled h, scaled d) : height(h), depth(d) { }
  virtual ~VerticalSpaceArea() { }

public:
  // TODO: caching of areas: it is likely that there will be a large
  // number of horizontal space areas all with the same width
  static SmartPtr<VerticalSpaceArea> create(const scaled& h, const scaled& d = scaled::zero())
  { return new VerticalSpaceArea(h, d); }

  virtual BoundingBox box(void) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;

  scaled getHeight(void) const { return height; }
  scaled getDepth(void) const { return depth; }

private:
  scaled height;
  scaled depth;
};

#endif // __VerticalSpaceArea_hh__
