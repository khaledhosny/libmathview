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
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLDummyElement.hh"
#include "MathMLElementFactory.hh"
#include "MathMLNormalizingContainerElement.hh"
#include "MathMLRowElement.hh"
#include "MathMLView.hh"
#include "defs.h"

MathMLNormalizingContainerElement::MathMLNormalizingContainerElement(const SmartPtr<class MathMLView>& view)
  : MathMLBinContainerElement(view)
{ }

MathMLNormalizingContainerElement::~MathMLNormalizingContainerElement()
{ }

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
	  setChild(elem);
	}
      else 
	{
	  SmartPtr<MathMLRowElement> row;
	  if (getChild() && is_a<MathMLRowElement>(getChild()) && !getChild()->getDOMElement())
	    // this must be an inferred mrow
	    row = smart_cast<MathMLRowElement>(getChild());
	  else
	    row = smart_cast<MathMLRowElement>(getFactory()->createRowElement());
	  assert(row && !row->getDOMElement());
	  setChild(row);

	  std::vector< SmartPtr<MathMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      SmartPtr<MathMLElement> elem = getFormattingNode(children.item(i));
	      assert(elem);
	      newContent.push_back(elem);
	    }

	  row->swapContent(newContent);
	}
#else
      if (!getChild()) setChild(MathMLRowElement::create());
#endif

      assert(getChild());
      getChild()->construct();

      resetDirtyStructure();
    }
}

AreaRef
MathMLNormalizingContainerElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      AreaRef res = getChild() ? getChild()->format(ctxt) : 0;
      if (res) res = ctxt.getDevice()->wrapper(ctxt, res);
      setArea(res);
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLNormalizingContainerElement::setDirtyStructure()
{
  MathMLBinContainerElement::setDirtyStructure();
  // if the structure of this element gets dirty, and there is
  // an inferred mrow, then the mrow itself has a dirty strcuture,
  // even if it has no corresponding DOM element
  if (getChild() && !getChild()->getDOMElement() && is_a<MathMLRowElement>(getChild()))
    getChild()->setDirtyStructure();
}
