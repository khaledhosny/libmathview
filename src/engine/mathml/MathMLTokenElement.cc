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
#include "BoundingBoxAux.hh"
#include "Globals.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLElementFactory.hh"
#include "MathMLGlyphNode.hh"
#include "MathMLIdentifierElement.hh"
#include "MathMLMarkNode.hh"
#include "MathMLNumberElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLSpaceNode.hh"
#include "MathMLStringNode.hh"
#include "MathMLTextElement.hh"
#include "MathMLTokenElement.hh"
#include "ValueConversion.hh"
#include "mathVariantAux.hh"
#include "traverseAux.hh"
#include "MathMLAttributeSignatures.hh"

MathMLTokenElement::MathMLTokenElement(const SmartPtr<class MathMLView>& view)
  : MathMLElement(view)
{ }

MathMLTokenElement::~MathMLTokenElement()
{ }

void
MathMLTokenElement::append(const String& s)
{
  content.appendChild(this, MathMLStringNode::create(s));
}

void
MathMLTokenElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  ChildList children(getDOMElement(), "*", "*");
	  unsigned n = children.get_length();

	  std::vector<SmartPtr<MathMLTextNode> > newContent;
	  newContent.reserve(n);
	  for (unsigned i = 0; i < n; i++)
	    {
	      DOM::Node node = children.item(i);
	      switch (node.get_nodeType())
		{
		case DOM::Node::TEXT_NODE:
		  {
		    // ok, we have a chunk of text
		    String s = collapseSpaces(fromDOMString(node.get_nodeValue()));

		    // ...but spaces at the at the beginning (end) are deleted only if this
		    // is the very first (last) chunk in the token.
		    if (!node.get_previousSibling()) s = trimSpacesLeft(s);
		    if (!node.get_nextSibling()) s = trimSpacesRight(s);

		    newContent.push_back(MathMLStringNode::create(s));
		  }
		break;

		case DOM::Node::ELEMENT_NODE:
		  {	    
		    if (node.get_namespaceURI() == MATHML_NS_URI)
		      {
			if (nodeLocalName(node) == "mglyph")
			  {
			    if (SmartPtr<MathMLTextNode> text = substituteMGlyphElement(node))
			      newContent.push_back(text);
			  }
			else if (nodeLocalName(node) == "malignmark")
			  {
			    if (SmartPtr<MathMLTextNode> text = substituteAlignMarkElement(node))
			      newContent.push_back(text);
			  }
			else
			  {
			    std::string s_name = nodeLocalName(node);
			    Globals::logger(LOG_WARNING, "element `%s' inside token (ignored)\n", s_name.c_str());
			  }
		      } 
		    else
		      {
			std::string s_name = node.get_nodeName();
			Globals::logger(LOG_WARNING, "element `%s' inside token (ignored)\n", s_name.c_str());
		      }
		  }
		break;
		
		default:
		  break;
		}
	    }
	  swapContent(newContent);
	}
#endif // HAVE_GMETADOM

      resetDirtyStructure();
    }
}

void
MathMLTokenElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, Token, mathvariant);
      REFINE_ATTRIBUTE(context, MathML, Token, mathsize);
      REFINE_ATTRIBUTE(context, MathML, Token, mathcolor);
      REFINE_ATTRIBUTE(context, MathML, Token, mathbackground);
      MathMLElement::refine(context);
    }
}

AreaRef
MathMLTokenElement::formatAux(MathFormattingContext& ctxt)
{
  RGBColor oldColor = ctxt.getColor();
  RGBColor oldBackground = ctxt.getBackground();

  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Token, mathsize))
    {
      if (IsTokenId(value))
	switch (ToTokenId(value))
	  {
	  case T_SMALL: ctxt.addScriptLevel(1); break;
	  case T_BIG: ctxt.addScriptLevel(-1); break;
	  case T_NORMAL: break; // noop
	  default: assert(false); break;
	  }
      else
	ctxt.setSize(ctxt.getDevice()->evaluate(ctxt, ToLength(value), ctxt.getSize()));
    } 
  
  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Token, mathvariant))
    ctxt.setVariant(toMathVariant(value));
  else if (is_a<MathMLIdentifierElement>(SmartPtr<MathMLTokenElement>(this)) && GetLogicalContentLength() == 1)
    ctxt.setVariant(ITALIC_VARIANT);

  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Token, mathcolor))
    ctxt.setColor(ToRGB(value));
