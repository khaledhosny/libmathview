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

#include "String.hh"
#include "stringAux.hh"
#include "StringFactory.hh"
#include "StringUnicode.hh"

StringFactory::StringFactory()
{
}

StringFactory::~StringFactory()
{
  for (std::vector<const String*>::iterator p = content.begin();
       p != content.end();
       p++)
    {
      assert(*p);
      delete *p;
    }
}

void
StringFactory::Append(Char ch)
{
  content.push_back(allocString(&ch, 1));
}

void
StringFactory::Append(const String* s)
{
  assert(s != NULL);
  if (s->GetLength() == 0) return;
  content.push_back(s->Clone());
}

unsigned
StringFactory::GetLength() const
{
  unsigned len = 0;

  for (std::vector<const String*>::const_iterator s = content.begin();
       s != content.end();
       s++)
    {
      assert(*s);
      len += (*s)->GetLength();
    }  
  
  return len;
}

String*
StringFactory::Pack() const
{
  Char big = 0;
  unsigned len = 0;

  for (std::vector<const String*>::const_iterator s1 = content.begin();
       s1 != content.end();
       s1++)
    {
      assert(*s1);
      Char sBig = (*s1)->GetBiggestChar();
      if (sBig > big) big = sBig;
      len += (*s1)->GetLength();
    }

  String* res = 0;
  if (isPlain(big)) res = new StringU<Char8>(len);
  else if (isUnicode16(big)) res = new StringU<Char16>(len);
  else res = new StringU<Char32>(len);

  unsigned offset = 0;

  for (std::vector<const String*>::const_iterator s2 = content.begin();
       s2 != content.end();
       s2++)
    {
      assert(*s2);
      for (unsigned i = 0; i < (*s2)->GetLength(); i++)
	{
	  res->SetChar(offset, (*s2)->GetChar(i));
	  offset++;
	}
    }
   
  return res;
}
