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

#include "FormattingContext.hh"
#include "GraphicDevice.hh"
#include "ShaperManager.hh"

GraphicDevice::GraphicDevice(void)
{ }

GraphicDevice::~GraphicDevice()
{ }

void
GraphicDevice::setFactory(const SmartPtr<AreaFactory>& f)
{ factory = f; }

void
GraphicDevice::setShaperManager(const SmartPtr<ShaperManager>& sm)
{ shaperManager = sm; }

SmartPtr<ShaperManager>
GraphicDevice::getShaperManager() const
{ return shaperManager; }

scaled
GraphicDevice::evaluate(const FormattingContext& context,
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
      return scaled(72.27 * (length.value / 25.4));
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
	    context.getMathSpace(FormattingContext::NEGATIVE_VERYVERYTHICK_SPACE +
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

double
GraphicDevice::dpi(const FormattingContext&) const
{
  return 72.0;
}

scaled
GraphicDevice::em(const FormattingContext& context) const
{
  return context.getSize();
}

scaled
GraphicDevice::defaultLineThickness(const FormattingContext& context) const
{
  // at least 1px thick
  return std::max(context.getSize() / 10, scaled(72.27f / dpi(context)));
}
