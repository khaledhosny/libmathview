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

MathMLActionElement::MathMLActionElement(const SmartPtr<class MathMLView>& view)
  : MathMLLinearContainerElement(view)
{
  selection = 0;
}

MathMLActionElement::~MathMLActionElement()
{
}

void
MathMLActionElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
   {
     REFINE_ATTRIBUTE(context, MathML, Action, actiontype);
     REFINE_ATTRIBUTE(context, MathML, Action, selection);
     MathMLLinearContainerElement::refine(context);
   }
}

#if 0
void
MathMLActionElement::Setup(RenderingEnvironment& env)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Action, actiontype))
	{
	  String action = ToString(value);
	  if (action != "toggle")
	    Globals::logger(LOG_WARNING, "action `%s' is not supported (ignored)", action.c_str());
	}
      else
	Globals::logger(LOG_WARNING, "no action specified for `maction' element");

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Action, selection))
	SetSelectedIndex(ToInteger(value));

      if (SmartPtr<MathMLElement> elem = GetSelectedElement()) elem->Setup(env);
      //MathMLLinearContainerElement::Setup(env);

      resetDirtyAttribute();
    }
}

void
MathMLActionElement::DoLayout(const class FormattingContext& ctxt)
{
  if (dirtyLayout(ctxt))
    {
      if (SmartPtr<MathMLElement> elem = GetSelectedElement())
	{
	  elem->DoLayout(ctxt);
	  box = elem->GetBoundingBox();
	}
      else
	box.unset();

      DoEmbellishmentLayout(this, box);

      resetDirtyLayout(ctxt);
    }
}
#endif

AreaRef
MathMLActionElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      AreaRef res;

      ctxt.push(this);
      if (SmartPtr<Value> vAction = GET_ATTRIBUTE_VALUE(MathML, Action, actiontype))
	{
	  String action = ToString(vAction);
	  
	  if (action == "toggle")
	    {
	      int selection = ToInteger(GET_ATTRIBUTE_VALUE(MathML, Action, selection));
	      if (selection > 0 && selection < getSize())
		{
		  SmartPtr<MathMLElement> elem = getChild(selection - 1);
		  assert(elem);
		  res = elem->format(ctxt);
		}
	    }
	  else
	    {
	      Globals::logger(LOG_WARNING, "action `%s' is not supported (ignored)", action.c_str());
	      if (getSize() > 0)
		{
		  SmartPtr<MathMLElement> elem = getChild(0);
		  assert(elem);
		  res = elem->format(ctxt);
		}
	    }
	}
      else
	Globals::logger(LOG_WARNING, "no action specified for `maction' element");
      setArea(res ? ctxt.getDevice()->wrapper(ctxt, res) : 0);
      ctxt.pop();

      resetDirtyLayout();
    }

  return getArea();
}

#if 0
void
MathMLActionElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;
  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (SmartPtr<MathMLElement> elem = GetSelectedElement()) elem->SetPosition(x, y);
}

void
MathMLActionElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      if (SmartPtr<MathMLElement> elem = GetSelectedElement()) elem->Render(area);
      ResetDirty();
    }
}
#endif

SmartPtr<MathMLElement>
MathMLActionElement::GetSelectedElement() const
{
  return (selection < getSize()) ? getChild(selection) : SmartPtr<MathMLElement>(0);
}

void
MathMLActionElement::SetSelectedIndex(unsigned i)
{
  if (getSize() > 0 && selection != (i - 1) % getSize())
    {
      selection = (i - 1) % getSize();
      if (SmartPtr<MathMLElement> elem = GetSelectedElement())
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
MathMLActionElement::GetSelectedIndex() const
{
  return (getSize() > 0) ? selection + 1 : 0;
}

#if 0
scaled
MathMLActionElement::GetLeftEdge() const
{
  if (SmartPtr<MathMLElement> elem = GetSelectedElement())
    return elem->GetLeftEdge();
  else
    return GetX();
}

scaled
MathMLActionElement::GetRightEdge() const
{
  if (SmartPtr<MathMLElement> elem = GetSelectedElement())
    return elem->GetRightEdge();
  else
    return GetX();
}

SmartPtr<MathMLElement>
MathMLActionElement::Inside(const scaled& x, const scaled& y)
{
  if (IsInside(x, y))
    {
      if (SmartPtr<MathMLElement> elem = GetSelectedElement())
	return elem->Inside(x, y);
      else
	return this;
    }
  else
    return 0;
}
#endif

SmartPtr<MathMLOperatorElement>
MathMLActionElement::getCoreOperator()
{
  if (SmartPtr<MathMLElement> elem = GetSelectedElement())
    return elem->getCoreOperator();
  else
    return 0;
}
