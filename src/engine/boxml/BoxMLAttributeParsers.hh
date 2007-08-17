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

#ifndef __BoxMLAttributeParsers_hh__
#define __BoxMLAttributeParsers_hh__

#include "CommonStringParsers.hh"

typedef ParseColor Parse_BoxML_Text_color;
typedef ParseColor Parse_BoxML_Text_background;
typedef ParseLength Parse_BoxML_Text_size;
typedef ParseChoice<ParseLength, ParseToken<T_AUTO> > Parse_BoxML_Text_width;
typedef ParseColor Parse_BoxML_Ink_color;
typedef ParseLength Parse_BoxML_Space_width;
typedef ParseLength Parse_BoxML_Space_height;
typedef ParseLength Parse_BoxML_Space_depth;
typedef ParseLength Parse_BoxML_H_spacing;
typedef ParseInteger Parse_BoxML_V_enter;
typedef ParseInteger Parse_BoxML_V_exit;
typedef ParseLength Parse_BoxML_V_indent;
typedef ParseLength Parse_BoxML_V_minlinespacing;
typedef ParseLength Parse_BoxML_HV_spacing;
typedef ParseLength Parse_BoxML_HV_indent;
typedef ParseLength Parse_BoxML_HV_minlinespacing;
typedef ParseLength Parse_BoxML_HOV_spacing;
typedef ParseLength Parse_BoxML_HOV_indent;
typedef ParseLength Parse_BoxML_HOV_minlinespacing;
typedef ParseLength Parse_BoxML_Layout_width;
typedef ParseLength Parse_BoxML_Layout_height;
typedef ParseLength Parse_BoxML_Layout_depth;
typedef ParseLength Parse_BoxML_At_x;
typedef ParseLength Parse_BoxML_At_y;
typedef ParseUnsignedInteger Parse_BoxML_Action_selection;
typedef ParseString Parse_BoxML_Action_actiontype;
typedef ParseOneOrMore<ParseKeyword> Parse_BoxML_Decor_type;
typedef ParseChoice<ParseColor,ParseToken<T_TRANSPARENT> > Parse_BoxML_Decor_color;
typedef ParseLength Parse_BoxML_Decor_thickness;

#endif // __BoxMLAttributeParsers_hh__
