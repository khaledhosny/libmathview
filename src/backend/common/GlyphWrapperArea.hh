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

#ifndef __GlyphWrapperArea_hh__
#define __GlyphWrapperArea_hh__

#include "BinContainerArea.hh"

class GMV_MathView_EXPORT GlyphWrapperArea : public BinContainerArea
{
protected:
  GlyphWrapperArea(const AreaRef& area, CharIndex l)
    : BinContainerArea(area), contentLength(l) { }
  virtual ~GlyphWrapperArea() { }

public:
  static SmartPtr<GlyphWrapperArea> create(const AreaRef& area, CharIndex l)
  { return new GlyphWrapperArea(area, l); }
  virtual AreaRef clone(const AreaRef& area) const
  { return create(area, length()); }

  virtual CharIndex length(void) const { return contentLength; }
  virtual CharIndex lengthTo(AreaIndex) const;
  virtual AreaRef node(AreaIndex) const;
  virtual void origin(AreaIndex, class Point&) const;
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point*, BoundingBox*) const;
  virtual bool searchByArea(class AreaId&, const AreaRef&) const;
  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;
  virtual bool searchByIndex(class AreaId&, CharIndex) const;
  
private:
  CharIndex contentLength;
};

#endif // __GlyphWrapperArea_hh__


