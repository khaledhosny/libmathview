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
#include "BoxMLVElement.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLVElement::BoxMLVElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLVElement::~BoxMLVElement()
{ }

SmartPtr<BoxMLVElement>
BoxMLVElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLVElement(context); }

// #include "scaledAux.hh"
// #include "BoundingBoxAux.hh"

AreaRef
BoxMLVElement::indentArea(const FormattingContext& ctxt,
			  const AreaRef& indentArea,
			  const AreaRef& area)
{
  std::vector<AreaRef> c;
  c.reserve(2);
  c.push_back(indentArea);
  c.push_back(area);
  return ctxt.BGD()->getFactory()->horizontalArray(c);
}

scaled
BoxMLVElement::getMinimumIndentation(const FormattingContext& ctxt,
				     const SmartPtr<Value>& indent)
{
  if (IsTokenId(indent))
    return scaled::zero();
  else
    return ctxt.BGD()->evaluate(ctxt, ToLength(indent), scaled::zero());
}

AreaRef
BoxMLVElement::formatVerticalArray(FormattingContext& ctxt,
				   const std::vector<AreaRef>& content,
				   const scaled& minLineSpacing,
				   int enter, int exit,
				   const scaled& indent)
{
  if (enter < 0) enter = content.size() + enter + 1;
  if (exit < 0) exit = content.size() + exit + 1;
  enter = content.size() - std::min(std::max(enter, 1), (int) content.size());
  exit = content.size() - std::min(std::max(exit, 1), (int) content.size());

  int enter_index = 0;
  int exit_index = 0;
  AreaRef prevArea = 0;
  scaled prevHeight;
  std::vector<AreaRef> c;
  c.reserve(content.size());

  const AreaRef iArea = ctxt.BGD()->getFactory()->horizontalSpace(indent);

  for (std::vector<AreaRef>::const_reverse_iterator p = content.rbegin();
       p != content.rend();
       p++)
    if (AreaRef area = *p)
      if (BoundingBox areaBox = area->box())
	{
	  if (p != content.rend() - 1)
	    area = indentArea(ctxt, iArea, area);

	  if (prevArea)
	    {
	      const scaled areaDepth = areaBox.depth + std::min(scaled::zero(), area->getStep());
	      if (prevHeight + areaDepth < minLineSpacing)
		c.push_back(ctxt.BGD()->getFactory()->verticalSpace(minLineSpacing - prevHeight - areaDepth, 0));
	    }
	  prevHeight = areaBox.height - std::min(scaled::zero(), -area->getStep());
	  prevArea = area;
	  
	  if (enter-- == 0) enter_index = c.size();
	  if (exit-- == 0) exit_index = c.size();
	  c.push_back(area);
	}

  AreaRef res;
  switch (c.size())
    {
    case 0:
      res = ctxt.BGD()->getFactory()->horizontalArray(c);
      break;
    case 1:
      res = c[0];
      break;
    default:
      res = ctxt.BGD()->getFactory()->verticalArray(c, enter_index);
      if (enter != exit)
	{
	  const SmartPtr<VerticalArrayArea> res1 = ctxt.BGD()->getFactory()->verticalArray(c, exit_index);
	  assert(res->box().defined());
	  assert(res1->box().defined());
	  const scaled step = res->box().height - res1->box().height + res1->node(exit_index)->getStep();
	  res = ctxt.BGD()->getFactory()->step(res, step);
	}
      break;
    }

  return res;
}

AreaRef
BoxMLVElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      const scaled indent = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, indent)), 0);
      int enter = ToInteger(GET_ATTRIBUTE_VALUE(BoxML, V, enter));
      int exit = ToInteger(GET_ATTRIBUTE_VALUE(BoxML, V, exit));
      const scaled minLineSpacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, minlinespacing)), 0);

      const scaled availableWidth = ctxt.getAvailableWidth();
      std::vector<AreaRef> c;
      c.reserve(content.getSize());
      for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	if (*p)
	  {
	    ctxt.setAvailableWidth(availableWidth - indent);
	    c.push_back((*p)->format(ctxt));
	  }

      AreaRef res = formatVerticalArray(ctxt, c, minLineSpacing, enter, exit, indent);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
