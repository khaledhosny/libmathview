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

#ifndef __TemplateStringScanners_hh__
#define __TemplateStringScanners_hh__

#include "RGBColor.hh"
#include "String.hh"

class Scan
{
public:
  static UCS4String::value_type
  toChar(const UCS4String::const_iterator& begin, const UCS4String::const_iterator&)
  {
    return *begin;
  }

  static String
  toString(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end)
  {
    return StringOfUCS4String(UCS4String(begin, end));
  }
};

// ScanAny: any character

class ScanAny : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    if (begin != end)
      {
	next = begin + 1;
	return true;
      }
    else
      return false;
  }

  static inline Char
  parse(const UCS4String::const_iterator& begin, const UCS4String::const_iterator&)
  {
    return *begin;
  }
};

// ScanSpace: an XML space

class ScanSpace : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    if (begin != end && isXmlSpace(*begin))
      {
	next = begin + 1;
	return true;
      }
    else
      return false;
  }
};

// ScanLiteral: character

template <UCS4String::value_type c>
class ScanLiteral : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    if (begin != end && *begin == c)
      {
	next = begin + 1;
	return true;
      }
    else
      return false;
  }
};

// ScanRange: range of characters

template <UCS4String::value_type first, UCS4String::value_type last>
class ScanRange : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    if (begin != end && *begin >= first && *begin <= last)
      {
	next = begin + 1;
	return true;
      }
    else
      return false;
  }
};

// ScanSeq: sequence of expressions

template <typename E1, typename E2>
class ScanSeq : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p;
    if (E1::scan(begin, end, p))
      return E2::scan(p, end, next);
    else
      return false;
  }
};

// ScanRep: repetition of expression

template <unsigned n0, typename E>
class ScanRep : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p = begin;
    unsigned n;
    for (n = 0; n < n0 && E::scan(p, end, next); n++)
      p = next;
    return n == n0;
  }
};

// ScanChoice: alternative expressions

template <typename E1, typename E2>
class ScanChoice : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    if (E1::scan(begin, end, next))
      {
	UCS4String::const_iterator next2;
	if (E2::scan(begin, end, next2))
	  next = std::max(next, next2);
	return true;
      }

    return E2::scan(begin, end, next);
  }
};

// ScanOption: optional expression

template <typename E>
class ScanZeroOrOne : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    if (E::scan(begin, end, next))
      return true;
    else
      {
	next = begin;
	return true;
      }
  }
};

// ScanOneOrMore: one or more

template <typename E>
class ScanOneOrMore : public Scan
{
public:
  static inline bool
  scan(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end, UCS4String::const_iterator& next)
  {
    UCS4String::const_iterator p = begin;
    while (E::scan(p, end, next))
      p = next;

    if (p != begin)
      {
	next = p;
	return true;
      }
    else
      return false;
  }
};

// ScanZeroOrMore

template <typename E>
class ScanZeroOrMore : public ScanZeroOrOne< ScanOneOrMore<E> >
{ };

// ScanSpaces: zero or more XML spaces

typedef ScanZeroOrMore<ScanSpace> ScanSpaces;

// Some more specialized scanners

typedef ScanLiteral<'-'> ScanMinus;
typedef ScanLiteral<'+'> ScanPlus;
typedef ScanLiteral<'%'> ScanPercentage;
typedef ScanLiteral<'{'> ScanLeftBrace;
typedef ScanLiteral<'}'> ScanRightBrace;
typedef ScanChoice<ScanLeftBrace,ScanRightBrace> ScanBrace;
typedef ScanChoice<ScanPlus,ScanMinus> ScanPlusOrMinus;
typedef ScanChoice<ScanPlusOrMinus,ScanChoice<ScanBrace,ScanPercentage> > ScanSpecialToken;
typedef ScanRange<'0','9'> ScanDecDigit;
typedef ScanZeroOrOne<ScanMinus> ScanOptionalMinus;
typedef ScanChoice< ScanRange<'a','z'>,ScanRange<'A','Z'> > ScanLetter;
typedef ScanChoice<ScanLetter,ScanMinus> ScanLetterOrMinus;
typedef ScanSeq<ScanLetter,ScanZeroOrMore<ScanLetterOrMinus> > ScanKeywordToken;
typedef ScanChoice<ScanDecDigit,ScanChoice<ScanRange<'a','f'>,ScanRange<'A','F'> > > ScanHexDigit;
typedef ScanRep<3,ScanHexDigit> ScanRGB4Color;
typedef ScanRep<4,ScanHexDigit> ScanRGB4AColor;
typedef ScanRep<6,ScanHexDigit> ScanRGB8Color;
typedef ScanRep<8,ScanHexDigit> ScanRGB8AColor;
typedef ScanSeq<ScanLiteral<'#'>,ScanChoice<ScanRGB8AColor,ScanChoice<ScanRGB8Color,ScanChoice<ScanRGB4AColor,ScanRGB4Color> > > > ScanRGBColorValue;

