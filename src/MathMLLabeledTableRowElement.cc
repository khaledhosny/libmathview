// Copyright (C) 2000-2002, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <assert.h>
#include <stddef.h>

#include "ChildList.hh"
#include "Globals.hh"
#include "StringUnicode.hh"
#include "ValueConversion.hh"
#include "MathMLDocument.hh"
#include "MathMLDummyElement.hh"
#include "MathMLTableElement.hh"
#include "MathMLTableRowElement.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLLabeledTableRowElement.hh"

MathMLLabeledTableRowElement::MathMLLabeledTableRowElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLLabeledTableRowElement::MathMLLabeledTableRowElement(const DOM::Element& node)
  : MathMLTableRowElement(node)
{
}
#endif

MathMLLabeledTableRowElement::~MathMLLabeledTableRowElement()
{
}

void
MathMLLabeledTableRowElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
      MathMLTableRowElement::Normalize(doc);

#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
	  if (children.item(0) && nodeLocalName(children.item(0)) != "mtr")
	    {
	      Ptr<MathMLElement> elem = doc->getFormattingNode(children.item(0));
	      assert(elem);
	      SetLabel(elem);
	    }
	  else if (!is_a<MathMLDummyElement>(label))
	    SetLabel(0);
	}
#endif // HAVE_GMETADOM

      if (!label) SetLabel(MathMLDummyElement::create());
      label->Normalize(doc);

      ResetDirtyStructure();
    }
}

void
MathMLLabeledTableRowElement::Setup(RenderingEnvironment& env)
{
  SetupAux(env, true);
}

Ptr<MathMLElement>
MathMLLabeledTableRowElement::Inside(scaled x, scaled y)
{
  Ptr<MathMLElement> inside;
  if (inside = MathMLTableRowElement::Inside(x, y)) return inside;
  if (label && (inside = label->Inside(x, y))) return inside;
  return 0;
}

#if 0
void
MathMLLabeledTableRowElement::SetDirty(const Rectangle* rect)
{
  MathMLTableRowElement::SetDirty(rect);
  if (label) label->SetDirty(rect);
}
#endif

void
MathMLLabeledTableRowElement::SetFlagDown(Flags f)
{
  MathMLLinearContainerElement::SetFlagDown(f);
  if (label) label->SetFlagDown(f);
}

void
MathMLLabeledTableRowElement::ResetFlagDown(Flags f)
{
  MathMLLinearContainerElement::ResetFlagDown(f);
  if (label) label->ResetFlagDown(f);  
}

Ptr<MathMLElement>
MathMLLabeledTableRowElement::GetLabel(void) const
{
  return label;
}

void
MathMLLabeledTableRowElement::SetLabel(const Ptr<MathMLElement>& elem)
{
  if (elem != label)
    {
      if (label) label->SetParent(0);
      if (elem) elem->SetParent(this);
      label = elem;
      SetDirtyLayout();
    }
}
