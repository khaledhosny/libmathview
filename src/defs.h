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

#ifndef defs_h
#define defs_h

#define MATHML_NS_URI "http://www.w3.org/1998/Math/MathML"

#define IMPOSSIBLE      0
#define NOT_IMPLEMENTED 0

#define EPSILON 0.01f

#define DPI 72

#define SP_PER_MM  186467.98
#define SP_PER_CM 1864679.8
#define SP_PER_PT   65536
#define SP_PER_BP   65781.76
#define SP_PER_PC  786432.0
#define SP_PER_IN 4736286.7
#define SP_PER_DD   70124.086
#define SP_PER_CC  841489.04
#define SP_PER_PX (SP_PER_IN / DPI)

#define DEFAULT_FONT_SIZE         12
#define DEFAULT_LINK_FOREGROUND   RGBColor(50, 100, 139)
#define DEFAULT_LINK_BACKGROUND   RGBColor::WHITE()
#define DEFAULT_SELECT_FOREGROUND RGBColor::BLACK()
#define DEFAULT_SELECT_BACKGROUND RGBColor(135, 206, 250)
#define DEFAULT_FOREGROUND        RGBColor::BLACK()
#define DEFAULT_BACKGROUND        RGBColor::WHITE()

#endif // defs_h
