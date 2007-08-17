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

#ifndef __MathMLAttributeParsers_hh__
#define __MathMLAttributeParsers_hh__

#include "CommonStringParsers.hh"

typedef Set<T_NORMAL,T_BOLD,T_ITALIC,T_BOLD_ITALIC,T_DOUBLE_STRUCK,T_BOLD_FRAKTUR,T_SCRIPT,
	    T_BOLD_SCRIPT,T_FRAKTUR,T_SANS_SERIF,T_BOLD_SANS_SERIF,T_SANS_SERIF_ITALIC,
	    T_SANS_SERIF_BOLD_ITALIC,T_MONOSPACE> TokenSet_MathVariant;
typedef Set<T_NEGATIVEVERYVERYTHICKMATHSPACE,T_NEGATIVEVERYTHICKMATHSPACE,T_NEGATIVETHICKMATHSPACE,
	    T_NEGATIVEMEDIUMMATHSPACE,T_NEGATIVETHINMATHSPACE,T_NEGATIVEVERYTHINMATHSPACE,
	    T_NEGATIVEVERYVERYTHINMATHSPACE,T_VERYVERYTHINMATHSPACE,T_VERYTHINMATHSPACE,
	    T_THINMATHSPACE,T_MEDIUMMATHSPACE,T_THICKMATHSPACE,T_VERYTHICKMATHSPACE,
	    T_VERYVERYTHICKMATHSPACE> TokenSet_NamedSpace;
typedef Set<T_LEFT,T_CENTER,T_RIGHT> TokenSet_HorizontalAlign;

typedef ParseTokenSet<TokenSet_MathVariant> ParseMathVariant;
typedef ParseTokenSet<TokenSet_NamedSpace> ParseNamedSpace;
typedef ParseChoice<ParseLength,ParseNamedSpace> ParseLengthOrNamedSpace;
typedef ParseTokenSet<TokenSet_HorizontalAlign> ParseHorizontalAlign;
typedef ParseTokenSet< Set<T__PLUS,T__MINUS> > ParseSign;
typedef ParseZeroOrOne<ParseSign> ParseOptionalSign;

// Element
typedef ParseString Parse_MathML_Element_class;
typedef ParseString Parse_MathML_Element_other;
typedef ParseString Parse_MathML_Element_id;

// Token
typedef ParseMathVariant Parse_MathML_Token_mathvariant;
typedef ParseChoice<ParseLength,ParseTokenSet< Set<T_SMALL,T_NORMAL,T_BIG> > > Parse_MathML_Token_mathsize;
typedef ParseColor Parse_MathML_Token_mathcolor;
typedef ParseColor Parse_MathML_Token_mathbackground;

// Operator
typedef ParseTokenSet< Set<T_PREFIX,T_INFIX,T_POSTFIX> > Parse_MathML_Operator_form;
typedef ParseBoolean Parse_MathML_Operator_fence;
typedef ParseBoolean Parse_MathML_Operator_separator;
typedef ParseLengthOrNamedSpace Parse_MathML_Operator_lspace;
typedef ParseLengthOrNamedSpace Parse_MathML_Operator_rspace;
typedef ParseBoolean Parse_MathML_Operator_stretchy;
typedef ParseBoolean Parse_MathML_Operator_symmetric;
typedef ParseChoice<ParseLengthOrNamedSpace,
		    ParseChoice<ParseNumber,
				ParseToken<T_INFINITY> > > Parse_MathML_Operator_maxsize;
typedef ParseChoice<ParseLengthOrNamedSpace,ParseNumber> Parse_MathML_Operator_minsize;
typedef ParseBoolean Parse_MathML_Operator_largeop;
typedef ParseBoolean Parse_MathML_Operator_movablelimits;
typedef ParseBoolean Parse_MathML_Operator_accent;

// Space
typedef ParseLengthOrNamedSpace Parse_MathML_Space_width;
typedef ParseLengthOrNamedSpace Parse_MathML_Space_height;
typedef ParseLengthOrNamedSpace Parse_MathML_Space_depth;
typedef ParseTokenSet< Set<T_AUTO,T_NEWLINE,T_INDENTINGNEWLINE,T_NOBREAK,T_GOODBREAK,T_BADBREAK> > Parse_MathML_Space_linebreak;

// StringLit
typedef ParseString Parse_MathML_StringLit_lquote;
typedef ParseString Parse_MathML_StringLit_rquote;

