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

#include <config.h>
#include <assert.h>

#include "CharMap.hh"

bool
CharMap::MapsStretchyChar(Char ch) const
{
  return (type == CHAR_MAP_STRETCHY && stretchy.code == ch);
}

bool
CharMap::MapsChar(Char ch) const
{
  bool res = false;

  switch (type) {
  case CHAR_MAP_RANGE:
    res = ch >= range.first && ch <= range.last;
    break;
  case CHAR_MAP_MULTI:
    res = ch >= multi.first && ch <= multi.last && multi.index[ch - multi.first] != NULLCHAR;
    break;
  case CHAR_MAP_SINGLE:
    res = ch == single.code;
    break;
  case CHAR_MAP_STRETCHY:
    // res = ch == stretchy.code;
    res = false;
    break;
  default:
    assert(IMPOSSIBLE);
    break;
  }

  return res;
}

bool
CharMap::MapsChar(Char ch, bool stretchy) const
{
  return stretchy ? MapsStretchyChar(ch) : MapsChar(ch);
}

// MapsSimpleChar: same as MapsChar, however, if the char appears to be
// stretchy, then it must have at least one simple char
bool
CharMap::MapsSimpleChar(Char ch) const
{
  if (MapsChar(ch)) return true;
  return (type == CHAR_MAP_STRETCHY && stretchy.code == ch && stretchy.simple[0] != NULLCHAR);
}

char
CharMap::Map(Char ch, bool large) const
{
  // WARNING: the following test has been suppressed. It is supposed that
  // who's calling Map has already checked that this CharMap can render the char
  //if (!MapsChar(ch)) return NULLCHAR;

  char res = NULLCHAR;

  switch (type) {
  case CHAR_MAP_RANGE:
    res = range.offset + ch - range.first;
    break;
  case CHAR_MAP_MULTI:
    res = multi.index[ch - multi.first];
    break;
  case CHAR_MAP_SINGLE:
    res = single.index;
    break;
  case CHAR_MAP_STRETCHY:
    if (large) {
      unsigned i;
      for (i = 0; i < MAX_SIMPLE_CHARS && stretchy.simple[i] != NULLCHAR; i++) ;
      res = stretchy.simple[i / 2];
    } else      
      res = stretchy.simple[0];
    break;
  }

  return res;
}

StretchId
CharMap::GetStretch() const
{
  return (type == CHAR_MAP_STRETCHY) ? stretchy.direction : STRETCH_NO;
}

void
FontifiedChar::GetBoundingBox(BoundingBox& box) const
{
  assert(font != NULL);
  font->CharBox(nch, box);
}
