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

#include "HorizontalArrayArea.hh"

SmartPtr<HorizontalArrayArea>
HorizontalArrayArea::create(const std::vector<AreaRef>& children)
{
  return new HorizontalArrayArea(children);
}

SmartPtr<Area>
HorizontalArrayArea::clone() const
{
  return new HorizontalArrayArea(content);
}

SmartPtr<Area>
HorizontalArrayArea::clone(const std::vector<AreaRef>& children) const
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
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    bbox.append((*p)->box());
  return bbox;
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
      d += (*p)->box().width;
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
      d += (*p)->box().width;
    }
  return edge;
}

scaled
HorizontalArrayArea::origin(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    return 0;
  else if (*id >= content.size())
    throw InvalidId();
  else
    {
      scaled d = 0;
      for (std::vector<AreaRef>::const_iterator p = content.begin();
	   p != content.begin() + *id;
	   p++)
	d += (*p)->box().width;
      return d + content[*id]->origin(id + 1, empty);
    }
}

scaled
HorizontalArrayArea::leftSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    throw NotAllowed();
  else if (*id >= content.size())
    throw InvalidId();
  else if (id + 1 == empty)
    {
      unsigned l = *id;
      scaled redge = scaled::min();
      while (redge == scaled::min() && l > 0)
	redge = content[l--]->rightEdge();
      if (redge != scaled::min())
	return origin(id, empty) + redge;
      else
	return 0;
    }	
  else
    return content[*id]->leftSide(id + 1, empty);
}

scaled
HorizontalArrayArea::rightSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    throw NotAllowed();
  else if (*id >= content.size())
    throw InvalidId();
  else if (id + 1 == empty)
    {
      unsigned r = *id;
      scaled ledge = scaled::max();
      while (ledge == scaled::max() && r + 1 < content.size())
	ledge = content[r++]->leftEdge();
      if (ledge != scaled::max())
	return origin(id, empty) + ledge;
      else
	return box().width;
    }	
  else
    return content[*id]->rightSide(id + 1, empty);
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

DOM::Element
HorizontalArrayArea::dump(const DOM::Document& doc) const
{
  DOM::Element el = doc.createElementNS(STD_AREAMODEL_NAMESPACE_URI, "a:h-array");
  return dumpAux(doc, el);
}
