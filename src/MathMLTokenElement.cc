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

#include "BoundingBoxAux.hh"
#include "FormattingContext.hh"
#include "Globals.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLGlyphNode.hh"
#include "MathMLIdentifierElement.hh"
#include "MathMLMarkNode.hh"
#include "MathMLNumberElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLSpaceNode.hh"
#include "MathMLStringNode.hh"
#include "MathMLTextElement.hh"
#include "MathMLTextNode.hh"
#include "MathMLTokenElement.hh"
#include "RenderingEnvironment.hh"
#include "ValueConversion.hh"
#include "frameAux.hh"
#include "mathVariantAux.hh"
#include "traverseAux.hh"

MathMLTokenElement::MathMLTokenElement(const SmartPtr<class MathMLView>& view)
  : MathMLElement(view)
{
}

MathMLTokenElement::~MathMLTokenElement()
{
}

void
MathMLTokenElement::SetSize(unsigned i)
{
  assert(i <= content.size());
  while (i < content.size()) RemoveChild(content.size() - 1);
  content.reserve(i);
}

SmartPtr<MathMLTextNode>
MathMLTokenElement::GetChild(unsigned i) const
{
  if (i < content.size()) return content[i];
  else return 0;
}

void
MathMLTokenElement::SetChild(unsigned i, const SmartPtr<MathMLTextNode>& child)
{
  assert(i < content.size());
  assert(child);
  if (content[i] != child)
    {
      assert(!child->GetParent());
      content[i]->Unlink();
      child->SetParent(this);
      content[i] = child;
      SetDirtyLayout();
    }
}

void
MathMLTokenElement::Append(const String& s)
{
  SmartPtr<MathMLTextNode> node = MathMLStringNode::create(s);
  AppendChild(node);
}

void
MathMLTokenElement::AppendChild(const SmartPtr<MathMLTextNode>& node)
{
  assert(node);
  assert(!node->GetParent());
  node->SetParent(this);
  content.push_back(node);
  SetDirtyLayout();
}

void
MathMLTokenElement::RemoveChild(unsigned i)
{
  assert(i < content.size());
  content[i]->Unlink();
  content.erase(content.begin() + i);
  SetDirtyLayout();
}

void
MathMLTokenElement::InsertChild(unsigned i, const SmartPtr<MathMLTextNode>& node)
{
  assert(i <= content.size());
  assert(node);
  assert(!node->GetParent());
  node->SetParent(this);
  content.insert(content.begin() + i, node);
  SetDirtyLayout();
}

void
MathMLTokenElement::SwapChildren(std::vector< SmartPtr<MathMLTextNode> >& newContent)
{
  if (newContent != content)
    {
      // reset parent should be done first because the same elements
      // may be present in the following loop as well
      for (std::vector< SmartPtr<MathMLTextNode> >::iterator p = content.begin();
	   p != content.end();
	   p++)
	(*p)->Unlink();

      for (std::vector< SmartPtr<MathMLTextNode> >::iterator p = newContent.begin();
	   p != newContent.end();
	   p++)
	{
	  assert(*p);
	  (*p)->SetParent(this);
	}

      content.swap(newContent);

      SetDirtyLayout();
    }
}

void
MathMLTokenElement::construct()
{
  if (DirtyStructure() && getDOMElement())
    {
#if defined(HAVE_GMETADOM)
      content.clear();

      for (DOM::Node p = getDOMElement().get_firstChild(); 
	   p;
	   p = p.get_nextSibling()) 
	{
	  switch (p.get_nodeType())
	    {
	    case DOM::Node::TEXT_NODE:
	      {
		// ok, we have a chunk of text
		String content = collapseSpaces(fromDOMString(p.get_nodeValue()));

		// ...but spaces at the at the beginning (end) are deleted only if this
		// is the very first (last) chunk in the token.
		if (!p.get_previousSibling()) content = trimSpacesLeft(content);
		if (!p.get_nextSibling()) content = trimSpacesRight(content);

		Append(content);
	      }
	    break;

	    case DOM::Node::ELEMENT_NODE:
	      {	    
		if (p.get_namespaceURI() == MATHML_NS_URI)
		  {
		    if (nodeLocalName(p) == "mglyph")
		      {
			if (SmartPtr<MathMLTextNode> text = SubstituteMGlyphElement(p))
			  AppendChild(text);
		      }
		    else if (nodeLocalName(p) == "malignmark")
		      {
			if (SmartPtr<MathMLTextNode> text = SubstituteAlignMarkElement(p))
			  AppendChild(text);
		      }
		    else
		      {
			std::string s_name = nodeLocalName(p);
			Globals::logger(LOG_WARNING, "element `%s' inside token (ignored)\n", s_name.c_str());
		      }
		  } 
		else
		  {
		    std::string s_name = p.get_nodeName();
		    Globals::logger(LOG_WARNING, "element `%s' inside token (ignored)\n", s_name.c_str());
		  }
	      }
	    break;
	  
	    default:
	      break;
	    }
	}
#endif // HAVE_GMETADOM

      ResetDirtyStructure();
    }
}

