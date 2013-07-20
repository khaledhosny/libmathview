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

#include "Cairo_AreaFactory.hh"
#include "Cairo_MathGraphicDevice.hh"
#include "MathMLElement.hh"
#include "FormattingContext.hh"

Cairo_MathGraphicDevice::Cairo_MathGraphicDevice(const SmartPtr<MathFont>& f)
  : MathGraphicDevice(f)
{ }

Cairo_MathGraphicDevice::~Cairo_MathGraphicDevice()
{ }

SmartPtr<Cairo_MathGraphicDevice>
Cairo_MathGraphicDevice::create(const SmartPtr<MathFont>& font)
{ return new Cairo_MathGraphicDevice(font); }

void
Cairo_MathGraphicDevice::setFactory(const SmartPtr<Cairo_AreaFactory>& f)
{
  MathGraphicDevice::setFactory(f);
  cairo_factory = f;
}

AreaRef
Cairo_MathGraphicDevice::wrapper(const FormattingContext& context,
                               const AreaRef& base) const
{
  return cairo_factory->wrapper(base, base->box(), context.getMathMLElement());
}
