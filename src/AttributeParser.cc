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

#include <vector>

#include "Variant.hh"
//#include "StringUnicode.hh"
#include "AttributeParser.hh"
//#include "ValueSequence.hh"
#include "ValueConversion.hh"

SmartPtr<Value>
keywordParser(KeywordId id, StringTokenizer& st)
{
  unsigned mark = st.GetMark();
  SmartPtr<Value> res;

  st.SkipSpaces();
  if (!st.MoreTokens()) return res;

  if (st.Parse(id)) res = Variant<KeywordId>::create(id);
  else st.SetMark(mark);

  return res;
}

SmartPtr<Value>
integerParser(StringTokenizer& st)
{
  unsigned mark = st.GetMark();
  SmartPtr<Value> res;
  int n;

  st.SkipSpaces();
  if (!st.MoreTokens()) return res;

  if (st.ParseInteger(n)) res = Variant<int>::create(n);
  else st.SetMark(mark);

  return res;
}

SmartPtr<Value>
unsignedIntegerParser(StringTokenizer& st)
{
  unsigned mark = st.GetMark();
  SmartPtr<Value> res;
  int n;

  st.SkipSpaces();
  if (!st.MoreTokens()) return res;

  if (st.ParseUnsignedInteger(n)) res = Variant<int>::create(n);
  else st.SetMark(mark);

  return res;
}

SmartPtr<Value>
sequenceParser(AttributeParser parser[], unsigned n, StringTokenizer& st)
{
  assert(parser);
  assert(n > 1);

  std::vector< SmartPtr<Value> > sequence;
  sequence.reserve(n);
  for (unsigned i = 0; i < n; i++)
    {
      SmartPtr<Value> value = (parser[i])(st);
      if (!value) return 0;
      sequence.push_back(value);
    }

  return Variant< std::vector< SmartPtr<Value> > >::create(sequence);
}

SmartPtr<Value>
alternativeParser(AttributeParser parser[], unsigned n, StringTokenizer& st)
{
  assert(parser);

  for (unsigned i = 0; i < n; i++)
    if (SmartPtr<Value> res = (parser[i](st)))
      return res;

  return 0;
}

SmartPtr<Value>
alternativeParser(KeywordId id[], unsigned n, StringTokenizer& st)
{
  assert(id);

  for (unsigned i = 0; i < n; i++)
    if (SmartPtr<Value> res = keywordParser(id[i], st))
      return res;

  return 0;
}

SmartPtr<Value>
listParser(AttributeParser start, AttributeParser parser, AttributeParser end,
	   StringTokenizer& st, bool possiblyEmpty)
{
  assert(parser);

  unsigned mark = st.GetMark();

  st.SkipSpaces();
  if (!st.MoreTokens()) return 0;

  if (start)
    {
      SmartPtr<Value> startValue = start(st);
      if (!startValue)
	{
	  st.SetMark(mark);
	  return 0;
	}
    }

  std::vector< SmartPtr<Value> > seq;
  for (SmartPtr<Value> p = parser(st); p; p = parser(st))
    seq.push_back(p);

  if (end)
    {
      SmartPtr<Value> endValue = end(st);
      if (!endValue)
	{
	  st.SetMark(mark);
	  return 0;
	}
    }

  if (!possiblyEmpty && seq.empty()) return 0;

  return Variant< std::vector< SmartPtr<Value> > >::create(seq);
}

SmartPtr<Value>
optionParser(AttributeParser parser, StringTokenizer& st)
{
  assert(parser);

  SmartPtr<Value> res = parser(st);
  if (!res) res = Variant<void>::create();

  return res;
}

SmartPtr<Value>
numberParser(StringTokenizer& st)
{
  float n;
  unsigned mark = st.GetMark();
  SmartPtr<Value> res;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.ParseNumber(n)) res = Variant<float>::create(n);
  else st.SetMark(mark);

  return res;
}

SmartPtr<Value>
unsignedNumberParser(StringTokenizer& st)
{
  float n;
  unsigned mark = st.GetMark();
  SmartPtr<Value> res;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.ParseUnsignedNumber(n)) res = Variant<float>::create(n);
  else st.SetMark(mark);

  return res;
}

