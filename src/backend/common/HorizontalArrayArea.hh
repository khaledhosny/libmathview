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

#ifndef __HorizontalArrayArea_hh__
#define __HorizontalArrayArea_hh__

#include "LinearContainerArea.hh"

class GMV_MathView_EXPORT HorizontalArrayArea : public LinearContainerArea
{
protected:
  HorizontalArrayArea(const std::vector<AreaRef>& children) : LinearContainerArea(children) { }
  virtual ~HorizontalArrayArea() { }

public:
  static SmartPtr<HorizontalArrayArea> create(const std::vector<AreaRef>&);
  virtual AreaRef clone(const std::vector<AreaRef>& c) const { return create(c); }

  virtual AreaRef flatten(void) const;
  virtual BoundingBox box(void) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual void strength(int&, int&, int&) const;
  virtual void origin(AreaIndex, class Point&) const;
  virtual scaled getStep(void) const;

  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;

  scaled leftSide(AreaIndex) const;
  scaled rightSide(AreaIndex) const;

private:
  static void flattenAux(std::vector<AreaRef>&, const std::vector<AreaRef>&);
};

#endif // __HorizontalArrayArea_hh__
