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
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLLabeledTableRowElement.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLTableElement.hh"
#include "MathMLTableRowElement.hh"
#include "MathMLView.hh"
#include "ValueConversion.hh"

MathMLLabeledTableRowElement::MathMLLabeledTableRowElement(const SmartPtr<MathMLView>& view)
  : MathMLTableRowElement(view)
{ }

MathMLLabeledTableRowElement::~MathMLLabeledTableRowElement()
{ }

void
MathMLLabeledTableRowElement::construct()
{
  if (dirtyStructure())
    {
      MathMLTableRowElement::construct();

#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  if (children.item(0) && nodeLocalName(children.item(0)) != "mtr")
	    {
	      SmartPtr<MathMLElement> elem = getFormattingNode(children.item(0));
	      assert(elem);
	      SetLabel(elem);
	    }
	  else if (!is_a<MathMLDummyElement>(label))
	    SetLabel(0);
	}
#endif // HAVE_GMETADOM

      if (!label) SetLabel(getFactory()->createDummyElement(getView()));
      label->construct();

      resetDirtyStructure();
    }
}

#if 0
void
MathMLLabeledTableRowElement::Setup(RenderingEnvironment& env)
{
  SetupAux(env, true);
}

SmartPtr<MathMLElement>
MathMLLabeledTableRowElement::Inside(const scaled& x, const scaled& y)
{
  SmartPtr<MathMLElement> inside;
  if (inside = MathMLTableRowElement::Inside(x, y)) return inside;
  if (label && (inside = label->Inside(x, y))) return inside;
  return 0;
}
#endif

void
MathMLLabeledTableRowElement::setFlagDown(Flags f)
{
  MathMLLinearContainerElement::setFlagDown(f);
  label.setFlagDown(f);
}

void
MathMLLabeledTableRowElement::resetFlagDown(Flags f)
{
  MathMLLinearContainerElement::resetFlagDown(f);
  label.resetFlagDown(f);
}
