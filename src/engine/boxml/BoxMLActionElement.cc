// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include "for_each_if.h"
#include "Adapters.hh"
#include "AbstractLogger.hh"
#include "BoxMLAttributeSignatures.hh"
#include "ValueConversion.hh"
#include "BoxMLActionElement.hh"
#include "BoxFormattingContext.hh"
#include "BoxGraphicDevice.hh"

BoxMLActionElement::BoxMLActionElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLLinearContainerElement(context)
{ }

BoxMLActionElement::~BoxMLActionElement()
{ }

SmartPtr<BoxMLActionElement>
BoxMLActionElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLActionElement(context); }

AreaRef
BoxMLActionElement::format(BoxFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(BoxML, Action, selection))
	selection = ToInteger(value) - 1;
      else
	selection = 0;

      if (SmartPtr<Value> vAction = GET_ATTRIBUTE_VALUE(BoxML, Action, actiontype))
	{
	  String action = ToString(vAction);

	  if (action == "toggle")
	    {
	      // nothing to do, selecting the element is just fine
	      // but we want to have cycling
	      selection %= getSize();
	    }
	  else
	    getLogger()->out(LOG_WARNING, "action `%s' is not supported (ignored)", action.c_str());
	}
      else
	getLogger()->out(LOG_WARNING, "no action specified for `maction' element");

      AreaRef res;
      if (SmartPtr<BoxMLElement> elem = getChild(selection))
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

SmartPtr<BoxMLElement>
BoxMLActionElement::getSelectedElement() const
{
  return (selection < content.getSize()) ? content.getChild(selection) : SmartPtr<BoxMLElement>(0);
}

void
BoxMLActionElement::setSelectedIndex(unsigned i)
{
  assert(false);
  if (content.getSize() > 0 && selection != (i - 1) % content.getSize())
    {
      selection = (i - 1) % content.getSize();
      if (SmartPtr<BoxMLElement> elem = getSelectedElement())
	{
	  elem->setDirtyLayout();
	  if (elem->dirtyAttribute() || elem->dirtyAttributeP()) this->setDirtyAttribute();
	}
      // has to set dirtyLayout itself because if the children hasn't been visited yet
      // then its Dirtylayout flag is still set and it won't be propagated up
      setDirtyLayout();
    }
}

unsigned
BoxMLActionElement::getSelectedIndex() const
{
  return (content.getSize() > 0) ? selection + 1 : 0;
}

scaled
BoxMLActionElement::getStep() const
{
  if (SmartPtr<BoxMLElement> elem = getSelectedElement())
    return elem->getStep();
  else
    return scaled::zero();
}
