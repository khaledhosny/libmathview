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

#ifndef __VerticalArrayArea_hh__
#define __VerticalArrayArea_hh__

#include "LinearContainerArea.hh"

class GMV_MathView_EXPORT VerticalArrayArea : public LinearContainerArea
{
protected:
  VerticalArrayArea(const std::vector<AreaRef>&, AreaIndex);
  virtual ~VerticalArrayArea() { }

public:
  static SmartPtr<VerticalArrayArea> create(const std::vector<AreaRef>& children, AreaIndex ref = 0)
  { return new VerticalArrayArea(children, ref); }
  virtual AreaRef clone(const std::vector<AreaRef>& children) const
  { return create(children, getRefArea()); }

  virtual AreaRef flatten(void) const;

  virtual BoundingBox box(void) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual void strength(int&, int&, int&) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual void origin(AreaIndex, class Point&) const;
  virtual CharIndex lengthTo(AreaIndex) const;

  AreaIndex getRefArea(void) const { return refArea; }

  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;
  virtual bool searchByIndex(class AreaId&, CharIndex) const;

protected:
  scaled prepareChildBoxes(std::vector<BoundingBox>&) const;

private:
  //static void flattenAux(std::vector<AreaRef>&, const std::vector<AreaRef>&, unsigned);

  AreaIndex refArea;
};

#endif // __VerticalArrayArea_hh__
