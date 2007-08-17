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

#ifndef __BinContainerArea_hh__
#define __BinContainerArea_hh__

#include "ContainerArea.hh"

class GMV_MathView_EXPORT BinContainerArea : public ContainerArea
{
protected:
  BinContainerArea(const AreaRef& area) : child(area) { }
  virtual ~BinContainerArea() { }

public:
  virtual AreaRef clone(const AreaRef&) const = 0;

  virtual BoundingBox box(void) const;
  virtual void render(class RenderingContext& context, const scaled& x, const scaled& y) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual void strength(int&, int&, int&) const;
  virtual AreaIndex size(void) const { return 1; }
  virtual AreaRef node(AreaIndex) const;
  virtual void origin(AreaIndex, class Point&) const;
  virtual AreaRef replace(AreaIndex, const AreaRef&) const;
  virtual CharIndex length(void) const;
  virtual CharIndex lengthTo(AreaIndex) const;
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point*, BoundingBox*) const;
  virtual scaled getStep(void) const;

  virtual bool searchByArea(class AreaId&, const AreaRef&) const;
  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;
  virtual bool searchByIndex(class AreaId&, CharIndex) const;

  virtual SmartPtr<const class GlyphStringArea> getGlyphStringArea(void) const;  
  virtual SmartPtr<const class GlyphArea> getGlyphArea(void) const;  

  AreaRef getChild(void) const { return child; }

private:
  AreaRef child;
};

#endif // __BinContainerArea_hh__
