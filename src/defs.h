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

#define IMPOSSIBLE      0
#define NOT_IMPLEMENTED 0

#define EPSILON 0.01f

#define NULLPTR 0

#define DPI 72

// changed these defs to float value, this will shut 
// up the compliler complaining about double to float conversions

#define SCALED_POINTS_PER_MM  186467.98f
#define SCALED_POINTS_PER_CM 1864679.8f
#define SCALED_POINTS_PER_PT   65536.0f
#define SCALED_POINTS_PER_PC  786432.0f
#define SCALED_POINTS_PER_IN (SCALED_POINTS_PER_PT * 72.27f)
#define SCALED_POINTS_PER_PX (SCALED_POINTS_PER_IN / DPI)

#define DEFAULT_FONT_SIZE         12
#define DEFAULT_LINK_FOREGROUND   MKRGB(50, 100, 139)
#define DEFAULT_LINK_BACKGROUND   WHITE_COLOR
#define DEFAULT_SELECT_FOREGROUND BLACK_COLOR
#define DEFAULT_SELECT_BACKGROUND MKRGB(135, 206, 250)
#define DEFAULT_FOREGROUND        BLACK_COLOR
#define DEFAULT_BACKGROUND        WHITE_COLOR

#endif // defs_h
