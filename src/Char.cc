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
#include <stddef.h>
#include <string.h>

#include "Char.hh"
#include "String.hh"
#include "unidefs.h"
#include "keyword.hh"

static struct NonMarkingCharData {
  Char    ch;
  Char    modifier;
  int     spacing; // 1/18em
  BreakId breakability;
} nmChar[] = {
  { 0x0009,           0x0000, 9 * 8, BREAK_GOOD }, // Tab
  { 0x000a,           0x0000, 0,     BREAK_YES },  // NewLine
  { 0x0020,           0x0000, 9,     BREAK_GOOD }, // Space
  { 0x00a0,           0x0000, 9,     BREAK_NO },   // NonBreakingSpace
  { 0x200b,           0x0000, 0,     BREAK_NO },   // ZeroWidthSpace
  { 0x200a,           0x0000, 1,     BREAK_NO },   // VeryThinSpace
  { 0x2009,           0x0000, 3,     BREAK_NO },   // ThinSpace
  { 0x2005,           0x0000, 4,     BREAK_NO },   // MediumSpace
  { 0x200a,           0xfe00, -1,    BREAK_NO },   // NegativeVeryThinSpace
  { 0x2009,           0xfe00, -3,    BREAK_NO },   // NegativeThinSPace
  { 0x205f,           0xfe00, -4,    BREAK_NO },   // NegativeMediumSpace
  { 0x2005,           0xfe00, -5,    BREAK_NO },   // NegativeThickSpace
#if 0
  // these entries have been removed since now they are treated
  // in a context-sensitive way
  { U_INVISIBLETIMES, 0x0000, 3,     BREAK_NO },   // InvisibleTimes
  { U_APPLYFUNCTION,  0x0000, 5,     BREAK_NO },   // ApplyFunction
#endif

  { 0x0000,           0x0000, 0,     BREAK_NO }
};

Char
getBiggestChar(const Char* s, unsigned len)
{
  assert(s != NULL);

  Char big = 0;
  for (unsigned i = 0; i < len; i++)
    if (s[i] > big) big = s[i];

  return big;
}

bool
isPlain(const char* s)
{
  return isPlain(s, strlen(s));
}

bool
isPlain(const char* s, unsigned length)
{
  for (unsigned i = 0; i < length; i++)
    if (!isPlain(s[i])) return false;

  return true;
}

bool
isPlain(const Char* s, unsigned length)
{
  for (unsigned i = 0; i < length; i++)
    if (!isPlain(s[i])) return false;

  return true;
}

bool
isPlain(const String& str, unsigned offset, unsigned length)
{
  assert(offset + length <= str.GetLength());

  for (unsigned i = 0; i < length && offset + i < str.GetLength(); i++)
    if (!isPlain(str.GetChar(offset + i))) return false;

  return true;
}

static int
getNonMarkingCharIndex(Char ch, Char modifier)
{
  for (unsigned i = 0; nmChar[i].ch != 0; i++)
    if (ch == nmChar[i].ch && modifier == nmChar[i].modifier) return i;
  return -1;
}

unsigned
isNonMarkingChar(Char ch, Char modifier, int* spacing, BreakId* breakability)
{
  int index = getNonMarkingCharIndex(ch, modifier);
  if (index < 0) return 0;
  if (spacing != NULL) *spacing = nmChar[index].spacing;
  if (breakability != NULL) *breakability = nmChar[index].breakability;
  return (modifier != 0) ? 2 : 1;
}

unsigned
isNonMarkingChar(Char ch, int* spacing, BreakId* breakability)
{
  return isNonMarkingChar(ch, 0, spacing, breakability);
}

unsigned
isNonMarkingChar(const String& s, unsigned offset, int* spacing, BreakId* breakability)
{
  assert(offset < s.GetLength());
  Char ch = s.GetChar(offset);
  Char modifier = (offset + 1 < s.GetLength() && isVariant(s.GetChar(offset + 1))) ? s.GetChar(offset + 1) : 0;
  return isNonMarkingChar(ch, modifier, spacing, breakability);
}

unsigned
isNonMarkingChar(const String& s, int* spacing, BreakId* breakability)
{
  return isNonMarkingChar(s, 0, spacing, breakability);
}

bool
isCombiningBelow(Char ch)
{
  return 
    (ch >= 0x031c && ch <= 0x0333) ||
    (ch >= 0x0339 && ch <= 0x033c);
}

bool
isCombiningOverlay(Char ch)
{
  return
    (ch >= 0x0334 && ch <= 0x0338) ||
    ch == 0x20d2 || ch == 0x20d3 ||
    ch == 0x20e5;
}

