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

#include "AreaIdFactory.hh"
#include "ReplacementContext.hh"
#include "SimpleArea.hh"

SmartPtr<Area>
SimpleArea::clone() const
{
  /* cloning a simple area makes no sense as cloning occurs only
   * when fitting an area, and simple areas fit into themselves.
   * There is also a technical difficulty in that cloning should
   * return a read-write area, but in this case returning this
   * would fail because of the const modifier of the method.
   */
  assert(false);
}

AreaRef
SimpleArea::fit(const scaled&, const scaled&, const scaled&) const
{
  return this;
}

AreaRef
SimpleArea::replace(const ReplacementContext& context) const
{
  if (AreaRef newArea = context.get())
    return newArea;
  else
    return this;
}

scaled
SimpleArea::origin(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    return 0;
  else
    throw InvalidId();
}

scaled
SimpleArea::leftSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  throw NotAllowed();
}

scaled
SimpleArea::rightSide(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  throw NotAllowed();
}

bool
SimpleArea::idOf(const AreaRef& area, AreaIdFactory& factory) const
{
  return area == this;
}

AreaRef
SimpleArea::node(AreaId::const_iterator id, AreaId::const_iterator empty) const
{
  if (id == empty)
    return this;
  else
    throw InvalidId();
}

void
SimpleArea::strength(int& w, int& h, int& d) const
{
  w = h = d = 0;
}

void
SimpleArea::render(RenderingContext&, const scaled&, const scaled&) const
{ }

