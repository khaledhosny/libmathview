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
#include <stddef.h>

#include "ChildList.hh"
#include "MathMLDocument.hh"
#include "MathMLRowElement.hh"
#include "MathMLDummyElement.hh"
#include "MathMLNormalizingContainerElement.hh"
#include "FormattingContext.hh"

MathMLNormalizingContainerElement::MathMLNormalizingContainerElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLNormalizingContainerElement::MathMLNormalizingContainerElement(const DOM::Element& node)
  : MathMLBinContainerElement(node)
{
}
#endif

MathMLNormalizingContainerElement::~MathMLNormalizingContainerElement()
{
}

void
MathMLNormalizingContainerElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
      unsigned n = children.get_length();
      if (n == 1)
	{
	  DOM::Node node = children.item(0);
	  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
	  Ptr<MathMLElement> elem = doc->getFormattingNode(node);
	  assert(elem);
	  SetChild(elem);
	}
      else 
	{
	  Ptr<MathMLRowElement> row;
	  if (GetChild() && is_a<MathMLRowElement>(GetChild()) && !GetChild()->GetDOMElement())
	    // this must be an inferred mrow
	    row = smart_cast<MathMLRowElement>(GetChild());
	  else
	    row = smart_cast<MathMLRowElement>(MathMLRowElement::create());
	  assert(row && !row->GetDOMElement());
	  SetChild(row);

	  std::vector< Ptr<MathMLElement> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      Ptr<MathMLElement> elem = doc->getFormattingNode(children.item(i));
	      assert(elem);
	      newContent.push_back(elem);
	    }

	  row->SwapChildren(newContent);
	}
#else
      if (!GetChild()) SetChild(MathMLRowElement::create());
#endif

      assert(GetChild());
      GetChild()->Normalize(doc);

      ResetDirtyStructure();
    }
}

void
MathMLNormalizingContainerElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      if (child)
	{
	  child->DoLayout(ctxt);
	  box = child->GetBoundingBox();
	}
      else
	box.Null();
      ResetDirtyLayout(ctxt);
    }
}

void
MathMLNormalizingContainerElement::Render(const DrawingArea& area)
{
  if (Dirty())
    {
      RenderBackground(area);
      if (child) child->Render(area);
      ResetDirty();
    }
}

void
MathMLNormalizingContainerElement::SetDirtyStructure()
{
  MathMLBinContainerElement::SetDirtyStructure();
  // if the structure of this element gets dirty, and there is
  // an inferred mrow, then the mrow itself has a dirty strcuture,
  // even if it has no corresponding DOM element
  if (child && !child->GetDOMElement() && is_a<MathMLRowElement>(child))
    child->SetDirtyStructure();
}