#if 0
  else
    if (hasLink()) ctxt.setColor(Globals::configuration.GetLinkForeground());
#endif

  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Token, mathbackground))
    ctxt.setBackground(ToRGB(value));
#if 0
  else if (hasLink() && !Globals::configuration.HasTransparentLinkBackground())
    ctxt.setBackground(Globals::configuration.GetLinkBackground());
#endif

  RGBColor newColor = ctxt.getColor();
  RGBColor newBackground = ctxt.getBackground();

  std::vector<AreaRef> c;
  c.reserve(getSize());
  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    c.push_back((*p)->format(ctxt));

  AreaRef res = ctxt.getDevice()->getFactory()->horizontalArray(c);

  if (oldColor != newColor)
    res = ctxt.getDevice()->getFactory()->color(res, newColor);

  if (!newBackground.transparent && newBackground != oldBackground)
    res = ctxt.getDevice()->getFactory()->background(res, newBackground);

  return res;
}

AreaRef
MathMLTokenElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      setArea(ctxt.getDevice()->wrapper(ctxt, formatAux(ctxt)));
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

bool
MathMLTokenElement::IsNonMarking() const
{
  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator text = content.begin();
       text != content.end();
       text++)
    {
      assert(*text);
      if (!is_a<MathMLSpaceNode>(*text)) return false;
    }

  return true;
}

#if 0
void
MathMLTokenElement::AddItalicCorrection()
{
  if (!is_a<MathMLIdentifierElement>(SmartPtr<MathMLElement>(this)) &&
      !is_a<MathMLNumberElement>(SmartPtr<MathMLElement>(this)) &&
      !is_a<MathMLTextElement>(SmartPtr<MathMLElement>(this))) return;
  
  if (getSize() == 0) return;

  SmartPtr<MathMLTextNode> lastNode = getChild(getSize() - 1);
  assert(lastNode);

  SmartPtr<MathMLElement> next = findRightSibling(this);
  if (!next) return;

  SmartPtr<MathMLOperatorElement> coreOp = next->getCoreOperatorTop();
  if (!coreOp) return;
  bool isFence = coreOp->IsFence();
  if (!isFence) return;
}
#endif

SmartPtr<MathMLTextNode>
MathMLTokenElement::substituteMGlyphElement(const DOM::Element& node)
{
  assert(node);

  DOM::GdomeString alt        = node.getAttribute("alt");
  DOM::GdomeString fontFamily = node.getAttribute("fontfamily");
  DOM::GdomeString index      = node.getAttribute("index");

  if (alt.empty() || fontFamily.empty() || index.empty()) {
    Globals::logger(LOG_WARNING, "malformed `mglyph' element (some required attribute is missing)\n");
    return MathMLStringNode::create("?");
  }

  SmartPtr<MathMLGlyphNode> glyph = MathMLGlyphNode::create(fromDOMString(fontFamily),
							    fromDOMString(index),
							    fromDOMString(alt));

  return glyph;
}

SmartPtr<MathMLTextNode>
MathMLTokenElement::substituteAlignMarkElement(const DOM::Element& node)
{
  assert(node);

  DOM::GdomeString edge = node.getAttribute("edge");

  TokenId align = T__NOTVALID;

  if (!edge.empty())
    {
      if      (edge == "left") align = T_LEFT;
      else if (edge == "right") align = T_RIGHT;
      else
	{
	  std::string s_edge = edge;
	  Globals::logger(LOG_WARNING,
			  "malformed `malignmark' element, attribute `edge' has invalid value `%s' (ignored)",
			  s_edge.c_str());
	}
    }

  return MathMLMarkNode::create(align);
}

String
MathMLTokenElement::GetRawContent() const
{
  String res;
  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator i = content.begin();
       i != content.end();
       i++)
    {
      assert(*i);
      res += (*i)->GetRawContent();
    }

  return res;
}

unsigned
MathMLTokenElement::GetLogicalContentLength() const
{
  unsigned len = 0;

  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator i = content.begin();
       i != content.end();
       i++)
    {
      assert(*i);
      len += (*i)->GetLogicalContentLength();
    }

  return len;
}
