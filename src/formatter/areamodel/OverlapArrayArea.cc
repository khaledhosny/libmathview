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

#include "OverlapArrayArea.hh"

SmartPtr<Area>
OverlapArrayArea::clone() const
{
  return new OverlapArrayArea(content);
}

SmartPtr<Area>
OverlapArrayArea::clone(const std::vector<AreaRef>& children) const
{
  return new OverlapArrayArea(content);
}

void
OverlapArrayArea::flattenAux(std::vector<AreaRef>& dest, const std::vector<AreaRef>& source)
{
  for (std::vector<AreaRef>::const_iterator p = source.begin();
       p != source.end();
       p++)
    {
      AreaRef flattened = (*p)->flatten();
      if (SmartPtr<const OverlapArrayArea> harea = smart_cast<const OverlapArrayArea>(flattened))
	flattenAux(dest, harea->content);
      else
	dest.push_back(flattened);
    }
}

AreaRef
OverlapArrayArea::flatten(void) const
{
  std::vector<AreaRef> newContent(content.size());
  flattenAux(newContent, content);
  if (newContent != content)
    return clone(newContent);
  else
    return this;
}

BoundingBox
OverlapArrayArea::box() const
{
  BoundingBox bbox;
  for (std::vector< AreaRef >::const_iterator p = content.begin();
       p != content.end();
       p++)
    bbox.overlap((*p)->box());
  return bbox;
}

void
OverlapArrayArea::strength(int& w, int& h, int& d) const
{
  w = h = d = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      int pw, ph, pd;
      (*p)->strength(pw, ph, pd);
      w = std::max(w, pw);
      h = std::max(h, ph);
      d = std::max(d, pd);
    }
}

AreaRef
OverlapArrayArea::fit(const scaled& width, const scaled& height, const scaled& depth) const
{
  std::vector<AreaRef> newContent;
  newContent.reserve(content.size());
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    newContent.push_back((*p)->fit(width, height, depth));
  if (newContent == content)
    return this;
  else
    return clone(newContent);
}

DOM::Element
OverlapArrayArea::dump(const DOM::Document& doc) const
{
  DOM::Element el = doc.createElementNS(STD_AREAMODEL_NAMESPACE_URI, "a:o-array");
  return dumpAux(doc, el);
}
