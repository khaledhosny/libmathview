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

#include <cassert>

#include <sstream>

#include "VerticalArrayArea.hh"

VerticalArrayArea::VerticalArrayArea(const std::vector<AreaRef>& children, unsigned r)
  : LinearContainerArea(children), refArea(r)
{
  assert(content.size() > 0);
  assert(refArea < content.size());
}

SmartPtr<Area>
VerticalArrayArea::clone() const
{
  return new VerticalArrayArea(content, refArea);
}

SmartPtr<Area>
VerticalArrayArea::clone(const std::vector<AreaRef>& children) const
{
  assert(children.size() == content.size());
  return new VerticalArrayArea(children, refArea);
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
      const int i = p - content.begin();
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
      const int i = p - content.begin();
      box.push_back((*p)->box());
      if (i < refArea)
	depth += box.back().verticalExtent();
      else if (i == refArea)
	depth += box.back().depth;
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
      const int i = p - content.begin();
      y += box[i].depth;
      (*p)->render(context, x, y);
      y += box[i].height;
    }  
}

bool
VerticalArrayArea::find(class SearchingContext& context, const scaled& x, const scaled& y0) const
{
  std::vector<BoundingBox> box;
  scaled depth = prepareChildBoxes(box);

  scaled y = y0 - depth;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      const int i = p - content.begin();
      y += box[i].depth;
      if ((*p)->find(context, x, y))
	return true;
      y += box[i].height;
    }  

  return false;
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

  scaled aheight = std::max(scaled::zero(), height - box0.height);
  scaled adepth = std::max(scaled::zero(), depth - box0.depth);

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

      scaled pheight = pbox.height;
      scaled pdepth = pbox.depth;

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

  //std::cout << "fit vertical done *** same? " << (newContent == content) << std::endl;

  if (newContent == content)
    return this;
  else
    return clone(newContent);
}

DOM::Element
VerticalArrayArea::dump(const DOM::Document& doc) const
{
  DOM::Element el = doc.createElementNS(STD_AREAMODEL_NAMESPACE_URI, "a:v-array");
  std::ostringstream os;
  os << refArea;
  el.setAttribute("ref", os.str());
  return dumpAux(doc, el);
}
