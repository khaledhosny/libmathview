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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>

#include "ValueConversion.hh"

RowAlignId ToRowAlignId(const Value* value)
{
  assert(value != NULL);

  RowAlignId res = ROW_ALIGN_NOTVALID;

  if (value->IsKeyword()) {
    switch (value->ToKeyword()) {
    case KW_TOP:      res = ROW_ALIGN_TOP; break;
    case KW_BOTTOM:   res = ROW_ALIGN_BOTTOM; break;
    case KW_CENTER:   res = ROW_ALIGN_CENTER; break;
    case KW_BASELINE: res = ROW_ALIGN_BASELINE; break;
    case KW_AXIS:     res = ROW_ALIGN_AXIS; break;
    default:          res = ROW_ALIGN_NOTVALID; break;
    }
  }

  return res;
}

ColumnAlignId ToColumnAlignId(const Value* value)
{
  assert(value != NULL);

  ColumnAlignId res = COLUMN_ALIGN_NOTVALID;

  if (value->IsKeyword()) {
    switch (value->ToKeyword()) {
    case KW_LEFT:   res = COLUMN_ALIGN_LEFT; break;
    case KW_RIGHT:  res = COLUMN_ALIGN_RIGHT; break;
    case KW_CENTER: res = COLUMN_ALIGN_CENTER; break;
    default:        res = COLUMN_ALIGN_NOTVALID; break;
    }
  }

  return res;
}

GroupAlignId ToGroupAlignId(const Value* value)
{
  assert(value != NULL);

  GroupAlignId res = GROUP_ALIGN_NOTVALID;

  if (value->IsKeyword()) {
    switch (value->ToKeyword()) {
    case KW_LEFT:         res = GROUP_ALIGN_LEFT; break;
    case KW_RIGHT:        res = GROUP_ALIGN_RIGHT; break;
    case KW_CENTER:       res = GROUP_ALIGN_CENTER; break;
    case KW_DECIMALPOINT: res = GROUP_ALIGN_DECIMALPOINT; break;
    default:              res = GROUP_ALIGN_NOTVALID; break;
    }
  }

  return res;
}

TableLineId ToLineId(const Value* value)
{
  assert(value != NULL);

  TableLineId res = TABLE_LINE_NOTVALID;

  if (value->IsKeyword()) {
    switch (value->ToKeyword()) {
    case KW_NONE:   res = TABLE_LINE_NONE; break;
    case KW_SOLID:  res = TABLE_LINE_SOLID; break;
    case KW_DASHED: res = TABLE_LINE_DASHED; break;
    default:        res = TABLE_LINE_NOTVALID; break;
    }
  }

  return res;
}

MathSpaceId ToNamedSpaceId(const Value* value)
{
  assert(value != NULL);

  MathSpaceId res = MATH_SPACE_NOTVALID;

  if (value->IsKeyword()) {
    switch (value->ToKeyword()) {
    case KW_VERYVERYTHINMATHSPACE:
      res = MATH_SPACE_VERYVERYTHIN;
      break;
    case KW_VERYTHINMATHSPACE:
      res = MATH_SPACE_VERYTHIN;
      break;
    case KW_THINMATHSPACE:
      res = MATH_SPACE_THIN;
      break;
    case KW_MEDIUMMATHSPACE:
      res = MATH_SPACE_MEDIUM;
      break;
    case KW_THICKMATHSPACE:
      res = MATH_SPACE_THICK;
      break;
    case KW_VERYTHICKMATHSPACE:
      res = MATH_SPACE_VERYTHICK;
      break;
    case KW_VERYVERYTHICKMATHSPACE:
      res = MATH_SPACE_VERYVERYTHICK;
      break;
    default:
      res = MATH_SPACE_NOTVALID;
      break;
    }
  }

  return res;
}

