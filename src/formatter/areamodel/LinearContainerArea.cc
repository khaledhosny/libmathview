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

#include "AreaIdFactory.hh"
#include "LinearContainerArea.hh"
#include "ReplacementContext.hh"

void
LinearContainerArea::render(class RenderingContext& context, const scaled& x, const scaled& y) const
{
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    (*p)->render(context, x, y);
}

bool
LinearContainerArea::find(SearchingContext& context, const scaled& x, const scaled& y) const
{
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    if ((*p)->find(context, x, y))
      return true;
  
  return false;
}

bool
LinearContainerArea::idOf(const AreaRef& area, AreaIdFactory& factory) const
{
  if (this == area)
    return true;
  else
    {
      for (std::vector<AreaRef>::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	{
	  factory.append(p - content.begin());
	  if ((*p)->idOf(area, factory))
	    return true;
	  factory.backtrack();
	}
      return false;
    }
}

AreaRef
LinearContainerArea::node(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    return this;
  else if (*id < content.size())
    return content[*id]->node(id + 1, empty);
  else
    throw InvalidId();
}

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

std::pair<scaled,scaled>
LinearContainerArea::origin(AreaId::const_iterator id, AreaId::const_iterator empty, const scaled& x, const scaled& y) const
{
  if (id == empty)
    return std::make_pair(x, y);
  else if (*id < content.size())
    return content[*id]->origin(id + 1, empty, x, y);
  else
    throw InvalidId();
}

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

scaled
LinearContainerArea::leftSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    throw NotAllowed();
  else if (*id >= content.size())
    throw InvalidId();
  else
    return content[*id]->leftSide(id + 1, empty);
}

scaled
LinearContainerArea::rightSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    throw NotAllowed();
  else if (*id > content.size())
    throw InvalidId();
  else
    return content[*id]->rightSide(id + 1, empty);
}

AreaRef
LinearContainerArea::getChild(unsigned i) const
{
  if (i - 1 < content.size())
    return content[i - 1];
  else
    throw InvalidIndex();
}

DOM::Element
LinearContainerArea::dumpAux(const DOM::Document& doc, const DOM::Element& el) const
{
  for (std::vector< AreaRef >::const_iterator p = content.begin();
       p != content.end();
       p++)
    el.appendChild((*p)->dump(doc));
  return el;
}
