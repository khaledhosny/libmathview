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

#include "Globals.hh"
#include "mathVariantAux.hh"
#include "ValueConversion.hh"
#include "MathMLAttribute.hh"
#include "MathMLStyleElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLAttributeList.hh"
#include "RenderingEnvironment.hh"
#include "AbstractRefinementContext.hh"

MathMLStyleElement::MathMLStyleElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLStyleElement::MathMLStyleElement(const DOM::Element& node)
  : MathMLNormalizingContainerElement(node)
{
}
#endif

MathMLStyleElement::~MathMLStyleElement()
{
}

void
MathMLStyleElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, Style, scriptlevel);
      REFINE_ATTRIBUTE(context, Style, displaystyle);
      REFINE_ATTRIBUTE(context, Style, scriptsizemultiplier);
      REFINE_ATTRIBUTE(context, Style, scriptminsize);
      REFINE_ATTRIBUTE(context, Style, color);
      REFINE_ATTRIBUTE(context, Style, background);
      REFINE_ATTRIBUTE(context, Style, negativeveryverythickmathspace);
      REFINE_ATTRIBUTE(context, Style, negativeverythickmathspace);
      REFINE_ATTRIBUTE(context, Style, negativethickmathspace);
      REFINE_ATTRIBUTE(context, Style, negativemediummathspace);
      REFINE_ATTRIBUTE(context, Style, negativethinmathspace);
      REFINE_ATTRIBUTE(context, Style, negativeverythinmathspace);
      REFINE_ATTRIBUTE(context, Style, negativeveryverythinmathspace);
      REFINE_ATTRIBUTE(context, Style, veryverythinmathspace);
      REFINE_ATTRIBUTE(context, Style, verythinmathspace);
      REFINE_ATTRIBUTE(context, Style, thinmathspace);
      REFINE_ATTRIBUTE(context, Style, mediummathspace);
      REFINE_ATTRIBUTE(context, Style, thickmathspace);
      REFINE_ATTRIBUTE(context, Style, verythickmathspace);
      REFINE_ATTRIBUTE(context, Style, veryverythickmathspace);
      REFINE_ATTRIBUTE(context, Style, fontsize);
      REFINE_ATTRIBUTE(context, Style, fontweight);
      REFINE_ATTRIBUTE(context, Style, fontstyle);
      REFINE_ATTRIBUTE(context, Style, fontfamily);
      REFINE_ATTRIBUTE(context, Style, mathvariant);
      REFINE_ATTRIBUTE(context, Style, mathsize);
      REFINE_ATTRIBUTE(context, Style, mathcolor);
      REFINE_ATTRIBUTE(context, Style, mathbackground);
      context.push(GetDOMElement());
      MathMLNormalizingContainerElement::refine(context);
      context.pop();
    }
}

