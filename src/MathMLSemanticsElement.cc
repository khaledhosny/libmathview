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
#include "MathMLDocument.hh"
#include "MathMLDummyElement.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLSemanticsElement.hh"
#include "MathMLView.hh"
#include "defs.h"

MathMLSemanticsElement::MathMLSemanticsElement(const SmartPtr<class MathMLView>& view)
  : MathMLBinContainerElement(view)
{
}

MathMLSemanticsElement::~MathMLSemanticsElement()
{
}

void
MathMLSemanticsElement::construct()
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  assert(IsA() == TAG_SEMANTICS);
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");

	  if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(0)))
	    SetChild(e);
	  else
	    {
	      ChildList children(getDOMElement(), MATHML_NS_URI, "annotation-xml");
	      for (unsigned i = 0; i < children.get_length(); i++)
		{
		  DOM::Element elem = children.item(i);
		  assert(elem);
		  if (elem.getAttribute("encoding") == "MathML-Presentation")
		    {
		      ChildList children(elem, MATHML_NS_URI, "*");
		      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(0)))
			SetChild(e);
		      else if (!is_a<MathMLDummyElement>(GetChild()))
			SetChild(getFactory()->createDummyElement(getView()));
		      break;
		    }
		}
	      if (!is_a<MathMLDummyElement>(GetChild()))
		SetChild(getFactory()->createDummyElement(getView()));
	    }
	}
#endif

      if (GetChild()) GetChild()->construct();

      ResetDirtyStructure();
    }
}

void
MathMLSemanticsElement::DoLayout(const FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      MathMLBinContainerElement::DoLayout(ctxt);
      DoEmbellishmentLayout(this, box);
      ResetDirtyLayout(ctxt);
    }
}

void
MathMLSemanticsElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (GetChild()) GetChild()->SetPosition(x, y);
}

SmartPtr<MathMLOperatorElement>
MathMLSemanticsElement::GetCoreOperator()
{
  if (GetChild()) return GetChild()->GetCoreOperator();
  else return 0;
}
