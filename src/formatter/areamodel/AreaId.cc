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

#include "AreaId.hh"

void
AreaId::append(int step)
{
  path.push_back(step);
}

void
AreaId::append(int step, const AreaRef& a)
{
  path.push_back(step);
  area.push_back(a);
}

void
AreaId::append(int step, const AreaRef& a, const scaled& x, const scaled& y)
{
  append(step, a);
  origin.push_back(std::make_pair(x, y));
}

void
AreaId::pop_back()
{
  assert(!path.empty());
  path.pop_back();
  while (area.size() > path.size()) area.pop_back();
  while (origin.size() > path.size()) origin.pop_back();
}

AreaRef
AreaId::getArea(int index) const
{
  if (index == 0) return root;
  else if (index > 0)
    {
      assert(index <= area.size());
      return area[index - 1];
    }
  else
    {
      assert(-index <= area.size());
      return area[area.size() + index];
    }
}

void
AreaId::getOrigin(scaled& x, scaled& y, int index) const
{
  validateOrigins();
  if (index >= 0)
    getOriginAux(index, x, y);
  else if (!origin.empty())
    getOriginAux(origin.size() + index - 1, x, y);
  else
    {
      assert(index == -1 || index == 0);
      x = y = scaled::zero();
    }
}

void
AreaId::getOriginAux(int index, scaled& x, scaled& y) const
{
  assert(index >= 0 && index < origin.size());
  x = y = scaled::zero();
  for (OriginVector::const_iterator p = origin.begin(); index > 0 && p != origin.end(); p++, index--)
    {
      x += (*p).first;
      y += (*p).second;
    }
}

void
AreaId::validateAreas() const
{
  AreaRef prev = root;
  while (area.size() < path.size())
    {
      area.push_back(prev->node(path[area.size()]));
      prev = area[area.size() - 1];
    }
}

void
AreaId::validateOrigins() const
{
  validateAreas();

  AreaRef prev = root;
  while (origin.size() < area.size())
    {
      std::pair<scaled,scaled> o;
      prev->origin(path[origin.size()], o.first, o.second);
      origin.push_back(o);
      prev = area[origin.size() - 1];
    }
}

