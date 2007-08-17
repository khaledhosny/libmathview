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

#include <cassert>

#include "MathVariant.hh"
#include "MathVariantMap.hh"
#include "ShaperManager.hh"
#include "MathGraphicDevice.hh"
#include "MathMLElement.hh"
#include "StandardSymbolsShaper.hh"
#include "ShapingContext.hh"

struct GlyphMap {
  Char8 index;
  Char16 ch;
};

GlyphMap
symbolMap[] = {
  { 0x20, 0x0020 },  // SPACE // space
  { 0x21, 0x0021 },  // EXCLAMATION MARK  // exclam
  { 0x22, 0x2200 },  // FOR ALL // universal
  { 0x23, 0x0023 },  // NUMBER SIGN // numbersign
  { 0x24, 0x2203 },  // THERE EXISTS  // existential
  { 0x25, 0x0025 },  // PERCENT SIGN  // percent
  { 0x26, 0x0026 },  // AMPERSAND // ampersand
  { 0x27, 0x220B },  // CONTAINS AS MEMBER  // suchthat
  { 0x28, 0x0028 },  // LEFT PARENTHESIS  // parenleft
  { 0x29, 0x0029 },  // RIGHT PARENTHESIS // parenright
  { 0x2A, 0x2217 },  // ASTERISK OPERATOR // asteriskmath
  { 0x2B, 0x002B },  // PLUS SIGN // plus
  { 0x2C, 0x002C },  // COMMA // comma
  { 0x2D, 0x2212 },  // MINUS SIGN  // minus
  { 0x2E, 0x002E },  // FULL STOP // period
  { 0x2F, 0x002F },  // SOLIDUS // slash

  { 0x30, 0x0030 },  // DIGIT ZERO  // zero
  { 0x31, 0x0031 },  // DIGIT ONE // one
  { 0x32, 0x0032 },  // DIGIT TWO // two
  { 0x33, 0x0033 },  // DIGIT THREE // three
  { 0x34, 0x0034 },  // DIGIT FOUR  // four
  { 0x35, 0x0035 },  // DIGIT FIVE  // five
  { 0x36, 0x0036 },  // DIGIT SIX // six
  { 0x37, 0x0037 },  // DIGIT SEVEN // seven
  { 0x38, 0x0038 },  // DIGIT EIGHT // eight
  { 0x39, 0x0039 },  // DIGIT NINE  // nine
  { 0x3A, 0x003A },  // COLON // colon
  { 0x3B, 0x003B },  // SEMICOLON // semicolon
  { 0x3C, 0x003C },  // LESS-THAN SIGN  // less
  { 0x3D, 0x003D },  // EQUALS SIGN // equal
  { 0x3E, 0x003E },  // GREATER-THAN SIGN // greater
  { 0x3F, 0x003F },  // QUESTION MARK // question

  { 0x40, 0x2245 },  // APPROXIMATELY EQUAL TO  // congruent
  { 0x41, 0x0391 },  // GREEK CAPITAL LETTER ALPHA  // Alpha
  { 0x42, 0x0392 },  // GREEK CAPITAL LETTER BETA // Beta
  { 0x43, 0x03A7 },  // GREEK CAPITAL LETTER CHI  // Chi
  { 0x44, 0x0394 },  // GREEK CAPITAL LETTER DELTA  // Delta
  { 0x44, 0x2206 },  // INCREMENT // Delta
  { 0x45, 0x0395 },  // GREEK CAPITAL LETTER EPSILON  // Epsilon
  { 0x46, 0x03A6 },  // GREEK CAPITAL LETTER PHI  // Phi
  { 0x47, 0x0393 },  // GREEK CAPITAL LETTER GAMMA  // Gamma
  { 0x48, 0x0397 },  // GREEK CAPITAL LETTER ETA  // Eta
  { 0x49, 0x0399 },  // GREEK CAPITAL LETTER IOTA // Iota
  { 0x4A, 0x03D1 },  // GREEK THETA SYMBOL  // theta1
  { 0x4B, 0x039A },  // GREEK CAPITAL LETTER KAPPA  // Kappa
  { 0x4C, 0x039B },  // GREEK CAPITAL LETTER LAMDA  // Lambda
  { 0x4D, 0x039C },  // GREEK CAPITAL LETTER MU // Mu
  { 0x4E, 0x039D },  // GREEK CAPITAL LETTER NU // Nu
  { 0x4F, 0x039F },  // GREEK CAPITAL LETTER OMICRON  // Omicron

  { 0x50, 0x03A0 },  // GREEK CAPITAL LETTER PI // Pi
  { 0x51, 0x0398 },  // GREEK CAPITAL LETTER THETA  // Theta
  { 0x52, 0x03A1 },  // GREEK CAPITAL LETTER RHO  // Rho
  { 0x53, 0x03A3 },  // GREEK CAPITAL LETTER SIGMA  // Sigma
  { 0x54, 0x03A4 },  // GREEK CAPITAL LETTER TAU  // Tau
  { 0x55, 0x03A5 },  // GREEK CAPITAL LETTER UPSILON  // Upsilon
  { 0x56, 0x03C2 },  // GREEK SMALL LETTER FINAL SIGMA  // sigma1
  { 0x57, 0x03A9 },  // GREEK CAPITAL LETTER OMEGA  // Omega
  { 0x57, 0x2126 },  // OHM SIGN  // Omega
  { 0x58, 0x039E },  // GREEK CAPITAL LETTER XI // Xi
  { 0x59, 0x03A8 },  // GREEK CAPITAL LETTER PSI  // Psi
  { 0x5A, 0x0396 },  // GREEK CAPITAL LETTER ZETA // Zeta
  { 0x5B, 0x005B },  // LEFT SQUARE BRACKET // bracketleft
  { 0x5C, 0x2234 },  // THEREFORE // therefore
  { 0x5D, 0x005D },  // RIGHT SQUARE BRACKET  // bracketright
  { 0x5E, 0x22A5 },  // UP TACK // perpendicular
  { 0x5F, 0x005F },  // LOW LINE  // underscore

  { 0x60, 0xF8E5 },  // RADICAL EXTENDER  // radicalex (CUS)
  { 0x61, 0x03B1 },  // GREEK SMALL LETTER ALPHA  // alpha
  { 0x62, 0x03B2 },  // GREEK SMALL LETTER BETA // beta
  { 0x63, 0x03C7 },  // GREEK SMALL LETTER CHI  // chi
  { 0x64, 0x03B4 },  // GREEK SMALL LETTER DELTA  // delta
  { 0x65, 0x03B5 },  // GREEK SMALL LETTER EPSILON  // epsilon
  { 0x66, 0x03C6 },  // GREEK SMALL LETTER PHI  // phi
  { 0x67, 0x03B3 },  // GREEK SMALL LETTER GAMMA  // gamma
  { 0x68, 0x03B7 },  // GREEK SMALL LETTER ETA  // eta
  { 0x69, 0x03B9 },  // GREEK SMALL LETTER IOTA // iota
  { 0x6A, 0x03D5 },  // GREEK PHI SYMBOL  // phi1
  { 0x6B, 0x03BA },  // GREEK SMALL LETTER KAPPA  // kappa
  { 0x6C, 0x03BB },  // GREEK SMALL LETTER LAMDA  // lambda
  { 0x6D, 0x00B5 },  // MICRO SIGN  // mu
  { 0x6D, 0x03BC },  // GREEK SMALL LETTER MU // mu
  { 0x6E, 0x03BD },  // GREEK SMALL LETTER NU // nu
  { 0x6F, 0x03BF },  // GREEK SMALL LETTER OMICRON  // omicron

  { 0x70, 0x03C0 },  // GREEK SMALL LETTER PI // pi
  { 0x71, 0x03B8 },  // GREEK SMALL LETTER THETA  // theta
  { 0x72, 0x03C1 },  // GREEK SMALL LETTER RHO  // rho
  { 0x73, 0x03C3 },  // GREEK SMALL LETTER SIGMA  // sigma
  { 0x74, 0x03C4 },  // GREEK SMALL LETTER TAU  // tau
  { 0x75, 0x03C5 },  // GREEK SMALL LETTER UPSILON  // upsilon
  { 0x76, 0x03D6 },  // GREEK PI SYMBOL // omega1
  { 0x77, 0x03C9 },  // GREEK SMALL LETTER OMEGA  // omega
  { 0x78, 0x03BE },  // GREEK SMALL LETTER XI // xi
  { 0x79, 0x03C8 },  // GREEK SMALL LETTER PSI  // psi
  { 0x7A, 0x03B6 },  // GREEK SMALL LETTER ZETA // zeta
  { 0x7B, 0x007B },  // LEFT CURLY BRACKET  // braceleft
  { 0x7C, 0x007C },  // VERTICAL LINE // bar
  { 0x7D, 0x007D },  // RIGHT CURLY BRACKET // braceright
  { 0x7E, 0x223C },  // TILDE OPERATOR  // similar

  { 0xA0, 0x20AC },  // EURO SIGN // Euro
  { 0xA1, 0x03D2 },  // GREEK UPSILON WITH HOOK SYMBOL  // Upsilon1
  { 0xA2, 0x2032 },  // PRIME // minute
  { 0xA3, 0x2264 },  // LESS-THAN OR EQUAL TO // lessequal
  { 0xA4, 0x2044 },  // FRACTION SLASH  // fraction
  { 0xA4, 0x2215 },  // DIVISION SLASH  // fraction
  { 0xA5, 0x221E },  // INFINITY  // infinity
  { 0xA6, 0x0192 },  // LATIN SMALL LETTER F WITH HOOK  // florin
  { 0xA7, 0x2663 },  // BLACK CLUB SUIT // club
  { 0xA8, 0x2666 },  // BLACK DIAMOND SUIT  // diamond
  { 0xA9, 0x2665 },  // BLACK HEART SUIT  // heart
  { 0xAA, 0x2660 },  // BLACK SPADE SUIT  // spade
  { 0xAB, 0x2194 },  // LEFT RIGHT ARROW  // arrowboth
  { 0xAC, 0x2190 },  // LEFTWARDS ARROW // arrowleft
  { 0xAD, 0x2191 },  // UPWARDS ARROW // arrowup
  { 0xAE, 0x2192 },  // RIGHTWARDS ARROW  // arrowright
  { 0xAF, 0x2193 },  // DOWNWARDS ARROW // arrowdown

  { 0xB0, 0x00B0 },  // DEGREE SIGN // degree
  { 0xB1, 0x00B1 },  // PLUS-MINUS SIGN // plusminus
  { 0xB2, 0x2033 },  // DOUBLE PRIME  // second
  { 0xB3, 0x2265 },  // GREATER-THAN OR EQUAL TO  // greaterequal
  { 0xB4, 0x00D7 },  // MULTIPLICATION SIGN // multiply
  { 0xB5, 0x221D },  // PROPORTIONAL TO // proportional
  { 0xB6, 0x2202 },  // PARTIAL DIFFERENTIAL  // partialdiff
  { 0xB7, 0x2022 },  // BULLET  // bullet
  { 0xB8, 0x00F7 },  // DIVISION SIGN // divide
  { 0xB9, 0x2260 },  // NOT EQUAL TO  // notequal
  { 0xBA, 0x2261 },  // IDENTICAL TO  // equivalence
  { 0xBB, 0x2248 },  // ALMOST EQUAL TO // approxequal
  { 0xBC, 0x2026 },  // HORIZONTAL ELLIPSIS // ellipsis
  { 0xBD, 0xF8E6 },  // VERTICAL ARROW EXTENDER // arrowvertex (CUS)
  { 0xBE, 0xF8E7 },  // HORIZONTAL ARROW EXTENDER // arrowhorizex (CUS)
  { 0xBF, 0x21B5 },  // DOWNWARDS ARROW WITH CORNER LEFTWARDS // carriagereturn

  { 0xC0, 0x2135 },  // ALEF SYMBOL // aleph
  { 0xC1, 0x2111 },  // BLACK-LETTER CAPITAL I  // Ifraktur
  { 0xC2, 0x211C },  // BLACK-LETTER CAPITAL R  // Rfraktur
  { 0xC3, 0x2118 },  // SCRIPT CAPITAL P  // weierstrass
  { 0xC4, 0x2297 },  // CIRCLED TIMES // circlemultiply
  { 0xC5, 0x2295 },  // CIRCLED PLUS  // circleplus
  { 0xC6, 0x2205 },  // EMPTY SET // emptyset
  { 0xC7, 0x2229 },  // INTERSECTION  // intersection
  { 0xC8, 0x222A },  // UNION // union
  { 0xC9, 0x2283 },  // SUPERSET OF // propersuperset
  { 0xCA, 0x2287 },  // SUPERSET OF OR EQUAL TO // reflexsuperset
  { 0xCB, 0x2284 },  // NOT A SUBSET OF // notsubset
  { 0xCC, 0x2282 },  // SUBSET OF // propersubset
  { 0xCD, 0x2286 },  // SUBSET OF OR EQUAL TO // reflexsubset
  { 0xCE, 0x2208 },  // ELEMENT OF  // element
  { 0xCF, 0x2209 },  // NOT AN ELEMENT OF // notelement

  { 0xD0, 0x2220 },  // ANGLE // angle
  { 0xD1, 0x2207 },  // NABLA // gradient
  { 0xD2, 0x00AE },  // REGISTERED SIGN SERIF // registerserif (CUS) CHANGED
  { 0xD3, 0x00A9 },  // COPYRIGHT SIGN SERIF  // copyrightserif (CUS) CHANGED
  { 0xD4, 0x2122 },  // TRADE MARK SIGN SERIF // trademarkserif (CUS) CHANGED
  { 0xD5, 0x220F },  // N-ARY PRODUCT // product
  { 0xD6, 0x221A },  // SQUARE ROOT // radical
  { 0xD7, 0x22C5 },  // DOT OPERATOR  // dotmath
  { 0xD8, 0x00AC },  // NOT SIGN  // logicalnot
  { 0xD9, 0x2227 },  // LOGICAL AND // logicaland
  { 0xDA, 0x2228 },  // LOGICAL OR  // logicalor
  { 0xDB, 0x21D4 },  // LEFT RIGHT DOUBLE ARROW // arrowdblboth
  { 0xDC, 0x21D0 },  // LEFTWARDS DOUBLE ARROW  // arrowdblleft
  { 0xDD, 0x21D1 },  // UPWARDS DOUBLE ARROW  // arrowdblup
  { 0xDE, 0x21D2 },  // RIGHTWARDS DOUBLE ARROW // arrowdblright
  { 0xDF, 0x21D3 },  // DOWNWARDS DOUBLE ARROW  // arrowdbldown

  { 0xE0, 0x25CA },  // LOZENGE // lozenge
  { 0xE1, 0x2329 },  // LEFT-POINTING ANGLE BRACKET // angleleft
  { 0xE2, 0xF8E8 },  // REGISTERED SIGN SANS SERIF  // registersans (CUS)
  { 0xE3, 0xF8E9 },  // COPYRIGHT SIGN SANS SERIF // copyrightsans (CUS)
  { 0xE4, 0xF8EA },  // TRADE MARK SIGN SANS SERIF  // trademarksans (CUS)
  { 0xE5, 0x2211 },  // N-ARY SUMMATION // summation

  { 0xF1, 0x232A },  // RIGHT-POINTING ANGLE BRACKET  // angleright
  { 0xF2, 0x222B },  // INTEGRAL  // integral
  { 0xF3, 0x2320 },  // TOP HALF INTEGRAL // integraltp
  { 0xF4, 0xF8F5 },  // INTEGRAL EXTENDER // integralex (CUS)
  { 0xF5, 0x2321 },  // BOTTOM HALF INTEGRAL  // integralbt
  { 0xF6, 0xF8F6 },  // RIGHT PAREN TOP // parenrighttp (CUS)
  { 0xF7, 0xF8F7 },  // RIGHT PAREN EXTENDER  // parenrightex (CUS)
  { 0xF8, 0xF8F8 },  // RIGHT PAREN BOTTOM  // parenrightbt (CUS)
  { 0xF9, 0xF8F9 },  // RIGHT SQUARE BRACKET TOP  // bracketrighttp (CUS)
  { 0xFA, 0xF8FA },  // RIGHT SQUARE BRACKET EXTENDER // bracketrightex (CUS)
  { 0xFB, 0xF8FB },  // RIGHT SQUARE BRACKET BOTTOM // bracketrightbt (CUS)
  { 0xFC, 0xF8FC },  // RIGHT CURLY BRACKET TOP // bracerighttp (CUS)
  { 0xFD, 0xF8FD },  // RIGHT CURLY BRACKET MID // bracerightmid (CUS)
  { 0xFE, 0xF8FE },  // RIGHT CURLY BRACKET BOTTOM // bracerightbt (CUS)

  { 0x00, 0x0000 }
};

