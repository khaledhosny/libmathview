// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include "Shaper.hh"

AreaRef
Shaper::composeStretchyCharH(const SmartPtr<class AreaFactory>& factory,
			     const AreaRef& normal,
			     const AreaRef& left, const AreaRef& glue, const AreaRef& right,
			     const scaled& span) const
{
  scaled normalSize = normal ? normal->box().width : 0;
  scaled leftSize = left ? left->box().width : 0;
  scaled rightSize = right ? right->box().width : 0;
  scaled glueSize = glue ? glue->box().width : 0;

  if (normalSize >= span) return normal;

  // Compute first the number of glue segments we have to use
  assert(glueSize > scaled::zero());
  int n = std::max(0, (span - leftSize - rightSize).getValue() / glueSize.getValue());

  // Then the final number of glyphs
  int gsN = (left ? 1 : 0) + n + (right ? 1 : 0);

  std::vector<AreaRef> h;
  h.reserve(gsN);

  if (left) h.push_back(left);
  for (int i = 0; i < n; i++) h.push_back(glue);
  if (right) h.push_back(right);

  return factory->horizontalArray(h);
}

AreaRef
Shaper::composeStretchyCharV(const SmartPtr<class AreaFactory>& factory,
			     const AreaRef& normal, 
			     const AreaRef& top, const AreaRef& glue, const AreaRef& middle, const AreaRef& bottom,
			     const scaled& span) const
{
  scaled normalSize = normal ? normal->box().verticalExtent() : 0;
  scaled topSize = top ? top->box().verticalExtent() : 0;
  scaled middleSize = middle ? middle->box().verticalExtent() : 0;
  scaled glueSize = glue ? glue->box().verticalExtent() : 0;
  scaled bottomSize = bottom ? bottom->box().verticalExtent() : 0;			

  if (normalSize >= span) return normal;

  assert(glueSize > scaled::zero());
  int n = std::max(0, (span - topSize - bottomSize - middleSize).getValue() / glueSize.getValue());

  //std::cerr << "STRETCHYING: " << span << " N = " << n << std::endl;

  if (n % 2 == 1 && middle) n++;
  
  int gsN = (top ? 1 : 0) + (middle ? 1 : 0) + n + (bottom ? 1 : 0);

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
    for (unsigned i = 0; i < n; i++) v.push_back(glue);
  if (top) v.push_back(top);

  return factory->verticalArray(v, 0);
}
