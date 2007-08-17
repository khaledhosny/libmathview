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
#include "BoxMLHElement.hh"
#include "BoxMLVElement.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLHElement::BoxMLHElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLHElement::~BoxMLHElement()
{ }

SmartPtr<BoxMLHElement>
BoxMLHElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLHElement(context); }

AreaRef
BoxMLHElement::formatHorizontalArray(FormattingContext& ctxt,
				     const std::vector<AreaRef>& content,
				     const scaled& spacing)
{
  const AreaRef spacingArea = ctxt.BGD()->getFactory()->horizontalSpace(spacing);

  std::vector<AreaRef> c;
  c.reserve(content.size());
  for (std::vector<AreaRef>::const_iterator p = content.begin();
       p != content.end();
       p++)
    if (*p)
      {
	c.push_back(*p);
	if (p + 1 != content.end() && spacing != scaled::zero())
	  c.push_back(spacingArea);
      }

  AreaRef res;
  if (c.size() == 1)
    res = c[0];
  else
    res = ctxt.BGD()->getFactory()->horizontalArray(c);

  return res;
}

AreaRef
BoxMLHElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      const scaled spacing = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, H, spacing)), scaled::zero());
      scaled availableWidth = ctxt.getAvailableWidth();

      std::vector<AreaRef> c;
      c.reserve(content.getSize());
      std::vector<AreaRef> cMax;
      cMax.reserve(content.getSize());
      std::vector<scaled> sc;
      sc.reserve(content.getSize());
      for (std::vector<SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	{
	  assert(*p);
	  ctxt.setAvailableWidth(availableWidth);
	  const AreaRef area = (*p)->format(ctxt);
	  c.push_back(area);
	  cMax.push_back((*p)->getMaxArea());
	  availableWidth -= area->box().width + spacing;
	}

      AreaRef res = formatHorizontalArray(ctxt, cMax, spacing);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setMaxArea(res);

      if (res->box().width > availableWidth)
	{
	  res = formatHorizontalArray(ctxt, c, spacing);
	  res = ctxt.BGD()->wrapper(ctxt, res);
	}
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
