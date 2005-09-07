// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef __BinContainerArea_hh__
#define __BinContainerArea_hh__

#include "ContainerArea.hh"

class GMV_EXPORT BinContainerArea : public ContainerArea
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

  AreaRef getChild(void) const { return child; }

private:
  AreaRef child;
};

#endif // __BinContainerArea_hh__