SmartPtr<Value>
unitParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_EM, KW_EX, KW_PT, KW_PC, KW_PX, KW_IN, KW_CM, KW_MM };

  return alternativeParser(id, 8, st);
}

SmartPtr<Value>
percentageParser(StringTokenizer& st)
{
  return keywordParser(KW_PERCENTAGE, st);
}

SmartPtr<Value>
unitPercentageParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      unitParser,
      percentageParser
    };

  return alternativeParser(parser, 2, st);
}

SmartPtr<Value>
unitOptionParser(StringTokenizer& st)
{
  return optionParser(unitPercentageParser, st);
}

SmartPtr<Value>
numberUnitParser(StringTokenizer& st)
{
  float n;
  unsigned mark = st.GetMark();
  SmartPtr<Value> res;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.ParseNumber(n))
    {
      SmartPtr<Value> unit = unitPercentageParser(st);
      if (unit)
	{
	  UnitId unitId = ToUnitId(unit);
	  if (unitId == UNIT_PERCENTAGE)
	    res = Variant<UnitValue>::create(UnitValue(n / 100, UNIT_PERCENTAGE));
	  else
	    res = Variant<UnitValue>::create(UnitValue(n, unitId));
	} else if (n == 0.0)
	  res = Variant<UnitValue>::create(UnitValue(0.0, UNIT_ZERO));
      else
	st.SetMark(mark);
  } else
    st.SetMark(mark);

  return res;
}

SmartPtr<Value>
numberUnitOptionParser(StringTokenizer& st)
{
  AttributeParser parser[] = { numberParser, unitOptionParser };

  return sequenceParser(parser, 2, st);
}

SmartPtr<Value>
numberUnitListParser(StringTokenizer& st)
{
  return listParser(0, numberUnitParser, 0, st);
}

SmartPtr<Value>
booleanParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_TRUE, KW_FALSE };
  return alternativeParser(id, 2, st);
}

SmartPtr<Value>
booleanListParser(StringTokenizer& st)
{
  return listParser(0, booleanParser, 0, st);
}

SmartPtr<Value>
fontWeightParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_NORMAL, KW_BOLD };
  return alternativeParser(id, 2, st);
}

SmartPtr<Value>
fontStyleParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_NORMAL, KW_ITALIC };
  return alternativeParser(id, 2, st);
}

SmartPtr<Value>
stringParser(StringTokenizer& st)
{
  unsigned mark = st.GetMark();

  st.SkipSpaces();
  if (!st.MoreTokens()) return 0;

  String s;
  if (st.ParseString(s)) return Variant<String>::create(s);
  else st.SetMark(mark);

  return 0;
}

SmartPtr<Value>
colorParser(StringTokenizer& st)
{
  KeywordId id[] = {
    KW_BLACK, KW_SILVER, KW_GRAY, KW_WHITE, KW_MAROON, KW_RED, KW_PURPLE,
    KW_FUCHSIA, KW_GREEN, KW_LIME, KW_OLIVE, KW_YELLOW, KW_NAVY, KW_BLUE,
    KW_TEAL, KW_AQUA
  };

  StringTokenizer stNC(toLowerCase(st.GetString()));

  SmartPtr<Value> res = alternativeParser(id, 16, stNC);
  if (res) return res;

  unsigned mark = st.GetMark();

  st.SkipSpaces();
  if (!st.MoreTokens()) return 0;

  RGBValue v;
  if (st.ParseRGB(v))
    return Variant<RGBValue>::create(v);
  else
    st.SetMark(mark);

  return 0;
}

SmartPtr<Value>
backgroundParser(StringTokenizer& st)
{
  SmartPtr<Value> res = keywordParser(KW_TRANSPARENT, st);
  if (!res) res = colorParser(st);

  return res;
}

SmartPtr<Value>
alignParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_TOP, KW_BOTTOM, KW_CENTER, KW_BASELINE, KW_AXIS };

  return alternativeParser(id, 5, st);
}

SmartPtr<Value>
integerOptionParser(StringTokenizer& st)
{
  return optionParser(integerParser, st);
}

SmartPtr<Value>
tableAlignParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      alignParser,
      integerOptionParser
    };

  return sequenceParser(parser, 2, st);
}

SmartPtr<Value>
rowAlignParser(StringTokenizer& st)
{
  return alignParser(st);
}

