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
#include "FormattingContext.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLDummyElement.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLNormalizingContainerElement.hh"
#include "MathMLRowElement.hh"
#include "MathMLView.hh"
#include "defs.h"

MathMLNormalizingContainerElement::MathMLNormalizingContainerElement(const SmartPtr<class MathMLView>& view)
  : MathMLBinContainerElement(view)
{
}

MathMLNormalizingContainerElement::~MathMLNormalizingContainerElement()
{
}

void
MathMLNormalizingContainerElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      ChildList children(getDOMElement(), MATHML_NS_URI, "*");
      unsigned n = children.get_length();
      if (n == 1)
	{
	  DOM::Node node = children.item(0);
	  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
	  SmartPtr<MathMLElement> elem = getFormattingNode(node);
	  assert(elem);
	  SetChild(elem);
	}
      else 
	{
	  SmartPtr<MathMLRowElement> row;
	  if (GetChild() && is_a<MathMLRowElement>(GetChild()) && !GetChild()->getDOMElement())
	    // this must be an inferred mrow
	    row = smart_cast<MathMLRowElement>(GetChild());
	  else
	    row = smart_cast<MathMLRowElement>(getFactory()->createRowElement(getView()));
	  assert(row && !row->getDOMElement());
	  SetChild(row);

	  std::vector< SmartPtr<MathMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      SmartPtr<MathMLElement> elem = getFormattingNode(children.item(i));
	      assert(elem);
	      newContent.push_back(elem);
	    }

	  row->SwapChildren(newContent);
	}
#else
      if (!GetChild()) SetChild(MathMLRowElement::create());
#endif

      assert(GetChild());
      GetChild()->construct();

      resetDirtyStructure();
    }
}

#if 0
void
MathMLNormalizingContainerElement::DoLayout(const class FormattingContext& ctxt)
{
  if (dirtyLayout(ctxt))
    {
      if (child)
	{
	  child->DoLayout(ctxt);
	  box = child->GetBoundingBox();
	}
      else
	box.unset();
      resetDirtyLayout(ctxt);
    }
}
#endif

AreaRef
MathMLNormalizingContainerElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      AreaRef res = child ? child->format(ctxt) : 0;
      if (res) res = ctxt.getDevice()->wrapper(ctxt, res);
      setArea(res);
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

#if 0
void
MathMLNormalizingContainerElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      RenderBackground(area);
      if (child) child->Render(area);
      ResetDirty();
    }
}
#endif

void
MathMLNormalizingContainerElement::setDirtyStructure()
{
  MathMLBinContainerElement::setDirtyStructure();
  // if the structure of this element gets dirty, and there is
  // an inferred mrow, then the mrow itself has a dirty strcuture,
  // even if it has no corresponding DOM element
  if (child && !child->getDOMElement() && is_a<MathMLRowElement>(child))
    child->setDirtyStructure();
}
