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

#ifndef CharMap_hh
#define CharMap_hh

#include "Char.hh"
#include "AFont.hh"

enum StretchId {
  STRETCH_NO,
  STRETCH_VERTICAL,
  STRETCH_HORIZONTAL,
  STRETCH_BOTH
};

enum CharMapType {
  CHAR_MAP_RANGE,
  CHAR_MAP_MULTI,
  CHAR_MAP_SINGLE,
  CHAR_MAP_STRETCHY
};

enum StretchyCharGlyphClass {
  SC_FIRST,  // index of the first glyph (== 0 if none)
  SC_MIDDLE, // index of the middle glyph (== 0 if none)
  SC_LAST,   // index of the last glyph (== 0 if none)
  SC_REPEAT  // index of the repeat glyph (usually != 0)
};

#define MAX_SIMPLE_CHARS 4

struct CharMap {
  CharMapType type;

  union {
    struct {
      Char     first;  // first char covered by the range
      Char     last;   // last char covered by the range (last >= first)
      unsigned offset; // offset of the glyph for the first char
    } range;

    struct {
      Char  first; // first char
      Char  last;  // last char
      char* index; // ptr of native chars (last - first + 1) elements
    } multi;

    struct {
      Char code;   // a single unicode char
      char index;  // native index
    } single;

    struct {
      Char      code;       // a single unicode char
      StretchId direction;
      char      simple[MAX_SIMPLE_CHARS];
      char      compound[SC_REPEAT + 1];
    } stretchy;
  };

  bool      MapsChar(Char) const;
  bool      MapsSimpleChar(Char) const;
  bool      MapsStretchyChar(Char) const;
  bool      MapsChar(Char, bool) const;
  StretchId GetStretch(void) const;
  char      Map(Char, bool = false) const;
};

struct FontifiedChar {
  char           nch;
  const AFont*   font;
  const CharMap* charMap;

  void   GetBoundingBox(BoundingBox&) const;
};

#endif // CharMap_hh
