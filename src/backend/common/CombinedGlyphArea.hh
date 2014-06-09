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

#ifndef __CombinedGlyphArea_hh__
#define __CombinedGlyphArea_hh__
  
#include "Area.hh"
#include "ContainerArea.hh"
#include "BoundingBox.hh"

class CombinedGlyphArea : public ContainerArea
{
protected:
  CombinedGlyphArea(const AreaRef& , const AreaRef&, const AreaRef&,
		    const scaled&, const scaled&, const scaled&); 
  virtual ~CombinedGlyphArea();

public:
  static SmartPtr<CombinedGlyphArea> create(const AreaRef& baseArea, const AreaRef& accentArea,
					    const AreaRef& underArea,
					    const scaled& skewchar, const scaled& verticalSpace,
					    const scaled& underSkew)
    { return new CombinedGlyphArea(baseArea, accentArea, underArea, 
				   skewchar, verticalSpace, underSkew); }
  virtual AreaRef clone(const AreaRef& nB, const AreaRef& nA, const AreaRef& nU,
			const scaled& nDx, const scaled& nDy, const scaled& nDxUnder) const 
    { return create(nB, nA, nU, nDx, nDy, nDxUnder); }
  virtual BoundingBox box(void) const;
  virtual void render(class RenderingContext&, const scaled& x, const scaled& y) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual void strength(int&, int&, int&) const;
  virtual AreaRef node(AreaIndex) const;
  virtual void origin(AreaIndex, class Point&) const;
  virtual CharIndex lengthTo(AreaIndex) const;

  virtual bool searchByArea(class AreaId&, const AreaRef&) const;
  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;
  virtual bool searchByIndex(class AreaId&, CharIndex) const;

  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point*, BoundingBox*) const;

  virtual SmartPtr<const class GlyphArea> getGlyphArea(void) const;
  virtual SmartPtr<const class GlyphStringArea> getGlyphStringArea(void) const;  

protected:
 BoundingBox bbox;
 AreaRef base;
 AreaRef accent;
 AreaRef under;
 scaled dx;
 scaled dy; 
 scaled dxUnder;

};

#endif // __CombinedGlyphArea_hh__
