// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __RGBColor_hh__
#define __RGBColor_hh__

#include <cstdint>

struct RGBColor
{
  RGBColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xff)
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

  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
};

#endif // __RGBColor_hh__
