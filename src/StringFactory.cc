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

#include <assert.h>

#include "String.hh"
#include "Iterator.hh"
#include "StringFactory.hh"
#include "StringUnicode.hh"

StringFactory::StringFactory()
{
}

StringFactory::~StringFactory()
{
  while (content.GetSize() > 0) {
    const String* s = content.RemoveFirst();
    delete s;
  }
}

void
StringFactory::Append(const String* s)
{
  assert(s != NULL);
  if (s->GetLength() == 0) return;
  content.Append(s->Clone());
}

String*
StringFactory::Pack() const
{
  Char big = 0;
  unsigned len = 0;

  for (Iterator<const String*> s1(content); s1.More(); s1.Next()) {
    assert(s1() != NULL);
    Char sBig = s1()->GetBiggestChar();
    if (sBig > big) big = sBig;
    len += s1()->GetLength();
  }

  String* res = NULL;
  if (isPlain(big)) res = new StringU<Char8>(len);
  else if (isUnicode16(big)) res = new StringU<Char16>(len);
  else res = new StringU<Char32>(len);

  unsigned offset = 0;

  for (Iterator<const String*> s2(content); s2.More(); s2.Next()) {
    assert(s2() != NULL);
    for (unsigned i = 0; i < s2()->GetLength(); i++) {
      res->SetChar(offset, s2()->GetChar(i));
      offset++;
    }
  }
   
  return res;
}
