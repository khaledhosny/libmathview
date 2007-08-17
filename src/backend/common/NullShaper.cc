// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include "AreaFactory.hh"
#include "ShapingContext.hh"
#include "ShaperManager.hh"
#include "NullShaper.hh"
#include "AbstractLogger.hh"

NullShaper::NullShaper(const SmartPtr<AbstractLogger>& l)
  : logger(l)
{ }

NullShaper::~NullShaper()
{ }

SmartPtr<NullShaper>
NullShaper::create(const SmartPtr<AbstractLogger>& l)
{ return new NullShaper(l); }

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
  logger->out(LOG_WARNING, "shaping U+%04X as a red box", context.thisChar());
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
