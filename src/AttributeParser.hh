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

#include "SmartPtr.hh"
#include "StringTokenizer.hh"
#include "MathMLObject.hh"
#include "Value.hh"

#define ATTRIBUTE_PARSER(name) SmartPtr<Value> name(StringTokenizer&);

typedef SmartPtr<Value> (*AttributeParser)(StringTokenizer&);
typedef SmartPtr<Value> (*AttributeListParser)(AttributeParser,
					       AttributeParser,
					       AttributeParser,
					       StringTokenizer&);

SmartPtr<Value> keywordParser(StringTokenizer&);

SmartPtr<Value> listParser(AttributeParser,
			   AttributeParser,
			   AttributeParser,
			   StringTokenizer&,
			   bool = false);

SmartPtr<Value> sequenceParser(AttributeParser*, unsigned, StringTokenizer&);
SmartPtr<Value> alternativeParser(AttributeParser*, unsigned, StringTokenizer&);
SmartPtr<Value> alternativeParser(KeywordId*, unsigned, StringTokenizer&);
SmartPtr<Value> optionParser(AttributeParser, StringTokenizer&);

ATTRIBUTE_PARSER(booleanParser);
ATTRIBUTE_PARSER(booleanListParser);
ATTRIBUTE_PARSER(integerParser);
ATTRIBUTE_PARSER(unsignedIntegerParser);
ATTRIBUTE_PARSER(numberParser);
ATTRIBUTE_PARSER(unsignedNumberParser);
ATTRIBUTE_PARSER(numberUnitParser);
ATTRIBUTE_PARSER(numberUnitListParser);
ATTRIBUTE_PARSER(stringParser);
ATTRIBUTE_PARSER(colorParser);
ATTRIBUTE_PARSER(backgroundParser);
ATTRIBUTE_PARSER(spaceParser);
ATTRIBUTE_PARSER(spaceListParser);
ATTRIBUTE_PARSER(fontWeightParser);
ATTRIBUTE_PARSER(fontStyleParser);
ATTRIBUTE_PARSER(groupAlignListListParser);
ATTRIBUTE_PARSER(groupAlignListParser);
ATTRIBUTE_PARSER(lineTypeParser);
ATTRIBUTE_PARSER(lineTypeListParser);
ATTRIBUTE_PARSER(rowAlignParser);
ATTRIBUTE_PARSER(rowAlignListParser);
ATTRIBUTE_PARSER(columnAlignParser);
ATTRIBUTE_PARSER(columnAlignListParser);
ATTRIBUTE_PARSER(columnWidthListParser);
ATTRIBUTE_PARSER(tableAlignParser);
ATTRIBUTE_PARSER(tableFrameParser);
ATTRIBUTE_PARSER(tableFrameSpacingParser);
ATTRIBUTE_PARSER(tableSideParser);
ATTRIBUTE_PARSER(tableWidthParser);
ATTRIBUTE_PARSER(scriptLevelParser);
ATTRIBUTE_PARSER(operatorFormParser);
ATTRIBUTE_PARSER(operatorMaxSizeParser);
ATTRIBUTE_PARSER(operatorMinSizeParser);
ATTRIBUTE_PARSER(fenceParser);
ATTRIBUTE_PARSER(separatorsParser);
ATTRIBUTE_PARSER(lineThicknessParser);
ATTRIBUTE_PARSER(fracAlignParser);
ATTRIBUTE_PARSER(paddedWidthParser);
ATTRIBUTE_PARSER(paddedValueParser);
ATTRIBUTE_PARSER(modeParser);
ATTRIBUTE_PARSER(displayParser);
ATTRIBUTE_PARSER(lineBreakParser);
ATTRIBUTE_PARSER(alignMarkEdgeParser);
ATTRIBUTE_PARSER(notationParser);
ATTRIBUTE_PARSER(mathVariantParser);
ATTRIBUTE_PARSER(mathSizeParser);

#undef ATTRIBUTE_PARSER

#endif // AttributeParser_hh
