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
#include "MathMLSpaceElement.hh"
#include "ValueConversion.hh"
#include "MathMLAttributeSignatures.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLSpaceElement::MathMLSpaceElement(const SmartPtr<class MathMLView>& view)
  : MathMLElement(view)
{
  breakability = T_AUTO;
}

MathMLSpaceElement::~MathMLSpaceElement()
{
}

void
MathMLSpaceElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute())
    {
      REFINE_ATTRIBUTE(context, MathML, Space, width);
      REFINE_ATTRIBUTE(context, MathML, Space, height);
      REFINE_ATTRIBUTE(context, MathML, Space, depth);
      REFINE_ATTRIBUTE(context, MathML, Space, linebreak);
      MathMLElement::refine(context);
    }
}

AreaRef
MathMLSpaceElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      scaled width;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Space, width))
	if (IsTokenId(value))
	  width = ctxt.getDevice()->evaluate(ctxt, ToLength(Resolve(value, ctxt)), scaled::zero());
	else
	  width = ctxt.getDevice()->evaluate(ctxt, ToLength(value), scaled::zero());
      else
	assert(IMPOSSIBLE);

      scaled height;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Space, height))
	height = ctxt.getDevice()->evaluate(ctxt, ToLength(value), scaled::zero());

      scaled depth;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Space, depth))
	depth = ctxt.getDevice()->evaluate(ctxt, ToLength(value), scaled::zero());

      
      if (!IsSet(T_WIDTH) && !IsSet(T_HEIGHT) && !IsSet(T_DEPTH))
	{
	  lineBreak = true;
	  autoLineBreak = IsSet(T_LINEBREAK);
	  if (!autoLineBreak)
	    breakability = ToTokenId(GET_ATTRIBUTE_VALUE(MathML, Space, linebreak));
	}
      else
	lineBreak = false;

      if (lineBreak)
	setArea(0);
      else
	{
	  AreaRef res = ctxt.getDevice()->getFactory()->box(ctxt.getDevice()->getFactory()->horizontalSpace(scaled::zero()), BoundingBox(width, height, depth));
	  setArea(ctxt.getDevice()->wrapper(ctxt, res));
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
