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

#include "ChildList.hh"
#include "Globals.hh"
#include "MathMLDummyElement.hh"
#include "MathMLFencedElement.hh"
#include "MathMLElementFactory.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLRowElement.hh"
#include "ValueConversion.hh"
#include "MathMLAttributeSignatures.hh"

MathMLFencedElement::MathMLFencedElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLBinContainerElement(context)
{ }

MathMLFencedElement::~MathMLFencedElement()
{
}

void
MathMLFencedElement::construct()
{
  if (dirtyStructure())
    {
      std::vector< SmartPtr<MathMLElement> > content;

#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  nArgs = children.get_length();

	  if (nArgs > 1) content.reserve(2 * nArgs - 1);
	  for (unsigned i = 0; i < nArgs; i++)
	    {
	      DOM::Node node = children.item(i);
	      assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);

	      content.push_back(getFormattingNode(node));

	      if (i + 1 < nArgs) content.push_back(getFactory()->createOperatorElement());
	    }
	}
#endif // HAVE_GMETADOM

      SmartPtr<MathMLRowElement> outerRow = smart_cast<MathMLRowElement>(getFactory()->createRowElement());
      outerRow->appendChild(getFactory()->createOperatorElement());
      if (content.size() == 1) outerRow->appendChild(content[0]);
      else if (content.size() > 1)
	{
	  SmartPtr<MathMLRowElement> innerRow = smart_cast<MathMLRowElement>(getFactory()->createRowElement());
	  innerRow->swapContent(content);
	  outerRow->appendChild(innerRow);
	}
      outerRow->appendChild(getFactory()->createOperatorElement());

      setChild(outerRow);
      outerRow->construct();

      resetDirtyStructure();
    }
}

void
MathMLFencedElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, Fenced, open);
      REFINE_ATTRIBUTE(context, MathML, Fenced, close);
      REFINE_ATTRIBUTE(context, MathML, Fenced, separators);

      String openFence = ToString(GET_ATTRIBUTE_VALUE(MathML, Fenced, open));
      String closeFence = ToString(GET_ATTRIBUTE_VALUE(MathML, Fenced, close));
      String separators = ToString(GET_ATTRIBUTE_VALUE(MathML, Fenced, separators));

      SmartPtr<MathMLRowElement> outerRow = smart_cast<MathMLRowElement>(getChild());
      assert(outerRow);

      SmartPtr<MathMLOperatorElement> open = smart_cast<MathMLOperatorElement>(outerRow->getChild(0));
      assert(open);
      open->setSize(0);
      open->append(openFence);
      open->SetFence();

      if (nArgs > 1)
	{
	  SmartPtr<MathMLRowElement> innerRow = smart_cast<MathMLRowElement>(outerRow->getChild(1));
	  assert(innerRow && !innerRow->getDOMElement());
	  for (unsigned i = 0; i < nArgs - 1; i++)
	    {
	      if (separators.empty())
		innerRow->setChild(i * 2 + 1, 0);
	      else
		{
		  SmartPtr<MathMLOperatorElement> sep = smart_cast<MathMLOperatorElement>(innerRow->getChild(i * 2 + 1));
		  unsigned offset = (i < separators.length()) ? i : separators.length() - 1;
		  sep->setSize(0);
		  sep->append(separators.substr(offset, 1));
		  sep->SetSeparator();
		}
	    }
	}

      SmartPtr<MathMLOperatorElement> close = smart_cast<MathMLOperatorElement>(outerRow->getChild((nArgs > 0) ? 2 : 1));
      assert(close);
      close->setSize(0);
      close->append(closeFence);
      close->SetFence();

      MathMLBinContainerElement::refine(context);
    }
}
