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
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLRowElement.hh"
#include "MathMLView.hh"
#include "RenderingEnvironment.hh"
#include "ValueConversion.hh"

MathMLFencedElement::MathMLFencedElement(const SmartPtr<class MathMLView>& view)
  : MathMLBinContainerElement(view)
{
  normalized = false;
}

MathMLFencedElement::~MathMLFencedElement()
{
}

void
MathMLFencedElement::construct()
{
  if (DirtyStructure())
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

	      if (i + 1 < nArgs) content.push_back(getFactory()->createOperatorElement(getView()));
	    }
	}
#endif // HAVE_GMETADOM

      SmartPtr<MathMLRowElement> outerRow = smart_cast<MathMLRowElement>(getFactory()->createRowElement(getView()));
      outerRow->Append(getFactory()->createOperatorElement(getView()));
      if (content.size() == 1) outerRow->Append(content[0]);
      else if (content.size() > 1)
	{
	  SmartPtr<MathMLRowElement> innerRow = smart_cast<MathMLRowElement>(getFactory()->createRowElement(getView()));
	  innerRow->SwapChildren(content);
	  outerRow->Append(innerRow);
	}
      outerRow->Append(getFactory()->createOperatorElement(getView()));

      SetChild(outerRow);
      outerRow->construct();

      ResetDirtyStructure();
    }
}

void
MathMLFencedElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, Fenced, open);
      REFINE_ATTRIBUTE(context, Fenced, close);
      REFINE_ATTRIBUTE(context, Fenced, separators);

      String openFence = ToString(GET_ATTRIBUTE_VALUE(Fenced, open));
      String closeFence = ToString(GET_ATTRIBUTE_VALUE(Fenced, close));
      String separators = ToString(GET_ATTRIBUTE_VALUE(Fenced, separators));

      SmartPtr<MathMLRowElement> outerRow = smart_cast<MathMLRowElement>(GetChild());
      assert(outerRow);

      SmartPtr<MathMLOperatorElement> open = smart_cast<MathMLOperatorElement>(outerRow->GetChild(0));
      assert(open);
      open->SetSize(0);
      open->Append(openFence);
      open->SetFence();

      if (nArgs > 1)
	{
	  SmartPtr<MathMLRowElement> innerRow = smart_cast<MathMLRowElement>(outerRow->GetChild(1));
	  assert(innerRow && !innerRow->getDOMElement());
	  for (unsigned i = 0; i < nArgs - 1; i++)
	    {
	      if (separators.empty())
		innerRow->SetChild(i * 2 + 1, 0);
	      else
		{
		  SmartPtr<MathMLOperatorElement> sep = smart_cast<MathMLOperatorElement>(innerRow->GetChild(i * 2 + 1));
		  unsigned offset = (i < separators.length()) ? i : separators.length() - 1;
		  sep->SetSize(0);
		  sep->Append(separators.substr(offset, 1));
		  sep->SetSeparator();
		}
	    }
	}

      SmartPtr<MathMLOperatorElement> close = smart_cast<MathMLOperatorElement>(outerRow->GetChild((nArgs > 0) ? 2 : 1));
      assert(close);
      close->SetSize(0);
      close->Append(closeFence);
      close->SetFence();

      MathMLBinContainerElement::refine(context);
    }
}

#if 0
void
MathMLFencedElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Fenced, open))
	openFence = ToString(value);
      else
	openFence.clear();

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Fenced, close))
	closeFence = ToString(value);
      else
	closeFence.clear();

      SmartPtr<Value> value;
      if (getDOMElement() && getDOMElement().hasAttribute("separators"))
	value = GET_ATTRIBUTE_VALUE(Fenced, separators);
      else
	value = GET_ATTRIBUTE_VALUE(Fenced, separators);
      if (value) separators = ToString(value);
      else separators.clear();

      MathMLBinContainerElement::Setup(env);

      ResetDirtyAttribute();
    }
}
#endif
