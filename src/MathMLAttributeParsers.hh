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

#ifndef __MathMLAttributeParsers_hh__
#define __MathMLAttributeParsers_hh__

#include "TemplateStringParsers.hh"

typedef Set<T_FALSE,T_TRUE> TokenSet_Boolean;
typedef Set<T_NORMAL,T_BOLD,T_ITALIC,T_BOLD_ITALIC,T_DOUBLE_STRUCK,T_BOLD_FRAKTUR,T_SCRIPT,
	    T_BOLD_SCRIPT,T_FRAKTUR,T_SANS_SERIF,T_BOLD_SANS_SERIF,T_SANS_SERIF_ITALIC,
	    T_SANS_SERIF_BOLD_ITALIC,T_MONOSPACE> TokenSet_MathVariant;
typedef Set<T_EM,T_EX,T_PX,T_IN,T_CM,T_MM,T_PT,T_PC,T__PERCENTAGE> TokenSet_Unit;
typedef Union< Set<T_BLACK,T_SILVER,T_GRAY,T_WHITE,T_MAROON,T_RED,T_PURPLE,T_FUCHSIA>,
	       Set<T_GREEN,T_LIME,T_OLIVE,T_YELLOW,T_NAVY,T_BLUE,T_TEAL,T_AQUA> > TokenSet_HTMLColor;
typedef Set<T_NEGATIVEVERYVERYTHICKMATHSPACE,T_NEGATIVEVERYTHICKMATHSPACE,T_NEGATIVETHICKMATHSPACE,
	    T_NEGATIVEMEDIUMMATHSPACE,T_NEGATIVETHINMATHSPACE,T_NEGATIVEVERYTHINMATHSPACE,
	    T_NEGATIVEVERYVERYTHINMATHSPACE,T_VERYVERYTHINMATHSPACE,T_VERYTHINMATHSPACE,
	    T_THINMATHSPACE,T_MEDIUMMATHSPACE,T_THICKMATHSPACE,T_VERYTHICKMATHSPACE,
	    T_VERYVERYTHICKMATHSPACE> TokenSet_NamedSpace;
typedef Set<T_LEFT,T_CENTER,T_RIGHT> TokenSet_HorizontalAlign;

typedef Parse<ScanToken,bool> ParseBoolean;
typedef Parse<ScanUnsignedInteger,int> ParseUnsignedInteger;
typedef Parse<ScanInteger,int> ParseInteger;
typedef ParseTokenSet<TokenSet_MathVariant> ParseMathVariant;
typedef ParseChoice< ParseRGBColor,ParseTokenSet<TokenSet_HTMLColor> > ParseColor;
typedef ParseTokenSet<TokenSet_NamedSpace> ParseNamedSpace;
typedef ParseChoice<ParseLength,ParseNamedSpace> ParseLengthOrNamedSpace;
typedef Parse<ScanUnsignedNumber,float> ParseUnsignedNumber;
typedef Parse<ScanNumber,float> ParseNumber;
typedef ParseTokenSet<TokenSet_HorizontalAlign> ParseHorizontalAlign;
typedef ParseTokenSet<TokenSet_Unit> ParseUnit;

// Element
typedef ParseString Parse_Element_class;
typedef ParseString Parse_Element_other;
typedef ParseString Parse_Element_id;

// Token
typedef ParseMathVariant Parse_Token_mathvariant;
typedef ParseChoice<ParseLength,ParseTokenSet< Set<T_SMALL,T_NORMAL,T_BIG> > > Parse_Token_mathsize;
typedef ParseColor Parse_Token_mathcolor;
typedef ParseColor Parse_Token_mathbackground;
typedef ParseLength Parse_Token_fontsize;
typedef ParseTokenSet< Set<T_NORMAL,T_BOLD> > Parse_Token_fontweight;
typedef ParseTokenSet< Set<T_NORMAL,T_ITALIC> > Parse_Token_fontstyle;
typedef ParseString Parse_Token_fontfamily;
typedef ParseColor Parse_Token_color;
typedef ParseTokenSet< Set<T__PLUS,T__MINUS> > ParseSign;
typedef ParseZeroOrOne<ParseSign> ParseOptionalSign;

