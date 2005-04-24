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
			  const SmartPtr<Value>& indent,
			  const AreaRef& area)
{
  if (IsTokenId(indent))
    switch (ToTokenId(indent))
      {
      case T_LEFT: return area;
      case T_CENTER: return ctxt.BGD()->getFactory()->center(area);
      case T_RIGHT: return ctxt.BGD()->getFactory()->right(area);
      default:
	assert(false);
	break;
      }
  else
    {
      std::vector<AreaRef> c;
      c.reserve(2);
      c.push_back(ctxt.BGD()->getFactory()->horizontalSpace(ctxt.BGD()->evaluate(ctxt, ToLength(indent), scaled::zero())));
      c.push_back(area);
      return ctxt.BGD()->getFactory()->horizontalArray(c);
    }
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
				   const std::vector<SmartPtr<Value> >& indentV,
				   scaled& step)
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
  for (std::vector<AreaRef>::const_reverse_iterator p = content.rbegin();
       p != content.rend();
       p++)
    if (AreaRef area = *p)
      if (BoundingBox areaBox = area->box())
	{
	  const SmartPtr<Value> indent = indentV[content.size() - (p - content.rbegin()) - 1];
	  area = indentArea(ctxt, indent, area);

	  if (prevArea)
	    {
	      if (prevHeight + areaBox.depth < minLineSpacing)
		c.push_back(ctxt.BGD()->getFactory()->verticalSpace(minLineSpacing - prevHeight - areaBox.depth, 0));
	    }
	  prevHeight = areaBox.height;
	  prevArea = area;
	  
	  if (enter-- == 0) enter_index = c.size();
	  if (exit-- == 0) exit_index = c.size();
	  c.push_back(area);
	}

  step = 0;
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
	  AreaRef res1 = ctxt.BGD()->getFactory()->verticalArray(c, exit_index);
	  assert(res->box().defined());
	  assert(res1->box().defined());
	  step = res->box().height - res1->box().height;

#if 0
	  for (unsigned i = 0; i < c.size(); i++)
	    {
	      std::cerr << "AREA " << i << " HAS BOX = " << c[i]->box() << std::endl;
	    }

	  std::cerr << enter_index << "/" << exit_index << " ************ " << res->box().height << " //// " << res1->box().height << std::endl;
#endif
	}
      break;
    }

#if 0
  std::cerr << "==================== " << this << " STEP IS " << step << std::endl;
#endif

  return res;
}

AreaRef
BoxMLVElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      const SmartPtr<Value> indentV = GET_ATTRIBUTE_VALUE(BoxML, V, indent);
      int enter = ToInteger(GET_ATTRIBUTE_VALUE(BoxML, V, enter));
      int exit = ToInteger(GET_ATTRIBUTE_VALUE(BoxML, V, exit));
      const scaled minLineSpacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, V, minlinespacing)), 0);

      const scaled availableWidth = ctxt.getAvailableWidth();
      std::vector<AreaRef> c;
      c.reserve(content.getSize());
      std::vector<SmartPtr<Value> > ic;
      ic.reserve(content.getSize());
      for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	if (*p)
	  {
	    const SmartPtr<Value> indent = GetComponent(indentV, p - content.begin());
	    ctxt.setAvailableWidth(availableWidth - getMinimumIndentation(ctxt, indent));
	    c.push_back((*p)->format(ctxt));
	    ic.push_back(indent);
	  }

      AreaRef res = formatVerticalArray(ctxt, c, minLineSpacing, enter, exit, ic, step);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
