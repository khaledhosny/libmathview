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

#include "BoxMLLayoutElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "BoxMLAtElement.hh"
#include "BoxedLayoutArea.hh"
#include "AreaFactory.hh"

BoxMLLayoutElement::BoxMLLayoutElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLLayoutElement::~BoxMLLayoutElement()
{ }

SmartPtr<BoxMLLayoutElement>
BoxMLLayoutElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLLayoutElement(context); }

AreaRef
BoxMLLayoutElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      scaled width = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Layout, width)), ctxt.getSize());
      scaled height = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Layout, height)), ctxt.getSize());
      scaled depth = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Layout, depth)), ctxt.getSize());

      std::vector<BoxedLayoutArea::XYArea> c;
      c.reserve(content.getSize());
      for (std::vector< SmartPtr<BoxMLElement> >::const_iterator p = content.begin();
	   p != content.end();
	   p++)
	if (SmartPtr<BoxMLAtElement> elem = smart_cast<BoxMLAtElement>(*p))
	  {
	    AreaRef area = elem->format(ctxt);
	    c.push_back(BoxedLayoutArea::XYArea(elem->getX(), elem->getY(), area));
	  }

      AreaRef res = ctxt.BGD()->getFactory()->boxedLayout(BoundingBox(width, height, depth), c);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setMaxArea(res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
