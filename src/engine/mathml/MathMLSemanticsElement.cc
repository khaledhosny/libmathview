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
#include "MathMLDummyElement.hh"
#include "MathMLElementFactory.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLSemanticsElement.hh"
#include "MathMLNamespaceContext.hh"
#include "defs.h"

MathMLSemanticsElement::MathMLSemanticsElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLBinContainerElement(context)
{ }

MathMLSemanticsElement::~MathMLSemanticsElement()
{ }

#if 0
void
MathMLSemanticsElement::construct(AbstractConstructionContext& ctxt)
{
  if (dirtyStructure())
    {
      ctxt.enter();
      if (SmartPtr<MathMLElement> e = ctxt.getElement(getChild(), false))
	setChild(e);
      else
	{
	  while (
	}

      ctxt.exit();
      resetDirtyStructure();
    }
}
#endif

void
MathMLSemanticsElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  assert(IsA() == T_SEMANTICS);
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");

	  if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(0)))
	    setChild(e);
	  else
	    {
	      ChildList children(getDOMElement(), MATHML_NS_URI, "annotation-xml");
	      for (unsigned i = 0; i < children.get_length(); i++)
		{
		  DOM::Element elem = children.item(i);
		  assert(elem);
		  String encoding = elem.getAttribute("encoding");
		  if (encoding == "MathML-Presentation")
		    {
		      ChildList children(elem, MATHML_NS_URI, "*");
		      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(0)))
			setChild(e);
		      else if (!is_a<MathMLDummyElement>(getChild()))
			setChild(getFactory()->createDummyElement());
		      break;
		    }
		  else if (encoding == "BoxML")
		    {
		      ChildList children(elem, BOXML_NS_URI, "*");
		      // FIXME: this should be a BoxMLElement
		      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(0)))
			setChild(e);
		      else if (!is_a<MathMLDummyElement>(getChild()))
			setChild(getFactory()->createDummyElement());
		      break;
		    }
		}
	      if (!is_a<MathMLDummyElement>(getChild()))
		setChild(getFactory()->createDummyElement());
	    }
	}
#endif // HAVE_GMETADOM

      if (getChild()) getChild()->construct();

      resetDirtyStructure();
    }
}

SmartPtr<MathMLOperatorElement>
MathMLSemanticsElement::getCoreOperator()
{
  return getChild() ? getChild()->getCoreOperator() : 0;
}
