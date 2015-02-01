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

#include "AreaId.hh"
#include "LinearContainerArea.hh"
#include "GlyphStringArea.hh"
#include "GlyphArea.hh"

void
LinearContainerArea::render(class RenderingContext& context, const scaled& x, const scaled& y) const
{
  for (const auto & elem : content)
    elem->render(context, x, y);
}

bool
LinearContainerArea::searchByArea(AreaId& id, const AreaRef& area) const
{
  if (this == area)
    return true;
  else
    {
      for (auto p = content.begin(); p != content.end(); p++)
	{
	  id.append(p - content.begin(), *p);
	  if ((*p)->searchByArea(id, area)) return true;
	  id.pop_back();
	}
      return false;
    }
}

bool
LinearContainerArea::searchByIndex(AreaId& id, CharIndex index) const
{
  for (auto p = content.begin(); p != content.end(); p++)
    {
      id.append(p - content.begin(), *p);
      if ((*p)->searchByIndex(id, index)) return true;
      id.pop_back();
      index -= (*p)->length();
    }
  return false;
}

scaled
LinearContainerArea::leftEdge(void) const
{
  scaled edge = scaled::max();
  for (const auto & elem : content)
    edge = std::min(edge, elem->leftEdge());
  return edge;
}

scaled
LinearContainerArea::rightEdge(void) const
{
  scaled edge = scaled::min();
  for (const auto & elem : content)
    edge = std::max(edge, elem->rightEdge());
  return edge;
}

AreaRef
LinearContainerArea::node(AreaIndex i) const
{
  assert(i >= 0 && i < content.size());
  return content[i];
}

CharIndex
LinearContainerArea::lengthTo(AreaIndex i) const
{
  assert(i >= 0 && i < content.size());
  CharIndex length = 0;
  for (auto p = content.begin(); p != content.begin() + i; p++)
    length += (*p)->length();
  return length;
}

SmartPtr<const GlyphStringArea>
LinearContainerArea::getGlyphStringArea() const
{
  SmartPtr<const GlyphStringArea> ptr = nullptr;
  for (const auto & elem : content)
  {
    if (!ptr && elem->getGlyphStringArea())
      ptr = elem->getGlyphStringArea();
    else if (ptr && elem->getGlyphStringArea())
      { ptr = nullptr;
	return ptr;
      }
  }

  return ptr;
}

SmartPtr<const GlyphArea> 
LinearContainerArea::getGlyphArea() const
{
  SmartPtr<const GlyphArea> ptr = nullptr;
  for (const auto & elem : content)
  {
    if (!ptr && elem->getGlyphArea())
      ptr = elem->getGlyphArea();
    else if (ptr && elem->getGlyphArea())
      { ptr = nullptr;
	return ptr;
      }
 }

  return ptr;
}
