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

#include "defs.h"
#include "String.hh"
#include "ValueConversion.hh"
#include "MathFormattingContext.hh"

bool
IsEmpty(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<void> >(value);
}

bool
IsTokenId(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<TokenId> >(value);
}

bool
IsSequence(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant< std::vector< SmartPtr<Value> > > >(value);
}

bool
IsLength(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<Length> >(value);
}

bool
IsRGBColor(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<RGBColor> >(value);
}

bool
IsNumber(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<float> >(value);
}

bool
ToBoolean(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<bool> > v = smart_cast< Variant<bool> >(value))
    return v->getValue();
  assert(false);
  return bool();
}

float
ToNumber(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<float> > v = smart_cast< Variant<float> >(value))
    return v->getValue();
  assert(false);
  return float();
}

int
ToInteger(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<int> > v = smart_cast< Variant<int> >(value))
    return v->getValue();
  assert(false);
  return int();
}

String
ToString(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<String> > v = smart_cast< Variant<String> >(value))
    return v->getValue();
  assert(false);
  return String();
}

Length
ToLength(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<Length> > v = smart_cast< Variant<Length> >(value))
    return v->getValue();
  assert(false);
  return Length();
}

RGBColor
ToRGBColor(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<RGBColor> > v = smart_cast< Variant<RGBColor> >(value))
    return v->getValue();
  assert(false);
  return RGBColor();
}

TokenId
ToTokenId(const SmartPtr<Value>& value)
{
  assert(value);

  if (SmartPtr< Variant<TokenId> > v = smart_cast< Variant<TokenId> >(value))
    return v->getValue();
  else
    return T__NOTVALID;
}

SmartPtr<ValueSequence>
ToSequence(const SmartPtr<Value>& value)
{
  return smart_cast<ValueSequence>(value);
}

Length::Unit
toUnitId(TokenId id)
{
  switch (id)
    {
    case T__PERCENTAGE: return Length::PERCENTAGE_UNIT;
    case T_EM: return Length::EM_UNIT;
    case T_EX: return Length::EX_UNIT;
    case T_PX: return Length::PX_UNIT;
    case T_IN: return Length::IN_UNIT;
    case T_CM: return Length::CM_UNIT;
    case T_MM: return Length::MM_UNIT;
    case T_PT: return Length::PT_UNIT;
    case T_PC: return Length::PC_UNIT;
    default: return Length::UNDEFINED_UNIT;
    }
}

Length::Unit
toUnitId(const SmartPtr<Value>& value)
{
  return toUnitId(ToTokenId(value));
}

MathVariant
toMathVariant(TokenId id)
{
  switch (id)
    {
    case T_NORMAL: return NORMAL_VARIANT;
    case T_BOLD: return BOLD_VARIANT;
    case T_ITALIC: return ITALIC_VARIANT;
    case T_BOLD_ITALIC: return BOLD_ITALIC_VARIANT;
    case T_DOUBLE_STRUCK: return DOUBLE_STRUCK_VARIANT;
    case T_BOLD_FRAKTUR: return BOLD_FRAKTUR_VARIANT;
    case T_SCRIPT: return SCRIPT_VARIANT;
    case T_BOLD_SCRIPT: return BOLD_SCRIPT_VARIANT;
    case T_FRAKTUR: return FRAKTUR_VARIANT;
    case T_SANS_SERIF: return SANS_SERIF_VARIANT;
    case T_BOLD_SANS_SERIF: return BOLD_SANS_SERIF_VARIANT;
    case T_SANS_SERIF_ITALIC: return SANS_SERIF_ITALIC_VARIANT;
    case T_SANS_SERIF_BOLD_ITALIC: return SANS_SERIF_BOLD_ITALIC_VARIANT;
    case T_MONOSPACE: return MONOSPACE_VARIANT;
    default: assert(false);
    }
}

MathVariant
toMathVariant(const SmartPtr<Value>& value)
{
  return toMathVariant(ToTokenId(value));
}

