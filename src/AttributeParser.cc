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

#include "StringUnicode.hh"
#include "AttributeParser.hh"
#include "ValueSequence.hh"
#include "ValueConversion.hh"

const Value* keywordParser(KeywordId id, StringTokenizer& st)
{
  unsigned mark = st.GetMark();
  const Value* res = NULL;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.Parse(id)) res = new Value(id);
  else st.SetMark(mark);

  return res;
}

const Value* integerParser(StringTokenizer& st)
{
  unsigned mark = st.GetMark();
  const Value* res = NULL;
  int n;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.ParseInteger(&n)) res = new Value(n);
  else st.SetMark(mark);

  return res;
}

const Value* unsignedIntegerParser(StringTokenizer& st)
{
  unsigned mark = st.GetMark();
  const Value* res = NULL;
  int n;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.ParseUnsignedInteger(&n)) res = new Value(n);
  else st.SetMark(mark);

  return res;
}

const Value* sequenceParser(AttributeParser parser[],
			    unsigned n,
			    StringTokenizer& st)
{
  assert(parser != NULL);
  assert(n > 1);

  ValueSequence* sequence = new ValueSequence;
  for (unsigned i = 0; i < n; i++) {
    const Value* value = (parser[i])(st);
    if (value == NULL) {
      delete sequence;
      return NULL;
    }
    sequence->AddValue(value);
  }

  const Value* res = new Value(sequence);

  return res;
}

const Value* alternativeParser(AttributeParser parser[],
			       unsigned n,
			       StringTokenizer& st)
{
  assert(parser != NULL);

  const Value* res = NULL;
  unsigned i = 0;

  while (res == NULL && i < n) {
    res = (parser[i])(st);
    if (res == NULL) i++;
  }

  return res;
}

const Value* alternativeParser(KeywordId id[],
			       unsigned n,
			       StringTokenizer& st)
{
  assert(id != NULL);

  const Value* res = NULL;
  unsigned i = 0;

  while (res == NULL && i < n) {
    res = keywordParser(id[i], st);
    if (res == NULL) i++;
  }

  return res;
}

const Value* listParser(AttributeParser start,
			AttributeParser parser,
			AttributeParser end,
			StringTokenizer& st,
			bool possiblyEmpty)
{
  assert(parser != NULL);

  ValueSequence* seq = new ValueSequence;
  assert(seq != NULL);

  unsigned mark = st.GetMark();

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (start != NULL) {
    const Value* startValue = start(st);
    if (startValue == NULL) {
      st.SetMark(mark);
      return NULL;
    }
    delete startValue;
  }

  const Value* p = parser(st);

  while (p != NULL) {
    seq->AddValue(p);
    p = parser(st);
  }

  if (end != NULL) {
    const Value* endValue = end(st);
    if (endValue == NULL) {
      st.SetMark(mark);
      delete p;
      return NULL;
    }
    delete endValue;
  }

  if (!possiblyEmpty && seq->IsEmpty()) return NULL;

  const Value* res = new Value(seq);

  return res;
}

const Value* optionParser(AttributeParser parser, StringTokenizer& st)
{
  assert(parser != NULL);

  const Value* res = parser(st);
  if (res == NULL) res = new Value;

  return res;
}

const Value* numberParser(StringTokenizer& st)
{
  float n;
  unsigned mark = st.GetMark();
  const Value* res = NULL;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.ParseNumber(&n)) res = new Value(n);
  else st.SetMark(mark);

  return res;
}

const Value* unsignedNumberParser(StringTokenizer& st)
{
  float n;
  unsigned mark = st.GetMark();
  const Value* res = NULL;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.ParseUnsignedNumber(&n)) res = new Value(n);
  else st.SetMark(mark);

  return res;
}

const Value* unitParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_EM, KW_EX, KW_PT, KW_PC, KW_PX, KW_IN, KW_CM, KW_MM };

  return alternativeParser(id, 8, st);
}

const Value* percentageParser(StringTokenizer& st)
{
  return keywordParser(KW_PERCENTAGE, st);
}

const Value* unitPercentageParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    unitParser,
    percentageParser
  };

  return alternativeParser(parser, 2, st);
}

const Value* unitOptionParser(StringTokenizer& st)
{
  return optionParser(unitPercentageParser, st);
}

const Value* numberUnitParser(StringTokenizer& st)
{
  float n;
  unsigned mark = st.GetMark();
  const Value* res = NULL;

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  if (st.ParseNumber(&n)) {
    const Value* unit = unitPercentageParser(st);
    if (unit != NULL) {
      UnitId unitId = ToUnitId(unit);
      delete unit;
      if (unitId == UNIT_PERCENTAGE) res = new Value(n / 100, UNIT_PERCENTAGE);
      else res = new Value(n, unitId);
    } else if (n == 0.0) res = new Value(0.0, UNIT_ZERO);
    else st.SetMark(mark);
  } else st.SetMark(mark);

  return res;
}

const Value* numberUnitOptionParser(StringTokenizer& st)
{
  AttributeParser parser[] = { numberParser, unitOptionParser };

  return sequenceParser(parser, 2, st);
}

