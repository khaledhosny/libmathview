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

#include "AreaIdFactory.hh"
#include "BinContainerArea.hh"
#include "ReplacementContext.hh"

BoundingBox
BinContainerArea::box() const
{
  return child->box();
}

void
BinContainerArea::render(class RenderingContext& context, const scaled& x, const scaled& y) const
{
  child->render(context, x, y);
}

scaled
BinContainerArea::leftEdge() const
{
  return child->leftEdge();
}

scaled
BinContainerArea::rightEdge() const
{
  return child->rightEdge();
}

scaled
BinContainerArea::origin(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    return 0;
  else if (*id == 0)
    return child->origin(id + 1, empty);
  else
    throw InvalidId();
}

AreaRef
BinContainerArea::replace(const ReplacementContext& context) const
{
  if (AreaRef newArea = context.get())
    return newArea;
  else
    {
      ReplacementContext newContext(context, 0);
      AreaRef newChild = child->replace(newContext);
      if (child == newChild)
	return this;
      else
	{
	  SmartPtr<Area> thisCloned = this->clone();
	  SmartPtr<BinContainerArea> newArea = smart_cast<BinContainerArea>(thisCloned);
	  newArea->child = newChild;
	  return newArea;
	}
    }
}

AreaRef
BinContainerArea::fit(const scaled& w, const scaled& h, const scaled& d) const
{
  return replace(ReplacementContext(AreaId(0), child->fit(w, h, d)));
}

AreaRef
BinContainerArea::node(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    return this;
  else if (*id == 0)
    return child->node(id + 1, empty);
  else
    throw InvalidId();
}

scaled
BinContainerArea::leftSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    throw NotAllowed();
  else if (*id != 0)
    throw InvalidId();
  else
    return child->leftSide(id + 1, empty);
}

scaled
BinContainerArea::rightSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    throw NotAllowed();
  else if (*id != 0)
    throw InvalidId();
  else
    return child->rightSide(id + 1, empty);
}

bool
BinContainerArea::idOf(const AreaRef& area, AreaIdFactory& factory) const
{
  if (area == this)
    return true;
  else
    {
      factory.append(0);
      if (child->idOf(area, factory)) return true;
      factory.backtrack();
      return false;
    }
}

void
BinContainerArea::strength(int& w, int& h, int& d) const
{
  child->strength(w, h, d);
}

DOM::Element
BinContainerArea::dumpAux(const DOM::Document& doc, const DOM::Element& el) const
{
  el.appendChild(child->dump(doc));
  return el;
}
