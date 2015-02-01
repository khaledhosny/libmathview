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
#include "OverlapArrayArea.hh"

AreaRef
OverlapArrayArea::clone(const std::vector<AreaRef>& content) const
{
  return create(content);
}

void
OverlapArrayArea::flattenAux(std::vector<AreaRef>& dest, const std::vector<AreaRef>& source)
{
  for (const auto & elem : source)
    {
      AreaRef flattened = elem->flatten();
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
  for (const auto & elem : content)
    bbox.overlap(elem->box());
  return bbox;
}

void
OverlapArrayArea::strength(int& w, int& h, int& d) const
{
  w = h = d = 0;
  for (const auto & elem : content)
    {
      int pw, ph, pd;
      elem->strength(pw, ph, pd);
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
  for (const auto & elem : content)
    newContent.push_back(elem->fit(width, height, depth));
  if (newContent == content)
    return this;
  else
    return clone(newContent);
}

bool
OverlapArrayArea::searchByCoords(AreaId& id, const scaled& x, const scaled& y) const
{
  // the last child area is also the last painted area. This means that
  // it may overlap some previous child area. Hence, if the user sees
  // something, we must first check the last painted area because it is
  // what the user sees for sure
  for (auto p = content.rbegin(); p != content.rend(); p++)
    {
      id.append(content.size() - (p - content.rbegin()) - 1, *p, scaled::zero(), scaled::zero());
      if ((*p)->searchByCoords(id, x, y)) return true;
      id.pop_back();
    }
  return false;
}

void
OverlapArrayArea::origin(AreaIndex i, struct Point&) const
{
  assert(i >= 0 && i < content.size());
}