const Value* numberUnitListParser(StringTokenizer& st)
{
  return listParser(NULL, numberUnitParser, NULL, st);
}

const Value* booleanParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_TRUE, KW_FALSE };
  return alternativeParser(id, 2, st);
}

const Value* booleanListParser(StringTokenizer& st)
{
  return listParser(NULL, booleanParser, NULL, st);
}

const Value* fontWeightParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_NORMAL, KW_BOLD };
  return alternativeParser(id, 2, st);
}

const Value* fontStyleParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_NORMAL, KW_ITALIC };
  return alternativeParser(id, 2, st);
}

const Value* stringParser(StringTokenizer& st)
{
  unsigned mark = st.GetMark();

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  String* s = new StringUnicode;
  if (st.ParseString(s)) return new Value(s);
  else st.SetMark(mark);

  return NULL;
}

const Value* colorParser(StringTokenizer& st)
{
  KeywordId id[] = {
    KW_BLACK, KW_SILVER, KW_GRAY, KW_WHITE, KW_MAROON, KW_RED, KW_PURPLE,
    KW_FUCHSIA, KW_GREEN, KW_LIME, KW_OLIVE, KW_YELLOW, KW_NAVY, KW_BLUE,
    KW_TEAL, KW_AQUA
  };

  const String& source = st.GetString();
  String* sourceNC = source.Clone();
  assert(sourceNC != NULL);
  sourceNC->ToLowerCase();
  StringTokenizer stNC(*sourceNC);

  const Value* res = alternativeParser(id, 16, stNC);
  delete sourceNC;
  if (res != NULL) return res;

  unsigned mark = st.GetMark();

  st.SkipSpaces();
  if (!st.MoreTokens()) return NULL;

  RGBValue v;
  if (st.ParseRGB(&v)) {
    Value* res = new Value;
    res->SetRGB(v);
    return res;
  } else st.SetMark(mark);

  return NULL;
}

const Value* backgroundParser(StringTokenizer& st)
{
  const Value* res = keywordParser(KW_TRANSPARENT, st);
  if (res == NULL) res = colorParser(st);

  return res;
}

const Value* alignParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_TOP, KW_BOTTOM, KW_CENTER, KW_BASELINE, KW_AXIS };

  return alternativeParser(id, 5, st);
}

const Value* integerOptionParser(StringTokenizer& st)
{
  return optionParser(integerParser, st);
}

const Value* tableAlignParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    alignParser,
    integerOptionParser
  };

  return sequenceParser(parser, 2, st);
}

const Value* rowAlignParser(StringTokenizer& st)
{
  return alignParser(st);
}

const Value* rowAlignListParser(StringTokenizer& st)
{
  return listParser(NULL, alignParser, NULL, st);
}

const Value* columnAlignParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LEFT, KW_CENTER, KW_RIGHT };

  return alternativeParser(id, 3, st);
}

const Value* columnAlignListParser(StringTokenizer& st)
{
  return listParser(NULL, columnAlignParser, NULL, st);
}

const Value* groupAlignParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LEFT, KW_CENTER, KW_RIGHT, KW_DECIMALPOINT };

  return alternativeParser(id, 4, st);
}

const Value* groupAlignListParser(StringTokenizer& st)
{
  return listParser(NULL, groupAlignParser, NULL, st);
}

const Value* leftBraceParser(StringTokenizer& st)
{
  return keywordParser(KW_LBRACE, st);
}

const Value* rightBraceParser(StringTokenizer& st)
{
  return keywordParser(KW_RBRACE, st);
}

const Value* bracedGroupAlignListParser(StringTokenizer& st)
{
  return listParser(leftBraceParser,
		    groupAlignParser,
		    rightBraceParser,
		    st);
}

const Value* groupAlignListListParser(StringTokenizer& st)
{
  return listParser(NULL, bracedGroupAlignListParser, NULL, st);
}

const Value* namedSpaceParser(StringTokenizer& st)
{
  KeywordId id[] = {
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

const Value* spaceParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    namedSpaceParser,
    numberUnitParser
  };

  return alternativeParser(parser, 2, st);
}

const Value* spaceListParser(StringTokenizer& st)
{
  return listParser(NULL, spaceParser, NULL, st);
}

const Value* columnWidthParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_AUTO, KW_FIT };

  const Value* value = alternativeParser(id, 2, st);
  if (value == NULL) value = spaceParser(st);

  return value;
}

const Value* columnWidthListParser(StringTokenizer& st)
{
  return listParser(NULL, columnWidthParser, NULL, st);
}

const Value* lineTypeParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_NONE, KW_SOLID, KW_DASHED };

  return alternativeParser(id, 3, st);
}

const Value* lineTypeListParser(StringTokenizer& st)
{
  return listParser(NULL, lineTypeParser, NULL, st);
}

const Value* tableFrameSpacingParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    spaceParser,
    spaceParser
  };

  return sequenceParser(parser, 2, st);
}

