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
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_WCTYPE_H
#include <wctype.h>
#endif
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#include "AFont.hh"
#include "frameAux.hh"
#include "stringAux.hh"
#include "Globals.hh"
#include "traverseAux.hh"
#include "StringFactory.hh"
#include "allocTextNode.hh"
#include "StringUnicode.hh"
#include "MathMLMarkNode.hh"
#include "MathMLCharNode.hh"
#include "MathMLCombinedCharNode.hh"
#include "MathMLTextNode.hh"
#include "mathVariantAux.hh"
#include "ValueConversion.hh"
#include "MathMLGlyphNode.hh"
#include "MathMLSpaceNode.hh"
#include "MathMLTextElement.hh"
#include "MathMLTokenElement.hh"
#include "MathMLNumberElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLIdentifierElement.hh"
#include "FormattingContext.hh"

MathMLTokenElement::MathMLTokenElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLTokenElement::MathMLTokenElement(const DOM::Element& node)
  : MathMLElement(node)
{
}
#endif

MathMLTokenElement::~MathMLTokenElement()
{
}

const AttributeSignature*
MathMLTokenElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_FONTSIZE,       numberUnitParser,  NULL, NULL },
    { ATTR_FONTWEIGHT,     fontWeightParser,  NULL, NULL },
    { ATTR_FONTSTYLE,      fontStyleParser,   NULL, NULL },
    { ATTR_FONTFAMILY,     stringParser,      NULL, NULL },
    { ATTR_COLOR,          colorParser,       NULL, NULL },
    { ATTR_MATHVARIANT,    mathVariantParser, NULL, NULL },
    { ATTR_MATHSIZE,       mathSizeParser,    NULL, NULL },
    { ATTR_MATHCOLOR,      colorParser,       NULL, NULL },
    { ATTR_MATHBACKGROUND, colorParser,       NULL, NULL },
    
    { ATTR_NOTVALID,       NULL,              NULL, NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLTokenElement::SetSize(unsigned i)
{
  assert(i <= content.size());
  while (i < content.size()) RemoveChild(content.size() - 1);
  content.reserve(i);
}

Ptr<MathMLTextNode>
MathMLTokenElement::GetChild(unsigned i) const
{
  if (i < content.size()) return content[i];
  else return 0;
}

void
MathMLTokenElement::SetChild(unsigned i, const Ptr<MathMLTextNode>& child)
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
MathMLTokenElement::Append(const String* s)
{
  assert(s != NULL);

  if (s->GetLength() == 0) return;

  Ptr<MathMLTextNode> last = 0;
  if (GetSize() > 0 && GetChild(GetSize() - 1)->IsText())
    {
      last = smart_cast<MathMLTextNode>(GetChild(GetSize() - 1));
      assert(last);
    }

  unsigned i = 0;
  unsigned sLength = s->GetLength();
  while (i < sLength)
    {
      Ptr<MathMLTextNode> node = 0;

      int spacing;
      unsigned len = isNonMarkingChar(*s, i, &spacing);
      if (len > 0)
	{
	  node = MathMLSpaceNode::create(spacing);
	  i += len;
	} 
      else if (i + 1 < sLength && isCombining(s->GetChar(i + 1)))
	{
	  node = allocCombinedCharNode(s->GetChar(i), s->GetChar(i + 1));
	  i += 2;
#if 0
	}
      else if (iswalnum(s->GetChar(i)))
	{
	  unsigned start = i;
	  while (i < sLength && iswalnum(s->GetChar(i))) i++;
	  assert(start < i);

	  const String* sText = allocString(*s, start, i - start);
	  node = allocTextNode(&sText);
#endif
	}
      else if (!isVariant(s->GetChar(i)))
	{
	  node = allocCharNode(s->GetChar(i));
	  i++;
	}
      else
	{
	  Globals::logger(LOG_WARNING, "ignoring variant modifier char U+%04x", s->GetChar(i));
	  i++;
	}
    
      if (node)
	{
	  AppendChild(node);
	  last = node;
	}
    }
}

void
MathMLTokenElement::AppendChild(const Ptr<MathMLTextNode>& node)
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
MathMLTokenElement::InsertChild(unsigned i, const Ptr<MathMLTextNode>& node)
{
  assert(i <= content.size());
  assert(node);
  assert(!node->GetParent());
  node->SetParent(this);
  content.insert(content.begin() + i, node);
  SetDirtyLayout();
}

void
MathMLTokenElement::SwapChildren(std::vector< Ptr<MathMLTextNode> >& newContent)
{
  if (newContent != content)
    {
      // reset parent should be done first because the same elements
      // may be present in the following loop as well
      for (std::vector< Ptr<MathMLTextNode> >::iterator p = content.begin();
	   p != content.end();
	   p++)
	(*p)->Unlink();

      for (std::vector< Ptr<MathMLTextNode> >::iterator p = newContent.begin();
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
MathMLTokenElement::Normalize(const Ptr<class MathMLDocument>&)
{
  if (DirtyStructure() && GetDOMElement())
    {
#if defined(HAVE_GMETADOM)
      content.clear();

      String* sContent = NULL;
      for (DOM::Node p = GetDOMElement().get_firstChild(); 
	   p;
	   p = p.get_nextSibling()) 
	{
	  switch (p.get_nodeType())
	    {
	    case DOM::Node::TEXT_NODE:
	      {
		// ok, we have a chunk of text
		DOM::GdomeString content = p.get_nodeValue();
		String* s = allocString(content);
		assert(s != NULL);

		// white-spaces are always collapsed...
		s->CollapseSpaces();
	      
		// ...but spaces at the at the beginning (end) are deleted only if this
		// is the very first (last) chunk in the token.
		if (!p.get_previousSibling()) s->TrimSpacesLeft();
		if (!p.get_nextSibling()) s->TrimSpacesRight();

		Append(s);
		delete s;
	      }
	    break;

#if 0
	    // to be rewritten or deleted
	    case DOM::Node::ENTITY_REFERENCE_NODE:
	      for (DOM::Node p = node.get_firstChild(); p != 0; p = p.get_nextSibling())
		MathMLizeTokenContent(p, parent);
	      break;
#endif 0

	    case DOM::Node::ELEMENT_NODE:
	      {	    
		if (p.get_namespaceURI() == MATHML_NS_URI)
		  {
		    if (nodeLocalName(p) == "mglyph")
		      {
			Ptr<MathMLTextNode> text = SubstituteMGlyphElement(p);
			if (text) AppendChild(text);
		      }
		    else if (nodeLocalName(p) == "malignmark")
		      {
			Ptr<MathMLTextNode> text = SubstituteAlignMarkElement(p);
			if (text) AppendChild(text);
		      }
		    else
		      {
			std::string s_name = nodeLocalName(p);
			Globals::logger(LOG_WARNING, "element `%s' inside token (ignored)\n", s_name.c_str());
		      }
		  } else
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
MathMLTokenElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute())
    {
      env.Push();

      if (!is_a<MathMLIdentifierElement>(Ptr<MathMLElement>(this)) &&
	  !is_a<MathMLOperatorElement>(Ptr<MathMLElement>(this)))
	env.SetFontMode(FONT_MODE_TEXT);

      const Value* value = NULL;

      value = GetAttributeValue(ATTR_MATHSIZE, env, false);
      if (value != NULL) {
	if (IsSet(ATTR_FONTSIZE))
	  Globals::logger(LOG_WARNING, "attribute `mathsize' overrides deprecated attribute `fontsize'");
    
	if (value->IsKeyword(KW_SMALL)) env.AddScriptLevel(1);
	else if (value->IsKeyword(KW_BIG)) env.AddScriptLevel(-1);
	else if (value->IsKeyword(KW_NORMAL)) ; // noop
	else env.SetFontSize(value->ToNumberUnit());
      } else {
	value = GetAttributeValue(ATTR_FONTSIZE, env, false);
	if (value != NULL) {
	  Globals::logger(LOG_WARNING, "the attribute `fontsize' is deprecated in MathML 2");
	  env.SetFontSize(value->ToNumberUnit());
	}
      }
      delete value;
  
      value = GetAttributeValue(ATTR_MATHVARIANT, env, false);
      if (value != NULL) {
	assert(value->IsKeyword());

	const MathVariantAttributes& attr = attributesOfVariant(value->ToKeyword());
	assert(attr.kw != KW_NOTVALID);
	env.SetFontFamily(attr.family);
	env.SetFontWeight(attr.weight);
	env.SetFontStyle(attr.style);

	if (IsSet(ATTR_FONTFAMILY) || IsSet(ATTR_FONTWEIGHT) || IsSet(ATTR_FONTSTYLE))
	  Globals::logger(LOG_WARNING, "attribute `mathvariant' overrides deprecated font-related attributes");

	delete value;
      } else {
	value = GetAttributeValue(ATTR_FONTFAMILY, env, false);
	if (value != NULL) {
	  Globals::logger(LOG_WARNING, "the attribute `fontfamily' is deprecated in MathML 2");
	  env.SetFontFamily(value->ToString());
	}
	delete value;

	value = GetAttributeValue(ATTR_FONTWEIGHT, env, false);
	if (value != NULL) {
	  Globals::logger(LOG_WARNING, "the attribute `fontweight' is deprecated in MathML 2");
	  env.SetFontWeight(ToFontWeightId(value));
	}
	delete value;

	value = GetAttributeValue(ATTR_FONTSTYLE, env, false);
	if (value != NULL) {
	  Globals::logger(LOG_WARNING, "the attribute `fontstyle' is deprecated in MathML 2");
	  env.SetFontStyle(ToFontStyleId(value));
	} else if (is_a<MathMLIdentifierElement>(Ptr<MathMLElement>(this))) {
	  if (GetLogicalContentLength() == 1) {
	    Ptr<MathMLTextNode> node = GetChild(0);
	    assert(node);

	    if (is_a<MathMLCharNode>(node)) {
	      Ptr<MathMLCharNode> cNode = smart_cast<MathMLCharNode>(node);
	      assert(cNode);

	      if (!isUpperCaseGreek(cNode->GetChar())) env.SetFontStyle(FONT_STYLE_ITALIC);
	      else env.SetFontStyle(FONT_STYLE_NORMAL);
	    } else
	      env.SetFontStyle(FONT_STYLE_NORMAL);
	  } else {
	    env.SetFontStyle(FONT_STYLE_NORMAL);
	    env.SetFontMode(FONT_MODE_TEXT);
	  }
	}
	delete value;
      }

      value = GetAttributeValue(ATTR_MATHCOLOR, env, false);
      if (value != NULL) {
	if (IsSet(ATTR_COLOR))
	  Globals::logger(LOG_WARNING, "attribute `mathcolor' overrides deprecated attribute `color'");
	env.SetColor(ToRGB(value));
      } else {
	value = GetAttributeValue(ATTR_COLOR, env, false);
	if (value != NULL) {
	  Globals::logger(LOG_WARNING, "attribute `color' is deprecated in MathML 2");
	  env.SetColor(ToRGB(value));
	} else
	  if (HasLink()) env.SetColor(Globals::configuration.GetLinkForeground());
      }
      delete value;

      value = GetAttributeValue(ATTR_MATHBACKGROUND, env, false);
      if (value != NULL) env.SetBackgroundColor(ToRGB(value));
      else if (HasLink() && !Globals::configuration.HasTransparentLinkBackground())
	env.SetBackgroundColor(Globals::configuration.GetLinkBackground());
      delete value;

      color      = env.GetColor();
      background = env.GetBackgroundColor();
      sppm       = env.GetScaledPointsPerEm();

      for (std::vector< Ptr<MathMLTextNode> >::const_iterator p = GetContent().begin();
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
      box.Null();
      for (std::vector< Ptr<MathMLTextNode> >::const_iterator text = GetContent().begin();
	   text != GetContent().end();
	   text++)
	{
	  assert(*text);
	  if (ctxt.GetLayoutType() == LAYOUT_MIN) (*text)->DoLayout(ctxt);

	  // if we do not insert MathMLSpaceNodes in the layout, they will not be
	  // positioned correctly, since positioning is done thru the layout.
	  // In such way, If a space node is the first inside a token, it will produce
	  // a zero-origin rectangle which is obviously incorrect
	  box.Append((*text)->GetBoundingBox());
	  box.Append((sppm * (*text)->GetSpacing()) / 18);
	}

      AddItalicCorrection();

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLTokenElement::SetPosition(scaled x, scaled y)
{
  MathMLElement::SetPosition(x, y);
  SetContentPosition(x, y);
}

void
MathMLTokenElement::SetContentPosition(scaled x, scaled y)
{
  for (std::vector< Ptr<MathMLTextNode> >::const_iterator text = GetContent().begin(); 
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
  if (Dirty())
    {
      RenderBackground(area);

      if (fGC[Selected()] == NULL)
	{
	  GraphicsContextValues values;

	  values.foreground = Selected() ? area.GetSelectionForeground() : color;
	  values.background = Selected() ? area.GetSelectionBackground() : background;
	  fGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
	}

      for (std::vector< Ptr<MathMLTextNode> >::const_iterator text = GetContent().begin();
	   text != GetContent().end();
	   text++)
	{
	  assert(*text);
	  (*text)->Render(area);
	}

      //area.DrawRectangle(fGC[0], *shape);

      ResetDirty();
    }
}

scaled
MathMLTokenElement::GetLeftEdge() const
{
  scaled edge = 0;

  for (std::vector< Ptr<MathMLTextNode> >::const_iterator text = GetContent().begin();
       text != GetContent().end();
       text++)
    {
      assert(*text);
      if (text == GetContent().begin()) edge = (*text)->GetLeftEdge();
      else edge = scaledMin(edge, (*text)->GetLeftEdge());
    }

  return edge;
}

scaled
MathMLTokenElement::GetRightEdge() const
{
  scaled edge = 0;

  for (std::vector< Ptr<MathMLTextNode> >::const_iterator text = GetContent().begin();
       text != GetContent().end();
       text++)
    {
      assert(*text);
      if (text == GetContent().begin()) edge = (*text)->GetRightEdge();
      else edge = scaledMax(edge, (*text)->GetRightEdge());
    }

  return edge;
}

scaled
MathMLTokenElement::GetDecimalPointEdge() const
{
  for (std::vector< Ptr<MathMLTextNode> >::const_iterator text = GetContent().begin();
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
  for (std::vector< Ptr<MathMLTextNode> >::const_iterator text = GetContent().begin();
       text != GetContent().end();
       text++)
    {
      assert(*text);
      if (!is_a<MathMLSpaceNode>(*text)) return false;
    }

  return true;
}

Ptr<MathMLCharNode>
MathMLTokenElement::GetCharNode() const
{
  if (GetSize() != 1) return 0;

  Ptr<MathMLTextNode> node = GetChild(0);
  assert(node);
  if (!is_a<MathMLCharNode>(node) || is_a<MathMLCombinedCharNode>(node)) return 0;

  return smart_cast<MathMLCharNode>(node);
}

void
MathMLTokenElement::AddItalicCorrection()
{
  if (!is_a<MathMLIdentifierElement>(Ptr<MathMLElement>(this)) &&
      !is_a<MathMLNumberElement>(Ptr<MathMLElement>(this)) &&
      !is_a<MathMLTextElement>(Ptr<MathMLElement>(this))) return;
  
  if (GetSize() == 0) return;

  Ptr<MathMLTextNode> lastNode = GetChild(GetSize() - 1);
  assert(lastNode);

  Ptr<MathMLElement> next = findRightSibling(this);
  if (!next) return;

  Ptr<MathMLOperatorElement> coreOp = next->GetCoreOperatorTop();
  if (!coreOp) return;
  bool isFence = coreOp->IsFence();
  if (!isFence) return;

  const BoundingBox& lastBox = lastNode->GetBoundingBox();
  Globals::logger(LOG_DEBUG, "adding italic correction: %d %d", sp2ipx(box.rBearing), sp2ipx(box.width));
  if (lastBox.rBearing > lastBox.width) box.Append(lastBox.rBearing - lastBox.width);
}

Ptr<MathMLTextNode>
MathMLTokenElement::SubstituteMGlyphElement(const DOM::Element& node)
{
  assert(node);

  DOM::GdomeString alt        = node.getAttribute("alt");
  DOM::GdomeString fontFamily = node.getAttribute("fontfamily");
  DOM::GdomeString index      = node.getAttribute("index");

  if (alt.empty() || fontFamily.empty() || index.empty()) {
    Globals::logger(LOG_WARNING, "malformed `mglyph' element (some required attribute is missing)\n");
    return MathMLCharNode::create('?');
  }

  std::string s_index = index;
  char* endPtr;
  unsigned nch = strtoul(s_index.c_str(), &endPtr, 10);

  if (endPtr == NULL || *endPtr != '\0') {
    Globals::logger(LOG_WARNING, "malformed `mglyph' element (parsing error in `index' attribute)\n");
    nch = '?';
  }

  std::string s_alt = alt;
  std::string s_fontFamily = fontFamily;
  Ptr<MathMLGlyphNode> glyph = MathMLGlyphNode::create(s_alt.c_str(), s_fontFamily.c_str(), nch);

  return glyph;
}

Ptr<MathMLTextNode>
MathMLTokenElement::SubstituteAlignMarkElement(const DOM::Element& node)
{
  assert(node);

  DOM::GdomeString edge = node.getAttribute("edge");

  MarkAlignType align = MARK_ALIGN_NOTVALID;

  if (!edge.empty())
    {
      if      (edge == "left") align = MARK_ALIGN_LEFT;
      else if (edge == "right") align = MARK_ALIGN_RIGHT;
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

String*
MathMLTokenElement::GetRawContent() const
{
  StringFactory c;

  for (std::vector< Ptr<MathMLTextNode> >::const_iterator i = GetContent().begin();
       i != GetContent().end();
       i++)
    {
      assert(*i);
      String* s = (*i)->GetRawContent();
      if (s != NULL)
	{
	  c.Append(s);
	  delete s;
	}
    }

  return c.Pack();
}

unsigned
MathMLTokenElement::GetLogicalContentLength() const
{
  unsigned len = 0;

  for (std::vector< Ptr<MathMLTextNode> >::const_iterator i = GetContent().begin();
       i != GetContent().end();
       i++)
    {
      assert(*i);
      len += (*i)->GetLogicalContentLength();
    }

  return len;
}