// Operator
typedef ParseTokenSet< Set<T_PREFIX,T_INFIX,T_POSTFIX> > Parse_Operator_form;
typedef ParseBoolean Parse_Operator_fence;
typedef ParseBoolean Parse_Operator_separator;
typedef ParseLengthOrNamedSpace Parse_Operator_lspace;
typedef ParseLengthOrNamedSpace Parse_Operator_rspace;
typedef ParseBoolean Parse_Operator_stretchy;
typedef ParseBoolean Parse_Operator_symmetric;
typedef ParseChoice<ParseLengthOrNamedSpace,
		    ParseChoice<ParseNumber,
				ParseToken<T_INFINITY> > > Parse_Operator_maxsize;
typedef ParseChoice<ParseLengthOrNamedSpace,ParseNumber> Parse_Operator_minsize;
typedef ParseBoolean Parse_Operator_largeop;
typedef ParseBoolean Parse_Operator_movablelimits;
typedef ParseBoolean Parse_Operator_accent;

// Space
typedef ParseLengthOrNamedSpace Parse_Space_width;
typedef ParseLength Parse_Space_height;
typedef ParseLength Parse_Space_depth;
typedef ParseTokenSet< Set<T_AUTO,T_NEWLINE,T_INDENTINGNEWLINE,T_NOBREAK,T_GOODBREAK,T_BADBREAK> > Parse_Space_linebreak;

// StringLit
typedef ParseString Parse_StringLit_lquote;
typedef ParseString Parse_StringLit_rquote;

// Glyph
typedef ParseString Parse_Glyph_alt;
typedef ParseString Parse_Glyph_fontfamily;
typedef ParseInteger Parse_Glyph_index;

// Fraction
typedef ParseChoice<ParseLength,
		    ParseChoice<ParseNumber,
				ParseTokenSet< Set<T_THIN,T_MEDIUM,T_THICK> > > > Parse_Fraction_linethickness;
typedef ParseHorizontalAlign Parse_Fraction_numalign;
typedef ParseHorizontalAlign Parse_Fraction_denomalign;
typedef ParseBoolean Parse_Fraction_bevelled;

// Style
typedef ParseSeq<ParseOptionalSign,ParseUnsignedInteger> Parse_Style_scriptlevel;
typedef ParseBoolean Parse_Style_displaystyle;
typedef ParseNumber Parse_Style_scriptsizemultiplier;
typedef ParseLength Parse_Style_scriptminsize;
typedef ParseColor Parse_Style_color;
typedef ParseColor Parse_Style_mathcolor;
typedef ParseChoice<ParseColor,ParseToken<T_TRANSPARENT> > Parse_Style_background;
typedef ParseLength Parse_Style_negativeveryverythickmathspace;
typedef ParseLength Parse_Style_negativeverythickmathspace;
typedef ParseLength Parse_Style_negativethickmathspace;
typedef ParseLength Parse_Style_negativemediummathspace;
typedef ParseLength Parse_Style_negativethinmathspace;
typedef ParseLength Parse_Style_negativeverythinmathspace;
typedef ParseLength Parse_Style_negativeveryverythinmathspace;
typedef ParseLength Parse_Style_veryverythickmathspace;
typedef ParseLength Parse_Style_verythickmathspace;
typedef ParseLength Parse_Style_thickmathspace;
typedef ParseLength Parse_Style_mediummathspace;
typedef ParseLength Parse_Style_thinmathspace;
typedef ParseLength Parse_Style_verythinmathspace;
typedef ParseLength Parse_Style_veryverythinmathspace;
typedef Parse_Token_fontsize Parse_Style_fontsize;
typedef Parse_Token_mathsize Parse_Style_mathsize;

// Padded
typedef ParseToken<T__PERCENTAGE> ParsePercentage;
typedef ParseTokenSet< Set<T_WIDTH,T_LSPACE,T_HEIGHT,T_DEPTH> > ParsePseudoUnit;
typedef ParseZeroOrOne<ParsePseudoUnit> ParseOptionalPseudoUnit;
typedef ParseChoice<ParseSeq<ParsePercentage,ParseOptionalPseudoUnit>,
		    ParseChoice<ParsePseudoUnit,ParseUnit> > ParseDimension;

typedef ParseSeq<ParseOptionalSign,
		 ParseSeq<ParseUnsignedNumber,
			  ParseChoice<ParseDimension,ParseNamedSpace> > > Parse_Padded_width;
typedef ParseSeq<ParseOptionalSign, ParseSeq<ParseUnsignedNumber, ParseDimension> > Parse_Padded_lspace;
typedef ParseSeq<ParseOptionalSign, ParseSeq<ParseUnsignedNumber, ParseDimension> > Parse_Padded_height;
typedef ParseSeq<ParseOptionalSign, ParseSeq<ParseUnsignedNumber, ParseDimension> > Parse_Padded_depth;

