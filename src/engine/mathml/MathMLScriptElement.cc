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

#include "MathMLOperatorElement.hh"
#include "MathMLScriptElement.hh"
#include "ValueConversion.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLAttributeSignatures.hh"

MathMLScriptElement::MathMLScriptElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLContainerElement(context)
{ }

MathMLScriptElement::~MathMLScriptElement()
{ }

AreaRef
MathMLScriptElement::format(MathFormattingContext& ctxt)
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

      AreaRef res = ctxt.getDevice()->script(ctxt, baseArea,
					     subScriptArea, subScriptShift,
					     superScriptArea, superScriptShift);
      res = formatEmbellishment(this, ctxt, res);
      setArea(ctxt.getDevice()->wrapper(ctxt, res));

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
