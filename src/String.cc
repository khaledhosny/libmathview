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
#include <stdio.h>
#include <string.h>
#ifdef HAVE_WCTYPE_H
#include <wctype.h>
#endif
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#include "String.hh"
#include "Globals.hh"

#ifdef DEBUG
int String::counter = 0;
#endif

String::String()
{
#ifdef DEBUG
  counter++;
#endif
}

String::~String()
{
#ifdef DEBUG
  counter--;
#endif
}

void
String::Dump() const
{
  for (unsigned i = 0; i < GetLength(); i++) {
    Char ch = GetChar(i);
    if (isPlain(ch) && (ch == ' ' || !iswspace(ch))) putchar(ch);
    else printf("&#x%04x;", ch);
  }

  putchar('\n');
}

char*
String::ToC() const
{
  char* res = new char[GetLength() + 1];
  assert(res != NULL);

  ToCAux(res);

  return res;
}

const char*
String::ToStaticC() const
{
  static char temp_string[256];

  assert(GetLength() < 256);

  ToCAux(temp_string);

  return temp_string;
}

void
String::ToCAux(char *dest) const
{
  assert(dest != NULL);

  for (unsigned i = 0; i < GetLength(); i++) {
    Char ch = GetChar(i);
    if (!isPlain(ch))
      Globals::logger(LOG_WARNING, "c-izing a Unicode string with extended chars!");
    dest[i] = ch & 0xff;
  }

  dest[GetLength()] = '\0';
}

bool
String::Equal(const String& s) const
{
  if (GetLength() != s.GetLength()) return false;

  for (unsigned i = 0; i < GetLength(); i++)
    if (GetChar(i) != s[i]) return false;

  return true;
}

bool
String::Equal(const char* s) const
{
  assert(s != NULL);

  unsigned len = strlen(s);
  if (GetLength() != len) return false;

  for (unsigned i = 0; i < len; i++)
    if (GetChar(i) != static_cast<Char>(s[i])) return false;

  return true;
}

void
String::ToLowerCase()
{
  for (unsigned i = 0; i < GetLength(); i++) {
    Char ch = GetChar(i);
    SetChar(i, (ch >= 'A' && ch <= 'Z') ? ch + 'a' - 'A' : ch);
    // SetChar(i, towlower(ch));
  }
}

Char
String::GetBiggestChar() const
{
  Char big = 0;

  for (unsigned i = 0; i < GetLength(); i++) {
    Char ch = GetChar(i);
    if (ch > big) big = ch;
  }

  return big;
}

Char
String::GetBiggestChar(unsigned offset, unsigned length) const
{
  assert(offset + length <= GetLength());

  Char big = 0;
  for (unsigned i = 0; i < length; i++) {
    Char ch = GetChar(offset + i);
    if (ch > big) big = ch;
  }

  return big;
}

size_t
String::Hash::operator()(const String* s) const
{
  assert(s != 0);

  size_t h = 0;
  for (unsigned i = 0; i < s->GetLength(); i++)
    {
      h = (h << 4) + s->GetChar(i);
      if (size_t g = h & 0xf0000000)
	{
	  h = h ^ (g >> 24);
	  h = h ^ g;
	}
    }

  return h;
}

bool
String::Eq::operator()(const String* s1, const String* s2) const
{
  assert(s1 != 0);
  assert(s2 != 0);
  return s1->Equal(*s2);
}

