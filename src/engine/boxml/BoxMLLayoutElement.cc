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
