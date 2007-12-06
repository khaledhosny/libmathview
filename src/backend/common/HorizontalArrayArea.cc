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

#include <config.h>

#include "AreaId.hh"
#include "Point.hh"
#include "HorizontalArrayArea.hh"

SmartPtr<HorizontalArrayArea>
HorizontalArrayArea::create(const std::vector<AreaRef>& children)
{
  return new HorizontalArrayArea(children);
}

void
HorizontalArrayArea::flattenAux(std::vector<AreaRef>& dest, const std::vector<AreaRef>& source)
{
  for (std::vector<AreaRef>::const_iterator p = source.begin();
       p != source.end();
       p++)
    {
      AreaRef flattened = (*p)->flatten();
      if (SmartPtr<const HorizontalArrayArea> harea = smart_cast<const HorizontalArrayArea>(flattened))
	flattenAux(dest, harea->content);
      else
	dest.push_back(flattened);
    }
}

AreaRef
HorizontalArrayArea::flatten(void) const
{
  std::vector<AreaRef> newContent(content.size());
  flattenAux(newContent, content);
  if (newContent != content)
    return clone(newContent);
  else
    return this;
}

BoundingBox
HorizontalArrayArea::box() const
{
  BoundingBox bbox;
  scaled step = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      bbox.append((*p)->box());
      const scaled childStep = (*p)->getStep();
      bbox.height -= childStep;
      bbox.depth += childStep;
      step += childStep;
    }
  // must restore the baseline
  bbox.height += step;
  bbox.depth -= step;

  return bbox;
}

void
HorizontalArrayArea::render(class RenderingContext& context, const scaled& x0, const scaled& y0) const
{
  scaled x = x0;
  scaled y = y0;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      (*p)->render(context, x, y);
      x += (*p)->box().horizontalExtent();
      y += (*p)->getStep();
    }
}

bool
HorizontalArrayArea::searchByCoords(AreaId& id, const scaled& x, const scaled& y0) const
{
  scaled offset;
  scaled y = y0;
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.end(); p++)
    {
      id.append(p - content.begin(), *p, offset, scaled::zero());
      if ((*p)->searchByCoords(id, x - offset, y)) return true;
      id.pop_back();
      offset += (*p)->box().horizontalExtent();
      y += (*p)->getStep();
    }

  return false;
}

scaled
HorizontalArrayArea::leftEdge() const
{
  scaled edge = scaled::max();
  scaled d = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      scaled pedge = (*p)->leftEdge();
      if (pedge < scaled::max()) edge = std::min(edge, d + pedge);
      d += (*p)->box().horizontalExtent();
    }
  return edge;
}

scaled
HorizontalArrayArea::rightEdge() const
{
  scaled edge = scaled::min();
  scaled d = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      scaled pedge = (*p)->rightEdge();
      if (pedge > scaled::min()) edge = std::max(edge, d + pedge);
      d += (*p)->box().horizontalExtent();
    }
  return edge;
}

scaled
HorizontalArrayArea::leftSide(AreaIndex i) const
{
  assert(i >= 0 && i < content.size());

  AreaIndex l = i;
  scaled redge = scaled::min();
  while (redge == scaled::min() && l > 0)
    redge = content[l--]->rightEdge();

  return (redge != scaled::min()) ? originX(i) + redge : scaled::zero();
}

scaled
HorizontalArrayArea::rightSide(AreaIndex i) const
{
  assert(i >= 0 && i < content.size());

  AreaIndex r = i;
  scaled ledge = scaled::max();
  while (ledge == scaled::max() && r + 1 < content.size())
    ledge = content[r++]->leftEdge();

  return (ledge != scaled::max()) ? originX(i) + ledge : box().width;
}

void
HorizontalArrayArea::strength(int& w, int& h, int& d) const
{
  w = h = d = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      int pw;
      int ph;
      int pd;
      (*p)->strength(pw, ph, pd);
      w += pw;
      h = std::max(h, ph);
      d = std::max(d, pd);
    }
}

AreaRef
HorizontalArrayArea::fit(const scaled& width, const scaled& height, const scaled& depth) const
{
  int sw, sh, sd;
  strength(sw, sh, sd);
  BoundingBox box0 = box();

  std::vector<AreaRef> newContent;
  newContent.reserve(content.size());
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      int pw, ph, pd;
      (*p)->strength(pw, ph, pd);
      BoundingBox pbox = (*p)->box();

      if (sw == 0 || pw == 0)
	newContent.push_back((*p)->fit(pbox.width, height, depth));
      else
	{
	  scaled pwidth = (std::max(pbox.width, width - box0.width) * pw) / sw;
	  newContent.push_back((*p)->fit(pwidth, height, depth));
	}
    }

  if (newContent == content)
    return this;
  else
    return clone(newContent);
}

void
HorizontalArrayArea::origin(AreaIndex i, Point& point) const
{
  assert(i >= 0 && i < content.size());
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.begin() + i; p++)
    {
      point.x += (*p)->box().horizontalExtent();
      point.y += (*p)->getStep();
    }
}

scaled
HorizontalArrayArea::getStep() const
{
  scaled step = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.end(); p++)
    step += (*p)->getStep();
  return step;
}