void
MathMLTokenElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, Token, fontsize);
      REFINE_ATTRIBUTE(context, Token, fontweight);
      REFINE_ATTRIBUTE(context, Token, fontstyle);
      REFINE_ATTRIBUTE(context, Token, fontfamily);
      REFINE_ATTRIBUTE(context, Token, color);
      REFINE_ATTRIBUTE(context, Token, mathvariant);
      REFINE_ATTRIBUTE(context, Token, mathsize);
      REFINE_ATTRIBUTE(context, Token, mathcolor);
      REFINE_ATTRIBUTE(context, Token, mathbackground);
      MathMLElement::refine(context);
    }
}

void
MathMLTokenElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute())
    {
      env.Push();

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, mathsize))
	{
	  if (IsSet(T_FONTSIZE))
	    Globals::logger(LOG_WARNING, "attribute `mathsize' overrides deprecated attribute `fontsize'");

	  if (IsTokenId(value))
	    switch (ToTokenId(value))
	      {
	      case T_SMALL: env.AddScriptLevel(1); break;
	      case T_BIG: env.AddScriptLevel(-1); break;
	      case T_NORMAL: break; // noop
	      default: assert(IMPOSSIBLE); break;
	      }
	  else
	    env.SetFontSize(ToLength(value));
	} 
      else if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, fontsize))
	{
	  Globals::logger(LOG_WARNING, "the attribute `fontsize' is deprecated in MathML 2");
	  env.SetFontSize(ToLength(value));
	}
  
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, mathvariant))
	{
	  assert(IsTokenId(value));

	  const MathVariantAttributes& attr = attributesOfVariant(ToTokenId(value));
	  assert(attr.kw != T__NOTVALID);

	  if (IsSet(T_FONTFAMILY) || IsSet(T_FONTWEIGHT) || IsSet(T_FONTSTYLE))
	    Globals::logger(LOG_WARNING, "attribute `mathvariant' overrides deprecated font-related attributes");
	}
      else
	{
	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, fontfamily))
	    {
	      Globals::logger(LOG_WARNING, "the attribute `fontfamily' is deprecated in MathML 2");
	    }

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, fontweight))
	    {
	      Globals::logger(LOG_WARNING, "the attribute `fontweight' is deprecated in MathML 2");
	    }

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, fontstyle))
	    {
	      Globals::logger(LOG_WARNING, "the attribute `fontstyle' is deprecated in MathML 2");
	    } 
	  else if (is_a<MathMLIdentifierElement>(SmartPtr<MathMLElement>(this)))
	    {
	    }
	}
      
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, mathcolor))
	{
	  if (IsSet(T_COLOR))
	    Globals::logger(LOG_WARNING, "attribute `mathcolor' overrides deprecated attribute `color'");
	  env.SetColor(ToRGB(value));
	} 
      else if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, color))
	{
	  Globals::logger(LOG_WARNING, "attribute `color' is deprecated in MathML 2");
	  env.SetColor(ToRGB(value));
	} 
      else
	if (HasLink()) env.SetColor(Globals::configuration.GetLinkForeground());

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Token, mathbackground))
	env.SetBackgroundColor(ToRGB(value));
      else if (HasLink() && !Globals::configuration.HasTransparentLinkBackground())
	env.SetBackgroundColor(Globals::configuration.GetLinkBackground());

      color      = env.GetColor();
      background = env.GetBackgroundColor();
      sppm       = env.GetScaledPointsPerEm();

      for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator p = GetContent().begin();
	   p != GetContent().end();
	   p++)
	{
	  assert(*p);
	  (*p)->Setup(env);
	}

      env.Drop();

      ResetDirtyAttribute();
    }
}

