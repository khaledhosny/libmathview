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
#include "ChildList.hh"
#include "MathMLDummyElement.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLRadicalElement.hh"
#include "MathMLView.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLRadicalElement::MathMLRadicalElement(const SmartPtr<class MathMLView>& view)
  : MathMLContainerElement(view)
{ }

MathMLRadicalElement::~MathMLRadicalElement()
{ }

void
MathMLRadicalElement::setBase(const SmartPtr<MathMLElement>& elem)
{
  if (elem != base)
    {
      if (elem) elem->setParent(this);
      base = elem;
      setDirtyLayout();
    }
}

void
MathMLRadicalElement::setIndex(const SmartPtr<MathMLElement>& elem)
{
  if (elem != index)
    {
      if (elem) elem->setParent(this);
      index = elem;
      setDirtyLayout();
    }
}

void
MathMLRadicalElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  if (IsA() == T_MSQRT)
	    {
	      ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	      if (children.get_length() == 1)
		{
		  DOM::Node node = children.item(0);
		  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
		  setBase(getFormattingNode(node));
		}
	      else
		setBase(getFormattingNode(getDOMElement()));

	      setIndex(0);
	    }
	  else
	    {
	      assert(IsA() == T_MROOT);	      
	      ChildList children(getDOMElement(), MATHML_NS_URI, "*");

	      switch (children.get_length())
		{
		case 0:
		  setBase(getFactory()->createDummyElement(getView()));
		  setIndex(getFactory()->createDummyElement(getView()));
		  break;
		case 1:
		  setBase(getFormattingNode(children.item(0)));
		  setIndex(getFactory()->createDummyElement(getView()));
		  break;
		default:
		  setBase(getFormattingNode(children.item(0)));
		  setIndex(getFormattingNode(children.item(1)));
		  break;
		}
	    }
	}
#endif

      if (base) base->construct();
      if (index) index->construct();

      resetDirtyStructure();
    }
}

void
MathMLRadicalElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      if (base) base->refine(context);
      if (index) index->refine(context);
      MathMLContainerElement::refine(context);
    }
}

AreaRef
MathMLRadicalElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      AreaRef baseArea = base->format(ctxt);
      AreaRef indexArea;
      if (index)
	{
	  ctxt.setDisplayStyle(false);
	  ctxt.addScriptLevel(2);
	  indexArea = index->format(ctxt);
	}
      AreaRef res = ctxt.getDevice()->radical(ctxt, baseArea, indexArea);

      setArea(ctxt.getDevice()->wrapper(ctxt, res));

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLRadicalElement::setFlagDown(Flags f)
{
  MathMLElement::setFlagDown(f);
  if (base) base->setFlagDown(f);
  if (index) index->setFlagDown(f);
}

void
MathMLRadicalElement::resetFlagDown(Flags f)
{
  MathMLElement::resetFlagDown(f);
  if (base) base->resetFlagDown(f);
  if (index) index->resetFlagDown(f);
}