SmartPtr<Value>
rowAlignListParser(StringTokenizer& st)
{
  return listParser(0, alignParser, 0, st);
}

SmartPtr<Value>
columnAlignParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LEFT, KW_CENTER, KW_RIGHT };

  return alternativeParser(id, 3, st);
}

SmartPtr<Value>
columnAlignListParser(StringTokenizer& st)
{
  return listParser(0, columnAlignParser, 0, st);
}

SmartPtr<Value>
groupAlignParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LEFT, KW_CENTER, KW_RIGHT, KW_DECIMALPOINT };

  return alternativeParser(id, 4, st);
}

SmartPtr<Value>
groupAlignListParser(StringTokenizer& st)
{
  return listParser(0, groupAlignParser, 0, st);
}

SmartPtr<Value>
leftBraceParser(StringTokenizer& st)
{
  return keywordParser(KW_LBRACE, st);
}

SmartPtr<Value>
rightBraceParser(StringTokenizer& st)
{
  return keywordParser(KW_RBRACE, st);
}

SmartPtr<Value>
bracedGroupAlignListParser(StringTokenizer& st)
{
  return listParser(leftBraceParser, groupAlignParser, rightBraceParser, st);
}

SmartPtr<Value>
groupAlignListListParser(StringTokenizer& st)
{
  return listParser(0, bracedGroupAlignListParser, 0, st);
}

SmartPtr<Value>
namedSpaceParser(StringTokenizer& st)
{
  KeywordId id[] =
    {
      KW_VERYVERYTHINMATHSPACE,
      KW_VERYTHINMATHSPACE,
      KW_THINMATHSPACE,
      KW_MEDIUMMATHSPACE,
      KW_THICKMATHSPACE,
      KW_VERYTHICKMATHSPACE,
      KW_VERYVERYTHICKMATHSPACE
    };

  return alternativeParser(id, 7, st);
}

SmartPtr<Value>
spaceParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      namedSpaceParser,
      numberUnitParser
    };

  return alternativeParser(parser, 2, st);
}

SmartPtr<Value>
spaceListParser(StringTokenizer& st)
{
  return listParser(0, spaceParser, 0, st);
}

SmartPtr<Value>
columnWidthParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_AUTO, KW_FIT };

  SmartPtr<Value> value = alternativeParser(id, 2, st);
  if (!value) value = spaceParser(st);

  return value;
}

SmartPtr<Value>
columnWidthListParser(StringTokenizer& st)
{
  return listParser(0, columnWidthParser, 0, st);
}

SmartPtr<Value>
lineTypeParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_NONE, KW_SOLID, KW_DASHED };

  return alternativeParser(id, 3, st);
}

SmartPtr<Value>
lineTypeListParser(StringTokenizer& st)
{
  return listParser(0, lineTypeParser, 0, st);
}

SmartPtr<Value>
tableFrameSpacingParser(StringTokenizer& st)
{
  AttributeParser parser[] = { spaceParser, spaceParser };

  return sequenceParser(parser, 2, st);
}

SmartPtr<Value>
tableWidthParser(StringTokenizer& st)
{
  if (SmartPtr<Value> res = keywordParser(KW_AUTO, st))
    return res;

  return numberUnitParser(st);
}

SmartPtr<Value>
tableSideParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LEFT, KW_RIGHT, KW_LEFTOVERLAP, KW_RIGHTOVERLAP };

  return alternativeParser(id, 4, st);
}

SmartPtr<Value>
plusMinusParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_PLUS, KW_MINUS };

  return alternativeParser(id, 2, st);
}

SmartPtr<Value>
plusMinusOptionParser(StringTokenizer& st)
{
  return optionParser(plusMinusParser, st);
}
  
SmartPtr<Value>
scriptLevelParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      plusMinusOptionParser,
      unsignedIntegerParser
    };

  return sequenceParser(parser, 2, st);
}

SmartPtr<Value>
operatorFormParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_PREFIX, KW_INFIX, KW_POSTFIX };

  return alternativeParser(id, 3, st);
}

SmartPtr<Value>
operatorMinSizeParser(StringTokenizer& st)
{
  if (SmartPtr<Value> res = namedSpaceParser(st))
    return res;

  AttributeParser parser[] =
    {
      unsignedNumberParser,
      unitOptionParser
    };
  
  return sequenceParser(parser, 2, st);
}

