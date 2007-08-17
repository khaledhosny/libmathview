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
#include "MathMLScriptElement.hh"
#include "ValueConversion.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLAttributeSignatures.hh"

MathMLScriptElement::MathMLScriptElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLContainerElement(context)
{ }

MathMLScriptElement::~MathMLScriptElement()
{ }

AreaRef
MathMLScriptElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      assert(getBase());
      AreaRef baseArea = getBase()->format(ctxt);

      ctxt.addScriptLevel(1);
      ctxt.setDisplayStyle(false);

      Length subScriptShift;
      AreaRef subScriptArea;
      if (getSubScript())
	{
	  subScriptArea = getSubScript()->format(ctxt);

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Script, subscriptshift))
	    {
	      assert(IsLength(value));
	      subScriptShift = ToLength(value);
	    }
	}

      Length superScriptShift;
      AreaRef superScriptArea;
      if (getSuperScript())
	{
	  superScriptArea = getSuperScript()->format(ctxt);

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Script, superscriptshift))
	    {
	      assert(IsLength(value));
	      superScriptShift = ToLength(value);
	    }
	}

      AreaRef res = ctxt.MGD()->script(ctxt, baseArea,
					     subScriptArea, subScriptShift,
					     superScriptArea, superScriptShift);
      res = formatEmbellishment(this, ctxt, res);
      setArea(ctxt.MGD()->wrapper(ctxt, res));

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

SmartPtr<class MathMLOperatorElement>
MathMLScriptElement::getCoreOperator()
{
  return getBase() ? getBase()->getCoreOperator() : 0;
}

void
MathMLScriptElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  base.setFlagDown(f);
  subScript.setFlagDown(f);
  superScript.setFlagDown(f);
}

void
MathMLScriptElement::resetFlagDown(Flags f)
{
  MathMLContainerElement::resetFlagDown(f);
  base.resetFlagDown(f);
  subScript.resetFlagDown(f);
  superScript.resetFlagDown(f);
}
