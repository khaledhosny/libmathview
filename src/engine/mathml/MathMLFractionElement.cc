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

#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "Globals.hh"
#include "MathMLDummyElement.hh"
#include "MathMLFractionElement.hh"
#include "MathMLOperatorElement.hh"
#include "ValueConversion.hh"
#include "MathMLAttributeSignatures.hh"

MathMLFractionElement::MathMLFractionElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLContainerElement(context)
{ }

MathMLFractionElement::~MathMLFractionElement()
{ }

AreaRef
MathMLFractionElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      Length thickness;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Fraction, linethickness))
	{
	  if (IsTokenId(value))
	    {
	      switch (ToTokenId(value))
		{
		case T_THIN:
		  thickness.set(0.5, Length::PURE_UNIT);
		  break;
		case T_MEDIUM:
		  thickness.set(1, Length::PURE_UNIT);
		  break;
		case T_THICK:
		  thickness.set(2, Length::PURE_UNIT);
		  break;
		default:
		  assert(false);
		  break;
		}
	    }
	  else if (IsNumber(value))
	    thickness.set(ToNumber(value), Length::PURE_UNIT);
	  else
	    thickness = ToLength(value);
	}
      else
	assert(false);

      TokenId numAlign = ToTokenId(GET_ATTRIBUTE_VALUE(MathML, Fraction, numalign));
      TokenId denomAlign = ToTokenId(GET_ATTRIBUTE_VALUE(MathML, Fraction, denomalign));
      bool bevelled = ToBoolean(GET_ATTRIBUTE_VALUE(MathML, Fraction, bevelled));

      AreaRef res;

      ctxt.push(this);
      ctxt.push(this);
      if (ctxt.getDisplayStyle()) ctxt.setDisplayStyle(false);
      else ctxt.addScriptLevel(1);
      AreaRef num = getNumerator()->format(ctxt);
      AreaRef denom = getDenominator()->format(ctxt);
      ctxt.pop();

      if (bevelled)
	res = ctxt.getDevice()->bevelledFraction(ctxt, num, denom, thickness);
      else
	{
	  switch (numAlign)
	    {
	    case T_LEFT: break; // nothing to do
	    case T_CENTER: num = ctxt.getDevice()->getFactory()->center(num); break;
	    case T_RIGHT: num = ctxt.getDevice()->getFactory()->right(num); break;
	    default: assert(false);
	    }

	  switch (denomAlign)
	    {
	    case T_LEFT: break; // nothing to do
	    case T_CENTER: denom = ctxt.getDevice()->getFactory()->center(denom); break;
	    case T_RIGHT: denom = ctxt.getDevice()->getFactory()->right(denom); break;
	    default: assert(false);
	    }
	
	  res = ctxt.getDevice()->fraction(ctxt, num, denom, thickness);
	}

      res = formatEmbellishment(this, ctxt, res);

      setArea(ctxt.getDevice()->wrapper(ctxt, res));

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLFractionElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  numerator.setFlagDown(f);
  denominator.setFlagDown(f);
}

void
MathMLFractionElement::resetFlagDown(Flags f)
{
  MathMLContainerElement::resetFlagDown(f);
  numerator.resetFlagDown(f);
  denominator.resetFlagDown(f);
}

SmartPtr<MathMLOperatorElement>
MathMLFractionElement::getCoreOperator()
{
  return getNumerator() ? getNumerator()->getCoreOperator() : 0;
}
