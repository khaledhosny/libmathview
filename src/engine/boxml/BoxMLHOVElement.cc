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

#include "BoxMLAttributeSignatures.hh"
#include "BoxMLHOVElement.hh"
#include "BoxMLHElement.hh"
#include "BoxMLVElement.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLHOVElement::BoxMLHOVElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLHOVElement::~BoxMLHOVElement()
{ }

SmartPtr<BoxMLHOVElement>
BoxMLHOVElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLHOVElement(context); }

AreaRef
BoxMLHOVElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      const scaled spacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, HOV, spacing)), 0);
      const scaled indent = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, HOV, indent)), 0);
      const scaled minLineSpacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, minlinespacing)), 0);

      const scaled availableWidth = ctxt.getAvailableWidth();

      std::vector<AreaRef> cMax;
      cMax.reserve(content.getSize());
      std::vector<AreaRef> hc;
      hc.reserve(content.getSize());
      std::vector<AreaRef> vc;
      vc.reserve(content.getSize());

      std::vector<SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
      while (p != content.end())
	{
	  const scaled availableWidthPerLine = vc.empty() ? availableWidth : availableWidth - indent;
	  scaled remainingWidthForThisLine = availableWidthPerLine;

	  hc.clear();

	  ctxt.setAvailableWidth(availableWidthPerLine);

	  AreaRef area;
	  AreaRef maxArea;
	  do
	    {
	      area = (*p)->format(ctxt);
	      maxArea = (*p)->getMaxArea();
	      cMax.push_back(maxArea);
	      if (maxArea->box().width > remainingWidthForThisLine)
		break;
	      hc.push_back(maxArea);
	      remainingWidthForThisLine -= maxArea->box().width + spacing;
	      p++;
	    }
	  while (p != content.end());

	  if (!hc.empty())
	    vc.push_back(BoxMLHElement::formatHorizontalArray(ctxt, hc, spacing));
	  else if (p != content.end())
	    {
	      if (maxArea->box().width <= availableWidthPerLine)
		vc.push_back(maxArea);
	      else
		vc.push_back(area);
	      p++;
	    }
	}

      AreaRef res = BoxMLHElement::formatHorizontalArray(ctxt, cMax, spacing);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setMaxArea(res);

      if (res->box().width <= availableWidth)
	setArea(res);
      else
	{
	  res = BoxMLVElement::formatVerticalArray(ctxt, vc, minLineSpacing, 1, -1, indent);
	  res = ctxt.BGD()->wrapper(ctxt, res);
	  setArea(res);
	}

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
