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

#include "Globals.hh"
#include "MathMLAttributeSignatures.hh"
#include "MathMLActionElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "ValueConversion.hh"

MathMLActionElement::MathMLActionElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLLinearContainerElement(context)
{
  selection = 0;
}

MathMLActionElement::~MathMLActionElement()
{ }

AreaRef
MathMLActionElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Action, selection))
	selection = ToInteger(value) - 1;
      else
	selection = 0;

      if (SmartPtr<Value> vAction = GET_ATTRIBUTE_VALUE(MathML, Action, actiontype))
	{
	  String action = ToString(vAction);

	  if (action == "toggle")
	    {
	      // nothing to do, selecting the element is just fine
	      // but we want to have cycling
	      selection %= getSize();
	    }
	  else
	    Globals::logger(LOG_WARNING, "action `%s' is not supported (ignored)", action.c_str());
	}
      else
	Globals::logger(LOG_WARNING, "no action specified for `maction' element");

      AreaRef res;
      if (SmartPtr<MathMLElement> elem = getChild(selection))
	res = elem->format(ctxt);
      else
	res = ctxt.getDevice()->dummy(ctxt);
      assert(res);

      setArea(ctxt.getDevice()->wrapper(ctxt, res));
      ctxt.pop();

      resetDirtyLayout();
    }

  return getArea();
}

SmartPtr<MathMLOperatorElement>
MathMLActionElement::getCoreOperator()
{
  if (selection < getSize())
    if (SmartPtr<MathMLElement> elem = getChild(selection))
      return elem->getCoreOperator();
  return 0;
}
