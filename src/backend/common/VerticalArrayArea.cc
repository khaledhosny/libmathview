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

#include <cassert>

#include "AreaId.hh"
#include "Point.hh"
#include "VerticalArrayArea.hh"

VerticalArrayArea::VerticalArrayArea(const std::vector<AreaRef>& children, AreaIndex r)
  : LinearContainerArea(children), refArea(r)
{
  assert(content.size() > 0);
  assert(refArea >= 0 && refArea < content.size());
}

// unsigned
// VerticalArrayArea::flattenAux(std::vector<AreaRef>& dest, const std::vector<AreaRef>& source, unsigned refArea)
// {
//   for (std::vector<AreaRef>::const_iterator p = source.begin();
//        p != source.end();
//        p++)
//     {
//       AreaRef flattened = (*p)->flatten();
//       if (SmartPtr<const VerticalArrayArea> harea = smart_cast<const VerticalArrayArea>(flattened))
// 	flattenAux(dest, harea->content);
//       else
// 	dest.push_back(flattened);
//     }
// }

AreaRef
VerticalArrayArea::flatten(void) const
{
  // FIXME
//   std::vector<AreaRef> newContent(content.size());
//   unsigned newRefArea = flattenAux(newContent, content, refArea);
//   if (newContent != content)
//     return clone(newContent, newRefArea);
//   else
    return this;
}

BoundingBox
VerticalArrayArea::box() const
{
  BoundingBox bbox = content[refArea]->box();
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      const AreaIndex i = p - content.begin();
      if (i < refArea)
	bbox.over((*p)->box());
      else if (i > refArea)
	bbox.under((*p)->box());
    }
  return bbox;
}

scaled
VerticalArrayArea::prepareChildBoxes(std::vector<BoundingBox>& box) const
{
  box.reserve(content.size());
  scaled depth = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      const AreaIndex i = p - content.begin();
      box.push_back((*p)->box());
      if (BoundingBox backBox = box.back())
	if (i < refArea)
	  depth += backBox.verticalExtent();
	else if (i == refArea)
	  depth += backBox.depth;
    }

  return depth;
}

void
VerticalArrayArea::render(class RenderingContext& context, const scaled& x, const scaled& y0) const
{
  std::vector<BoundingBox> box;
  scaled depth = prepareChildBoxes(box);

  scaled y = y0 - depth;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      const AreaIndex i = p - content.begin();
      if (box[i]) y += box[i].depth;
      (*p)->render(context, x, y);
      if (box[i]) y += box[i].height;
    }  
}

void
VerticalArrayArea::strength(int& w, int& h, int& d) const
{
  w = h = d = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      int pw, ph, pd;
      (*p)->strength(pw, ph, pd);
      w = std::max(w, pw);
      if (p - content.begin() < refArea)
	d += ph + pd;
      else if (p - content.begin() > refArea)
	h += ph + pd;
      else
	{
	  d += pd;
	  h += ph;
	}
    }
}

AreaRef
VerticalArrayArea::fit(const scaled& width, const scaled& height, const scaled& depth) const
{
  int sw, sh, sd;
  strength(sw, sh, sd);
  BoundingBox box0 = box();

  scaled aheight = box0 ? std::max(scaled::zero(), height - box0.height) : scaled::zero();
  scaled adepth = box0 ? std::max(scaled::zero(), depth - box0.depth) : scaled::zero();

  std::vector<AreaRef> newContent;
  newContent.reserve(content.size());
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      //std::cout << "fit vertical " << (p - content.begin()) << " width = " << width.toInt() << std::endl;

      int pw, ph, pd;
      (*p)->strength(pw, ph, pd);
      BoundingBox pbox = (*p)->box();

      scaled pheight = pbox ? pbox.height : scaled::zero();
      scaled pdepth = pbox ? pbox.depth : scaled::zero();

      if (p - content.begin() < refArea && sd > 0)
	{
	  pheight += (adepth * ph) / sd;
	  pdepth += (adepth * pd) / sd;
	}
      else if (p - content.begin() > refArea && sh > 0)
	{
	  pheight += (aheight * ph) / sh;
	  pdepth += (aheight * pd) / sh;
	}
      else if (p - content.begin() == refArea && sh + sd > 0)
	{
	  if (sh > 0) pheight += (aheight * ph) / sh;
	  if (sd > 0) pdepth += (adepth * pd) / sd;
	}

      newContent.push_back((*p)->fit(width, pheight, pdepth));
    }

  if (newContent == content)
    return this;
  else
    return clone(newContent);
}

bool
VerticalArrayArea::searchByCoords(AreaId& id, const scaled& x, const scaled& y) const
{
  std::vector<BoundingBox> box;
  scaled depth = prepareChildBoxes(box);

  scaled offset = -depth;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      const AreaIndex i = p - content.begin();
      offset += box[i].depth;
      id.append(i, *p, scaled::zero(), offset);
      if ((*p)->searchByCoords(id, x, y - offset)) return true;
      id.pop_back();
      offset += box[i].height;
    }  

  return false;
}

bool
VerticalArrayArea::searchByIndex(AreaId& id, CharIndex index) const
{
  for (std::vector<AreaRef>::const_reverse_iterator p = content.rbegin(); p != content.rend(); p++)
    {
      id.append(content.size() - (p - content.rbegin()) - 1, *p);
      if ((*p)->searchByIndex(id, index)) return true;
      id.pop_back();
      index -= (*p)->length();
    }
  return false;
}

void
VerticalArrayArea::origin(AreaIndex i, Point& point) const
{
  assert(i >= 0 && i < content.size());
  if (i < refArea)
    {
      if (BoundingBox idBox = content[i]->box())
	point.y -= idBox.height;
      if (BoundingBox refBox = content[refArea]->box())
	point.y -= refBox.depth;
      for (std::vector<AreaRef>::const_iterator p = content.begin() + i + 1;
	   p != content.begin() + refArea;
	   p++)
	if (BoundingBox b = (*p)->box())
	  point.y -= b.verticalExtent();
    }
  else if (i > refArea)
    {
      if (BoundingBox refBox = content[refArea]->box())
	point.y += refBox.height;
      if (BoundingBox idBox = content[i]->box())
	point.y += idBox.depth;
      for (std::vector<AreaRef>::const_iterator p = content.begin() + refArea + 1;
	   p != content.begin() + i;
	   p++)
	if (BoundingBox b = (*p)->box())
	  point.y += b.verticalExtent();
    }
}

CharIndex
VerticalArrayArea::lengthTo(AreaIndex i) const
{
  assert(i >= 0 && i < content.size());
  CharIndex length = 0;
  for (std::vector<AreaRef>::const_reverse_iterator p = content.rbegin();
       p != content.rbegin() + i;
       p++)
    length += (*p)->length();
  return length;
}