static StandardSymbolsShaper::HStretchyChar hMap[] =
  {
    //        N     L     G     R
    { 0x2190, 0xAC, 0xAC, 0xBE, 0    },
    { 0x2192, 0xAE, 0,    0xBE, 0xAE },
    { 0x2194, 0xAB, 0xAC, 0xBE, 0xAE },
    { 0,      0,    0,    0,    0    }
  };

static StandardSymbolsShaper::VStretchyChar vMap[] =
  {
    //        N     T     G     M     B
    { 0x0028, 0x28, 0xE6, 0xE7, 0,    0xE8 },
    { 0x0029, 0x29, 0xF6, 0xF7, 0,    0xF8 },
    { 0x005B, 0x5B, 0xE9, 0xEA, 0,    0xEB },
    { 0x005D, 0x5D, 0xF9, 0xFA, 0,    0xFB },
    { 0x007B, 0x7B, 0xEC, 0xEF, 0xED, 0xEE },
    { 0x007C, 0x7C, 0,    0x7C, 0,    0    },
    { 0x007D, 0x7D, 0xFC, 0xEF, 0xFD, 0xFE },
    { 0x2191, 0xAD, 0xAD, 0xBD, 0,    0    },
    { 0x2193, 0xAF, 0,    0xBD, 0,    0xAF },
    { 0x2195, 0,    0xAD, 0xBD, 0,    0xAF },
    { 0x222B, 0xF2, 0xF3, 0xF4, 0,    0xF5 },
    { 0,      0,    0,    0,    0,    0    }
  };