void
MathMLTokenElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      box.unset();
      for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator text = GetContent().begin();
	   text != GetContent().end();
	   text++)
	{
	  assert(*text);
	  if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_MIN) (*text)->DoLayout(ctxt);

	  // if we do not insert MathMLSpaceNodes in the layout, they will not be
	  // positioned correctly, since positioning is done thru the layout.
	  // In such way, If a space node is the first inside a token, it will produce
	  // a zero-origin rectangle which is obviously incorrect
	  box.append((*text)->GetBoundingBox());
	  box.width += ((sppm * (*text)->GetSpacing()) / 18);
	}

      AddItalicCorrection();

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLTokenElement::SetPosition(const scaled& x, const scaled& y)
{
  MathMLElement::SetPosition(x, y);
  SetContentPosition(x, y);
}

void
MathMLTokenElement::SetContentPosition(const scaled& x0, const scaled& y)
{
  scaled x = x0;

  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator text = GetContent().begin(); 
       text != GetContent().end();
       text++)
    {
      assert(*text);
      (*text)->SetPosition(x, y);
      x += (*text)->GetBoundingBox().width;
      x += (sppm * (*text)->GetSpacing()) / 18;
    }
}

void
MathMLTokenElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      RenderBackground(area);

      if (fGC[Selected()] == NULL)
	{
	  GraphicsContextValues values;

	  values.foreground = Selected() ? area.GetSelectionForeground() : color;
	  values.background = Selected() ? area.GetSelectionBackground() : background;
	  fGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
	}

      for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator text = GetContent().begin();
	   text != GetContent().end();
	   text++)
	{
	  assert(*text);
	  (*text)->Render(area);
	}

      ResetDirty();
    }
}

scaled
MathMLTokenElement::GetLeftEdge() const
{
  scaled edge = 0;

  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator text = GetContent().begin();
       text != GetContent().end();
       text++)
    {
      assert(*text);
      if (text == GetContent().begin()) edge = (*text)->GetLeftEdge();
      else edge = std::min(edge, (*text)->GetLeftEdge());
    }

  return edge;
}

scaled
MathMLTokenElement::GetRightEdge() const
{
  scaled edge = 0;

  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator text = GetContent().begin();
       text != GetContent().end();
       text++)
    {
      assert(*text);
      if (text == GetContent().begin()) edge = (*text)->GetRightEdge();
      else edge = std::max(edge, (*text)->GetRightEdge());
    }

  return edge;
}

scaled
MathMLTokenElement::GetDecimalPointEdge() const
{
  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator text = GetContent().begin();
       text != GetContent().end();
       text++)
    {
      assert(*text);
      if ((*text)->HasDecimalPoint()) return (*text)->GetDecimalPointEdge();
    }

  return GetRightEdge();
}

bool
MathMLTokenElement::IsNonMarking() const
{
  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator text = GetContent().begin();
       text != GetContent().end();
       text++)
    {
      assert(*text);
      if (!is_a<MathMLSpaceNode>(*text)) return false;
    }

  return true;
}

void
MathMLTokenElement::AddItalicCorrection()
{
  if (!is_a<MathMLIdentifierElement>(SmartPtr<MathMLElement>(this)) &&
      !is_a<MathMLNumberElement>(SmartPtr<MathMLElement>(this)) &&
      !is_a<MathMLTextElement>(SmartPtr<MathMLElement>(this))) return;
  
  if (GetSize() == 0) return;

  SmartPtr<MathMLTextNode> lastNode = GetChild(GetSize() - 1);
  assert(lastNode);

  SmartPtr<MathMLElement> next = findRightSibling(this);
  if (!next) return;

  SmartPtr<MathMLOperatorElement> coreOp = next->GetCoreOperatorTop();
  if (!coreOp) return;
  bool isFence = coreOp->IsFence();
  if (!isFence) return;
}

SmartPtr<MathMLTextNode>
MathMLTokenElement::SubstituteMGlyphElement(const DOM::Element& node)
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
MathMLTokenElement::SubstituteAlignMarkElement(const DOM::Element& node)
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
  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator i = GetContent().begin();
       i != GetContent().end();
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

  for (std::vector< SmartPtr<MathMLTextNode> >::const_iterator i = GetContent().begin();
       i != GetContent().end();
       i++)
    {
      assert(*i);
      len += (*i)->GetLogicalContentLength();
    }

  return len;
}
