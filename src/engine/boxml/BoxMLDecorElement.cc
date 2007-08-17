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

#include "BoxMLDecorElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLDecorElement::BoxMLDecorElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLBinContainerElement(context)
{ }

BoxMLDecorElement::~BoxMLDecorElement()
{ }

SmartPtr<BoxMLDecorElement>
BoxMLDecorElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLDecorElement(context); }

AreaRef
BoxMLDecorElement::decorate(FormattingContext& ctxt,
			    const AreaRef& child,
			    const scaled& thickness,
			    const RGBColor& color,
			    const String& type)
{
  SmartPtr<AreaFactory> factory = ctxt.BGD()->getFactory();
  std::vector<AreaRef> c;
  c.reserve(3);

  AreaRef res = child;

  AreaRef vobj;
  if (color.transparent()) 
    vobj = factory->horizontalSpace(thickness); 
  else
    vobj = factory->verticalLine(thickness, color);
  AreaRef hobj;
  if (color.transparent())
    hobj = factory->verticalSpace(thickness, 0);
  else 
    hobj = factory->horizontalLine(thickness, color);

  if (type == "box" || type == "left") c.push_back(vobj);
  c.push_back(res);
  if (type == "box" || type == "right") c.push_back(vobj);
  if (c.size() > 1) res = factory->horizontalArray(c);

  c.clear();
  if (type == "box" || type == "bottom") c.push_back(hobj);
  c.push_back(res);
  if (type == "box" || type == "top") c.push_back(hobj);
  if (c.size() > 1)
    res = factory->verticalArray(c, (type == "top") ? 0 : 1);

  c.clear();
  c.push_back(res);
  if (type == "verticalstrike") c.push_back(factory->center(vobj));
  else if (type == "horizontalstrike") c.push_back(factory->middle(hobj));
  else if (type == "baselinestrike") c.push_back(hobj);
  if (c.size() > 1) res = factory->overlapArray(c);

  return res;
}

AreaRef
BoxMLDecorElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      SmartPtr<ValueSequence> type = ToSequence(GET_ATTRIBUTE_VALUE(BoxML, Decor, type));
      SmartPtr<Value> color = GET_ATTRIBUTE_VALUE(BoxML, Decor, color);
      const scaled thickness = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Decor, thickness)),
							  ctxt.BGD()->defaultLineThickness(ctxt));
      RGBColor col;
      if (color && IsTokenId(color) && ToTokenId(color) == T_TRANSPARENT)
	col = RGBColor(0, 0, 0, 0);
      else if (color)
	col = ToRGB(color);

      if (SmartPtr<BoxMLElement> child = getChild())
	{
	  child->format(ctxt);
	  
	  AreaRef res = child->getMaxArea();
	  for (unsigned i = 0; i < type->getSize(); i++)
	    res = decorate(ctxt, res, thickness, col, ToString(type->getValue(i)));
	  res = ctxt.BGD()->wrapper(ctxt, res);
	  setMaxArea(res);

	  if (res->box().width > ctxt.getAvailableWidth())
	    {
	      res = child->getArea();
	      for (unsigned i = 0; i < type->getSize(); i++)
		res = decorate(ctxt, res, thickness, col, ToString(type->getValue(i)));
	      res = ctxt.BGD()->wrapper(ctxt, res);
	    }
	  setArea(res);
	}
      else
	setArea(0);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
