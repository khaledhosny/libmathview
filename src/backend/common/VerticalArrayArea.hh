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

#ifndef __VerticalArrayArea_hh__
#define __VerticalArrayArea_hh__

#include "LinearContainerArea.hh"

class VerticalArrayArea : public LinearContainerArea
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
  virtual void origin(AreaIndex, struct Point&) const;
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