#define NORMAL_FONT_INDEX     0
#define H_STRETCHY_FONT_INDEX 1
#define V_STRETCHY_FONT_INDEX 2

void
StandardSymbolsShaper::registerShaper(const SmartPtr<ShaperManager>& sm, unsigned shaperId)
{
  assert(sm);

  for (unsigned i = 0; symbolMap[i].ch; i++)
    registerChar(sm, shaperId, symbolMap[i].ch, symbolMap[i].index);

  for (unsigned i = 0; vMap[i].ch != 0; i++)
    registerStretchyCharV(sm, shaperId, vMap[i], i);

  for (unsigned i = 0; hMap[i].ch != 0; i++)
    registerStretchyCharH(sm, shaperId, hMap[i], i);
}

void
StandardSymbolsShaper::registerChar(const SmartPtr<ShaperManager>& sm, unsigned shaperId, Char16 ch, Char8 glyphId)
{
  assert(ch);
  sm->registerChar(ch, GlyphSpec(shaperId, NORMAL_FONT_INDEX, glyphId));
}

void
StandardSymbolsShaper::registerStretchyCharV(const SmartPtr<ShaperManager>& sm, unsigned shaperId,
					     const VStretchyChar& vMap, Char8 glyphId)
{ sm->registerStretchyChar(vMap.ch, GlyphSpec(shaperId, V_STRETCHY_FONT_INDEX, glyphId)); }

