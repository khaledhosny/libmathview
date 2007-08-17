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

#ifndef __IgnoreArea_hh__
#define __IgnoreArea_hh__

#include "BinContainerArea.hh"

class GMV_MathView_EXPORT IgnoreArea : public BinContainerArea
{
protected:
  IgnoreArea(const AreaRef& area) : BinContainerArea(area) { }
  virtual ~IgnoreArea() { }

public:
  static SmartPtr<IgnoreArea> create(const AreaRef& area) { return new IgnoreArea(area); }
  virtual AreaRef clone(const AreaRef& area) const { return create(area); }

  virtual BoundingBox box(void) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
};

#endif // __IgnoreArea_hh__