SmartPtr<Value>
operatorMaxSizeParser(StringTokenizer& st)
{
  if (SmartPtr<Value> res = keywordParser(KW_INFINITY, st))
    return res;
  else
    return operatorMinSizeParser(st);
}

SmartPtr<Value>
fenceParser(StringTokenizer& st)
{
  return Variant<String>::create(st.GetString());
}

SmartPtr<Value>
separatorsParser(StringTokenizer& st)
{
  return Variant<String>::create(deleteSpaces(st.GetString()));
}

SmartPtr<Value>
fracAlignParser(StringTokenizer& st)
{
  return columnAlignParser(st);
}

SmartPtr<Value>
lineThicknessParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_THIN, KW_MEDIUM, KW_THICK };

  if (SmartPtr<Value> value = alternativeParser(id, 3, st))
    return value;
  else
    return numberUnitOptionParser(st);
}

SmartPtr<Value>
pseudoUnitParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_WIDTH, KW_LSPACE, KW_HEIGHT, KW_DEPTH };

  return alternativeParser(id, 4, st);
}

SmartPtr<Value>
pseudoUnitOptionParser(StringTokenizer& st)
{
  return optionParser(pseudoUnitParser, st);
}

SmartPtr<Value>
percentagePseudoUnitParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      percentageParser,
      pseudoUnitOptionParser
    };

  return sequenceParser(parser, 2, st);
}

SmartPtr<Value>
dimensionParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      percentagePseudoUnitParser,
      pseudoUnitParser,
      unitParser
    };

  return alternativeParser(parser, 3, st);
}

SmartPtr<Value>
paddedWidthDimensionParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      dimensionParser,
      namedSpaceParser
    };

  return alternativeParser(parser, 2, st);
}

SmartPtr<Value>
paddedWidthParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      plusMinusOptionParser,
      unsignedNumberParser,
      paddedWidthDimensionParser
    };

  return sequenceParser(parser, 3, st);
}

SmartPtr<Value>
paddedValueParser(StringTokenizer& st)
{
  AttributeParser parser[] =
    {
      plusMinusOptionParser,
      unsignedNumberParser,
      dimensionParser
    };

  return sequenceParser(parser, 3, st);
}

SmartPtr<Value>
modeParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_DISPLAY, KW_INLINE };

  return alternativeParser(id, 2, st);
}

SmartPtr<Value>
displayParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_BLOCK, KW_INLINE };

  return alternativeParser(id, 2, st);
}

SmartPtr<Value>
lineBreakParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_AUTO, KW_NEWLINE, KW_INDENTINGNEWLINE,
		     KW_NOBREAK, KW_BADBREAK, KW_GOODBREAK };

  return alternativeParser(id, 6, st);
}

SmartPtr<Value>
alignMarkEdgeParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LEFT, KW_RIGHT };

  return alternativeParser(id, 2, st);
}

SmartPtr<Value>
notationParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LONGDIV, KW_ACTUARIAL, KW_RADICAL };

  return alternativeParser(id, 3, st);
}

SmartPtr<Value>
mathVariantParser(StringTokenizer& st)
{
  /* The order of the following keywords is _VERY_ important.
   * If a keyword is a prefix of another keyword and it is listed
   * before the longer one, then the shortest keyword will always
   * match first. Example: bold, bold-italic. bold is matched even
   * if the attribute has value bold-italic
   */
  KeywordId id[] = {
    KW_NORMAL, KW_ITALIC, KW_DOUBLE_STRUCK, 
    KW_SCRIPT, KW_FRAKTUR, KW_MONOSPACE,
    KW_BOLD_SCRIPT, KW_BOLD_ITALIC, KW_BOLD_FRAKTUR, KW_BOLD_SANS_SERIF, KW_BOLD,
    KW_SANS_SERIF_ITALIC, KW_SANS_SERIF_BOLD_ITALIC, KW_SANS_SERIF };

  return alternativeParser(id, 14, st);
}

SmartPtr<Value>
mathSizeParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_SMALL, KW_NORMAL, KW_BIG };

  if (SmartPtr<Value> value = alternativeParser(id, 3, st))
    return value;
  else
    return numberUnitParser(st);
}
