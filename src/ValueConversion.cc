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

bool
IsEmpty(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<void> >(value);
}

bool
IsKeyword(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<KeywordId> >(value);
}

bool
IsSequence(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant< std::vector< SmartPtr<Value> > > >(value);
}

bool
IsNumberUnit(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<UnitValue> >(value);
}

bool
IsRGBValue(const SmartPtr<Value>& value)
{
  assert(value);
  return is_a< Variant<RGBValue> >(value);
}

bool
ToBoolean(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_FALSE: return false;
    case KW_TRUE: return true;
    default:
      assert(IMPOSSIBLE);
      return false;
    }
}

float
ToNumber(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<float> > v = smart_cast< Variant<float> >(value))
    return v->getValue();
  assert(false);
  return 0;
}

int
ToInteger(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<int> > v = smart_cast< Variant<int> >(value))
    return v->getValue();
  assert(false);
  return 0;
}

String
ToString(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<String> > v = smart_cast< Variant<String> >(value))
    return v->getValue();
  assert(false);
  return 0;
}

UnitValue
ToNumberUnit(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<UnitValue> > v = smart_cast< Variant<UnitValue> >(value))
    return v->getValue();
  assert(false);
  return UnitValue();
}

RGBValue
ToRGBValue(const SmartPtr<Value>& value)
{
  if (SmartPtr< Variant<RGBValue> > v = smart_cast< Variant<RGBValue> >(value))
    return v->getValue();
  assert(false);
  return BLACK_COLOR;
}

KeywordId
ToKeywordId(const SmartPtr<Value>& value)
{
  assert(value);

  if (SmartPtr< Variant<KeywordId> > v = smart_cast< Variant<KeywordId> >(value))
    return v->getValue();
  else
    return KW_NOTVALID;
}

SmartPtr<ValueSequence>
ToSequence(const SmartPtr<Value>& value)
{
  return smart_cast<ValueSequence>(value);
}

RowAlignId
ToRowAlignId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_TOP: return ROW_ALIGN_TOP;
    case KW_BOTTOM: return ROW_ALIGN_BOTTOM;
    case KW_CENTER: return ROW_ALIGN_CENTER;
    case KW_BASELINE: return ROW_ALIGN_BASELINE;
    case KW_AXIS: return ROW_ALIGN_AXIS;
    default: return ROW_ALIGN_NOTVALID;
    }
}

ColumnAlignId
ToColumnAlignId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_LEFT: return COLUMN_ALIGN_LEFT;
    case KW_RIGHT: return COLUMN_ALIGN_RIGHT;
    case KW_CENTER: return COLUMN_ALIGN_CENTER;
    default: return COLUMN_ALIGN_NOTVALID;
    }
}

GroupAlignId
ToGroupAlignId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_LEFT: return GROUP_ALIGN_LEFT;
    case KW_RIGHT: return GROUP_ALIGN_RIGHT;
    case KW_CENTER: return GROUP_ALIGN_CENTER;
    case KW_DECIMALPOINT: return GROUP_ALIGN_DECIMALPOINT;
    default: return GROUP_ALIGN_NOTVALID;
    }
}

TableLineId
ToLineId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_NONE: return TABLE_LINE_NONE;
    case KW_SOLID: return TABLE_LINE_SOLID;
    case KW_DASHED: return TABLE_LINE_DASHED;
    default: return TABLE_LINE_NOTVALID;
    }
}

MathSpaceId
ToNamedSpaceId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_VERYVERYTHINMATHSPACE: return MATH_SPACE_VERYVERYTHIN;
    case KW_VERYTHINMATHSPACE: return MATH_SPACE_VERYTHIN;
    case KW_THINMATHSPACE: return MATH_SPACE_THIN;
    case KW_MEDIUMMATHSPACE: return MATH_SPACE_MEDIUM;
    case KW_THICKMATHSPACE: return MATH_SPACE_THICK;
    case KW_VERYTHICKMATHSPACE: return MATH_SPACE_VERYTHICK;
    case KW_VERYVERYTHICKMATHSPACE: return MATH_SPACE_VERYVERYTHICK;
    default: return MATH_SPACE_NOTVALID;
    }
}

