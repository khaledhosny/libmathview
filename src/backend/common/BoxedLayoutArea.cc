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

#include <config.h>

#include <vector>

#include "AreaId.hh"
#include "Point.hh"
#include "BoxedLayoutArea.hh"

void
BoxedLayoutArea::render(class RenderingContext& context, const scaled& x, const scaled& y) const
{
  for (std::vector<XYArea>::const_iterator p = content.begin();
       p != content.end();
       p++)
    p->area->render(context, x + p->dx, y + p->dy);
}

bool
BoxedLayoutArea::searchByArea(AreaId& id, const AreaRef& area) const
{
  if (this == area)
    return true;
  else
    {
      for (std::vector<XYArea>::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	{
	  id.append(p - content.begin(), p->area);
	  if (p->area->searchByArea(id, area))
	    return true;
	  id.pop_back();
	}
      return false;
    }
}

bool
BoxedLayoutArea::searchByCoords(AreaId& id, const scaled& x, const scaled& y) const
{
  // See OverlapArrayArea for the reason why the search must be done
  // from the last to the first area
  for (std::vector<XYArea>::const_reverse_iterator p = content.rbegin();
       p != content.rend();
       p++)
    {
      id.append(content.size() - (p - content.rbegin()) - 1, p->area, p->dx, p->dy);
      if (p->area->searchByCoords(id, x - p->dx, y - p->dy)) return true;
      id.pop_back();
    }
  return false;
}

bool
BoxedLayoutArea::searchByIndex(AreaId& id, CharIndex index) const
{
  for (std::vector<XYArea>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      id.append(p - content.begin(), p->area);
      if (p->area->searchByIndex(id, index))
	return true;
      id.pop_back();
      index -= p->area->length();
    }
  return false;
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
    return clone(newContent);
}

AreaRef
BoxedLayoutArea::node(AreaIndex i) const
{
  assert(i >= 0 && i < content.size());
  return content[i].area;
}

void
BoxedLayoutArea::origin(AreaIndex i, Point& p) const
{
  assert(i >= 0 && i < content.size());
  p.x += content[i].dx;
  p.y += content[i].dy;
}

CharIndex
BoxedLayoutArea::lengthTo(AreaIndex i) const
{
  assert(i >= 0 && i < content.size());
  CharIndex length = 0;
  for (std::vector<XYArea>::const_iterator p = content.begin(); p != content.begin() + i; p++)
    length += p->area->length();
  return length;
}
