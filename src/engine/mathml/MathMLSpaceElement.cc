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
