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

#include <vector>

#include "Shaper.hh"
#include "AreaFactory.hh"

AreaRef
Shaper::composeStretchyCharH(const SmartPtr<class AreaFactory>& factory,
			     const AreaRef& normal,
			     const AreaRef& left, const AreaRef& glue, const AreaRef& right,
			     const scaled& span) const
{
  const scaled normalSize = normal ? normal->box().width : 0;
  const scaled leftSize = left ? left->box().width : 0;
  const scaled rightSize = right ? right->box().width : 0;
  const scaled glueSize = glue ? glue->box().width : 0;

  if (normalSize >= span) return normal;

  // Compute first the number of glue segments we have to use
  const int n =
    (glueSize > scaled::zero())
    ? std::max(0, (span - leftSize - rightSize).getValue() / glueSize.getValue())
    : 0;

  // Then the final number of glyphs
  const int gsN = (left ? 1 : 0) + n + (right ? 1 : 0);
  if (gsN == 0) return normal;

  std::vector<AreaRef> h;
  h.reserve(gsN);

  if (left) h.push_back(left);
  for (int i = 0; i < n; i++) h.push_back(glue);
  if (right) h.push_back(right);

  return factory->glyphWrapper(factory->horizontalArray(h), 1);
}

AreaRef
Shaper::composeStretchyCharV(const SmartPtr<class AreaFactory>& factory,
			     const AreaRef& normal, 
			     const AreaRef& top, const AreaRef& glue, const AreaRef& middle, const AreaRef& bottom,
			     const scaled& span) const
{
  const scaled normalSize = normal ? normal->box().verticalExtent() : 0;
  const scaled topSize = top ? top->box().verticalExtent() : 0;
  const scaled glueSize = glue ? glue->box().verticalExtent() : 0;
  const scaled middleSize = middle ? middle->box().verticalExtent() : 0;
  const scaled bottomSize = bottom ? bottom->box().verticalExtent() : 0;			

  if (normalSize >= span) return normal;

  int n =
    (glueSize > scaled::zero())
    ? std::max(0, (span - topSize - bottomSize - middleSize).getValue() / glueSize.getValue())
    : 0;

  //std::cerr << "STRETCHYING: " << span << " N = " << n << std::endl;

  if (n % 2 == 1 && middle) n++;
  
  const int gsN = (top ? 1 : 0) + (middle ? 1 : 0) + n + (bottom ? 1 : 0);
  if (gsN == 0) return normal;

  std::vector<AreaRef> v;
  v.reserve(gsN);

  if (bottom) v.push_back(bottom);
  if (middle)
    {
      for (int i = 0; i < n / 2; i++) v.push_back(glue);
      v.push_back(middle);
      for (int i = 0; i < n / 2; i++) v.push_back(glue);
    }
  else
    for (int i = 0; i < n; i++) v.push_back(glue);
  if (top) v.push_back(top);

  // FIXME the 1 constant should not be hardcoded, the method
  // should have one more parameter saying how long the stretchy
  // character was
  return factory->glyphWrapper(factory->verticalArray(v, 0), 1);
}

bool
Shaper::isDefaultShaper() const
{ return false; }

bool
Shaper::shapeCombiningChar(const ShapingContext&) const
{ return false; }

bool
Shaper::computeCombiningCharOffsetsAbove(const AreaRef& base,
				    	 const AreaRef& script,
				    	 scaled& dx,
				    	 scaled& dy) const
{
  //default value of dx and dy
  dx = (base->box().width - script->box().width) / 2;
  dy = base->box().height + script->box().depth;
    
  return true;
}

bool
Shaper::computeCombiningCharOffsetsBelow(const AreaRef& base,
				         const AreaRef& script,
				         scaled& dxUnder) const
{
  dxUnder = (base->box().width - script->box().width) / 2;

  return true;
}
