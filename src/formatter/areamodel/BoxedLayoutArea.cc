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

#include <config.h>

#include <vector>
#include <sstream>

#include "BoundingBoxAux.hh"
#include "AreaIdFactory.hh"
#include "BoxedLayoutArea.hh"
#include "ReplacementContext.hh"

SmartPtr<Area>
BoxedLayoutArea::clone() const
{
  return clone(bbox, content);
}

SmartPtr<Area>
BoxedLayoutArea::clone(const BoundingBox& newBox, const std::vector<XYArea>& newContent) const
{
  return new BoxedLayoutArea(newBox, newContent);
}

void
BoxedLayoutArea::render(class RenderingContext& context, const scaled& x, const scaled& y) const
{
  for (std::vector<XYArea>::const_iterator p = content.begin();
       p != content.end();
       p++)
    p->area->render(context, x + p->dx, y + p->dy);
}

bool
BoxedLayoutArea::find(class SearchingContext& context, const scaled& x, const scaled& y) const
{
  for (std::vector<XYArea>::const_iterator p = content.begin();
       p != content.end();
       p++)
    if (p->area->find(context, x + p->dx, y + p->dy))
      return true;

  return false;
}

bool
BoxedLayoutArea::idOf(const AreaRef& area, AreaIdFactory& factory) const
{
  if (this == area)
    return true;
  else
    {
      for (std::vector<XYArea>::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	{
	  factory.append(p - content.begin());
	  if (p->area->idOf(area, factory))
	    return true;
	  factory.backtrack();
	}
      return false;
    }
}
AreaRef
BoxedLayoutArea::getChild(unsigned i) const
{
  if (i - 1 < content.size())
    return content[i - 1].area;
  else
    throw InvalidIndex();
}

AreaRef
BoxedLayoutArea::replace(const ReplacementContext& context) const
{
  if (AreaRef newArea = context.get())
    return newArea;
  else
    {
      std::vector<XYArea> newContent;
      newContent.reserve(content.size());
      for (std::vector<XYArea>::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	{
	  ReplacementContext newContext(context, p - content.begin());
	  newContent.push_back(XYArea(p->dx, p->dy, content[p - content.begin()].area->replace(newContext)));
	}

      if (content == newContent)
	return this;
      else
	return clone(bbox, newContent);
    }
}

scaled
BoxedLayoutArea::leftEdge() const
{
  return scaled::min(); // FIXME
}

scaled
BoxedLayoutArea::rightEdge() const
{
  return scaled::min(); // FIXME
}

void
BoxedLayoutArea::strength(int& w, int& h, int& d) const
{
  w = h = d = 0;
}

AreaRef
BoxedLayoutArea::fit(const scaled& width, const scaled& height, const scaled& depth) const
{
  std::vector<XYArea> newContent;
  newContent.reserve(content.size());
  for (std::vector<XYArea>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      BoundingBox pbox = p->area->box();
      newContent.push_back(XYArea(p->dx, p->dy, p->area->fit(pbox.width, pbox.height, pbox.depth)));
    }

  if (newContent == content)
    return this;
  else
    return clone(bbox, newContent);
}

DOM::Element
BoxedLayoutArea::dump(const DOM::Document& doc) const
{
  DOM::Element el = doc.createElementNS(STD_AREAMODEL_NAMESPACE_URI, "a:boxed-layout");
  std::ostringstream os;
  os << bbox;
  el.setAttribute("box", os.str());

  for (std::vector<XYArea>::const_iterator p = content.begin();
       p != content.end();
       p++)
    el.appendChild(p->area->dump(doc));

  return el;
}

std::pair<scaled,scaled>
BoxedLayoutArea::origin(AreaId::const_iterator id, AreaId::const_iterator empty,
			const scaled& x, const scaled& y) const
{
  assert(false); // IS THIS MEANT TO BE IMPLEMENTED SERIOUSLY?
  if (id == empty)
    return std::make_pair(x, y);
  else
    throw InvalidId();
}

scaled
BoxedLayoutArea::leftSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  throw NotAllowed();
}

scaled
BoxedLayoutArea::rightSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  throw NotAllowed();
}

AreaRef
BoxedLayoutArea::node(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  throw InvalidId();
}
