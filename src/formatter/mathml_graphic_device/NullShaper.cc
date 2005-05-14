// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include "AreaFactory.hh"
#include "ShapingContext.hh"
#include "ShaperManager.hh"
#include "NullShaper.hh"

void
NullShaper::registerShaper(const SmartPtr<ShaperManager>& sm, unsigned shaperId)
{
  assert(sm);
}

void
NullShaper::unregisterShaper(const SmartPtr<ShaperManager>&, unsigned)
{ }

void
NullShaper::shape(ShapingContext& context) const
{
  assert(!context.done());
  SmartPtr<AreaFactory> factory = context.getFactory();
  std::vector<AreaRef> c;
  c.reserve(2);
  c.push_back(factory->horizontalSpace(context.getSize()));
  c.push_back(factory->verticalSpace(context.getSize(), 0));
  context.pushArea(1, factory->background(factory->horizontalArray(c), RGBColor::RED()));
}

