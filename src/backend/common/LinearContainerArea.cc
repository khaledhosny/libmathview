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

#include "AreaId.hh"
#include "LinearContainerArea.hh"
#include "GlyphStringArea.hh"
#include "GlyphArea.hh"

void
LinearContainerArea::render(class RenderingContext& context, const scaled& x, const scaled& y) const
{
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    (*p)->render(context, x, y);
}

bool
LinearContainerArea::searchByArea(AreaId& id, const AreaRef& area) const
{
  if (this == area)
    return true;
  else
    {
      for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.end(); p++)
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
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.end(); p++)
    {
      id.append(p - content.begin(), *p);
      if ((*p)->searchByIndex(id, index)) return true;
      id.pop_back();
      index -= (*p)->length();
    }
  return false;
}

#if 0
AreaRef
LinearContainerArea::replace(const ReplacementContext& context) const
{
  if (AreaRef newArea = context.get())
    return newArea;
  else
    {
      std::vector<AreaRef> newContent;
      newContent.reserve(content.size());
      for (std::vector<AreaRef>::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	{
	  ReplacementContext newContext(context, p - content.begin());
	  newContent.push_back(content[p - content.begin()]->replace(newContext));
	}
      if (content == newContent)
	return this;
      else
	{
	  SmartPtr<Area> thisCloned = this->clone();
	  SmartPtr<LinearContainerArea> newArea = smart_cast<LinearContainerArea>(thisCloned);
	  assert(newArea);
	  newArea->content = newContent;
	  return newArea;
	}
    }
}
#endif

scaled
LinearContainerArea::leftEdge(void) const
{
  scaled edge = scaled::max();
  for (std::vector< AreaRef >::const_iterator p = content.begin();
       p != content.end();
       p++)
    edge = std::min(edge, (*p)->leftEdge());
  return edge;
}

scaled
LinearContainerArea::rightEdge(void) const
{
  scaled edge = scaled::min();
  for (std::vector< AreaRef >::const_iterator p = content.begin();
       p != content.end();
       p++)
    edge = std::max(edge, (*p)->rightEdge());
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
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.begin() + i; p++)
    length += (*p)->length();
  return length;
}

SmartPtr<const GlyphStringArea>
LinearContainerArea::getGlyphStringArea() const
{
  SmartPtr<const GlyphStringArea> ptr = NULL;
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.end(); p++)
  {
    if (!ptr && (((*p)->getGlyphStringArea())))  
      ptr = (*p)->getGlyphStringArea(); 	
    else if (ptr && (((*p)->getGlyphStringArea())))
      { ptr = NULL;
	return ptr;
      }
  }

  return ptr;
}

SmartPtr<const GlyphArea> 
LinearContainerArea::getGlyphArea() const
{
  SmartPtr<const GlyphArea> ptr = NULL;
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.end(); p++)
  {
    if (!ptr && (((*p)->getGlyphArea())))
      ptr = (*p)->getGlyphArea();
    else if (ptr && (((*p)->getGlyphArea()))) 
      { ptr = NULL;
	return ptr;
      }
 }

  return ptr;
}
