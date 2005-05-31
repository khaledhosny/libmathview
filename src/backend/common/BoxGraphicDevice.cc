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

#include <cassert>

#include "AreaFactory.hh"
#include "BoxGraphicDevice.hh"
#include "BoxMLElement.hh"
#include "FormattingContext.hh"
#include "ShaperManager.hh"

BoxGraphicDevice::BoxGraphicDevice(const SmartPtr<AbstractLogger>& logger)
  : GraphicDevice(logger)
{ }

BoxGraphicDevice::~BoxGraphicDevice()
{ }

scaled
BoxGraphicDevice::ex(const FormattingContext& context) const
{
  return string(context, "x", scaled::min())->box().height;
}

AreaRef
BoxGraphicDevice::wrapper(const FormattingContext&, const AreaRef& area) const
{
  return area;
}

AreaRef
BoxGraphicDevice::dummy(const FormattingContext&) const
{
  //assert(false);
  return getFactory()->horizontalSpace(scaled::zero());
}

AreaRef
BoxGraphicDevice::string(const FormattingContext& context, const String& str, const scaled&) const
{
  const UCS4String source = UCS4StringOfString(str);
  const AreaRef res = getShaperManager()->shape(context,
						context.getBoxMLElement(),
						context.BGD()->getFactory(),
						source);
  return res;
}