UnitId ToUnitId(const Value* value)
{
  assert(value != NULL);

  UnitId res = UNIT_NOTVALID;

  if (value->IsKeyword()) {
    switch (value->ToKeyword()) {
    case KW_PERCENTAGE: res = UNIT_PERCENTAGE; break;
    case KW_EM:         res = UNIT_EM; break;
    case KW_EX: 	res = UNIT_EX; break;
    case KW_PX: 	res = UNIT_PX; break;
    case KW_IN: 	res = UNIT_IN; break;
    case KW_CM: 	res = UNIT_CM; break;
    case KW_MM: 	res = UNIT_MM; break;
    case KW_PT: 	res = UNIT_PT; break;
    case KW_PC: 	res = UNIT_PC; break;
    default:            res = UNIT_NOTVALID; break;
    }
  }

  return res;
}

FontStyleId ToFontStyleId(const Value* value)
{
  assert(value != NULL);

  if      (value->IsKeyword(KW_NORMAL)) return FONT_STYLE_NORMAL;
  else if (value->IsKeyword(KW_ITALIC)) return FONT_STYLE_ITALIC;
  else return FONT_STYLE_NOTVALID;
}

FontWeightId ToFontWeightId(const Value* value)
{
  assert(value != NULL);

  if      (value->IsKeyword(KW_NORMAL)) return FONT_WEIGHT_NORMAL;
  else if (value->IsKeyword(KW_BOLD)) return FONT_WEIGHT_BOLD;
  else return FONT_WEIGHT_NOTVALID;
}

RGBValue ToRGB(const Value* value)
{
  assert(value != NULL);

  if (value->IsRGB()) return value->ToRGB();

  RGBValue res = BLACK_COLOR;

  if (value->IsKeyword()) {
    switch (value->ToKeyword()) {
    case KW_BLACK: res = BLACK_COLOR; break;
    case KW_SILVER: res = SILVER_COLOR; break;
    case KW_GRAY: res = GRAY_COLOR; break;
    case KW_WHITE: res = WHITE_COLOR; break;
    case KW_MAROON: res = MAROON_COLOR; break;
    case KW_RED: res = RED_COLOR; break;
    case KW_PURPLE: res = PURPLE_COLOR; break;
    case KW_FUCHSIA: res = FUCHSIA_COLOR; break;
    case KW_GREEN: res = GREEN_COLOR; break;
    case KW_LIME: res = LIME_COLOR; break;
    case KW_OLIVE: res = OLIVE_COLOR; break;
    case KW_YELLOW: res = YELLOW_COLOR; break;
    case KW_NAVY: res = NAVY_COLOR; break;
    case KW_BLUE: res = BLUE_COLOR; break;
    case KW_TEAL: res = TEAL_COLOR; break;
    case KW_AQUA: res = AQUA_COLOR; break;
    default:      assert(IMPOSSIBLE); break;
    }
  }
    
  return res; 
}

OperatorFormId ToFormId(const Value* value)
{
  assert(value != NULL);
  assert(value->IsKeyword());

  OperatorFormId res = OP_FORM_NOTVALID;

  switch (value->ToKeyword()) {
  case KW_PREFIX: res = OP_FORM_PREFIX; break;
  case KW_INFIX: res = OP_FORM_INFIX; break;
  case KW_POSTFIX: res = OP_FORM_POSTFIX; break;
  default: assert(IMPOSSIBLE); break;
  }

  return res;
}

FractionAlignId ToFractionAlignId(const Value* value)
{
  assert(value != NULL);

  FractionAlignId res = FRAC_ALIGN_NOTVALID;

  if (value->IsKeyword()) {
    switch (value->ToKeyword()) {
    case KW_LEFT:   res = FRAC_ALIGN_LEFT; break;
    case KW_RIGHT:  res = FRAC_ALIGN_RIGHT; break;
    case KW_CENTER: res = FRAC_ALIGN_CENTER; break;
    default:        res = FRAC_ALIGN_NOTVALID; break;
    }
  }

  return res;
}
