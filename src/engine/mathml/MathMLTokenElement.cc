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

#include "MathML.hh"
#include "AreaFactory.hh"
#include "BoundingBoxAux.hh"
#include "Globals.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLGlyphNode.hh"
#include "MathMLIdentifierElement.hh"
#include "MathMLMarkNode.hh"
#include "MathMLNumberElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLSpaceNode.hh"
#include "MathMLStringNode.hh"
#include "MathMLTextElement.hh"
#include "MathMLTokenElement.hh"
#include "MathMLValueConversion.hh"
#include "mathVariantAux.hh"
#include "traverseAux.hh"
#include "MathMLAttributeSignatures.hh"

MathMLTokenElement::MathMLTokenElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLElement(context)
{ }

MathMLTokenElement::~MathMLTokenElement()
{ }

void
MathMLTokenElement::append(const String& s)
{
  content.appendChild(this, MathMLStringNode::create(s));
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

  AreaRef res;
  if (c.size() == 1) res = c[0];
  else res = ctxt.getDevice()->getFactory()->horizontalArray(c);

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

unsigned
MathMLTokenElement::getContentLength() const
{ return GetLogicalContentLength(); }
