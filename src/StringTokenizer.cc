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

#include <stddef.h>
#include <assert.h>
#ifdef HAVE_WCTYPE_H
#include <wctype.h>
#endif
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#include "MathMLAttribute.hh"
#include "StringTokenizer.hh"

static unsigned hexOfChar(Char ch)
{
  assert(iswxdigit(ch));
  ch = towupper(ch);
  if (ch <= '9') return ch - '0';
  else return ch - 'A' + 10;
}

StringTokenizer::StringTokenizer(const String& s0) : s(s0)
{
  Reset();
}

StringTokenizer::~StringTokenizer()
{
}

void StringTokenizer::Reset()
{
  offset = 0;
}

void StringTokenizer::SkipSpaces()
{
  while (offset < s.GetLength() && iswspace(s.GetChar(offset))) offset++;
}

void StringTokenizer::SkipToken()
{
  while (offset < s.GetLength() && !iswspace(s.GetChar(offset))) offset++;
}

bool StringTokenizer::MoreTokens()
{
  return offset < s.GetLength();
}

bool StringTokenizer::Parse(const char *str)
{
  assert(str != NULL);

  unsigned i = 0;
  while (offset + i < s.GetLength() &&
	 str[i] != '\0' &&
	 s.GetChar(offset + i) == static_cast<Char>(str[i])) i++;

  if (str[i] == '\0') {
    offset += i;
    return true;
  }

  return false;
}

bool StringTokenizer::Parse(KeywordId id)
{
  return Parse(NameOfKeywordId(id));
}

const char* StringTokenizer::ParseToken()
{
  unsigned i = 0;
  static char tempS[TEMP_BUFFER_SIZE + 1];

  while (offset < s.GetLength() &&
	 !iswspace(s.GetChar(offset)) &&
	 i < TEMP_BUFFER_SIZE) {
    tempS[i] = s.GetChar(offset);
    i++;
    offset++;
  }

  tempS[i] = 0;

  return tempS;
}

bool StringTokenizer::ParseKeyword(KeywordId* v)
{
  KeywordId id = KeywordIdOfName(ParseToken());
  if (id == KW_NOTVALID) return false;

  if (v != NULL) *v = id;
  return true;
}

bool StringTokenizer::ParseUnsignedInteger(int* v)
{
  int n = 0;
  unsigned nDigits = 0;
  while (offset < s.GetLength() && iswdigit(s.GetChar(offset))) {
    n = n * 10 + s.GetChar(offset) - '0';
    nDigits++;
    offset++;
  }

  if (nDigits == 0) return false;
   
  if (v != NULL) *v = n;

  return true;
}

bool StringTokenizer::ParseInteger(int* v)
{
  int n;
  bool negative = false;

  if (s.GetChar(offset) == '-') {
    negative = true;
    offset++;
  }

  if (!ParseUnsignedInteger(&n)) return false;

  if (v != NULL) *v = negative ? -((int) n) : (int) n;

  return true;
}

bool StringTokenizer::ParseUnsignedNumber(float* v)
{
  float res = 0.0;
  bool decimal = false; // true if decimal point found
  unsigned n = 0;       // number of digits after decimal point
  unsigned nDigits = 0;

  while (offset < s.GetLength() &&
	 (iswdigit(s.GetChar(offset)) ||
	  (!decimal && s.GetChar(offset) == '.'))) {
    if (s.GetChar(offset) == '.') {
      decimal = true;
    } else {
      res = res * 10 + s.GetChar(offset) - '0';
      if (decimal) n++;
    }
    nDigits++;
    offset++;
  }

  while (n-- > 0) res /= 10;

  if (nDigits == 0 || (decimal && n == 0)) return false;

  if (v != NULL) *v = res;

  return true;
}

bool StringTokenizer::ParseNumber(float* v)
{
  float n;
  bool negative = false;

  if (s.GetChar(offset) == '-') {
    negative = true;
    offset++;
  }

  if (!ParseUnsignedNumber(&n)) return false;

  if (v != NULL) *v = negative ? -n : n;

  return true;
}

bool StringTokenizer::ParseChar(Char* v)
{
  if (offset >= s.GetLength()) return false;

  if (v != NULL) *v = s.GetChar(offset++);

  return true;
}

bool StringTokenizer::ParseString(String* str)
{
  assert(str != NULL);

  unsigned start = offset;
  while (offset < s.GetLength() && !iswspace(s.GetChar(offset))) offset++;

  unsigned length = offset - start;
  if (length > 0) {
    Char* token = new Char[length];

    for (unsigned i = 0; i < length; i++)
      token[i] = s.GetChar(start + i);

    str->Set(token, length);

    delete [] token;
  } else
    str->Set(static_cast<char*>(NULL), 0);

  return true;
}

bool StringTokenizer::ParseRGB(RGBValue* v)
{
  unsigned i;

  if (offset >= s.GetLength()) return false;
  if (s.GetChar(offset) != '#') return false;

  for (i = 1;
       i <= 6 &&
	 offset + i < s.GetLength() &&
	 iswxdigit(s.GetChar(offset + i));
       i++) ;
  i--;

  if (i != 3 && i != 6) return false;

  unsigned red = 0;
  unsigned green = 0;
  unsigned blue = 0;
  
  if (i == 3) {
    i = offset + 1;
    red   = hexOfChar(s.GetChar(i++)) * 17;
    green = hexOfChar(s.GetChar(i++)) * 17;
    blue  = hexOfChar(s.GetChar(i++)) * 17;
  } else {
    i = offset + 1;
    red   = hexOfChar(s.GetChar(i++));
    red   = red * 16 + hexOfChar(s.GetChar(i++));
    green = hexOfChar(s.GetChar(i++));
    green = green * 16 + hexOfChar(s.GetChar(i++));
    blue  = hexOfChar(s.GetChar(i++));
    blue  = blue * 16 + hexOfChar(s.GetChar(i++));
  }

  if (v != NULL) *v = MKRGB(red, green, blue);

  return true;
}

void StringTokenizer::SetMark(unsigned n)
{
  assert(n < s.GetLength());
  offset = n;
}

