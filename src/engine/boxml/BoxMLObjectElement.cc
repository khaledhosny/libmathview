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

#include "defs.h"
#include "ChildList.hh"
#include "BoxMLObjectElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

BoxMLObjectElement::BoxMLObjectElement(const SmartPtr<View>& view)
  : BoxMLElement(view)
{ }

BoxMLObjectElement::~BoxMLObjectElement()
{ }

SmartPtr<BoxMLObjectElement>
BoxMLObjectElement::create(const SmartPtr<View>& view)
{
  return new BoxMLObjectElement(view);
}

void
BoxMLObjectElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  // Beware that ChildList will select text nodes if the namespace "*" 
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  if (DOM::Node node = children.item(0))
	    {
	      assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
	      SmartPtr<Element> el = getFormattingNode(node);
	      setChild(el);
	    }
	  else
	    setChild(0);
	}
#endif // HAVE_GMETADOM

      if (SmartPtr<Element> child = getChild())
	child->construct();

      resetDirtyStructure();
    }
}

void
BoxMLObjectElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      if (SmartPtr<Element> child = getChild())
	child->refine(context);
      BoxMLElement::refine(context);
    }
}

AreaRef
BoxMLObjectElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      if (SmartPtr<Element> child = getChild())
	setArea(ctxt.getDevice()->wrapper(ctxt, child->format(ctxt)));
      else
	setArea(ctxt.getDevice()->dummy(ctxt));
      
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
