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

#include <algorithm>
#include <functional>

#include "defs.h"
#include "for_each_if.h"
#include "Adaptors.hh"
#include "ChildList.hh"
#include "MathMLLinearContainerElement.hh"

MathMLLinearContainerElement::MathMLLinearContainerElement(const SmartPtr<MathMLView>& view)
  : MathMLContainerElement(view)
{ }

MathMLLinearContainerElement::~MathMLLinearContainerElement()
{ }

#if 0
void
MathMLLinearContainerElement::construct(AbstractConstructionContext& ctxt)
{
  if (dirtyStructure())
    {
      unsigned i = 0;
      while (SmartPtr<Element> elem = ctxt.getElement(getChild(i), false))
	setChild(i++, elem);
      setSize(i);
      resetDirtyStructure();
    }
}
#endif

void
MathMLLinearContainerElement::construct()
{
  if (dirtyStructure())
    {
      // editing is supported with DOM only
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  unsigned n = children.get_length();

	  std::vector< SmartPtr<MathMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      DOM::Node node = children.item(i);
	      assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);

	      if (SmartPtr<MathMLElement> elem = getFormattingNode(node))
		newContent.push_back(elem);
	      else
		{
		  // it might be that we get a NULL. In that case it would probably make
		  // sense to create a dummy element, because we filtered MathML
		  // elements only
		}
	    }
	  content.swapContent(newContent);
	}
#endif // HAVE_GMETADOM
      
      // it is better to normalize elements only after all the rendering
      // interfaces have been collected, because the structure might change
      // depending on the actual number of children
      for_each_if(content.begin(), content.end(), NotNullPredicate(), ConstructAdaptor());
      resetDirtyStructure();
    }
}

void
MathMLLinearContainerElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      for_each_if(content.begin(), content.end(), NotNullPredicate(), std::bind2nd(RefineAdaptor(), &context));
      MathMLContainerElement::refine(context);
    }
}

void
MathMLLinearContainerElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  content.setFlagDown(f);
}

void
MathMLLinearContainerElement::resetFlagDown(Flags f)
{
  MathMLContainerElement::resetFlagDown(f);
  content.resetFlagDown(f);
}
