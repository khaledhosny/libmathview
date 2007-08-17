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

#ifndef __BoxedLayoutArea_hh__
#define __BoxedLayoutArea_hh__

#include <vector>

#include "ContainerArea.hh"

class GMV_MathView_EXPORT BoxedLayoutArea : public ContainerArea
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
  virtual AreaRef clone(const std::vector<XYArea>& c) const { return create(bbox, c); }

  virtual BoundingBox box(void) const { return bbox; }
  virtual void strength(int&, int&, int&) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual AreaIndex size(void) const { return content.size(); }
  virtual AreaRef node(AreaIndex) const;
  virtual void origin(AreaIndex, class Point&) const;
  virtual CharIndex lengthTo(AreaIndex) const;
  //virtual AreaRef replace(unsigned, const AreaRef&) const;

  virtual bool searchByArea(class AreaId&, const AreaRef&) const;
  virtual bool searchByCoords(class AreaId&, const scaled&, const scaled&) const;
  virtual bool searchByIndex(class AreaId&, CharIndex) const;

protected:
  BoundingBox bbox;
  std::vector<XYArea> content;
};

#endif // __BoxedLayoutArea_hh__