// Fenced
typedef Parse<ScanAny,Char> ParseCharacter;
typedef ParseString Parse_Fenced_open;
typedef ParseString Parse_Fenced_close;
//typedef ParseZeroOrOne<ParseOneOrMore<ParseCharacter> > Parse_Fenced_separators;
typedef ParseString Parse_Fenced_separators;

// Enclose
typedef ParseString Parse_Enclose_notation;

// Script
typedef ParseLength Parse_Script_subscriptshift;
typedef ParseLength Parse_Script_superscriptshift;

// UnderOver
typedef ParseBoolean Parse_UnderOver_accentunder;
typedef ParseBoolean Parse_UnderOver_accent;

// Table
typedef ParseTokenSet< Set<T_LEFT,T_CENTER,T_RIGHT,T_DECIMALPOINT> > ParseGroupAlignment;
typedef ParseOneOrMore<ParseGroupAlignment> ParseGroupAlignmentList;
typedef ParseOneOrMore<ParseSeq< ParseToken<T__LBRACE>,
				 ParseSeq< ParseGroupAlignmentList, ParseToken<T__RBRACE> > > > ParseGroupAlignmentListList;
		  
typedef ParseTokenSet< Set<T_TOP,T_CENTER,T_BOTTOM,T_BASELINE,T_AXIS> > ParseVerticalAlign;
typedef ParseChoice<ParseLengthOrNamedSpace, ParseTokenSet< Set<T_AUTO,T_FIT> > > ParseColumnWidthSpec;
typedef ParseTokenSet< Set<T_NONE,T_SOLID,T_DASHED> > ParseLineSpec;

typedef ParseSeq<ParseVerticalAlign,ParseZeroOrOne<ParseInteger> > Parse_Table_align;
typedef ParseOneOrMore<ParseVerticalAlign> Parse_Table_rowalign;
typedef ParseOneOrMore<ParseHorizontalAlign> Parse_Table_columnalign;
typedef ParseGroupAlignmentListList Parse_Table_groupalign;
typedef ParseOneOrMore<ParseBoolean> Parse_Table_alignmentscope;
typedef ParseOneOrMore<ParseColumnWidthSpec> Parse_Table_columnwidth;
typedef ParseChoice<ParseLength,ParseToken<T_AUTO> > Parse_Table_width;
typedef ParseOneOrMore<ParseLength> Parse_Table_rowspacing;
typedef ParseOneOrMore<ParseLengthOrNamedSpace> Parse_Table_columnspacing;
typedef ParseOneOrMore<ParseLineSpec> Parse_Table_rowlines;
typedef ParseOneOrMore<ParseLineSpec> Parse_Table_columnlines;
typedef ParseLineSpec Parse_Table_frame;
typedef ParseSeq<ParseLengthOrNamedSpace,ParseLengthOrNamedSpace> Parse_Table_framespacing;
typedef ParseBoolean Parse_Table_equalrows;
typedef ParseBoolean Parse_Table_equalcolumns;
typedef ParseBoolean Parse_Table_displaystyle;
typedef ParseTokenSet< Set<T_LEFT,T_RIGHT,T_LEFTOVERLAP,T_RIGHTOVERLAP> > Parse_Table_side;
typedef ParseLengthOrNamedSpace Parse_Table_minlabelspacing;

// TableRow
typedef ParseVerticalAlign Parse_TableRow_rowalign;
typedef ParseOneOrMore<ParseColumnWidthSpec> Parse_TableRow_columnalign;
typedef ParseGroupAlignmentListList Parse_TableRow_groupalign;

// TableCell
typedef ParseInteger Parse_TableCell_rowspan;
typedef ParseInteger Parse_TableCell_columnspan;
typedef ParseVerticalAlign Parse_TableCell_rowalign;
typedef ParseHorizontalAlign Parse_TableCell_columnalign;
typedef ParseGroupAlignmentList Parse_TableCell_groupalign;

// AlignMark
typedef ParseTokenSet< Set<T_LEFT,T_RIGHT> > Parse_AlignMark_edge;

// AlignGroup
typedef ParseGroupAlignment Parse_AlignGroup_groupalign;

// Action
typedef ParseString Parse_Action_actiontype;
typedef ParseInteger Parse_Action_selection;

// math
typedef ParseTokenSet< Set<T_DISPLAY,T_INLINE> > Parse_math_mode;
typedef ParseTokenSet< Set<T_BLOCK,T_INLINE> > Parse_math_display;

#endif // __MathMLAttributeParsers_hh__
