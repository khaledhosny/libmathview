// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
//
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __GlyphWrapperArea_hh__
#define __GlyphWrapperArea_hh__

#include "BinContainerArea.hh"

class GlyphWrapperArea : public BinContainerArea
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


