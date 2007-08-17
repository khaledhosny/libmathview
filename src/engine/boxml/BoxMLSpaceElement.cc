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

#include "BoxMLSpaceElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"
#include "AreaFactory.hh"

BoxMLSpaceElement::BoxMLSpaceElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLElement(context)
{ }

BoxMLSpaceElement::~BoxMLSpaceElement()
{ }

SmartPtr<BoxMLSpaceElement>
BoxMLSpaceElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLSpaceElement(context); }

AreaRef
BoxMLSpaceElement::makeSpaceArea(FormattingContext& ctxt)
{
  scaled width = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Space, width)), 0);
  scaled height = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Space, height)), 0);
  scaled depth = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, Space, depth)), 0);

  std::vector<AreaRef> c;
  c.reserve(2);
  c.push_back(ctxt.BGD()->getFactory()->verticalSpace(height, depth));
  c.push_back(ctxt.BGD()->getFactory()->horizontalSpace(width));
  return ctxt.BGD()->getFactory()->horizontalArray(c);
}

AreaRef
BoxMLSpaceElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      AreaRef res = makeSpaceArea(ctxt);
      res = ctxt.BGD()->wrapper(ctxt, res);
      setMaxArea(res);
      setArea(res);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
