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

#include "MathMLOperatorElement.hh"
#include "MathMLUnderOverElement.hh"
#include "MathMLNamespaceContext.hh"
#include "ValueConversion.hh"
#include "traverseAux.hh"
#include "MathGraphicDevice.hh"
#include "FormattingContext.hh"
#include "MathMLAttributeSignatures.hh"

MathMLUnderOverElement::MathMLUnderOverElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLContainerElement(context)
{ }

MathMLUnderOverElement::~MathMLUnderOverElement()
{ }

AreaRef
MathMLUnderOverElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      bool accent = false;
      bool accentUnder = false;

      const SmartPtr<MathMLOperatorElement> baseOp = getBase() ? getBase()->getCoreOperator() : 0;
      const SmartPtr<MathMLOperatorElement> underOp = getUnderScript() ? getUnderScript()->getCoreOperator() : 0;
      const SmartPtr<MathMLOperatorElement> overOp = getOverScript() ? getOverScript()->getCoreOperator() : 0;

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, UnderOver, accentunder))
	accentUnder = ToBoolean(value);
      else if (underOp)
	accentUnder = underOp->IsAccent();
      else
	accentUnder = false;

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, UnderOver, accent))
	accent = ToBoolean(value);
      else if (overOp)
	accent = overOp->IsAccent();
      else
	accent = false;

      ctxt.push(this);
      ctxt.setDisplayStyle(false);
      if (!accentUnder) ctxt.addScriptLevel(1);
      AreaRef underArea = getUnderScript() ? getUnderScript()->format(ctxt) : 0;
      ctxt.pop();

      ctxt.push(this);
      ctxt.setDisplayStyle(false);
      if (!accent) ctxt.addScriptLevel(1);
      AreaRef overArea = getOverScript() ? getOverScript()->format(ctxt) : 0;
      ctxt.pop();

      ctxt.push(this);
      AreaRef baseArea = getBase() ? getBase()->format(ctxt) : 0;
      ctxt.pop();

      AreaRef res;
      if (baseOp && baseOp->HasMovableLimits() && !ctxt.getDisplayStyle())
	{
	  res = ctxt.MGD()->script(ctxt, baseArea, underArea, Length(), overArea, Length());
	}
      else
	{
	  const scaled baseW = baseArea->box().width;
	  const scaled underW = underArea ? underArea->box().width : scaled::zero();
	  const scaled overW = overArea ? overArea->box().width : scaled::zero();

	  const scaled w = std::max(baseW, std::max(underW, overW));

	  if (baseOp && baseOp->IsStretchy())
	    {
	      ctxt.push(this);
	      ctxt.setStretchToWidth(w);
	      ctxt.setStretchOperator(baseOp);
	      getBase()->setDirtyLayout();
	      baseArea = getBase()->format(ctxt);
	      ctxt.pop();
	    }
	  
	  if (underOp && underOp->IsStretchy())
	    {
	      ctxt.push(this);
	      ctxt.setDisplayStyle(false);
	      if (!accentUnder) ctxt.addScriptLevel(1);
	      ctxt.setStretchToWidth(w);
	      ctxt.setStretchOperator(underOp);
	      getUnderScript()->setDirtyLayout();
	      underArea = getUnderScript()->format(ctxt);
	      ctxt.pop();
	    }

	  if (overOp && overOp->IsStretchy())
	    {
	      ctxt.push(this);
	      ctxt.setDisplayStyle(false);
	      if (!accent) ctxt.addScriptLevel(1);
	      ctxt.setStretchToWidth(w);
	      ctxt.setStretchOperator(overOp);
	      getOverScript()->setDirtyLayout();
	      overArea = getOverScript()->format(ctxt);
	      ctxt.pop();
	    }

	  ctxt.push(this);
	  res = ctxt.MGD()->underOver(ctxt, baseArea,
					    underArea, accentUnder,
					    overArea, accent);
	  ctxt.pop();
	}

      ctxt.push(this);
      res = formatEmbellishment(this, ctxt, res);
      setArea(ctxt.MGD()->wrapper(ctxt, res));
      ctxt.pop();

      resetDirtyLayout();
    }

  return getArea();
}

SmartPtr<MathMLOperatorElement>
MathMLUnderOverElement::getCoreOperator()
{
  return getBase() ? getBase()->getCoreOperator() : 0;
}

void
MathMLUnderOverElement::setDirtyAttribute()
{
  setDirtyAttributeD();
}

void
MathMLUnderOverElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  base.setFlagDown(f);
  underScript.setFlagDown(f);
  overScript.setFlagDown(f);
}

void
MathMLUnderOverElement::resetFlagDown(Flags f)
{
  MathMLContainerElement::resetFlagDown(f);
  base.resetFlagDown(f);
  underScript.resetFlagDown(f);
  overScript.resetFlagDown(f);
}