void
StandardSymbolsShaper::registerStretchyCharH(const SmartPtr<ShaperManager>& sm, unsigned shaperId,
					     const HStretchyChar& hMap, Char8 glyphId)
{ sm->registerStretchyChar(hMap.ch, GlyphSpec(shaperId, H_STRETCHY_FONT_INDEX, glyphId)); }

void
StandardSymbolsShaper::unregisterShaper(const SmartPtr<ShaperManager>&, unsigned)
{
  // nothing to do???
}

void
StandardSymbolsShaper::shape(ShapingContext& context) const
{
  for (unsigned n = context.chunkSize(); n > 0; n--)
    {
      AreaRef res;
      switch (context.getSpec().getFontId())
	{
	case H_STRETCHY_FONT_INDEX:
	  res = shapeStretchyCharH(context);
	  break;
	case V_STRETCHY_FONT_INDEX:
	  res = shapeStretchyCharV(context);
	  break;
	default:
	  break;
	}

      // If we get here then either the character was not required
      // to stretch, or one of the stretchying methods has failed,
      // hence we shape it with no stretchying
      if (!res) res = shapeChar(context);
      if (!res) break;

      context.pushArea(1, res);
    }
}

AreaRef
StandardSymbolsShaper::shapeChar(const ShapingContext& context) const
{
  return getGlyphArea(context.getFactory(), context.getSpec().getGlyphId(), context.getSize());
}

