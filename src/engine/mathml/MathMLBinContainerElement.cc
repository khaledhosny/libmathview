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
#include "MathMLBinContainerElement.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLBinContainerElement::MathMLBinContainerElement(const SmartPtr<class MathMLView>& view)
  : MathMLContainerElement(view)
{ }

MathMLBinContainerElement::~MathMLBinContainerElement()
{ }

void
MathMLBinContainerElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      ChildList children(getDOMElement(), MATHML_NS_URI, "*");
      if (children.get_length() > 0)
	{
	  DOM::Node node = children.item(0);
	  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
	  SmartPtr<MathMLElement> elem = getFormattingNode(node);
	  assert(elem);
	  setChild(elem);
	}
#endif // HAVE_GMETADOM

      if (getChild()) getChild()->construct();
      resetDirtyStructure();
    }
}

void
MathMLBinContainerElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      if (getChild()) getChild()->refine(context);
      MathMLContainerElement::refine(context);
    }  
}

AreaRef
MathMLBinContainerElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      AreaRef res = getChild() ? getChild()->format(ctxt) : 0;
      setArea(res ? ctxt.getDevice()->wrapper(ctxt, res) : 0);
      ctxt.pop();

      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLBinContainerElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  content.setFlagDown(f);
}

void
MathMLBinContainerElement::resetFlagDown(Flags f)
{
    MathMLContainerElement::resetFlagDown(f);
    content.resetFlagDown(f);
}
