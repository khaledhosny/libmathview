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

#include "MathMLElement.hh"
#include "MathGraphicDevice.hh"

MathGraphicDevice::MathGraphicDevice()
  : shaperManager(ShaperManager::create())
{ }

MathGraphicDevice::~MathGraphicDevice()
{
  shaperManager->unregisterShapers();
}

double
MathGraphicDevice::dpi(const MathFormattingContext&) const
{
  return 72.0;
}

scaled
MathGraphicDevice::evaluate(const MathFormattingContext& context,
			    const Length& length, const scaled& defaultValue) const
{
  switch (length.type)
    {
    case Length::UNDEFINED_UNIT:
      return defaultValue;
    case Length::PURE_UNIT:
      // error?
      return defaultValue * length.value;
    case Length::INFINITY_UNIT:
      return scaled::max();
    case Length::LT_UNIT:
      return defaultLineThickness(context) * length.value;
    case Length::EM_UNIT:
      return em(context) * length.value;
    case Length::EX_UNIT:
      return ex(context) * length.value;
    case Length::PX_UNIT:
      return scaled((72.27 * length.value) / dpi(context));
    case Length::IN_UNIT:
      return scaled(72.27 * length.value);
    case Length::CM_UNIT:
      return scaled(72.27 * (length.value / 2.54));
    case Length::MM_UNIT:
      return scaled(72.27 * (length.value / 254.0));
    case Length::PT_UNIT:
      return scaled(length.value);
    case Length::PC_UNIT:
      return scaled(12 * 72.27 * length.value);
    case Length::PERCENTAGE_UNIT:
      return (defaultValue * length.value) / 100.0;
    default:
      if (length.type >= Length::NEGATIVE_VERYVERYTHICK_SPACE &&
	  length.type <= Length::VERYVERYTHICK_SPACE)
	{
	  Length newLength =
	    context.getMathSpace(MathFormattingContext::NEGATIVE_VERYVERYTHICK_SPACE +
				 length.type - Length::NEGATIVE_VERYVERYTHICK_SPACE);
	  assert(newLength.type < Length::NEGATIVE_VERYVERYTHICK_SPACE ||
		 newLength.type > Length::VERYVERYTHICK_SPACE);
	  return evaluate(context, newLength, defaultValue);
	}
      else
	{
	  assert(false);
	  return defaultValue;
	}
    }
}

scaled
MathGraphicDevice::em(const MathFormattingContext& context) const
{
  return context.getSize();
}

scaled
MathGraphicDevice::ex(const MathFormattingContext& context) const
{
  return string(context, "x")->box().height;
}

scaled
MathGraphicDevice::axis(const MathFormattingContext& context) const
{
  return ex(context) / 2;
}

scaled
MathGraphicDevice::defaultLineThickness(const MathFormattingContext& context) const
{
  // at least 1px thick
  return std::max(context.getSize() / 10, scaled(72.27 / dpi(context)));
}

AreaRef
MathGraphicDevice::wrapper(const MathFormattingContext&, const AreaRef& area) const
{
  return area;
}

AreaRef
MathGraphicDevice::dummy(const MathFormattingContext& context) const
{
  return getFactory()->color(string(context, StringOfUCS4String(UCS4String(1, 0xfffd))), RGBColor::RED());
}