class ScanSign : public ScanZeroOrOne<ScanPlusOrMinus>
{
public:
  static int
  parse(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end)
  {
    if (begin != end)
      return (*begin == '+') ? 1 : -1;
    else
      return 0;
  }
};

class ScanUnsignedInteger : public ScanOneOrMore<ScanDecDigit>
{
public:
  static int
  parse(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end)
  {
    int v = 0;
    for (UCS4String::const_iterator p = begin; p != end; p++)
      v = v * 10 + *p - '0';
    return v;
  }
};

class ScanInteger : public ScanSeq<ScanOptionalMinus,ScanUnsignedInteger>
{
public:
  static int
  parse(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end)
  {
    if (*begin == '-')
      return -ScanUnsignedInteger::parse(begin + 1, end);
    else
      return ScanUnsignedInteger::parse(begin, end);
  }
};

class ScanToken : public ScanChoice<ScanKeywordToken,ScanSpecialToken>
{
public:
  static TokenId
  parse(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end)
  {
    return tokenIdOfString(Scan::toString(begin, end));
  }
};

class ScanRGBColor : public ScanRGBColorValue
{
private:
  static unsigned
  hexOfChar(UCS4String::value_type ch)
  {
    if (ch >= '0' && ch <= '9') return ch - '0';
    else if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    else return ch - 'a' + 10;
  }

public:
  static RGBColor
  parse(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end)
  {
    switch (end - begin)
      {
      case 4:
	return RGBColor(17 * hexOfChar(*(begin + 1)),
			17 * hexOfChar(*(begin + 2)),
			17 * hexOfChar(*(begin + 3)));
      case 5:
	return RGBColor(17 * hexOfChar(*(begin + 1)),
			17 * hexOfChar(*(begin + 2)),
			17 * hexOfChar(*(begin + 3)),
			17 * hexOfChar(*(begin + 4)));
      case 7:
	return RGBColor(16 * hexOfChar(*(begin + 1)) + hexOfChar(*(begin + 2)),
			16 * hexOfChar(*(begin + 3)) + hexOfChar(*(begin + 4)),
			16 * hexOfChar(*(begin + 5)) + hexOfChar(*(begin + 6)));
      case 9:
	return RGBColor(16 * hexOfChar(*(begin + 1)) + hexOfChar(*(begin + 2)),
			16 * hexOfChar(*(begin + 3)) + hexOfChar(*(begin + 4)),
			16 * hexOfChar(*(begin + 5)) + hexOfChar(*(begin + 6)),
			16 * hexOfChar(*(begin + 7)) + hexOfChar(*(begin + 8)));
      default:
	assert(false);
      }
  }
};

typedef ScanSeq<ScanLiteral<'.'>,ScanUnsignedInteger> ScanDecimalPart;

class ScanUnsignedNumber : public ScanChoice<ScanSeq<ScanUnsignedInteger,ScanDecimalPart>,
			   ScanChoice<ScanDecimalPart,ScanUnsignedInteger> >
{
public:
  static float
  parse(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end)
  {
    bool decimal = false; // true if decimal point found
    unsigned n = 0;       // number of digits after decimal point

    float v = float();
    UCS4String::const_iterator p = begin;
    while (p != end)
      {
	if (*p == '.')
	  decimal = true;
	else
	  {
	    v = v * 10 + *p - '0';
	    if (decimal) n++;
	  }
	p++;
      }
    
    while (n-- > 0) v /= 10;
    
    return v;
  }
};

class ScanNumber : public ScanSeq<ScanOptionalMinus,ScanUnsignedNumber>
{
public:
  static float
  parse(const UCS4String::const_iterator& begin, const UCS4String::const_iterator& end)
  {
    if (*begin == '-')
      return -ScanUnsignedNumber::parse(begin + 1, end);
    else
      return ScanUnsignedNumber::parse(begin, end);
  }
};

#endif // __TemplateStringScanners_hh__
