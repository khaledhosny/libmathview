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

class BinContainerArea : public ContainerArea
{
protected:
  BinContainerArea(const AreaRef& area) : child(area) { }
  virtual ~BinContainerArea() { }

public:
  virtual BoundingBox box(void) const;
  virtual void        render(class RenderingContext& context, const scaled& x, const scaled& y) const;

  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;

  virtual AreaRef replace(const ReplacementContext&) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;

  AreaRef getChild(void) const { return child; }

  virtual bool idOf(const AreaRef&, AreaIdFactory&) const;
  virtual AreaRef node(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual scaled hOffset(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual scaled vOffset(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual scaled leftSide(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual scaled rightSide(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual void strength(int&, int&, int&) const;

protected:
  DOM::Element dumpAux(const DOM::Document&, const DOM::Element&) const;

private:
  AreaRef child;
};

#endif // __BinContainerArea_hh__
