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

#include "MathGraphicDevice.hh"

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
	  assert(0);
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
  return max(context.getSize() / 10, scaled(72.27 / dpi(context)));
}

AreaRef
MathGraphicDevice::horizontalArray(const MathFormattingContext&,
				   const std::vector<AreaRef>& content) const
{
  return factory->createHorizontalArrayArea(content);
}

AreaRef
MathGraphicDevice::verticalArray(const MathFormattingContext&,
				 const std::vector<AreaRef>& content,
				 unsigned ref) const
{
  return factory->createVerticalArrayArea(content, ref);
}

AreaRef
MathGraphicDevice::overlapArray(const MathFormattingContext&,
				const std::vector<AreaRef>& content) const
{
  return factory->createOverlapArrayArea(content);
}

AreaRef
MathGraphicDevice::horizontalSpace(const MathFormattingContext&, const scaled& width) const
{
  return factory->createHorizontalSpaceArea(width);
}

AreaRef
MathGraphicDevice::verticalSpace(const MathFormattingContext&, const scaled& height, const scaled& depth) const
{
  return factory->createVerticalSpaceArea(height, depth);
}

AreaRef
MathGraphicDevice::horizontalFiller(const MathFormattingContext&) const
{
  return factory->createHorizontalFillerArea();
}

AreaRef
MathGraphicDevice::verticalFiller(const MathFormattingContext&) const
{
  return factory->createVerticalFillerArea();
}

AreaRef
MathGraphicDevice::color(const MathFormattingContext& context, const AreaRef& area) const
{
  return factory->createColorArea(area, context.getColor());
}

AreaRef
MathGraphicDevice::ink(const MathFormattingContext&, const AreaRef& area) const
{
  return factory->createInkArea(area);
}

AreaRef
MathGraphicDevice::box(const MathFormattingContext&, const AreaRef& area, const BoundingBox& b) const
{
  return factory->createBoxArea(area, b);
}

AreaRef
MathGraphicDevice::shift(const MathFormattingContext&, const AreaRef& area, const scaled& s) const
{
  return factory->createShiftArea(area, s);
}

AreaRef
MathGraphicDevice::horizontalLine(const MathFormattingContext& context, const scaled& thickness) const
{
  scaled halfThickness = thickness / 2;
  std::vector<AreaRef> h(2);
  h.push_back(verticalSpace(context, halfThickness, thickness - halfThickness));
  h.push_back(horizontalFiller(context));
  return color(context, ink(context, horizontalArray(context, h)));
}

AreaRef
MathGraphicDevice::verticalLine(const MathFormattingContext& context, const scaled& thickness) const
{
  std::vector<AreaRef> v(2);
  v.push_back(horizontalSpace(context, thickness));
  v.push_back(verticalFiller(context));
  return color(context, ink(context, verticalArray(context, v, 1)));
}

AreaRef
MathGraphicDevice::center(const MathFormattingContext& context, const AreaRef& area) const
{
  std::vector<AreaRef> h(3);
  h.push_back(horizontalFiller(context));
  h.push_back(area);
  h.push_back(horizontalFiller(context));
  return horizontalArray(context, h);
}

AreaRef
MathGraphicDevice::left(const MathFormattingContext& context, const AreaRef& area) const
{
  std::vector<AreaRef> h(2);
  h.push_back(area);
  h.push_back(horizontalFiller(context));
  return horizontalArray(context, h);
}

AreaRef
MathGraphicDevice::right(const MathFormattingContext& context, const AreaRef& area) const
{
  std::vector<AreaRef> h(2);
  h.push_back(horizontalFiller(context));
  h.push_back(area);
  return horizontalArray(context, h);
}

AreaRef
MathGraphicDevice::middle(const MathFormattingContext& context, const AreaRef& area) const
{
  std::vector<AreaRef> v(3);
  v.push_back(verticalFiller(context));
  v.push_back(area);
  v.push_back(verticalFiller(context));
  return verticalArray(context, v, 1);
}

AreaRef
MathGraphicDevice::top(const MathFormattingContext& context, const AreaRef& area) const
{
  std::vector<AreaRef> v(2);
  v.push_back(verticalFiller(context));
  v.push_back(area);
  return verticalArray(context, v, 0);
}

AreaRef
MathGraphicDevice::bottom(const MathFormattingContext& context, const AreaRef& area) const
{
  std::vector<AreaRef> v(2);
  v.push_back(area);
  v.push_back(verticalFiller(context));
  return verticalArray(context, v, 1);
}

AreaRef
MathGraphicDevice::freeze(const MathFormattingContext& context, const AreaRef& area) const
{
  return box(context, area, area->box());
}

AreaRef
MathGraphicDevice::wrapper(const MathFormattingContext&, const AreaRef& area) const
{
  return area;
}