RGBColor
ToRGB(const SmartPtr<Value>& value)
{
  if (IsRGBColor(value))
    return ToRGBColor(value);

  switch (ToTokenId(value))
    {
    case T_BLACK: return RGBColor::BLACK();
    case T_SILVER: return RGBColor::SILVER();
    case T_GRAY: return RGBColor::GRAY();
    case T_WHITE: return RGBColor::WHITE();
    case T_MAROON: return RGBColor::MAROON();
    case T_RED: return RGBColor::RED();
    case T_PURPLE: return RGBColor::PURPLE();
    case T_FUCHSIA: return RGBColor::FUCHSIA();
    case T_GREEN: return RGBColor::GREEN();
    case T_LIME: return RGBColor::LIME();
    case T_OLIVE: return RGBColor::OLIVE();
    case T_YELLOW: return RGBColor::YELLOW();
    case T_NAVY: return RGBColor::NAVY();
    case T_BLUE: return RGBColor::BLUE();
    case T_TEAL: return RGBColor::TEAL();
    case T_AQUA: return RGBColor::AQUA();
    default:
      assert(false);
      return RGBColor::BLACK();
    }
}

SmartPtr<Value>
GetComponent(const SmartPtr<Value>& value, int i, int j)
{
  if (i < 0)
    return value;
  else
    {
      SmartPtr<ValueSequence> vSeq = smart_cast<ValueSequence>(value);
      assert(vSeq);
      assert(vSeq->getSize() > 0);
      return GetComponent(vSeq->getValue(std::min(i, static_cast<int>(vSeq->getSize() - 1))), j, -1);
    }
}

#if 0
SmartPtr<Value>
Resolve(const SmartPtr<Value>& value, const RenderingEnvironment& env, int i, int j)
{
  assert(value);

  SmartPtr<Value> realValue = GetComponent(value, i, j);
  assert(realValue);

  if (IsTokenId(value))
    return Variant<Length>::create(env.GetMathSpace(RenderingEnvironment::mathSpaceIdOfTokenId(ToTokenId(value))));
  else
    return realValue;
}
#endif

MathFormattingContext::PropertyId
toMathSpaceId(TokenId id)
{
  switch (id)
    {
    case T_NEGATIVEVERYVERYTHICKMATHSPACE: return MathFormattingContext::NEGATIVE_VERYVERYTHICK_SPACE;
    case T_NEGATIVEVERYTHICKMATHSPACE: return MathFormattingContext::NEGATIVE_VERYTHICK_SPACE;
    case T_NEGATIVETHICKMATHSPACE: return MathFormattingContext::NEGATIVE_THICK_SPACE;
    case T_NEGATIVEMEDIUMMATHSPACE: return MathFormattingContext::NEGATIVE_MEDIUM_SPACE;
    case T_NEGATIVETHINMATHSPACE: return MathFormattingContext::NEGATIVE_THIN_SPACE;
    case T_NEGATIVEVERYTHINMATHSPACE: return MathFormattingContext::NEGATIVE_VERYTHIN_SPACE;
    case T_NEGATIVEVERYVERYTHINMATHSPACE: return MathFormattingContext::NEGATIVE_VERYVERYTHIN_SPACE;
    case T_VERYVERYTHINMATHSPACE: return MathFormattingContext::VERYVERYTHIN_SPACE;
    case T_VERYTHINMATHSPACE: return MathFormattingContext::VERYTHIN_SPACE;
    case T_THINMATHSPACE: return MathFormattingContext::THIN_SPACE;
    case T_MEDIUMMATHSPACE: return MathFormattingContext::MEDIUM_SPACE;
    case T_THICKMATHSPACE: return MathFormattingContext::THICK_SPACE;
    case T_VERYTHICKMATHSPACE: return MathFormattingContext::VERYTHICK_SPACE;
    case T_VERYVERYTHICKMATHSPACE: return MathFormattingContext::VERYVERYTHICK_SPACE;
    default:
      assert(false);
      return MathFormattingContext::ZERO_SPACE;
    }
}

SmartPtr<Value>
Resolve(const SmartPtr<Value>& value, const MathFormattingContext& ctxt, int i, int j)
{
  assert(value);

  SmartPtr<Value> realValue = GetComponent(value, i, j);
  assert(realValue);

  if (IsTokenId(value))
    return Variant<Length>::create(ctxt.getMathSpace(toMathSpaceId(ToTokenId(value))));
  else
    return realValue;
}
