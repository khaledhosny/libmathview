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

#include "AreaId.hh"
#include "LinearContainerArea.hh"

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
LinearContainerArea::searchByIndex(AreaId& id, int index) const
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
LinearContainerArea::node(unsigned i) const
{
  assert(i < content.size());
  return content[i];
}

int
LinearContainerArea::lengthTo(unsigned i) const
{
  assert(i < content.size());
  int length = 0;
  for (std::vector<AreaRef>::const_iterator p = content.begin(); p != content.begin() + i; p++)
    length += (*p)->length();
  return length;
}
