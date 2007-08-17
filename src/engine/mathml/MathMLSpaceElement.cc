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

#include <cassert>

#include "defs.h"
#include "AreaFactory.hh"
#include "MathMLSpaceElement.hh"
#include "MathMLValueConversion.hh"
#include "MathMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLSpaceElement::MathMLSpaceElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLElement(context)
{
  breakability = T_AUTO;
}

MathMLSpaceElement::~MathMLSpaceElement()
{ }

AreaRef
MathMLSpaceElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      scaled width;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Space, width))
	width = ctxt.MGD()->evaluate(ctxt, toLength(value, ctxt), scaled::zero());
      else
	assert(false);

      scaled height;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Space, height))
	height = ctxt.MGD()->evaluate(ctxt, toLength(value, ctxt), scaled::zero());

      scaled depth;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Space, depth))
	depth = ctxt.MGD()->evaluate(ctxt, toLength(value, ctxt), scaled::zero());

      
#if 0
      // fixme this should detect the presence of attributes in a different way
      if (!IsSet(T_WIDTH) && !IsSet(T_HEIGHT) && !IsSet(T_DEPTH))
	{
	  lineBreak = true;
	  autoLineBreak = IsSet(T_LINEBREAK);
	  if (!autoLineBreak)
	    breakability = ToTokenId(GET_ATTRIBUTE_VALUE(MathML, Space, linebreak));
	}
      else
#endif
	lineBreak = false;

      if (lineBreak)
	setArea(0);
      else
	{
	  AreaRef res = ctxt.MGD()->getFactory()->box(ctxt.MGD()->getFactory()->horizontalSpace(scaled::zero()), BoundingBox(width, height, depth));
	  setArea(ctxt.MGD()->wrapper(ctxt, res));
	}

      ctxt.pop();
      resetDirtyLayout();
    }
  
  return getArea();
}

bool
MathMLSpaceElement::IsSpace() const
{
  return true;
}

bool
MathMLSpaceElement::IsSpaceLike() const
{
  return true;
}
