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
AreaId::append(AreaIndex step)
{
  pathV.push_back(step);
}

void
AreaId::append(AreaIndex step, const AreaRef& a)
{
  assert(pathV.size() == areaV.size());
  pathV.push_back(step);
  areaV.push_back(a);
}

void
AreaId::append(AreaIndex step, const AreaRef& a, const scaled& x, const scaled& y)
{
  assert(pathV.size() == originV.size());
  append(step, a);
  originV.push_back(Point(x, y));
}

void
AreaId::pop_back()
{
  assert(!pathV.empty());
  pathV.pop_back();
  while (areaV.size() > pathV.size()) areaV.pop_back();
  while (originV.size() > pathV.size()) originV.pop_back();
  while (lengthV.size() > pathV.size()) lengthV.pop_back();
}

AreaRef
AreaId::getArea(int index) const
{
  validateAreas();

  if (index == 0)
    return root;
  else if (index > 0)
    {
      assert(index <= areaV.size());
      return areaV[index - 1];
    }
  else
    {
      assert(-index <= areaV.size());
      return areaV[areaV.size() + index];
    }
}

void
AreaId::accumulateOrigin(Point& p, int begin, int end) const
{
  validateOrigins();

  const int beginA = (begin >= 0) ? begin : originV.size() + begin + 1;
  const int endA = (end >= 0) ? end : originV.size() + end + 1;
  assert(beginA >= 0 && beginA <= originV.size());
  assert(endA >= 0 && endA <= originV.size());

  accumulateOriginAux(originV.begin() + beginA, originV.begin() + endA, p);
}

void
AreaId::getOrigin(Point& p, int begin, int end) const
{
  p.set(scaled::zero(), scaled::zero());
  accumulateOrigin(p, begin, end);
}

CharIndex
AreaId::getLength(int begin, int end) const
{
  validateLengths();

  const int beginA = (begin >= 0) ? begin : lengthV.size() + begin + 1;
  const int endA = (end >= 0) ? end : lengthV.size() + end + 1;
  assert(beginA >= 0 && beginA <= lengthV.size());
  assert(endA >= 0 && endA <= lengthV.size());

  CharIndex acc = 0;
  accumulateLengthAux(lengthV.begin() + beginA, lengthV.begin() + endA, acc);
  return acc;
}

void
AreaId::accumulateLengthAux(const LengthVector::const_iterator& begin, const LengthVector::const_iterator& end,
			    CharIndex& index) const
{
  for (LengthVector::const_iterator p = begin; p != end; p++)
    index += *p;
}

void
AreaId::accumulateOriginAux(const OriginVector::const_iterator& begin, const OriginVector::const_iterator& end,
			    Point& point) const
{
  for (OriginVector::const_iterator p = begin; p != end; p++)
    {
      point.x += p->x;
      point.y += p->y;
    }
}

void
AreaId::validateAreas() const
{
  AreaRef prev = root;
  while (areaV.size() < pathV.size())
    {
      areaV.push_back(prev->node(pathV[areaV.size()]));
      prev = areaV[areaV.size() - 1];
    }
}

void
AreaId::validateOrigins() const
{
  validateAreas();

  AreaRef prev = root;
  while (originV.size() < areaV.size())
    {
      Point o;
      prev->origin(pathV[originV.size()], o);
      originV.push_back(o);
      prev = areaV[originV.size() - 1];
    }
}

void
AreaId::validateLengths() const
{
  validateAreas();

  AreaRef prev = root;
  while (lengthV.size() < areaV.size())
    {
      lengthV.push_back(prev->lengthTo(pathV[lengthV.size()]));
      prev = areaV[lengthV.size() - 1];
    }
}
