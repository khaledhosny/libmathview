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

#ifndef AttributeParser_hh
#define AttributeParser_hh

#include <config.h>

#include "StringTokenizer.hh"
#include "MathMLObject.hh"
#include "Value.hh"

#define PARSER(name) const Value* name(StringTokenizer&);

typedef const Value* (*AttributeParser)(StringTokenizer&);
typedef const Value* (*AttributeListParser)(AttributeParser,
					    AttributeParser,
					    AttributeParser,
					    StringTokenizer&);

const Value* keywordParser(StringTokenizer&);

const Value* listParser(AttributeParser,
			AttributeParser,
			AttributeParser,
			StringTokenizer&,
			bool = false);

const Value* sequenceParser(AttributeParser*, unsigned, StringTokenizer&);
const Value* alternativeParser(AttributeParser*, unsigned, StringTokenizer&);
const Value* alternativeParser(KeywordId*, unsigned, StringTokenizer&);
const Value* optionParser(AttributeParser, StringTokenizer&);

PARSER(booleanParser);
PARSER(booleanListParser);
PARSER(integerParser);
PARSER(unsignedIntegerParser);
PARSER(numberParser);
PARSER(unsignedNumberParser);
PARSER(numberUnitParser);
PARSER(numberUnitListParser);
PARSER(stringParser);
PARSER(colorParser);
PARSER(backgroundParser);
PARSER(spaceParser);
PARSER(spaceListParser);
PARSER(fontWeightParser);
PARSER(fontStyleParser);
PARSER(groupAlignListListParser);
PARSER(groupAlignListParser);
PARSER(lineTypeParser);
PARSER(lineTypeListParser);
PARSER(rowAlignParser);
PARSER(rowAlignListParser);
PARSER(columnAlignParser);
PARSER(columnAlignListParser);
PARSER(columnWidthListParser);
PARSER(tableAlignParser);
PARSER(tableFrameParser);
PARSER(tableFrameSpacingParser);
PARSER(tableSideParser);
PARSER(tableWidthParser);
PARSER(scriptLevelParser);
PARSER(operatorFormParser);
PARSER(operatorMaxSizeParser);
PARSER(operatorMinSizeParser);
PARSER(fenceParser);
PARSER(separatorsParser);
PARSER(lineThicknessParser);
PARSER(fracAlignParser);
PARSER(paddedWidthParser);
PARSER(paddedValueParser);
PARSER(modeParser);
PARSER(displayParser);
PARSER(lineBreakParser);
PARSER(alignMarkEdgeParser);
PARSER(notationParser);
PARSER(mathVariantParser);
PARSER(mathSizeParser);

#endif // AttributeParser_hh
