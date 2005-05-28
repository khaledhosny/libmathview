// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include "Scanner.hh"
#include "Variant.hh"
#include "token.hh"
#include "TemplateStringScanners.hh"

typedef ScanChoice<ScanLetterOrMinus,ScanDecDigit> ScanLetterOrMinusOrDigit;
typedef ScanSeq<ScanLetter,ScanZeroOrMore<ScanLetterOrMinusOrDigit> > ScanIdToken;

Scanner::Scanner(const UCS4String& s, bool mode)
  : raw(mode), token(UNDEFINED), p(s.begin()), end(s.end())
{ }

Scanner::~Scanner()
{ }

Scanner::TokenId
Scanner::scanToken(void)
{
  while (true)
    {
      UCS4String acc;
      UCS4String::const_iterator begin = p;
      while (p != end && raw)
	{
	  if (*p == '$')
	    if (p + 1 < end && *(p + 1) == '$')
	      {
		acc.append(begin, p);
		acc.append(1, '$');
		begin = p = p + 2;
	      }
	    else
	      {
		acc.append(begin, p);
		begin = p = p + 1;
		raw = false;
	      }
	  else
	    p++;
	}

      acc.append(begin, p);
      if (!acc.empty())
	{
	  value = Variant<UCS4String>::create(acc);
	  return RAW;
	}

      while (p != end && isXmlSpace(*p))
	p++;

      if (p == end)
	return EOS;

      switch (*p)
	{
	case '(':
	  p++;
	  return LPAREN;
	case ')':
	  p++;
	  return RPAREN;
	case '#':
	  p++;
	  return SHARP;
	case '{':
	  p++;
	  return LBRACE;
	case '}': 
	  p++;
	  raw = true;
	  return RBRACE;
	default:
	  {
	    UCS4String::const_iterator next;
	    if (ScanNumber::scan(p, end, next))
	      {
		value = Variant<float>::create(ScanNumber::parse(p, next));
		p = next;
		return NUMBER;
	      }
	    else if (ScanIdToken::scan(p, end, next))
	      {
		value = Variant<UCS4String>::create(UCS4String(p, next));
		p = next;
		return ID;
	      }
	    else
	      return ERROR;
	  }
	}
    }
}

Scanner::TokenId
Scanner::getToken()
{
  if (token == UNDEFINED)
    token = scanToken();
  assert(token != UNDEFINED);
  return token;
}

void
Scanner::advance()
{
  token = UNDEFINED;
}

UCS4String
Scanner::getString() const
{
  const SmartPtr<Variant<UCS4String> > v = smart_cast<Variant<UCS4String> >(value);
  assert(v);
  return v->getValue();
}

float
Scanner::getNumber() const
{
  const SmartPtr<Variant<float> > v = smart_cast<Variant<float> >(value);
  assert(v);
  return v->getValue();
}
