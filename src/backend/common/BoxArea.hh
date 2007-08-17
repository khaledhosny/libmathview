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

#ifndef __BoxArea_hh__
#define __BoxArea_hh__

#include "BinContainerArea.hh"

class GMV_MathView_EXPORT BoxArea : public BinContainerArea
{
protected:
  BoxArea(const AreaRef&, const BoundingBox&);
  virtual ~BoxArea() { }

public:
  static SmartPtr<BoxArea> create(const AreaRef& area, const BoundingBox& b) { return new BoxArea(area, b); }
  virtual AreaRef clone(const AreaRef& area) const { return create(area, bbox); }

  virtual BoundingBox box(void) const { return bbox; }
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual void strength(int&, int&, int&) const;

  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;

private:
  BoundingBox bbox;
};

#endif // __BoxArea_hh__
