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

#ifndef __Area_hh__
#define __Area_hh__

#include "BoundingBox.hh"
#include "Object.hh"
#include "SmartPtr.hh"

typedef SmartPtr<const class Area> AreaRef;

typedef int CharIndex;
typedef int AreaIndex;

class GMV_MathView_EXPORT Area : public Object
{
protected:
  Area(void) { };
  virtual ~Area() { };

public:
  virtual BoundingBox box(void) const = 0;
  virtual void render(class RenderingContext&, const scaled& x, const scaled& y) const = 0;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const = 0;
  virtual scaled leftEdge(void) const = 0;
  virtual scaled rightEdge(void) const = 0;
  virtual void strength(int&, int&, int&) const = 0;
  virtual AreaIndex size(void) const { return 0; }
  virtual AreaRef node(AreaIndex) const = 0;
  virtual void origin(AreaIndex, class Point&) const = 0;
  virtual CharIndex length(void) const { return 0; }
  virtual CharIndex lengthTo(AreaIndex) const = 0;
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point* = 0, BoundingBox* = 0) const;
  virtual scaled getStep(void) const;

  virtual bool searchByArea(class AreaId&, const AreaRef&) const = 0;
  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const = 0;
  virtual bool searchByIndex(class AreaId&, CharIndex) const = 0;
  virtual AreaRef flatten(void) const { return this; }

  virtual SmartPtr<class Element> getElement(void) const;

  virtual SmartPtr<const class GlyphStringArea> getGlyphStringArea(void) const;  
  virtual SmartPtr<const class GlyphArea> getGlyphArea(void) const;

  scaled originX(AreaIndex) const;
  scaled originY(AreaIndex) const;
};

#endif // __Area_hh__
