// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "AreaFactory.hh"
#include "mathVariantAux.hh"
#include "ValueConversion.hh"
#include "MathMLStyleElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLStyleElement::MathMLStyleElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLNormalizingContainerElement(context)
{ }

MathMLStyleElement::~MathMLStyleElement()
{ }

AreaRef
MathMLStyleElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, displaystyle))
	ctxt.setDisplayStyle(ToBoolean(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, scriptsizemultiplier))
	ctxt.setSizeMultiplier(ToNumber(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, scriptminsize))
	ctxt.setMinSize(ctxt.MGD()->evaluate(ctxt, ToLength(value), ctxt.getMinSize()));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, scriptlevel))
	{
	  SmartPtr<Value> p = GetComponent(value, 0);
	  if (IsEmpty(p))
	    {
	      SmartPtr<Value> p = GetComponent(value, 1);
	      assert(p);
      
	      int scriptLevel = ToInteger(p);
	      if (scriptLevel < 0) scriptLevel = 0;
	      ctxt.setScriptLevel(scriptLevel);
	    }
	  else
	    {
	      int sign = (ToTokenId(p) == T__MINUS) ? -1 : 1;
	      p = GetComponent(value, 1);
	      assert(p);
      
	      int scriptLevel = ToInteger(p);
	      if (scriptLevel < 0) scriptLevel = 0;
	      ctxt.addScriptLevel(sign * scriptLevel);
	    }
	}

      const RGBColor oldColor = ctxt.getColor();
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, mathcolor))
	ctxt.setColor(ToRGB(value));
      else if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, color))
	ctxt.setColor(ToRGB(value));
      const RGBColor color = ctxt.getColor();

      const RGBColor oldBackground = ctxt.getBackground();
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, mathbackground))
	{
	  if (!IsTokenId(value) || ToTokenId(value) != T_TRANSPARENT)
	    ctxt.setBackground(ToRGB(value));
	}
      else if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, background))
	{
	  if (!IsTokenId(value) || ToTokenId(value) != T_TRANSPARENT)
	    ctxt.setBackground(ToRGB(value));
	}
      const RGBColor background = ctxt.getBackground();

      static const AttributeId spaceId[] = {
	ATTRIBUTE_ID(MathML, Style, negativeveryverythickmathspace),
	ATTRIBUTE_ID(MathML, Style, negativeverythickmathspace),
	ATTRIBUTE_ID(MathML, Style, negativethickmathspace),
	ATTRIBUTE_ID(MathML, Style, negativemediummathspace),
	ATTRIBUTE_ID(MathML, Style, negativethinmathspace),
	ATTRIBUTE_ID(MathML, Style, negativeverythinmathspace),
	ATTRIBUTE_ID(MathML, Style, negativeveryverythinmathspace),
	0,
	ATTRIBUTE_ID(MathML, Style, veryverythinmathspace),
	ATTRIBUTE_ID(MathML, Style, verythinmathspace),
	ATTRIBUTE_ID(MathML, Style, thinmathspace),
	ATTRIBUTE_ID(MathML, Style, mediummathspace),
	ATTRIBUTE_ID(MathML, Style, thickmathspace),
	ATTRIBUTE_ID(MathML, Style, verythickmathspace),
	ATTRIBUTE_ID(MathML, Style, veryverythickmathspace)
      };

      for (unsigned i = 0; i < (sizeof(spaceId) / sizeof(AttributeId)); i++)
	if (spaceId[i])
	  if (SmartPtr<Value> value = getAttributeValue(*spaceId[i]))
	    ctxt.setMathSpace(FormattingContext::NEGATIVE_VERYVERYTHICK_SPACE + i, ToLength(value));

      // the following attributes, thought not directly supported by <mstyle>
      // must be parsed here since they are always inherited by other elements

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Style, mathsize))
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
	    ctxt.setSize(ctxt.MGD()->evaluate(ctxt, ToLength(value), ctxt.getSize()));
	}

      AreaRef res = getChild()->format(ctxt);
      if (color != oldColor) res = ctxt.MGD()->getFactory()->color(res, color);
      if (background != oldBackground) res = ctxt.MGD()->getFactory()->background(res, background);
      setArea(res);
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

bool
MathMLStyleElement::IsSpaceLike() const
{
  assert(getChild());
  return getChild()->IsSpaceLike();
}

void
MathMLStyleElement::setDirtyAttribute()
{
  setDirtyAttributeD();
}

SmartPtr<MathMLOperatorElement>
MathMLStyleElement::getCoreOperator()
{
  return getChild() ? getChild()->getCoreOperator() : 0;
}
