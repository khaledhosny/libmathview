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

#include "Iterator.hh"
#include "MultiString.hh"
#include "StringUnicode.hh"

MultiString::MultiString()
{
}

MultiString::~MultiString()
{
  for (Iterator<String*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);
    delete i();
  }
}

unsigned
MultiString::GetLength(void) const
{
  unsigned length = 0;
  for (Iterator<String*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);
    length += i()->GetLength();
  }

  return length;
}

Char
MultiString::GetBiggestChar(void) const
{
  Char big = 0;
  for (Iterator<String*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);
    Char ch = i()->GetBiggestChar();
    if (ch > big) big = ch;
  }

  return big;
}

void
MultiString::Append(const String* s) const
{
  assert(s != NULL);
  content.Append(s);
}

void
MultiString::Append(MultiString& ms)
{
  while (ms.content.GetSize() > 0) {
    String* s = ms.RemoveFirst();
    Append(s);
  }
}

String*
MultiString::Collapse() const
{
  unsigned length = GetLength();
  Char big        = GetBiggestChar();

  if (length == 0) return NULL;

  String* s = NULL;

  if (isPlain(big)) {
    Char8* ptr = new Char8[length];
    s = new StringC(ptr, length);
  } else if (isUnicode16(big)) {
    Char16* ptr = new Char16[length];
    s = new StringU16(ptr, length);
  } else {
    Char32* ptr = new Char32[length];
    s = new StringU32(ptr, length);
  }

  unsigned l = 0;
  for (Iterator<String*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);

    for (unsigned k = 0; k < i()->GetLength(); k++)
      s->SetChar(l++, i()->GetChar(k));
  }

  return s;
}

bool
MultiString::HasLeadingSpace() const
{
  if (content.GetSize() == 0) return false;

  String* s = content.GetFirst();
  assert(s != NULL);
  if (s->GetLength() > 1 || !iswspace(s->GetChar(0))) return false;

  return true;
}

bool
MultiString::HasTrailingSpace() const
{
  if (content.GetSize() == 0) return false;

  String* s = content.GetLast();
  assert(s != NULL);
  if (s->GetLength() > 1 || !iswspace(s->GetChar(0))) return false;

  return true;
}

void
MultiString::TrimLeadingSpaces()
{
  while (HasLeadingSpace()) {
    String* s = RemoveFirst();
    delete s;
  }
}

void
MultiString::TrimTrailingSpaces()
{
  while (HasTrailingSpace()) {
    String* s = RemoveFirst();
    delete s;
  }
}
