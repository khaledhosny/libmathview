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

#ifndef keyword_hh
#define keyword_hh

#include "keyword.dec"
#include "tag.dec"
#include "attribute.dec"

// Layout

enum LayoutId {
  LAYOUT_MIN,
  LAYOUT_MAX,
  LAYOUT_AUTO
};

// Breakability

enum BreakId {
  BREAK_AUTO = -1,
  BREAK_NO,
  BREAK_BAD,
  BREAK_GOOD,
  BREAK_YES,
  BREAK_INDENT
};

// Units

enum UnitId {
  UNIT_NOTVALID = -1,
  UNIT_ZERO,
  UNIT_EM,
  UNIT_EX,
  UNIT_PX,
  UNIT_IN,
  UNIT_CM,
  UNIT_MM,
  UNIT_PT,
  UNIT_PC,
  UNIT_PERCENTAGE,
  UNIT_SCALED             // for internal use only (?)
};

// Table Identifiers

enum ColumnWidthId {
  COLUMN_WIDTH_NOTVALID = -1,
  COLUMN_WIDTH_AUTO,
  COLUMN_WIDTH_FIXED,
  COLUMN_WIDTH_PERCENTAGE,
  COLUMN_WIDTH_FIT
};

enum TableLineId {
  TABLE_LINE_NOTVALID = -1,
  TABLE_LINE_NONE,
  TABLE_LINE_SOLID,
  TABLE_LINE_DASHED
};

enum TableAlignId {
  TABLE_ALIGN_TOP,
  TABLE_ALIGN_BOTTOM,
  TABLE_ALIGN_CENTER,
  TABLE_ALIGN_BASELINE,
  TABLE_ALIGN_AXIS
};

enum TableSideId {
  TABLE_SIDE_LEFT,
  TABLE_SIDE_RIGHT,
  TABLE_SIDE_LEFTOVERLAP,
  TABLE_SIDE_RIGHTOVERLAP
};

enum RowAlignId {
  ROW_ALIGN_NOTVALID = -1,
  ROW_ALIGN_TOP,
  ROW_ALIGN_BOTTOM,
  ROW_ALIGN_CENTER,
  ROW_ALIGN_BASELINE,
  ROW_ALIGN_AXIS
};

enum ColumnAlignId {
  COLUMN_ALIGN_NOTVALID = -1,
  COLUMN_ALIGN_LEFT,
  COLUMN_ALIGN_CENTER,
  COLUMN_ALIGN_RIGHT
};

enum SpacingId {
  SPACING_NOTVALID = -1,
  SPACING_FIXED,
  SPACING_PERCENTAGE
};

enum GroupAlignId {
  GROUP_ALIGN_NOTVALID = -1,
  GROUP_ALIGN_LEFT,
  GROUP_ALIGN_CENTER,
  GROUP_ALIGN_RIGHT,
  GROUP_ALIGN_DECIMALPOINT
};

enum MarkAlignType {
  MARK_ALIGN_NOTVALID,
  MARK_ALIGN_LEFT,
  MARK_ALIGN_RIGHT
};

// Math Spaces

enum MathSpaceId {
  MATH_SPACE_NOTVALID = -1,
  MATH_SPACE_VERYVERYTHIN,
  MATH_SPACE_VERYTHIN,
  MATH_SPACE_THIN,
  MATH_SPACE_MEDIUM,
  MATH_SPACE_THICK,
  MATH_SPACE_VERYTHICK,
  MATH_SPACE_VERYVERYTHICK,
  MATH_SPACE_LAST
};

// Font Weights

enum FontWeightId {
  FONT_WEIGHT_NOTVALID = -1,
  FONT_WEIGHT_NORMAL,
  FONT_WEIGHT_BOLD
};

// Font Styles

enum FontStyleId {
  FONT_STYLE_NOTVALID = -1,
  FONT_STYLE_NORMAL,
  FONT_STYLE_ITALIC
};

// Operator forms

enum OperatorFormId {
  OP_FORM_NOTVALID = -1,
  OP_FORM_PREFIX,
  OP_FORM_INFIX,
  OP_FORM_POSTFIX
};

// Fractions alignment

enum FractionAlignId {
  FRAC_ALIGN_NOTVALID = -1,
  FRAC_ALIGN_LEFT,
  FRAC_ALIGN_CENTER,
  FRAC_ALIGN_RIGHT
};

void        InitKeywordValues(void);

KeywordId   KeywordIdOfName(const char*);
AttributeId AttributeIdOfName(const char*, const char*);
AttributeId AttributeIdOfName(const char*);
TagId       TagIdOfName(const char*);

const char* NameOfKeywordId(KeywordId);
const char* NameOfAttributeId(AttributeId);
const char* NameOfTagId(TagId);
const char* NameOfUnitId(UnitId);

const class Value* ValueOfKeywordId(KeywordId);

#endif // keyword_hh
