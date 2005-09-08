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

#ifndef __RGBColor_hh__
#define __RGBColor_hh__

#include "gmv_defines.h"
#include "Char.hh"

struct GMV_MathView_EXPORT RGBColor
{
  RGBColor(UChar8 r = 0, UChar8 g = 0, UChar8 b = 0, UChar8 a = 0xff)
    : red(r), green(g), blue(b), alpha(a)
  { }

  static RGBColor BLACK   (void) { return RGBColor(0x00,0x00,0x00); }
  static RGBColor SILVER  (void) { return RGBColor(0xc0,0xc0,0xc0); }
  static RGBColor GRAY    (void) { return RGBColor(0x80,0x80,0x80); }
  static RGBColor WHITE   (void) { return RGBColor(0xff,0xff,0xff); }
  static RGBColor MAROON  (void) { return RGBColor(0x80,0x00,0x00); }
  static RGBColor RED     (void) { return RGBColor(0xff,0x00,0x00); }
  static RGBColor PURPLE  (void) { return RGBColor(0x80,0x00,0x80); }
  static RGBColor FUCHSIA (void) { return RGBColor(0xff,0x00,0xff); }
  static RGBColor GREEN   (void) { return RGBColor(0x00,0x80,0x00); }
  static RGBColor LIME    (void) { return RGBColor(0x00,0xff,0x00); }
  static RGBColor OLIVE   (void) { return RGBColor(0x80,0x80,0x00); }
  static RGBColor YELLOW  (void) { return RGBColor(0xff,0xff,0x00); }
  static RGBColor NAVY    (void) { return RGBColor(0x00,0x00,0x80); }
  static RGBColor BLUE    (void) { return RGBColor(0x00,0x00,0xff); }
  static RGBColor TEAL    (void) { return RGBColor(0x00,0x80,0x80); }
  static RGBColor AQUA    (void) { return RGBColor(0x00,0xff,0xff); }

  void set(unsigned r, unsigned g, unsigned b, unsigned a)
  { red = r; green = g; blue = b; alpha = a; }

  bool operator==(const RGBColor& c) const
  { return red == c.red && green == c.green && blue == c.blue && alpha == c.alpha; }
  bool operator!=(const RGBColor& c) const
  { return !(*this == c); }

  bool transparent(void) const { return alpha == 0; }

  UChar8 red;
  UChar8 green;
  UChar8 blue;
  UChar8 alpha;
};

#endif // __RGBColor_hh__
