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

#include "AreaFactory.hh"
#include "ShapingContext.hh"
#include "ShaperManager.hh"
#include "NullShaper.hh"

NullShaper::NullShaper(void)
{ }

NullShaper::~NullShaper()
{ }

SmartPtr<NullShaper>
NullShaper::create(void)
{ return new NullShaper(); }

void
NullShaper::registerShaper(const SmartPtr<ShaperManager>& sm, unsigned)
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
  //logger->out(LOG_WARNING, "shaping U+%04X as a red box", context.thisChar());
  SmartPtr<AreaFactory> factory = context.getFactory();
  std::vector<AreaRef> c;
  c.reserve(2);
  const scaled hSpan = (context.getHSpan() != scaled::zero()) ? context.getHSpan() : context.getSize();
  const scaled vSpan = (context.getVSpan() != scaled::zero()) ? context.getVSpan() : context.getSize();
  c.push_back(factory->horizontalSpace(hSpan));
  c.push_back(factory->verticalSpace(vSpan, 0));
  context.pushArea(1, factory->background(factory->horizontalArray(c), RGBColor::RED()));
}

bool
NullShaper::isDefaultShaper() const
{ return true; }