UnitId
ToUnitId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_PERCENTAGE: return UNIT_PERCENTAGE;
    case KW_EM: return UNIT_EM;
    case KW_EX: return UNIT_EX;
    case KW_PX: return UNIT_PX;
    case KW_IN: return UNIT_IN;
    case KW_CM: return UNIT_CM;
    case KW_MM: return UNIT_MM;
    case KW_PT: return UNIT_PT;
    case KW_PC: return UNIT_PC;
    default: return UNIT_NOTVALID;
    }
}

FontStyleId
ToFontStyleId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_NORMAL: return FONT_STYLE_NORMAL;
    case KW_ITALIC: return FONT_STYLE_ITALIC;
    default: return FONT_STYLE_NOTVALID;
    }
}

FontWeightId
ToFontWeightId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_NORMAL: return FONT_WEIGHT_NORMAL;
    case KW_BOLD: return FONT_WEIGHT_BOLD;
    default: return FONT_WEIGHT_NOTVALID;
    }
}

RGBValue
ToRGB(const SmartPtr<Value>& value)
{
  if (IsRGBValue(value))
    return ToRGBValue(value);

  switch (ToKeywordId(value))
    {
    case KW_BLACK: return BLACK_COLOR;
    case KW_SILVER: return SILVER_COLOR;
    case KW_GRAY: return GRAY_COLOR;
    case KW_WHITE: return WHITE_COLOR;
    case KW_MAROON: return MAROON_COLOR;
    case KW_RED: return RED_COLOR;
    case KW_PURPLE: return PURPLE_COLOR;
    case KW_FUCHSIA: return FUCHSIA_COLOR;
    case KW_GREEN: return GREEN_COLOR;
    case KW_LIME: return LIME_COLOR;
    case KW_OLIVE: return OLIVE_COLOR;
    case KW_YELLOW: return YELLOW_COLOR;
    case KW_NAVY: return NAVY_COLOR;
    case KW_BLUE: return BLUE_COLOR;
    case KW_TEAL: return TEAL_COLOR;
    case KW_AQUA: return AQUA_COLOR;
    default:
      assert(false);
      return BLACK_COLOR;
    }
}

OperatorFormId
ToFormId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_PREFIX: return OP_FORM_PREFIX;
    case KW_INFIX: return OP_FORM_INFIX;
    case KW_POSTFIX: return OP_FORM_POSTFIX;
    default: return OP_FORM_NOTVALID;
    }
}

FractionAlignId
ToFractionAlignId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_LEFT: return FRAC_ALIGN_LEFT;
    case KW_RIGHT: return FRAC_ALIGN_RIGHT;
    case KW_CENTER: return FRAC_ALIGN_CENTER;
    default: return FRAC_ALIGN_NOTVALID;
    }
}

MarkAlignType
ToMarkAlignId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_LEFT: return MARK_ALIGN_LEFT;
    case KW_RIGHT: return MARK_ALIGN_RIGHT;
    default: return MARK_ALIGN_NOTVALID;
    }
}

BreakId
ToBreakId(const SmartPtr<Value>& value)
{
  switch (ToKeywordId(value))
    {
    case KW_AUTO: return BREAK_AUTO;
    case KW_NEWLINE: return BREAK_YES;
    case KW_INDENTINGNEWLINE: return BREAK_INDENT;
    case KW_NOBREAK: return BREAK_NO;
    case KW_BADBREAK: return BREAK_BAD;
    case KW_GOODBREAK:return BREAK_GOOD;
    default: 
      assert(IMPOSSIBLE);
      return BREAK_AUTO;
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