void
MathMLStyleElement::Setup(RenderingEnvironment& env)
{
  // can be optimized here
  if (DirtyAttribute() || DirtyAttributeP())
    {
      env.Push();

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, displaystyle))
	env.SetDisplayStyle(ToBoolean(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, scriptsizemultiplier))
	env.SetScriptSizeMultiplier(ToNumber(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, scriptminsize))
	env.SetScriptMinSize(ToNumberUnit(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, scriptlevel))
	{
	  SmartPtr<Value> p = GetComponent(value, 0);
	  if (IsEmpty(p))
	    {
	      SmartPtr<Value> p = GetComponent(value, 1);
	      assert(p);
      
	      int scriptLevel = ToInteger(p);
	      if (scriptLevel < 0) scriptLevel = 0;
	      env.SetScriptLevel(scriptLevel);
	    }
	  else
	    {
	      int sign = (ToKeywordId(p) == KW_MINUS) ? -1 : 1;
	      p = GetComponent(value, 1);
	      assert(p);
      
	      int scriptLevel = ToInteger(p);
	      if (scriptLevel < 0) scriptLevel = 0;
	      env.AddScriptLevel(sign * scriptLevel);
	    }
	}

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mathcolor))
	{
	  if (IsSet(ATTR_COLOR))
	    Globals::logger(LOG_WARNING, "attribute `mathcolor' overrides deprecated attribute `color'");
	  env.SetColor(ToRGB(value));
	} 
      else if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, color))
	{
	  Globals::logger(LOG_WARNING, "attribute `color' is deprecated in MathML 2");
	  env.SetColor(ToRGB(value));
	}

      RGBValue oldBackground = env.GetBackgroundColor();
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mathbackground))
	{
	  if (IsSet(ATTR_BACKGROUND))
	    Globals::logger(LOG_WARNING, "attribute `mathbackground' overrides deprecated attribute `background'");
	  if (!IsKeyword(value) || ToKeywordId(value) != KW_TRANSPARENT)
	    env.SetBackgroundColor(ToRGB(value));
	}
      else if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, background))
	{
	  Globals::logger(LOG_WARNING, "attribute `background' is deprecated in MathML 2");
	  if (!IsKeyword(value) || ToKeywordId(value) != KW_TRANSPARENT)
	    env.SetBackgroundColor(ToRGB(value));
	}
      background = env.GetBackgroundColor();
      differentBackground = background != oldBackground;

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, veryverythinmathspace))
	env.SetMathSpace(MATH_SPACE_VERYVERYTHIN, ToNumberUnit(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, verythinmathspace))
	env.SetMathSpace(MATH_SPACE_VERYTHIN, ToNumberUnit(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, thinmathspace))
	env.SetMathSpace(MATH_SPACE_THIN, ToNumberUnit(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mediummathspace))
	env.SetMathSpace(MATH_SPACE_MEDIUM, ToNumberUnit(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, thickmathspace))
	env.SetMathSpace(MATH_SPACE_THICK, ToNumberUnit(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, verythinmathspace))
	env.SetMathSpace(MATH_SPACE_VERYTHICK, ToNumberUnit(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, veryverythickmathspace))
	env.SetMathSpace(MATH_SPACE_VERYVERYTHICK, ToNumberUnit(value));

      // the following attributes, thought not directly supported by <mstyle>
      // must be parsed here since they are always inherited by other elements

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mathsize))
	{
	  if (IsSet(ATTR_FONTSIZE))
	    Globals::logger(LOG_WARNING, "attribute `mathsize' overrides deprecated attribute `fontsize'");
    
	  if (IsKeyword(value))
	    switch (ToKeywordId(value))
	      {
	      case KW_SMALL: env.AddScriptLevel(1); break;
	      case KW_BIG: env.AddScriptLevel(-1); break;
	      case KW_NORMAL: break; // noop
	      default: assert(IMPOSSIBLE); break;
	      }
	  else
	    env.SetFontSize(ToNumberUnit(value));
	}
      else if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, fontsize))
	{
	  Globals::logger(LOG_WARNING, "the attribute `fontsize' is deprecated in MathML 2");
	  env.SetFontSize(ToNumberUnit(value));
	}

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mathvariant))
	{
	  assert(IsKeyword(value));

	  const MathVariantAttributes& attr = attributesOfVariant(ToKeywordId(value));
	  assert(attr.kw != KW_NOTVALID);
	  env.SetFontFamily(attr.family);
	  env.SetFontWeight(attr.weight);
	  env.SetFontStyle(attr.style);

	  if (IsSet(ATTR_FONTFAMILY) || IsSet(ATTR_FONTWEIGHT) || IsSet(ATTR_FONTSTYLE))
	    Globals::logger(LOG_WARNING, "attribute `mathvariant' overrides deprecated font-related attributes");
	} 
      else
	{
	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, fontfamily))
	    {
	      Globals::logger(LOG_WARNING, "the attribute `fontfamily` is deprecated in MathML 2");
	      env.SetFontFamily(ToString(value));
	    }

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, fontweight))
	    {
	      Globals::logger(LOG_WARNING, "the attribute `fontweight` is deprecated in MathML 2");
	      env.SetFontWeight(ToFontWeightId(value));
	    }

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, fontstyle))
	    {
	      Globals::logger(LOG_WARNING, "the attribute `fontstyle` is deprecated in MathML 2");
	      env.SetFontStyle(ToFontStyleId(value));
	    }
	}

      MathMLNormalizingContainerElement::Setup(env);
      env.Drop();
      ResetDirtyAttribute();
    }
}

void
MathMLStyleElement::DoLayout(const FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      MathMLNormalizingContainerElement::DoLayout(ctxt);
      DoEmbellishmentLayout(this, box);
      ResetDirtyLayout(ctxt);
    }
}

void
MathMLStyleElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (GetChild()) GetChild()->SetPosition(x, y);
}

void
MathMLStyleElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
#if 0 
      if (IsDirty()) {
	if (differentBackground && !IsSelected()) {
	  if (bGC[0] == NULL) {
	    GraphicsContextValues values;
	    values.foreground = values.background = background;
	    bGC[0] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
	  }

	  area.Clear(bGC[0], GetRectangle());
	}
      }
#endif

      RenderBackground(area);

      assert(child);
      child->Render(area);

      ResetDirty();
    }
}

bool
MathMLStyleElement::IsSpaceLike() const
{
  assert(child);
  return child->IsSpaceLike();
}

void
MathMLStyleElement::SetDirtyAttribute()
{
  SetDirtyAttributeD();
}

SmartPtr<MathMLOperatorElement>
MathMLStyleElement::GetCoreOperator()
{
  if (GetChild()) return GetChild()->GetCoreOperator();
  else return 0;
}
