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

#include "Globals.hh"
#include "for_each_if.h"
#include "Adaptors.hh"
#include "BoxMLAttributeSignatures.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "ValueConversion.hh"
#include "BoxMLActionElement.hh"

BoxMLActionElement::BoxMLActionElement(const SmartPtr<View>& view)
  : BoxMLLinearContainerElement(view)
{
}

BoxMLActionElement::~BoxMLActionElement()
{
}

SmartPtr<BoxMLActionElement>
BoxMLActionElement::create(const SmartPtr<View>& view)
{
  return new BoxMLActionElement(view);
}

void
BoxMLActionElement::refine(class AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, BoxML, Action, selection);
      REFINE_ATTRIBUTE(context, BoxML, Action, actiontype);
      BoxMLLinearContainerElement::refine(context);
    }
}

AreaRef
BoxMLActionElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      AreaRef res;
      if (SmartPtr<Value> vAction = GET_ATTRIBUTE_VALUE(BoxML, Action, actiontype))
	{
	  String action = ToString(vAction);
	  
	  if (action == "toggle")
	    {
	      int selection = ToInteger(GET_ATTRIBUTE_VALUE(BoxML, Action, selection));
	      if (selection > 0 && selection <= getSize())
		{
		  SmartPtr<BoxMLElement> elem = getChild(selection - 1);
		  assert(elem);
		  res = elem->format(ctxt);
		}
	    }
	  else
	    {
	      Globals::logger(LOG_WARNING, "action `%s' is not supported (ignored)", action.c_str());
	      if (getSize() > 0)
		{
		  SmartPtr<BoxMLElement> elem = getChild(0);
		  assert(elem);
		  res = elem->format(ctxt);
		}
	    }
	}
      else
	Globals::logger(LOG_WARNING, "no action specified for `maction' element");
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