// Glyph
typedef ParseString Parse_MathML_Glyph_alt;
typedef ParseString Parse_MathML_Glyph_fontfamily;
typedef ParseInteger Parse_MathML_Glyph_index;

// Fraction
typedef ParseChoice<ParseLength,
		    ParseChoice<ParseNumber,
				ParseTokenSet< Set<T_THIN,T_MEDIUM,T_THICK> > > > Parse_MathML_Fraction_linethickness;
typedef ParseHorizontalAlign Parse_MathML_Fraction_numalign;
typedef ParseHorizontalAlign Parse_MathML_Fraction_denomalign;
typedef ParseBoolean Parse_MathML_Fraction_bevelled;

// Style
typedef ParseSeq<ParseOptionalSign,ParseUnsignedInteger> Parse_MathML_Style_scriptlevel;
typedef ParseBoolean Parse_MathML_Style_displaystyle;
typedef ParseNumber Parse_MathML_Style_scriptsizemultiplier;
typedef ParseLength Parse_MathML_Style_scriptminsize;
typedef ParseColor Parse_MathML_Style_mathcolor;
typedef ParseChoice<ParseColor,ParseToken<T_TRANSPARENT> > Parse_MathML_Style_mathbackground;
typedef Parse_MathML_Style_mathcolor Parse_MathML_Style_color;
typedef Parse_MathML_Style_mathbackground Parse_MathML_Style_background;
typedef ParseLength Parse_MathML_Style_negativeveryverythickmathspace;
typedef ParseLength Parse_MathML_Style_negativeverythickmathspace;
typedef ParseLength Parse_MathML_Style_negativethickmathspace;
typedef ParseLength Parse_MathML_Style_negativemediummathspace;
typedef ParseLength Parse_MathML_Style_negativethinmathspace;
typedef ParseLength Parse_MathML_Style_negativeverythinmathspace;
typedef ParseLength Parse_MathML_Style_negativeveryverythinmathspace;
typedef ParseLength Parse_MathML_Style_veryverythickmathspace;
typedef ParseLength Parse_MathML_Style_verythickmathspace;
typedef ParseLength Parse_MathML_Style_thickmathspace;
typedef ParseLength Parse_MathML_Style_mediummathspace;
typedef ParseLength Parse_MathML_Style_thinmathspace;
typedef ParseLength Parse_MathML_Style_verythinmathspace;
typedef ParseLength Parse_MathML_Style_veryverythinmathspace;
typedef Parse_MathML_Token_mathsize Parse_MathML_Style_mathsize;

// Padded
typedef ParseToken<T__PERCENTAGE> ParsePercentage;
typedef ParseTokenSet< Set<T_WIDTH,T_LSPACE,T_HEIGHT,T_DEPTH> > ParsePseudoUnit;
typedef ParseZeroOrOne<ParsePseudoUnit> ParseOptionalPseudoUnit;
typedef ParseChoice<ParseSeq<ParsePercentage,ParseOptionalPseudoUnit>,
		    ParseChoice<ParsePseudoUnit,ParseUnit> > ParseDimension;

typedef ParseSeq<ParseOptionalSign,
		 ParseSeq<ParseUnsignedNumber,
			  ParseChoice<ParseDimension,ParseNamedSpace> > > Parse_MathML_Padded_width;
typedef ParseSeq<ParseOptionalSign, ParseSeq<ParseUnsignedNumber, ParseDimension> > Parse_MathML_Padded_lspace;
typedef ParseSeq<ParseOptionalSign, ParseSeq<ParseUnsignedNumber, ParseDimension> > Parse_MathML_Padded_height;
typedef ParseSeq<ParseOptionalSign, ParseSeq<ParseUnsignedNumber, ParseDimension> > Parse_MathML_Padded_depth;

// Fenced
typedef Parse<ScanAny,Char> ParseCharacter;
typedef ParseString Parse_MathML_Fenced_open;
typedef ParseString Parse_MathML_Fenced_close;
//typedef ParseZeroOrOne<ParseOneOrMore<ParseCharacter> > Parse_MathML_Fenced_separators;
typedef ParseString Parse_MathML_Fenced_separators;

// Enclose
typedef ParseOneOrMore<ParseKeyword> Parse_MathML_Enclose_notation;

// Script
typedef ParseLength Parse_MathML_Script_subscriptshift;
typedef ParseLength Parse_MathML_Script_superscriptshift;

