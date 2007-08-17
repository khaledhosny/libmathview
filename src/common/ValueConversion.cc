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

#include "defs.h"
#include "String.hh"
#include "ValueConversion.hh"

typedef Variant<void> Variant_void;

bool
IsEmpty(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a<const Variant_void>(value);
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
{ return smart_cast<ValueSequence>(value); }

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
  if (value)
    if (i < 0)
      return value;
    else
      {
	SmartPtr<ValueSequence> vSeq = smart_cast<ValueSequence>(value);
	assert(vSeq);
	assert(vSeq->getSize() > 0);
	return GetComponent(vSeq->getValue(std::min(i, static_cast<int>(vSeq->getSize() - 1))), j, -1);
      }
  else
    return 0;
}

