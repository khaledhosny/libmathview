// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <assert.h>

#include "ChildList.hh"
#include "MathMLDocument.hh"
#include "MathMLDummyElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLSemanticsElement.hh"

MathMLSemanticsElement::MathMLSemanticsElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLSemanticsElement::MathMLSemanticsElement(const DOM::Element& node)
  : MathMLBinContainerElement(node)
{
}
#endif

MathMLSemanticsElement::~MathMLSemanticsElement()
{
}

void
MathMLSemanticsElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  assert(IsA() == TAG_SEMANTICS);
	  ChildList children(GetDOMElement(), MATHML_NS_URI, "*");

	  if (Ptr<MathMLElement> e = doc->getFormattingNode(children.item(0)))
	    SetChild(e);
	  else
	    {
	      ChildList children(GetDOMElement(), MATHML_NS_URI, "annotation-xml");
	      for (unsigned i = 0; i < children.get_length(); i++)
		{
		  DOM::Element elem = children.item(i);
		  assert(elem);
		  if (elem.getAttribute("encoding") == "MathML-Presentation")
		    {
		      ChildList children(elem, MATHML_NS_URI, "*");
		      if (Ptr<MathMLElement> e = doc->getFormattingNode(children.item(0)))
			SetChild(e);
		      else if (!is_a<MathMLDummyElement>(GetChild()))
			SetChild(MathMLDummyElement::create());
		      break;
		    }
		}
	      if (!is_a<MathMLDummyElement>(GetChild()))
		SetChild(MathMLDummyElement::create());
	    }
	}
#endif

      if (GetChild()) GetChild()->Normalize(doc);

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
MathMLSemanticsElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (GetChild()) GetChild()->SetPosition(x, y);
}

#if 0
bool
MathMLSemanticsElement::IsExpanding() const
{
  assert(content.GetSize() == 1);
  assert(content.GetFirst());
  return content.GetFirst()->IsExpanding();
}
#endif

Ptr<MathMLOperatorElement>
MathMLSemanticsElement::GetCoreOperator()
{
  if (GetChild()) return GetChild()->GetCoreOperator();
  else return 0;
}