// UnderOver
typedef ParseBoolean Parse_MathML_UnderOver_accentunder;
typedef ParseBoolean Parse_MathML_UnderOver_accent;

// MultiScripts
typedef ParseLength Parse_MathML_MultiScripts_subscriptshift;
typedef ParseLength Parse_MathML_MultiScripts_superscriptshift;

// Table
typedef ParseTokenSet< Set<T_LEFT,T_CENTER,T_RIGHT,T_DECIMALPOINT> > ParseGroupAlignment;
typedef ParseOneOrMore<ParseGroupAlignment> ParseGroupAlignmentList;
typedef ParseOneOrMore<ParseSeq< ParseToken<T__LBRACE>,
				 ParseSeq< ParseGroupAlignmentList, ParseToken<T__RBRACE> > > > ParseGroupAlignmentListList;
		  
typedef ParseTokenSet< Set<T_TOP,T_CENTER,T_BOTTOM,T_BASELINE,T_AXIS> > ParseVerticalAlign;
typedef ParseChoice<ParseLengthOrNamedSpace, ParseTokenSet< Set<T_AUTO,T_FIT> > > ParseColumnWidthSpec;
typedef ParseTokenSet< Set<T_NONE,T_SOLID,T_DASHED> > ParseLineSpec;

typedef ParseSeq<ParseVerticalAlign,ParseZeroOrOne<ParseInteger> > Parse_MathML_Table_align;
typedef ParseOneOrMore<ParseVerticalAlign> Parse_MathML_Table_rowalign;
typedef ParseOneOrMore<ParseHorizontalAlign> Parse_MathML_Table_columnalign;
typedef ParseGroupAlignmentListList Parse_MathML_Table_groupalign;
typedef ParseOneOrMore<ParseBoolean> Parse_MathML_Table_alignmentscope;
typedef ParseOneOrMore<ParseColumnWidthSpec> Parse_MathML_Table_columnwidth;
typedef ParseChoice<ParseLength,ParseToken<T_AUTO> > Parse_MathML_Table_width;
typedef ParseOneOrMore<ParseLength> Parse_MathML_Table_rowspacing;
typedef ParseOneOrMore<ParseLengthOrNamedSpace> Parse_MathML_Table_columnspacing;
typedef ParseOneOrMore<ParseLineSpec> Parse_MathML_Table_rowlines;
typedef ParseOneOrMore<ParseLineSpec> Parse_MathML_Table_columnlines;
typedef ParseLineSpec Parse_MathML_Table_frame;
typedef ParseSeq<ParseLengthOrNamedSpace,ParseLengthOrNamedSpace> Parse_MathML_Table_framespacing;
typedef ParseBoolean Parse_MathML_Table_equalrows;
typedef ParseBoolean Parse_MathML_Table_equalcolumns;
typedef ParseBoolean Parse_MathML_Table_displaystyle;
typedef ParseTokenSet< Set<T_LEFT,T_RIGHT,T_LEFTOVERLAP,T_RIGHTOVERLAP> > Parse_MathML_Table_side;
typedef ParseLengthOrNamedSpace Parse_MathML_Table_minlabelspacing;

// TableRow
typedef ParseVerticalAlign Parse_MathML_TableRow_rowalign;
typedef ParseOneOrMore<ParseHorizontalAlign> Parse_MathML_TableRow_columnalign;
typedef ParseGroupAlignmentListList Parse_MathML_TableRow_groupalign;

// TableCell
typedef ParseInteger Parse_MathML_TableCell_rowspan;
typedef ParseInteger Parse_MathML_TableCell_columnspan;
typedef ParseVerticalAlign Parse_MathML_TableCell_rowalign;
typedef ParseHorizontalAlign Parse_MathML_TableCell_columnalign;
typedef ParseGroupAlignmentList Parse_MathML_TableCell_groupalign;

// AlignMark
typedef ParseTokenSet< Set<T_LEFT,T_RIGHT> > Parse_MathML_AlignMark_edge;

// AlignGroup
typedef ParseGroupAlignment Parse_MathML_AlignGroup_groupalign;

// Action
typedef ParseString Parse_MathML_Action_actiontype;
typedef ParseInteger Parse_MathML_Action_selection;

// math
typedef ParseTokenSet< Set<T_DISPLAY,T_INLINE> > Parse_MathML_math_mode;
typedef ParseTokenSet< Set<T_BLOCK,T_INLINE> > Parse_MathML_math_display;

#endif // __MathMLAttributeParsers_hh__
