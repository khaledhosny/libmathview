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

#include "Globals.hh"
#include "ChildList.hh"
#include "MathMLRowElement.hh"
#include "MathMLFencedElement.hh"
#include "MathMLOperatorElement.hh"
#include "RenderingEnvironment.hh"
#include "ValueConversion.hh"

MathMLFencedElement::MathMLFencedElement()
{
  normalized = false;
}

#if defined(HAVE_GMETADOM)
MathMLFencedElement::MathMLFencedElement(const DOM::Element& node)
  : MathMLBinContainerElement(node)
{
  normalized = false;
}
#endif

MathMLFencedElement::~MathMLFencedElement()
{
}

const AttributeSignature*
MathMLFencedElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_OPEN,       fenceParser,      "(",  NULL },
    { ATTR_CLOSE,      fenceParser,      ")",  NULL },
    { ATTR_SEPARATORS, separatorsParser, ",",  NULL },
    { ATTR_NOTVALID,   NULL,             NULL, NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLBinContainerElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLFencedElement::Normalize(const SmartPtr<MathMLDocument>&)
{
  if (DirtyStructure())
    {
      // normalization is delayed after setup, because it depends on
      // the value of some attributes
      // maybe it can be optimized if it knows that none of its children were
      // added or removed, because that way it can just propagate the
      // normalization method
      normalized = false;
      ResetDirtyStructure();
    }
}

void
MathMLFencedElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      if (SmartPtr<Value> value = GetAttributeValue(ATTR_OPEN, env))
	openFence = ToString(value);
      else
	openFence.clear();

      if (SmartPtr<Value> value = GetAttributeValue(ATTR_CLOSE, env))
	closeFence = ToString(value);
      else
	closeFence.clear();

      SmartPtr<Value> value;
      if (GetDOMElement() && GetDOMElement().hasAttribute("separators"))
	value = GetAttributeValue(ATTR_SEPARATORS, env, false);
      else
	value = GetAttributeValue(ATTR_SEPARATORS, env);
      if (value) separators = ToString(value);
      else separators.clear();

      DelayedNormalize(env.GetDocument());
      MathMLBinContainerElement::Setup(env);

      ResetDirtyAttribute();
    }
}

void
MathMLFencedElement::DelayedNormalize(const SmartPtr<MathMLDocument>& doc)
{
  if (!normalized)
    {
#if defined(HAVE_GMETADOM)
      ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
      unsigned nChildren = children.get_length();

      for (unsigned i = 0; i < nChildren; i++)
	{
	  DOM::Node node = children.item(i);
	  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
	  SmartPtr<MathMLElement> elem = doc->getFormattingNode(node);
	  assert(elem);
	  // we detach the element from its parent, which can be an
	  // element created by mfenced when it expanded
	  elem->Unlink();
	}
#endif // HAVE_GMETADOM

      SmartPtr<MathMLRowElement> mainRow = smart_cast<MathMLRowElement>(MathMLRowElement::create());
      assert(mainRow);

      SmartPtr<MathMLRowElement> mrow = 0;
      SmartPtr<MathMLOperatorElement> fence = 0;

      if (!openFence.empty())
	{
	  fence = smart_cast<MathMLOperatorElement>(MathMLOperatorElement::create());
	  assert(fence);
	  fence->Append(openFence);
	  fence->SetFence();
	  mainRow->Append(fence);
	}

#if defined(HAVE_GMETADOM)
      bool moreArguments = nChildren > 1;

      if (moreArguments) mrow = smart_cast<MathMLRowElement>(MathMLRowElement::create());
      else mrow = mainRow;
      assert(mrow);

      for (unsigned i = 0; i < nChildren; i++)
	{
	  DOM::Node node = children.item(i);
	  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
	  SmartPtr<MathMLElement> arg = doc->getFormattingNode(node);
	  assert(arg);

	  mrow->Append(arg);

	  if (!separators.empty() && i < nChildren - 1)
	    {
	      unsigned offset = (i < separators.length()) ? i : separators.length() - 1;
	      String sep = separators.substr(offset, 1);

	      SmartPtr<MathMLOperatorElement> separator = smart_cast<MathMLOperatorElement>(MathMLOperatorElement::create());
	      assert(separator);
	      separator->SetSeparator();
	      separator->Append(sep);
	      mrow->Append(separator);
	    }
	}

      if (moreArguments) mainRow->Append(mrow);
#endif // HAVE_GMETADOM

      if (!closeFence.empty())
	{
	  fence = smart_cast<MathMLOperatorElement>(MathMLOperatorElement::create());
	  assert(fence);
	  fence->Append(closeFence);
	  fence->SetFence();
	  mainRow->Append(fence);
	}

      SetChild(mainRow);
      mainRow->Normalize(doc);
      // the mainRow will typically have the dirtyStructure flag set,
      // hence we have to clean it again
      ResetDirtyStructure();

      normalized = true;
    }
}
