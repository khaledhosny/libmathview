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

#ifndef __BoxedLayoutArea_hh__
#define __BoxedLayoutArea_hh__

#include "ContainerArea.hh"

class BoxedLayoutArea : public ContainerArea
{
public:
  struct XYArea
  {
    XYArea(const scaled& dx0, const scaled& dy0, const AreaRef& area0)
      : dx(dx0), dy(dy0), area(area0) { }

    bool operator==(const XYArea& o) const
    { return dx == o.dx && dy == o.dy && area == o.area; }

    scaled dx;
    scaled dy;
    AreaRef area;
  };

protected:
  BoxedLayoutArea(const BoundingBox& b, const std::vector<XYArea>& c) : bbox(b), content(c) { }
  virtual ~BoxedLayoutArea() { }

public:
  static SmartPtr<BoxedLayoutArea> create(const BoundingBox& b, const std::vector<XYArea>& c)
  { return new BoxedLayoutArea(b, c); }
  virtual SmartPtr<Area> clone(void) const;
  virtual SmartPtr<Area> clone(const BoundingBox&, const std::vector<XYArea>&) const;

  virtual BoundingBox box(void) const { return bbox; }
  virtual void strength(int&, int&, int&) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual AreaRef replace(const class ReplacementContext&) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual DOM::Element dump(const DOM::Document&) const;

  AreaRef getChild(unsigned) const;
  unsigned getSize(void) const { return content.size(); }

  class InvalidIndex { };

protected:
  virtual bool idOf(const AreaRef&, AreaIdFactory&) const;
  virtual AreaRef node(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual scaled origin(AreaId::const_iterator id, AreaId::const_iterator empty) const;
  virtual scaled leftSide(AreaId::const_iterator id, AreaId::const_iterator empty) const;
  virtual scaled rightSide(AreaId::const_iterator id, AreaId::const_iterator empty) const;

  BoundingBox bbox;
  std::vector<XYArea> content;
};

#endif // __BoxedLayoutArea_hh__
