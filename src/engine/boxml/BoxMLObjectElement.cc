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
#include "BoxFormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "BoxMLNamespaceContext.hh"
#include "View.hh"
#include "NamespaceRegistry.hh"

#include "MathMLElement.hh"
#include "MathFormattingContext.hh"
#include "MathMLNamespaceContext.hh"
#include "MathGraphicDevice.hh"

BoxMLObjectElement::BoxMLObjectElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLElement(context)
{ }

BoxMLObjectElement::~BoxMLObjectElement()
{ }

SmartPtr<BoxMLObjectElement>
BoxMLObjectElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLObjectElement(context); }

void
BoxMLObjectElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  DOM::Node p = getDOMElement().get_firstChild();
	  while (p && p.get_nodeType() != DOM::Node::ELEMENT_NODE)
	    p = p.get_nextSibling();

	  if (p)
	    if (SmartPtr<NamespaceContext> contentContext =
		getNamespaceContext()->getView()->getRegistry()->get(p.get_namespaceURI()))
	      setChild(contentContext->construct(p));
	    else
	      // cannot handle this namespace
	      setChild(0);
	  else
	    // found no child
	    setChild(0);
	}
#endif // HAVE_GMETADOM

      resetDirtyStructure();
    }
}

void
BoxMLObjectElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      if (SmartPtr<MathMLElement> child = smart_cast<MathMLElement>(getChild()))
	child->refine(context);
      BoxMLElement::refine(context);
    }
}

AreaRef
BoxMLObjectElement::format(BoxFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      if (SmartPtr<MathMLElement> child = smart_cast<MathMLElement>(getChild()))
	{
	  SmartPtr<MathMLNamespaceContext> contentContext = child->getMathMLNamespaceContext();
	  assert(contentContext);

	  MathFormattingContext mCtxt(contentContext->getGraphicDevice());
	  mCtxt.setSize(ctxt.getSize());
	  mCtxt.setActualSize(ctxt.getSize());
	  mCtxt.setColor(ctxt.getColor());
	  mCtxt.setBackground(ctxt.getBackground());

	  AreaRef res = child->format(mCtxt);
	  res = ctxt.getDevice()->wrapper(ctxt, res);
	  setArea(res);
	}
      else
	setArea(ctxt.getDevice()->dummy(ctxt));
      
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
