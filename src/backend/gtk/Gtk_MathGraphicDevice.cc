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

#include <cassert>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "Gtk_AreaFactory.hh"
#include "Gtk_MathGraphicDevice.hh"
#include "MathMLElement.hh"
#include "FormattingContext.hh"

Gtk_MathGraphicDevice::Gtk_MathGraphicDevice(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>&)
  : MathGraphicDevice(l)
{ }

Gtk_MathGraphicDevice::~Gtk_MathGraphicDevice()
{ }

SmartPtr<Gtk_MathGraphicDevice>
Gtk_MathGraphicDevice::create(const SmartPtr<AbstractLogger>& logger,
			      const SmartPtr<Configuration>& conf)
{ return new Gtk_MathGraphicDevice(logger, conf); }

void
Gtk_MathGraphicDevice::setFactory(const SmartPtr<Gtk_AreaFactory>& f)
{
  MathGraphicDevice::setFactory(f);
  gtk_factory = f;
}

AreaRef
Gtk_MathGraphicDevice::wrapper(const FormattingContext& context,
			       const AreaRef& base) const
{
  return gtk_factory->wrapper(base, base->box(), context.getMathMLElement());
}
