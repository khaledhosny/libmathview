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

#include "ChildList.hh"
#include "Globals.hh"
#include "MathMLDummyElement.hh"
#include "MathMLElementFactory.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLUnderOverElement.hh"
#include "MathMLView.hh"
#include "ValueConversion.hh"
#include "scaledConv.hh"
#include "traverseAux.hh"
#include "MathGraphicDevice.hh"
#include "MathFormattingContext.hh"
#include "MathMLAttributeSignatures.hh"

MathMLUnderOverElement::MathMLUnderOverElement(const SmartPtr<class MathMLView>& view)
  : MathMLContainerElement(view)
{ }

MathMLUnderOverElement::~MathMLUnderOverElement()
{ }

void
MathMLUnderOverElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  assert(IsA() == T_MUNDER || IsA() == T_MOVER || IsA() == T_MUNDEROVER);
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  
	  if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(0)))
	    setBase(e);
	  else if (!is_a<MathMLDummyElement>(getBase()))
	    setBase(getFactory()->createDummyElement());

	  switch (IsA())
	    {
	    case T_MUNDER:
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		setUnderScript(e);
	      else if (!is_a<MathMLDummyElement>(getUnderScript()))
		setUnderScript(getFactory()->createDummyElement());
	      setOverScript(0);
	      break;
	    case T_MOVER:
	      setUnderScript(0);
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		setOverScript(e);
	      else if (!is_a<MathMLDummyElement>(getOverScript()))
		setOverScript(getFactory()->createDummyElement());
	      break;
	    case T_MUNDEROVER:
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		setUnderScript(e);
	      else if (!is_a<MathMLDummyElement>(getUnderScript()))
		setUnderScript(getFactory()->createDummyElement());
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(2)))
		setOverScript(e);
	      else if (!is_a<MathMLDummyElement>(getOverScript()))
		setOverScript(getFactory()->createDummyElement());
	      break;
	    default:
	      assert(0);
	    }
	}
#endif // HAVE_GMETADOM

      assert(getBase());
      getBase()->construct();
      if (getUnderScript()) getUnderScript()->construct();
      if (getOverScript()) getOverScript()->construct();

      resetDirtyStructure();
    }
}

void
MathMLUnderOverElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, UnderOver, accentunder);
      REFINE_ATTRIBUTE(context, MathML, UnderOver, accent);
      if (getBase()) getBase()->refine(context);
      if (getUnderScript()) getUnderScript()->refine(context);
      if (getOverScript()) getOverScript()->refine(context);
      MathMLContainerElement::refine(context);
    }
}

AreaRef
MathMLUnderOverElement::format(MathFormattingContext& ctxt)
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
	  res = ctxt.getDevice()->script(ctxt, baseArea, underArea, Length(), overArea, Length());
	}
      else
	{
	  const scaled baseW = baseArea->box().width;
	  const scaled underW = underArea ? underArea->box().width : scaled::zero();
	  const scaled overW = overArea ? overArea->box().width : scaled::zero();

	  const scaled w = std::max(baseW, std::max(underW, overW));

	  if (baseOp)
	    {
	      ctxt.push(this);
	      ctxt.setStretchToWidth(w);
	      ctxt.setStretchOperator(baseOp);
	      baseArea = getBase() ? getBase()->format(ctxt) : 0;
	      ctxt.pop();
	    }
	  
	  if (underOp)
	    {
	      ctxt.push(this);
	      ctxt.setStretchToWidth(w);
	      ctxt.setStretchOperator(underOp);
	      underArea = getUnderScript() ? getUnderScript()->format(ctxt) : 0;
	      ctxt.pop();
	    }

	  if (overOp)
	    {
	      ctxt.push(this);
	      ctxt.setStretchToWidth(w);
	      ctxt.setStretchOperator(overOp);
	      overArea = getOverScript() ? getOverScript()->format(ctxt) : 0;
	      ctxt.pop();
	    }

	  res = ctxt.getDevice()->underOver(ctxt, baseArea,
					    underArea, accentUnder,
					    overArea, accent);
	}

      res = formatEmbellishment(this, ctxt, res);
      setArea(ctxt.getDevice()->wrapper(ctxt, res));

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
