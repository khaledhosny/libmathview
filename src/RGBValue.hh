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

#ifndef RGBValue_hh
#define RGBValue_hh

typedef unsigned RGBValue;

#define MKRGB(r,g,b)   (((r) & 0xff) | (((g) & 0xff) << 8) | (((b) & 0xff) << 16))
#define GETRED(rgb)    ((rgb) & 0xff)
#define GETGREEN(rgb)  (((rgb) >> 8) & 0xff)
#define GETBLUE(rgb)   (((rgb) >> 16) & 0xff)
#define GETALPHA(rgb)  (((rgb) >> 24) & 0xff)
 
#define BLACK_COLOR    MKRGB(0x00,0x00,0x00)
#define SILVER_COLOR   MKRGB(0xc0,0xc0,0xc0)
#define GRAY_COLOR     MKRGB(0x80,0x80,0x80)
#define WHITE_COLOR    MKRGB(0xff,0xff,0xff)
#define MAROON_COLOR   MKRGB(0x80,0x00,0x00)
#define RED_COLOR      MKRGB(0xff,0x00,0x00)
#define PURPLE_COLOR   MKRGB(0x80,0x00,0x80)
#define FUCHSIA_COLOR  MKRGB(0xff,0x00,0xff)
#define GREEN_COLOR    MKRGB(0x00,0x80,0x00)
#define LIME_COLOR     MKRGB(0x00,0xff,0x00)
#define OLIVE_COLOR    MKRGB(0x80,0x80,0x00)
#define YELLOW_COLOR   MKRGB(0xff,0xff,0x00)
#define NAVY_COLOR     MKRGB(0x00,0x00,0x80)
#define BLUE_COLOR     MKRGB(0x00,0x00,0xff)
#define TEAL_COLOR     MKRGB(0x00,0x80,0x80)
#define AQUA_COLOR     MKRGB(0x00,0xff,0xff)

#define SELECTED_COLOR MKRGB(0xe6, 0xe6, 0xfa)

#endif // RGBValue_hh
