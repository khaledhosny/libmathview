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

#include <cassert>
#include <cctype>

#include "MathMLAttribute.hh"
#include "StringTokenizer.hh"

static unsigned
hexOfChar(Char ch)
{
  assert(isxdigit(ch));
  ch = toupper(ch);
  if (ch <= '9') return ch - '0';
  else return ch - 'A' + 10;
}

StringTokenizer::StringTokenizer(const String& s0) : s(toUCS4String(s0))
{
  Reset();
}

StringTokenizer::~StringTokenizer()
{
}

void
StringTokenizer::Reset()
{
  offset = 0;
}

void
StringTokenizer::SkipSpaces()
{
  while (offset < s.length() && isXmlSpace(s[offset])) offset++;
}

void
StringTokenizer::SkipToken()
{
  while (offset < s.length() && !isXmlSpace(s[offset])) offset++;
}

bool
StringTokenizer::MoreTokens()
{
  return offset < s.length();
}

bool
StringTokenizer::Parse(const char *str)
{
  assert(str);

  unsigned i = 0;
  while (offset + i < s.length() &&
	 str[i] != '\0' &&
	 s[offset + i] == static_cast<Char>(str[i])) i++;

  if (str[i] == '\0')
    {
      offset += i;
      return true;
    }

  return false;
}

bool
StringTokenizer::Parse(KeywordId id)
{
  return Parse(NameOfKeywordId(id));
}

String
StringTokenizer::ParseToken()
{
  unsigned offset0 = offset;
  while (offset < s.length() && !isXmlSpace(s[offset])) offset++;
  return fromUCS4String(s.substr(offset0, offset - offset0));
}

bool
StringTokenizer::ParseKeyword(KeywordId& v)
{
  v = KeywordIdOfName(ParseToken().c_str());
  return v != KW_NOTVALID;
}

bool
StringTokenizer::ParseUnsignedInteger(int& v)
{
  unsigned nDigits = 0;
  v = 0;
  while (offset < s.length() && isdigit(s[offset]))
    {
      v = v * 10 + s[offset] - '0';
      nDigits++;
      offset++;
    }

  return nDigits != 0;
}

bool
StringTokenizer::ParseInteger(int& v)
{
  bool negative = false;

  if (offset < s.length() && s[offset] == '-')
    {
      negative = true;
      offset++;
    }

  if (!ParseUnsignedInteger(v)) return false;
  if (negative) v = -v;

  return true;
}

bool
StringTokenizer::ParseUnsignedNumber(float& v)
{
  bool decimal = false; // true if decimal point found
  unsigned n = 0;       // number of digits after decimal point
  unsigned nDigits = 0;

  v = 0.0f;
  while (offset < s.length() &&
	 (isdigit(s[offset]) || (!decimal && s[offset] == '.')))
    {
      if (s[offset] == '.')
	{
	  decimal = true;
	} 
      else
	{
	  v = v * 10 + s[offset] - '0';
	  if (decimal) n++;
	}
      nDigits++;
      offset++;
    }

  while (n-- > 0) v /= 10;

  return (nDigits != 0 && (!decimal || n != 0));
}

bool
StringTokenizer::ParseNumber(float& v)
{
  bool negative = false;

  if (s[offset] == '-')
    {
      negative = true;
      offset++;
    }

  if (!ParseUnsignedNumber(v)) return false;
  if (negative) v = -v;

  return true;
}

bool
StringTokenizer::ParseChar(Char& v)
{
  if (offset >= s.length()) return false;

  v = s[offset++];

  return true;
}

bool
StringTokenizer::ParseString(String& str)
{
  unsigned start = offset;
  while (offset < s.length() && !isXmlSpace(s[offset])) offset++;

  str = fromUCS4String(s.substr(start, offset - start));

  return true;
}

bool
StringTokenizer::ParseRGB(RGBValue& v)
{
  unsigned i;

  if (offset >= s.length()) return false;
  if (s[offset] != '#') return false;

  for (i = 1; i <= 6 && offset + i < s.length() && isxdigit(s[offset + i]); i++) ;
  i--;

  if (i != 3 && i != 6) return false;

  unsigned red = 0;
  unsigned green = 0;
  unsigned blue = 0;
  
  if (i == 3)
    {
      i = offset + 1;
      red   = hexOfChar(s[i++]) * 17;
      green = hexOfChar(s[i++]) * 17;
      blue  = hexOfChar(s[i++]) * 17;
    } 
  else
    {
      i = offset + 1;
      red   = hexOfChar(s[i++]);
      red   = red * 16 + hexOfChar(s[i++]);
      green = hexOfChar(s[i++]);
      green = green * 16 + hexOfChar(s[i++]);
      blue  = hexOfChar(s[i++]);
      blue  = blue * 16 + hexOfChar(s[i++]);
    }

  v = MKRGB(red, green, blue);

  return true;
}

void
StringTokenizer::SetMark(unsigned n)
{
  assert(n < s.length());
  offset = n;
}