const Value* tableWidthParser(StringTokenizer& st)
{
  const Value* res = keywordParser(KW_AUTO, st);

  if (res != NULL) return res;

  return numberUnitParser(st);
}

const Value* tableSideParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LEFT, KW_RIGHT, KW_LEFTOVERLAP, KW_RIGHTOVERLAP };

  return alternativeParser(id, 4, st);
}

const Value* plusMinusParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_PLUS, KW_MINUS };

  return alternativeParser(id, 2, st);
}

const Value* plusMinusOptionParser(StringTokenizer& st)
{
  return optionParser(plusMinusParser, st);
}
  
const Value* scriptLevelParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    plusMinusOptionParser,
    unsignedIntegerParser
  };

  return sequenceParser(parser, 2, st);
}

const Value* operatorFormParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_PREFIX, KW_INFIX, KW_POSTFIX };

  return alternativeParser(id, 3, st);
}

const Value* operatorMinSizeParser(StringTokenizer& st)
{
  const Value* res = namedSpaceParser(st);
  if (res != NULL) return res;

  AttributeParser parser[] = {
    unsignedNumberParser,
    unitOptionParser
  };
  
  return sequenceParser(parser, 2, st);
}

const Value* operatorMaxSizeParser(StringTokenizer& st)
{
  const Value* res = keywordParser(KW_INFINITY, st);
  if (res != NULL) return res;

  return operatorMinSizeParser(st);
}

const Value* fenceParser(StringTokenizer& st)
{
  const Value* value = new Value(st.GetString().Clone());
  return value;
}

const Value* separatorsParser(StringTokenizer& st)
{
  String* sep = st.GetString().Clone();
  sep->DeleteSpaces();

  const Value* value = NULL;

  if (sep->GetLength() > 0) value = new Value(sep);
  else {
    value = new Value(static_cast<String*>(NULL));
    delete sep;
  }

  return value;
}

const Value* fracAlignParser(StringTokenizer& st)
{
  return columnAlignParser(st);
}

const Value* lineThicknessParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_THIN, KW_MEDIUM, KW_THICK };

  const Value* value = alternativeParser(id, 3, st);
  if (value != NULL) return value;

  return numberUnitOptionParser(st);
}

const Value* pseudoUnitParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_WIDTH, KW_LSPACE, KW_HEIGHT, KW_DEPTH };

  return alternativeParser(id, 4, st);
}

const Value* pseudoUnitOptionParser(StringTokenizer& st)
{
  return optionParser(pseudoUnitParser, st);
}

const Value* percentagePseudoUnitParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    percentageParser,
    pseudoUnitOptionParser
  };

  return sequenceParser(parser, 2, st);
}

const Value* dimensionParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    percentagePseudoUnitParser,
    pseudoUnitParser,
    unitParser
  };

  return alternativeParser(parser, 3, st);
}

const Value* paddedWidthDimensionParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    dimensionParser,
    namedSpaceParser
  };

  return alternativeParser(parser, 2, st);
}

const Value* paddedWidthParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    plusMinusOptionParser,
    unsignedNumberParser,
    paddedWidthDimensionParser
  };

  return sequenceParser(parser, 3, st);
}

const Value* paddedValueParser(StringTokenizer& st)
{
  AttributeParser parser[] = {
    plusMinusOptionParser,
    unsignedNumberParser,
    dimensionParser
  };

  return sequenceParser(parser, 3, st);
}

const Value* modeParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_DISPLAY, KW_INLINE };

  return alternativeParser(id, 2, st);
}

const Value* displayParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_BLOCK, KW_INLINE };

  return alternativeParser(id, 2, st);
}

const Value* lineBreakParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_AUTO, KW_NEWLINE, KW_INDENTINGNEWLINE,
		     KW_NOBREAK, KW_BADBREAK, KW_GOODBREAK };

  return alternativeParser(id, 6, st);
}

const Value* alignMarkEdgeParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LEFT, KW_RIGHT };

  return alternativeParser(id, 2, st);
}

const Value* notationParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_LONGDIV, KW_ACTUARIAL, KW_RADICAL };

  return alternativeParser(id, 3, st);
}

const Value* mathVariantParser(StringTokenizer& st)
{
  KeywordId id[] = {
    KW_NORMAL, KW_BOLD, KW_ITALIC, KW_BOLD_ITALIC, KW_DOUBLE_STRUCK, KW_BOLD_FRAKTUR,
    KW_SCRIPT, KW_BOLD_SCRIPT, KW_FRAKTUR, KW_SANS_SERIF, KW_BOLD_SANS_SERIF,
    KW_SANS_SERIF_ITALIC, KW_SANS_SERIF_BOLD_ITALIC, KW_MONOSPACE };

  return alternativeParser(id, 13, st);
}

const Value* mathSizeParser(StringTokenizer& st)
{
  KeywordId id[] = { KW_SMALL, KW_NORMAL, KW_BIG };

  const Value* value = alternativeParser(id, 3, st);
  if (value != NULL) return value;

  return numberUnitParser(st);
}
