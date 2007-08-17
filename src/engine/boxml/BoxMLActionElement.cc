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

#include "for_each_if.h"
#include "Adapters.hh"
#include "AbstractLogger.hh"
#include "BoxMLAttributeSignatures.hh"
#include "ValueConversion.hh"
#include "BoxMLActionElement.hh"
#include "FormattingContext.hh"
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
BoxMLActionElement::format(FormattingContext& ctxt)
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

      if (SmartPtr<BoxMLElement> elem = getChild(selection))
	{
	  elem->format(ctxt);
	  AreaRef res = ctxt.BGD()->wrapper(ctxt, elem->getMaxArea());
	  setMaxArea(res);
	  if (res->box().width > ctxt.getAvailableWidth())
	    res = ctxt.BGD()->wrapper(ctxt, elem->getArea());
	  setArea(res);
	}
      else
	setArea(ctxt.BGD()->wrapper(ctxt, ctxt.BGD()->dummy(ctxt)));

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