AreaRef
StandardSymbolsShaper::shapeStretchyCharH(const ShapingContext& context) const
{
  SmartPtr<AreaFactory> factory = context.getFactory();
  const scaled size = context.getSize();
  const HStretchyChar* charSpec = &hMap[context.getSpec().getGlyphId()];

  AreaRef normal = (charSpec->normal != 0) ? getGlyphArea(factory, charSpec->normal, size) : 0;
  AreaRef left = (charSpec->left != 0) ? getGlyphArea(factory, charSpec->left, size) : 0;
  AreaRef glue = (charSpec->glue != 0) ? getGlyphArea(factory, charSpec->glue, size) : 0;
  AreaRef right = (charSpec->right != 0) ? getGlyphArea(factory, charSpec->right, size) : 0;

  return composeStretchyCharH(factory, normal, left, glue, right, context.getHSpan());
}

AreaRef
StandardSymbolsShaper::shapeStretchyCharV(const ShapingContext& context) const
{ 
  SmartPtr<AreaFactory> factory = context.getFactory();
  const scaled size = context.getSize();
  const scaled span = context.getVSpan() - (1 * size) / 10;
  const VStretchyChar* charSpec = &vMap[context.getSpec().getGlyphId()];

  AreaRef normal = (charSpec->normal != 0) ? getGlyphArea(factory, charSpec->normal, size) : 0;
  AreaRef top = (charSpec->top != 0) ? getGlyphArea(factory, charSpec->top, size) : 0;
  AreaRef glue = (charSpec->glue != 0) ? getGlyphArea(factory, charSpec->glue, size) : 0;
  AreaRef middle = (charSpec->middle != 0) ? getGlyphArea(factory, charSpec->middle, size) : 0;
  AreaRef bottom = (charSpec->bottom != 0) ? getGlyphArea(factory, charSpec->bottom, size) : 0;

  return composeStretchyCharV(factory, normal, top, glue, middle, bottom, span);
}

