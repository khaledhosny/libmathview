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

#ifndef __BoxMLAttributeParsers_hh__
#define __BoxMLAttributeParsers_hh__

#include "CommonStringParsers.hh"

typedef ParseColor Parse_BoxML_Text_color;
typedef ParseColor Parse_BoxML_Text_background;
typedef ParseLength Parse_BoxML_Text_size;
typedef ParseColor Parse_BoxML_Ink_color;
typedef ParseLength Parse_BoxML_Space_width;
typedef ParseLength Parse_BoxML_Space_height;
typedef ParseLength Parse_BoxML_Space_depth;
typedef ParseTokenSet< Set<T_BASELINE,T_TOP,T_CENTER,T_BOTTOM> > Parse_BoxML_H_align;
typedef ParseInteger Parse_BoxML_V_enter;
typedef ParseInteger Parse_BoxML_V_exit;
typedef ParseTokenSet< Set<T_LEFT,T_CENTER,T_RIGHT> > Parse_BoxML_V_align;
typedef ParseLength Parse_BoxML_V_minlinespacing;
typedef ParseLength Parse_BoxML_Layout_width;
typedef ParseLength Parse_BoxML_Layout_height;
typedef ParseLength Parse_BoxML_Layout_depth;
typedef ParseLength Parse_BoxML_At_x;
typedef ParseLength Parse_BoxML_At_y;
typedef ParseUnsignedInteger Parse_BoxML_Action_selection;
typedef ParseString Parse_BoxML_Action_actiontype;

#endif // __BoxMLAttributeParsers_hh__
