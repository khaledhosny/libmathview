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

#ifndef __TemplateStringParsers_hh__
#define __TemplateStringParsers_hh__

#include "TokenSet.hh"
#include "TemplateStringScanners.hh"
#include "Variant.hh"
#include "Length.hh"
#include "ValueConversion.hh"

typedef Set<T_FALSE,T_TRUE> TokenSet_Boolean;
typedef Set<T_EM,T_EX,T_PX,T_IN,T_CM,T_MM,T_PT,T_PC,T__PERCENTAGE> TokenSet_Unit;

class ParseBin
{
public:
  static inline bool sequence(void) { return false; }

  static bool
  parseInSequence(const UCS4String::const_iterator&,
		  const UCS4String::const_iterator&,
		  UCS4String::const_iterator&,
		  std::vector< SmartPtr<Value> >&)
  {
    assert(false);
  }
};

template <typename Scanner, typename NativeType>
class Parse : public ParseBin
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p;
    ScanSpaces::scan(begin, end, p);
    if (Scanner::scan(p, end, next))
      {
	NativeType v = Scanner::parse(p, next);
	return Variant<NativeType>::create(v);
      }
    return 0;
  }
};

template <>
class Parse<ScanToken, bool>
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p;
    ScanSpaces::scan(begin, end, p);
    if (ScanToken::scan(p, end, next))
      {
	TokenId id = ScanToken::parse(p, next);
	if (TokenSet_Boolean::has(id))
	  return Variant<bool>::create(id == T_TRUE);
      }
    return 0;
  }
};

class ParseLength : public ParseBin
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p;
    ScanSpaces::scan(begin, end, p);
    if (ScanNumber::scan(p, end, next))
      {
	float v = ScanNumber::parse(p, next);
	p = next;
	ScanSpaces::scan(p, end, next);
	p = next;
	if (ScanToken::scan(p, end, next))
	  {
	    TokenId id = ScanToken::parse(p, next);
	    if (TokenSet_Unit::has(id))
	      return Variant<Length>::create(Length(v, toUnitId(id)));
	  }
      }
    return 0;
  }  
};

class ParseString : public ParseBin
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    next = end;
    return Variant<String>::create(Scan::toString(begin, end));
  }
};

class ParseKeyword : public ParseBin
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p;
    ScanSpaces::scan(begin, end, p);
    if (ScanKeywordToken::scan(p, end, next))
      return Variant<String>::create(Scan::toString(p, next));
    else
      return 0;
  }
};

template <typename TokenSet>
class ParseTokenSet : public ParseBin
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p;
    ScanSpaces::scan(begin, end, p);
    if (ScanToken::scan(p, end, next))
      {
	TokenId id = ScanToken::parse(p, next);
	if (TokenSet::has(id)) return Variant<TokenId>::create(id);
      }
    return 0;
  }
};

template <TokenId id>
class ParseToken : public ParseTokenSet< Singleton<id> >
{ };

template <typename P1, typename P2>
class ParseChoice : public ParseBin
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    if (SmartPtr<Value> v = P1::parse(begin, end, next))
      return v;
    else
      return P2::parse(begin, end, next);
  }
};

template <typename P1, typename P2>
class ParseSeq
{
public:
  static inline bool sequence(void) { return true; }

  static bool
  parseInSequence(const UCS4String::const_iterator& begin,
		  const UCS4String::const_iterator& end,
		  UCS4String::const_iterator& next,
		  std::vector< SmartPtr<Value> >& content)
  {
    UCS4String::const_iterator p;
    if (P1::sequence())
      {
	if (!P1::parseInSequence(begin, end, p, content))
	  return false;
      }
    else if (SmartPtr<Value> v = P1::parse(begin, end, p))
      content.push_back(v);
    else
      return false;

    if (P2::sequence())
      {
	if (!P2::parseInSequence(p, end, next, content))
	  return false;
      }
    else if (SmartPtr<Value> v = P2::parse(p, end, next))
      content.push_back(v);
    else
      return false;

    return true;
  }

  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    std::vector< SmartPtr<Value> > content;
    if (parseInSequence(begin, end, next, content))
      return Variant<std::vector< SmartPtr<Value> > >::create(content);
    else
      return 0;
  }
};

template <typename P>
class ParseZeroOrOne : public ParseBin
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    if (SmartPtr<Value> v = P::parse(begin, end, next))
      return v;
    else
      {
	next = begin;
	return Variant<void>::create();
      }
  }  
};

template <typename P>
class ParseOneOrMore : public ParseBin
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p = begin;
    std::vector< SmartPtr<Value> > content;
    while (SmartPtr<Value> v = P::parse(p, end, next))
      {
	content.push_back(v);
	p = next;
      }

    if (!content.empty())
      {
	next = p;
	return Variant< std::vector< SmartPtr<Value> > >::create(content);
      }
    else
      return 0;
  }
};

template <typename P>
class ParseZeroOrMore : public ParseZeroOrOne< ParseOneOrMore<P> >
{ };

class ParseRGBColor
{
public:
  static SmartPtr<Value>
  parse(const UCS4String::const_iterator& begin,
	const UCS4String::const_iterator& end,
	UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p;
    ScanSpaces::scan(begin, end, p);
    if (ScanRGBColor::scan(p, end, next))
      return Variant<RGBColor>::create(ScanRGBColor::parse(p, next));
    else
      return 0;
  }
};

#endif // __TemplateStringParsers_hh__

