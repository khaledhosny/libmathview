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

#include "defs.h"
#include "for_each_if.h"
#include "Adaptors.hh"
#include "ChildList.hh"
#include "BoxMLLinearContainerElement.hh"

BoxMLLinearContainerElement::BoxMLLinearContainerElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLElement(context)
{ }

BoxMLLinearContainerElement::~BoxMLLinearContainerElement()
{ }

void
BoxMLLinearContainerElement::construct()
{
  if (dirtyStructure())
    {
      // editing is supported with DOM only
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  unsigned n = children.get_length();

	  //std::cout << "constructing linear box element " << n << std::endl;

	  std::vector< SmartPtr<BoxMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      DOM::Node node = children.item(i);
	      assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);

	      SmartPtr<MathMLElement> el = getFormattingNode(node);
	      //std::cout << "created formatting node " << is_a<MathMLElement>(el) << std::endl;
	      
	      if (SmartPtr<BoxMLElement> elem = smart_cast<BoxMLElement>(el))
		newContent.push_back(elem);
	    }

	  //std::cout << "finished constructing content A: " << newContent.size() << std::endl;
	  content.swapContent(this, newContent);
	  //std::cout << "finished constructing content B: " << content.getSize() << std::endl;
	}
#endif // HAVE_GMETADOM
      
      // it is better to normalize elements only after all the rendering
      // interfaces have been collected, because the structure might change
      // depending on the actual number of children
      content.for_each(ConstructAdaptor());
      resetDirtyStructure();
    }
}

void
BoxMLLinearContainerElement::refine(class AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      content.for_each(std::bind2nd(RefineAdaptor(), &context));
      BoxMLElement::refine(context);
    }
}
