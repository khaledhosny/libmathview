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

MathMLStyleElement::MathMLStyleElement(const SmartPtr<class MathMLView>& view)
  : MathMLNormalizingContainerElement(view)
{
}

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
#if 0
      REFINE_ATTRIBUTE(context, Style, fontweight);
      REFINE_ATTRIBUTE(context, Style, fontstyle);
      REFINE_ATTRIBUTE(context, Style, fontfamily);
      REFINE_ATTRIBUTE(context, Style, mathvariant);
#endif
      REFINE_ATTRIBUTE(context, Style, mathsize);
#if 0
      REFINE_ATTRIBUTE(context, Style, mathcolor);
      REFINE_ATTRIBUTE(context, Style, mathbackground);
#endif
      context.push(getDOMElement());
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
	env.SetScriptMinSize(ToLength(value));

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
	      int sign = (ToTokenId(p) == T__MINUS) ? -1 : 1;
	      p = GetComponent(value, 1);
	      assert(p);
      
	      int scriptLevel = ToInteger(p);
	      if (scriptLevel < 0) scriptLevel = 0;
	      env.AddScriptLevel(sign * scriptLevel);
	    }
	}

#if 0
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mathcolor))
	{
	  if (IsSet(T_COLOR))
	    Globals::logger(LOG_WARNING, "attribute `mathcolor' overrides deprecated attribute `color'");
	  env.SetColor(ToRGBColor(value));
	} 
      else
#endif
	if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, color))
	{
	  //Globals::logger(LOG_WARNING, "attribute `color' is deprecated in MathML 2");
	  env.SetColor(ToRGBColor(value));
	}

      RGBColor oldBackground = env.GetBackgroundColor();
#if 0
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mathbackground))
	{
	  if (IsSet(T_BACKGROUND))
	    Globals::logger(LOG_WARNING, "attribute `mathbackground' overrides deprecated attribute `background'");
	  if (!IsTokenId(value) || ToTokenId(value) != T_TRANSPARENT)
	    env.SetBackgroundColor(ToRGBColor(value));
	}
      else
#endif
	if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, background))
	{
	  //Globals::logger(LOG_WARNING, "attribute `background' is deprecated in MathML 2");
	  if (!IsTokenId(value) || ToTokenId(value) != T_TRANSPARENT)
	    env.SetBackgroundColor(ToRGBColor(value));
	}
      background = env.GetBackgroundColor();
      differentBackground = background != oldBackground;

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, veryverythinmathspace))
	env.SetMathSpace(RenderingEnvironment::MATH_SPACE_VERYVERYTHIN, ToLength(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, verythinmathspace))
	env.SetMathSpace(RenderingEnvironment::MATH_SPACE_VERYTHIN, ToLength(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, thinmathspace))
	env.SetMathSpace(RenderingEnvironment::MATH_SPACE_THIN, ToLength(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mediummathspace))
	env.SetMathSpace(RenderingEnvironment::MATH_SPACE_MEDIUM, ToLength(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, thickmathspace))
	env.SetMathSpace(RenderingEnvironment::MATH_SPACE_THICK, ToLength(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, verythinmathspace))
	env.SetMathSpace(RenderingEnvironment::MATH_SPACE_VERYTHICK, ToLength(value));

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, veryverythickmathspace))
	env.SetMathSpace(RenderingEnvironment::MATH_SPACE_VERYVERYTHICK, ToLength(value));

      // the following attributes, thought not directly supported by <mstyle>
      // must be parsed here since they are always inherited by other elements

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mathsize))
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
      else if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, fontsize))
	{
	  Globals::logger(LOG_WARNING, "the attribute `fontsize' is deprecated in MathML 2");
	  env.SetFontSize(ToLength(value));
	}

#if 0
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, mathvariant))
	{
	  assert(IsTokenId(value));

	  const MathVariantAttributes& attr = attributesOfVariant(ToTokenId(value));
	  assert(attr.kw != T__NOTVALID);
	  env.SetFontFamily(attr.family);
	  env.SetFontWeight(attr.weight);
	  env.SetFontStyle(attr.style);

	  if (IsSet(T_FONTFAMILY) || IsSet(T_FONTWEIGHT) || IsSet(T_FONTSTYLE))
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
	      env.SetFontWeight(ToTokenId(value));
	    }

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Style, fontstyle))
	    {
	      Globals::logger(LOG_WARNING, "the attribute `fontstyle` is deprecated in MathML 2");
	      env.SetFontStyle(ToTokenId(value));
	    }
	}
#endif

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
