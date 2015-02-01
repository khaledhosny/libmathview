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

#ifndef __BinContainerArea_hh__
#define __BinContainerArea_hh__

#include "ContainerArea.hh"

class BinContainerArea : public ContainerArea
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
  virtual void origin(AreaIndex, struct Point&) const;
  virtual AreaRef replace(AreaIndex, const AreaRef&) const;
  virtual CharIndex length(void) const;
  virtual CharIndex lengthTo(AreaIndex) const;
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, struct Point*, BoundingBox*) const;
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
