// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include "Scanner.hh"
#include "Variant.hh"
#include "token.hh"
#include "TemplateStringScanners.hh"
#include <iostream>

typedef ScanChoice<ScanLetterOrMinus,ScanDecDigit> ScanLetterOrMinusOrDigit;
typedef ScanSeq<ScanLetter,ScanZeroOrMore<ScanLetterOrMinusOrDigit> > ScanIdToken;

Scanner::Scanner(const UCS4String& s, bool mode)
  : raw(mode), token(UNDEFINED), buffer(s), p(buffer.begin()), end(buffer.end())
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
      while (raw && p != end && *p != '{')
	p++;

      raw = false;
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
	case '$' :
	  p++;
	  return DOLLAR;
	case ',':
	  p++;
	  return COMMA;
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
