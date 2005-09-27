// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <cassert>

#include "Area.hh"
#include "AreaFactory.hh"
#include "MathVariant.hh"
#include "MathVariantMap.hh"
#include "ComputerModernShaper.hh"
#include "ShapingContext.hh"
#include "ShaperManager.hh"
#include "Configuration.hh"

enum
  {
    NIL = ComputerModernFamily::FE_NIL,
    CMR = ComputerModernFamily::FE_CMR,
    CMM = ComputerModernFamily::FE_CMMI,
    CMS = ComputerModernFamily::FE_CMSY,
    CME = ComputerModernFamily::FE_CMEX
  };

static ComputerModernShaper::PlainChar cmmMap[] =
  {
    { 0x002C, 0x3B },  // COMMA
    { 0x002E, 0x3A },  // FULL STOP
    { 0x002F, 0x3D },  // SOLIDUS
    // these are funny (short) digits and we don't want them
//     { 0x0030, 0x30 },  // DIGIT ZERO
//     { 0x0031, 0x31 },  // DIGIT ONE
//     { 0x0032, 0x32 },  // DIGIT TWO
//     { 0x0033, 0x33 },  // DIGIT THREE
//     { 0x0034, 0x34 },  // DIGIT FOUR
//     { 0x0035, 0x35 },  // DIGIT FIVE
//     { 0x0036, 0x36 },  // DIGIT SIX
//     { 0x0037, 0x37 },  // DIGIT SEVEN
//     { 0x0038, 0x38 },  // DIGIT EIGHT
//     { 0x0039, 0x39 },  // DIGIT NINE
    { 0x003C, 0x3C },  // LESS-THAN SIGN
    { 0x003E, 0x3E },  // GREATER-THAN SIGN
    { 0x0041, 0x41 },  // LATIN CAPITAL LETTER A
    { 0x0042, 0x42 },  // LATIN CAPITAL LETTER B
    { 0x0043, 0x43 },  // LATIN CAPITAL LETTER C
    { 0x0044, 0x44 },  // LATIN CAPITAL LETTER D
    { 0x0045, 0x45 },  // LATIN CAPITAL LETTER E
    { 0x0046, 0x46 },  // LATIN CAPITAL LETTER F
    { 0x0047, 0x47 },  // LATIN CAPITAL LETTER G
    { 0x0048, 0x48 },  // LATIN CAPITAL LETTER H
    { 0x0049, 0x49 },  // LATIN CAPITAL LETTER I
    { 0x004A, 0x4A },  // LATIN CAPITAL LETTER J
    { 0x004B, 0x4B },  // LATIN CAPITAL LETTER K
    { 0x004C, 0x4C },  // LATIN CAPITAL LETTER L
    { 0x004D, 0x4D },  // LATIN CAPITAL LETTER M
    { 0x004E, 0x4E },  // LATIN CAPITAL LETTER N
    { 0x004F, 0x4F },  // LATIN CAPITAL LETTER O
    { 0x0050, 0x50 },  // LATIN CAPITAL LETTER P
    { 0x0051, 0x51 },  // LATIN CAPITAL LETTER Q
    { 0x0052, 0x52 },  // LATIN CAPITAL LETTER R
    { 0x0053, 0x53 },  // LATIN CAPITAL LETTER S
    { 0x0054, 0x54 },  // LATIN CAPITAL LETTER T
    { 0x0055, 0x55 },  // LATIN CAPITAL LETTER U
    { 0x0056, 0x56 },  // LATIN CAPITAL LETTER V
    { 0x0057, 0x57 },  // LATIN CAPITAL LETTER W
    { 0x0058, 0x58 },  // LATIN CAPITAL LETTER X
    { 0x0059, 0x59 },  // LATIN CAPITAL LETTER Y
    { 0x005A, 0x5A },  // LATIN CAPITAL LETTER Z
    { 0x0061, 0x61 },  // LATIN SMALL LETTER A
    { 0x0062, 0x62 },  // LATIN SMALL LETTER B
    { 0x0063, 0x63 },  // LATIN SMALL LETTER C
    { 0x0064, 0x64 },  // LATIN SMALL LETTER D
    { 0x0065, 0x65 },  // LATIN SMALL LETTER E
    { 0x0066, 0x66 },  // LATIN SMALL LETTER F
    { 0x0067, 0x67 },  // LATIN SMALL LETTER G
    { 0x0068, 0x68 },  // LATIN SMALL LETTER H
    { 0x0069, 0x69 },  // LATIN SMALL LETTER I
    { 0x006A, 0x6A },  // LATIN SMALL LETTER J
    { 0x006B, 0x6B },  // LATIN SMALL LETTER K
    { 0x006C, 0x6C },  // LATIN SMALL LETTER L
    { 0x006D, 0x6D },  // LATIN SMALL LETTER M
    { 0x006E, 0x6E },  // LATIN SMALL LETTER N
    { 0x006F, 0x6F },  // LATIN SMALL LETTER O
    { 0x0070, 0x70 },  // LATIN SMALL LETTER P
    { 0x0071, 0x71 },  // LATIN SMALL LETTER Q
    { 0x0072, 0x72 },  // LATIN SMALL LETTER R
    { 0x0073, 0x73 },  // LATIN SMALL LETTER S
    { 0x0074, 0x74 },  // LATIN SMALL LETTER T
    { 0x0075, 0x75 },  // LATIN SMALL LETTER U
    { 0x0076, 0x76 },  // LATIN SMALL LETTER V
    { 0x0077, 0x77 },  // LATIN SMALL LETTER W
    { 0x0078, 0x78 },  // LATIN SMALL LETTER X
    { 0x0079, 0x79 },  // LATIN SMALL LETTER Y
    { 0x007A, 0x7A },  // LATIN SMALL LETTER Z
    { 0x0131, 0x7B },  // LATIN SMALL LETTER DOTLESS I
    { 0x025B, 0x22 },  // LATIN SMALL LETTER OPEN E, epsiv, varepsilon
    { 0x0311, 0x7F },  // COMBINING INVERTED BREVE
    { 0x0393, 0x00 },  // GREEK CAPITAL LETTER GAMMA
    { 0x0394, 0x01 },  // GREEK CAPITAL LETTER DELTA
    { 0x0398, 0x02 },  // GREEK CAPITAL LETTER THETA
    { 0x039B, 0x03 },  // GREEK CAPITAL LETTER LAMDA
    { 0x039E, 0x04 },  // GREEK CAPITAL LETTER XI
    { 0x03A0, 0x05 },  // GREEK CAPITAL LETTER PI
    { 0x03A3, 0x06 },  // GREEK CAPITAL LETTER SIGMA
    { 0x03A5, 0x07 },  // GREEK CAPITAL LETTER UPSILON
    { 0x03A6, 0x08 },  // GREEK CAPITAL LETTER PHI
    { 0x03A8, 0x09 },  // GREEK CAPITAL LETTER PSI
    { 0x03A9, 0x0A },  // GREEK CAPITAL LETTER OMEGA
    { 0x03B1, 0x0B },  // GREEK SMALL LETTER ALPHA
    { 0x03B2, 0x0C },  // GREEK SMALL LETTER BETA
    { 0x03B3, 0x0D },  // GREEK SMALL LETTER GAMMA
    { 0x03B4, 0x0E },  // GREEK SMALL LETTER DELTA
    { 0x03B5, 0x0F },  // GREEK SMALL LETTER EPSILON
    { 0x03B6, 0x10 },  // GREEK SMALL LETTER ZETA
    { 0x03B7, 0x11 },  // GREEK SMALL LETTER ETA
    { 0x03B8, 0x12 },  // GREEK SMALL LETTER THETA
    { 0x03B9, 0x13 },  // GREEK SMALL LETTER IOTA
    { 0x03BA, 0x14 },  // GREEK SMALL LETTER KAPPA
    { 0x03BB, 0x15 },  // GREEK SMALL LETTER LAMDA
    { 0x03BC, 0x16 },  // GREEK SMALL LETTER MU
    { 0x03BD, 0x17 },  // GREEK SMALL LETTER NU
    { 0x03BE, 0x18 },  // GREEK SMALL LETTER XI
    { 0x03C0, 0x19 },  // GREEK SMALL LETTER PI
    { 0x03C1, 0x1A },  // GREEK SMALL LETTER RHO
    { 0x03C2, 0x26 },  // GREEK SMALL LETTER FINAL SIGMA
    { 0x03C3, 0x1B },  // GREEK SMALL LETTER SIGMA
    { 0x03C4, 0x1C },  // GREEK SMALL LETTER TAU
    { 0x03C5, 0x1D },  // GREEK SMALL LETTER UPSILON
    { 0x03C6, 0x1E },  // GREEK SMALL LETTER PHI
    { 0x03C7, 0x1F },  // GREEK SMALL LETTER CHI
    { 0x03C8, 0x20 },  // GREEK SMALL LETTER PSI
    { 0x03C9, 0x21 },  // GREEK SMALL LETTER OMEGA
    { 0x03D1, 0x23 },  // GREEK THETA SYMBOL
    { 0x03D5, 0x27 },  // GREEK PHI SYMBOL, phiv, varphi
    { 0x03D6, 0x24 },  // GREEK PI SYMBOL
    { 0x03F1, 0x25 },  // GREEK RHO SYMBOL
    { 0x20D7, 0x7E },  // COMBINING RIGHT ARROW ABOVE
    { 0x2113, 0x60 },  // SCRIPT SMALL L
    { 0x2118, 0x7D },  // SCRIPT CAPITAL P
    { 0x21BC, 0x28 },  // LEFTWARDS HARPOON WITH BARB UPWARDS
    { 0x21BD, 0x29 },  // LEFTWARDS HARPOON WITH BARB DOWNWARDS
    { 0x21C0, 0x2A },  // RIGHTWARDS HARPOON WITH BARB UPWARDS
    { 0x21C1, 0x2B },  // RIGHTWARDS HARPOON WITH BARB DOWNWARDS
    { 0x2202, 0x40 },  // PARTIAL DIFFERENTIAL
    { 0x22C6, 0x3F },  // STAR OPERATOR
    { 0x2322, 0x5F },  // frown
    { 0x2323, 0x5E },  // smile
    { 0x25B9, 0x2E },  // WHITE RIGHT-POINTING SMALL TRIANGLE
    { 0x25C3, 0x2F },  // WHITE LEFT-POINTING SMALL TRIANGLE
    { 0x266D, 0x5B },  // MUSIC FLAT SIGN
    { 0x266E, 0x5C },  // MUSIC NATURAL SIGN
    { 0x266F, 0x5D },  // MUSIC SHARP SIGN

    { 0x0000, 0x00 }
  };

static UChar8 cmmTTFMap[] =
  {
    /* 0x00 */ 0xa1, // Gamma
    /* 0x01 */ 0xa2, // Delta
    /* 0x02 */ 0xa3, // Theta
    /* 0x03 */ 0xa4, // Lambda
    /* 0x04 */ 0xa5, // Xi
    /* 0x05 */ 0xa6, // Pi
    /* 0x06 */ 0xa7, // Sigma
    /* 0x07 */ 0xa8, // Upsilon
    /* 0x08 */ 0xa9, // Phi
    /* 0x09 */ 0xaa, // Psi
    /* 0x0a */ 0xad, // Omega
    /* 0x0b */ 0xae, // alpha
    /* 0x0c */ 0xaf, // beta
    /* 0x0d */ 0xb0, // gamma
    /* 0x0e */ 0xb1, // delta
    /* 0x0f */ 0xb2, // epsilon1
    /* 0x10 */ 0xb3, // zeta
    /* 0x11 */ 0xb4, // eta
    /* 0x12 */ 0xb5, // theta
    /* 0x13 */ 0xb6, // iota
    /* 0x14 */ 0x80, // kappa
    /* 0x15 */ 0xb8, // lambda
    /* 0x16 */ 0xb9, // mu
    /* 0x17 */ 0xba, // nu
    /* 0x18 */ 0xbb, // xi
    /* 0x19 */ 0xbc, // pi
    /* 0x1a */ 0xbd, // rho
    /* 0x1b */ 0xbe, // sigma
    /* 0x1c */ 0xbf, // tau
    /* 0x1d */ 0xc0, // upsilon
    /* 0x1e */ 0xc1, // phi
    /* 0x1f */ 0xc2, // chi
    /* 0x20 */ 0xc3, // psi
    /* 0x21 */ 0x21, // omega
    /* 0x22 */ 0x22, // epsilon
    /* 0x23 */ 0x23, // theta1
    /* 0x24 */ 0x24, // pi1
    /* 0x25 */ 0x25, // rho1
    /* 0x26 */ 0x26, // sigma1
    /* 0x27 */ 0x27, // phi1
    /* 0x28 */ 0x28, // arrowlefttophalf
    /* 0x29 */ 0x29, // arrowleftbothalf
    /* 0x2a */ 0x2a, // arrowrighttophalf
    /* 0x2b */ 0x2b, // arrowrightbothalf
    /* 0x2c */ 0x2c, // arrowhookleft
    /* 0x2d */ 0x2d, // arrowhookright
    /* 0x2e */ 0x2e, // triangleright
    /* 0x2f */ 0x2f, // triangleleft
    /* 0x30 */ 0x30, // zerooldstyle
    /* 0x31 */ 0x31, // oneoldstyle
    /* 0x32 */ 0x32, // twooldstyle
    /* 0x33 */ 0x33, // threeoldstyle
    /* 0x34 */ 0x34, // fouroldstyle
    /* 0x35 */ 0x35, // fiveoldstyle
    /* 0x36 */ 0x36, // sixoldstyle
    /* 0x37 */ 0x37, // sevenoldstyle
    /* 0x38 */ 0x38, // eightoldstyle
    /* 0x39 */ 0x39, // nineoldstyle
    /* 0x3a */ 0x3a, // period
    /* 0x3b */ 0x3b, // comma
    /* 0x3c */ 0x3c, // less
    /* 0x3d */ 0x3d, // slash
    /* 0x3e */ 0x3e, // greater
    /* 0x3f */ 0x3f, // star
    /* 0x40 */ 0x40, // partialdiff
    /* 0x41 */ 0x41, // A
    /* 0x42 */ 0x42, // B
    /* 0x43 */ 0x43, // C
    /* 0x44 */ 0x44, // D
    /* 0x45 */ 0x45, // E
    /* 0x46 */ 0x46, // F
    /* 0x47 */ 0x47, // G
    /* 0x48 */ 0x48, // H
    /* 0x49 */ 0x49, // I
    /* 0x4a */ 0x4a, // J
    /* 0x4b */ 0x4b, // K
    /* 0x4c */ 0x4c, // L
    /* 0x4d */ 0x4d, // M
    /* 0x4e */ 0x4e, // N
    /* 0x4f */ 0x4f, // O
    /* 0x50 */ 0x50, // P
    /* 0x51 */ 0x51, // Q
    /* 0x52 */ 0x52, // R
    /* 0x53 */ 0x53, // S
    /* 0x54 */ 0x54, // T
    /* 0x55 */ 0x55, // U
    /* 0x56 */ 0x56, // V
    /* 0x57 */ 0x57, // W
    /* 0x58 */ 0x58, // X
    /* 0x59 */ 0x59, // Y
    /* 0x5a */ 0x5a, // Z
    /* 0x5b */ 0x5b, // flat
    /* 0x5c */ 0x5c, // natural
    /* 0x5d */ 0x5d, // sharp
    /* 0x5e */ 0x5e, // slurbelow
    /* 0x5f */ 0x5f, // slurabove
    /* 0x60 */ 0x60, // lscript
    /* 0x61 */ 0x61, // a
    /* 0x62 */ 0x62, // b
    /* 0x63 */ 0x63, // c
    /* 0x64 */ 0x64, // d
    /* 0x65 */ 0x65, // e
    /* 0x66 */ 0x66, // f
    /* 0x67 */ 0x67, // g
    /* 0x68 */ 0x68, // h
    /* 0x69 */ 0x69, // i
    /* 0x6a */ 0x6a, // j
    /* 0x6b */ 0x6b, // k
    /* 0x6c */ 0x6c, // l
    /* 0x6d */ 0x6d, // m
    /* 0x6e */ 0x6e, // n
    /* 0x6f */ 0x6f, // o
    /* 0x70 */ 0x70, // p
    /* 0x71 */ 0x71, // q
    /* 0x72 */ 0x72, // r
    /* 0x73 */ 0x73, // s
    /* 0x74 */ 0x74, // t
    /* 0x75 */ 0x75, // u
    /* 0x76 */ 0x76, // v
    /* 0x77 */ 0x77, // w
    /* 0x78 */ 0x78, // x
    /* 0x79 */ 0x79, // y
    /* 0x7a */ 0x7a, // z
    /* 0x7b */ 0x7b, // dotlessi
    /* 0x7c */ 0x7c, // dotlessj
    /* 0x7d */ 0x7d, // weierstrass
    /* 0x7e */ 0x7e, // vector
    /* 0x7f */ 0xc4, // tie
  };

static ComputerModernShaper::PlainChar cmrMap[] =
  {
    { 0x0021, 0x21 },  // EXCLAMATION MARK
    { 0x0023, 0x23 },  // NUMBER SIGN
    { 0x0024, 0x24 },  // DOLLAR SIGN
    { 0x0025, 0x25 },  // PERCENT SIGN
    { 0x0026, 0x26 },  // AMPERSAND
    { 0x0027, 0x27 },  // APOSTROPHE
    { 0x0028, 0x28 },  // LEFT PARENTHESIS
    { 0x0029, 0x29 },  // RIGHT PARENTHESIS
    { 0x002A, 0x2A },  // ASTERISK
    { 0x002B, 0x2B },  // PLUS SIGN
    { 0x002C, 0x2C },  // COMMA
    { 0x002D, 0x2D },  // HYPHEN-MINUS
    { 0x002E, 0x2E },  // FULL STOP
    { 0x002F, 0x2F },  // SOLIDUS
    { 0x0030, 0x30 },  // DIGIT ZERO
    { 0x0031, 0x31 },  // DIGIT ONE
    { 0x0032, 0x32 },  // DIGIT TWO
    { 0x0033, 0x33 },  // DIGIT THREE
    { 0x0034, 0x34 },  // DIGIT FOUR
    { 0x0035, 0x35 },  // DIGIT FIVE
    { 0x0036, 0x36 },  // DIGIT SIX
    { 0x0037, 0x37 },  // DIGIT SEVEN
    { 0x0038, 0x38 },  // DIGIT EIGHT
    { 0x0039, 0x39 },  // DIGIT NINE
    { 0x003A, 0x3A },  // COLON
    { 0x003B, 0x3B },  // SEMICOLON
    { 0x003D, 0x3D },  // EQUALS SIGN
    { 0x003F, 0x3F },  // QUESTION MARK
    { 0x0040, 0x40 },  // COMMERCIAL AT
    { 0x0041, 0x41 },  // LATIN CAPITAL LETTER A
    { 0x0042, 0x42 },  // LATIN CAPITAL LETTER B
    { 0x0043, 0x43 },  // LATIN CAPITAL LETTER C
    { 0x0044, 0x44 },  // LATIN CAPITAL LETTER D
    { 0x0045, 0x45 },  // LATIN CAPITAL LETTER E
    { 0x0046, 0x46 },  // LATIN CAPITAL LETTER F
    { 0x0047, 0x47 },  // LATIN CAPITAL LETTER G
    { 0x0048, 0x48 },  // LATIN CAPITAL LETTER H
    { 0x0049, 0x49 },  // LATIN CAPITAL LETTER I
    { 0x004A, 0x4A },  // LATIN CAPITAL LETTER J
    { 0x004B, 0x4B },  // LATIN CAPITAL LETTER K
    { 0x004C, 0x4C },  // LATIN CAPITAL LETTER L
    { 0x004D, 0x4D },  // LATIN CAPITAL LETTER M
    { 0x004E, 0x4E },  // LATIN CAPITAL LETTER N
    { 0x004F, 0x4F },  // LATIN CAPITAL LETTER O
    { 0x0050, 0x50 },  // LATIN CAPITAL LETTER P
    { 0x0051, 0x51 },  // LATIN CAPITAL LETTER Q
    { 0x0052, 0x52 },  // LATIN CAPITAL LETTER R
    { 0x0053, 0x53 },  // LATIN CAPITAL LETTER S
    { 0x0054, 0x54 },  // LATIN CAPITAL LETTER T
    { 0x0055, 0x55 },  // LATIN CAPITAL LETTER U
    { 0x0056, 0x56 },  // LATIN CAPITAL LETTER V
    { 0x0057, 0x57 },  // LATIN CAPITAL LETTER W
    { 0x0058, 0x58 },  // LATIN CAPITAL LETTER X
    { 0x0059, 0x59 },  // LATIN CAPITAL LETTER Y
    { 0x005A, 0x5A },  // LATIN CAPITAL LETTER Z
    { 0x005B, 0x5B },  // LEFT SQUARE BRACKET
    { 0x005D, 0x5D },  // RIGHT SQUARE BRACKET
    { 0x0061, 0x61 },  // LATIN SMALL LETTER A
    { 0x0062, 0x62 },  // LATIN SMALL LETTER B
    { 0x0063, 0x63 },  // LATIN SMALL LETTER C
    { 0x0064, 0x64 },  // LATIN SMALL LETTER D
    { 0x0065, 0x65 },  // LATIN SMALL LETTER E
    { 0x0066, 0x66 },  // LATIN SMALL LETTER F
    { 0x0067, 0x67 },  // LATIN SMALL LETTER G
    { 0x0068, 0x68 },  // LATIN SMALL LETTER H
    { 0x0069, 0x69 },  // LATIN SMALL LETTER I
    { 0x006A, 0x6A },  // LATIN SMALL LETTER J
    { 0x006B, 0x6B },  // LATIN SMALL LETTER K
    { 0x006C, 0x6C },  // LATIN SMALL LETTER L
    { 0x006D, 0x6D },  // LATIN SMALL LETTER M
    { 0x006E, 0x6E },  // LATIN SMALL LETTER N
    { 0x006F, 0x6F },  // LATIN SMALL LETTER O
    { 0x0070, 0x70 },  // LATIN SMALL LETTER P
    { 0x0071, 0x71 },  // LATIN SMALL LETTER Q
    { 0x0072, 0x72 },  // LATIN SMALL LETTER R
    { 0x0073, 0x73 },  // LATIN SMALL LETTER S
    { 0x0074, 0x74 },  // LATIN SMALL LETTER T
    { 0x0075, 0x75 },  // LATIN SMALL LETTER U
    { 0x0076, 0x76 },  // LATIN SMALL LETTER V
    { 0x0077, 0x77 },  // LATIN SMALL LETTER W
    { 0x0078, 0x78 },  // LATIN SMALL LETTER X
    { 0x0079, 0x79 },  // LATIN SMALL LETTER Y
    { 0x007A, 0x7A },  // LATIN SMALL LETTER Z
    { 0x00A1, 0x3C },  // INVERTED EXCLAMATION MARK
    { 0x00BF, 0x3E },  // INVERTED QUESTION MARK
    { 0x00C6, 0x1D },  // LATIN CAPITAL LETTER AE
    { 0x00D8, 0x1F },  // LATIN CAPITAL LETTER O WITH STROKE
    { 0x00DF, 0x19 },  // LATIN SMALL LETTER SHARP S
    { 0x00E6, 0x1A },  // LATIN SMALL LETTER AE
    { 0x00F8, 0x1C },  // LATIN SMALL LETTER O WITH STROKE
    { 0x0131, 0x10 },  // LATIN SMALL LETTER DOTLESS I
    { 0x0152, 0x1E },  // LATIN CAPITAL LIGATURE OE
    { 0x0153, 0x1B },  // LATIN SMALL LIGATURE OE
    { 0x0300, 0x12 },  // COMBINING GRAVE ACCENT
    { 0x0301, 0x13 },  // COMBINING ACUTE ACCENT
    { 0x0302, 0x5E },  // COMBINING CIRCUMFLEX ACCENT
    { 0x0303, 0x7E },  // COMBINING TILDE
    { 0x0305, 0x16 },  // COMBINING OVERLINE
    { 0x0306, 0x15 },  // COMBINING BREVE
    { 0x0307, 0x5F },  // COMBINING DOT ABOVE
    { 0x0308, 0x7F },  // COMBINING DIAERESIS
    { 0x030A, 0x17 },  // COMBINING RING ABOVE
    { 0x030B, 0x7D },  // COMBINING DOUBLE ACUTE ACCENT
    { 0x030C, 0x14 },  // COMBINING CARON
    { 0x0327, 0x18 },  // COMBINING CEDILLA
    { 0x0337, 0x20 },  // COMBINING SHORT SOLIDUS OVERLAY
    { 0x0393, 0x00 },  // GREEK CAPITAL LETTER GAMMA
    { 0x0394, 0x01 },  // GREEK CAPITAL LETTER DELTA
    { 0x0398, 0x02 },  // GREEK CAPITAL LETTER THETA
    { 0x039B, 0x03 },  // GREEK CAPITAL LETTER LAMDA
    { 0x039E, 0x04 },  // GREEK CAPITAL LETTER XI
    { 0x03A0, 0x05 },  // GREEK CAPITAL LETTER PI
    { 0x03A3, 0x06 },  // GREEK CAPITAL LETTER SIGMA
    { 0x03A5, 0x07 },  // GREEK CAPITAL LETTER UPSILON
    { 0x03A6, 0x08 },  // GREEK CAPITAL LETTER PHI
    { 0x03A8, 0x09 },  // GREEK CAPITAL LETTER PSI
    { 0x03A9, 0x0A },  // GREEK CAPITAL LETTER OMEGA
    { 0x2013, 0x7B },  // EN DASH
    { 0x2014, 0x7C },  // EM DASH
    { 0x2018, 0x60 },  // LEFT SINGLE QUOTATION MARK
    { 0x201C, 0x5C },  // LEFT DOUBLE QUOTATION MARK
    { 0x201D, 0x22 },  // RIGHT DOUBLE QUOTATION MARK
    { 0xFB00, 0x0B },  // LATIN SMALL LIGATURE FF
    { 0xFB01, 0x0C },  // LATIN SMALL LIGATURE FI
    { 0xFB02, 0x0D },  // LATIN SMALL LIGATURE FL
    { 0xFB03, 0x0E },  // LATIN SMALL LIGATURE FFI
    { 0xFB04, 0x0F },  // LATIN SMALL LIGATURE FFL

    { 0x0000, 0x00 }
  };

static UChar8 cmrTTFMap[] =
  {
    /* 0x00 */ 0xa1, // Gamma
    /* 0x01 */ 0xa2, // Delta
    /* 0x02 */ 0xa3, // Theta
    /* 0x03 */ 0xa4, // Lambda
    /* 0x04 */ 0xa5, // Xi
    /* 0x05 */ 0xa6, // Pi
    /* 0x06 */ 0xa7, // Sigma
    /* 0x07 */ 0xa8, // Upsilon
    /* 0x08 */ 0xa9, // Phi
    /* 0x09 */ 0xaa, // Psi
    /* 0x0a */ 0xad, // Omega
    /* 0x0b */ 0xae, // ff
    /* 0x0c */ 0xaf, // fi
    /* 0x0d */ 0xb0, // fl
    /* 0x0e */ 0xb1, // ffi
    /* 0x0f */ 0xb2, // ffl
    /* 0x10 */ 0xb3, // dotlessi
    /* 0x11 */ 0xb4, // dotlessj
    /* 0x12 */ 0xb5, // grave
    /* 0x13 */ 0xb6, // acute
    /* 0x14 */ 0xb8, // caron
    /* 0x15 */ 0xb8, // breve
    /* 0x16 */ 0xb9, // macron
    /* 0x17 */ 0xba, // ring
    /* 0x18 */ 0xbb, // cedilla
    /* 0x19 */ 0xbc, // germandbls
    /* 0x1a */ 0xbd, // ae
    /* 0x1b */ 0xbe, // oe
    /* 0x1c */ 0xbf, // oslash
    /* 0x1d */ 0xc0, // AE
    /* 0x1e */ 0xc1, // OE
    /* 0x1f */ 0xc2, // Oslash
    /* 0x20 */ 0xc3, // suppress
    /* 0x21 */ 0x21, // exclam
    /* 0x22 */ 0x22, // quotedblright
    /* 0x23 */ 0x23, // numbersign
    /* 0x24 */ 0x24, // dollar
    /* 0x25 */ 0x25, // percent
    /* 0x26 */ 0x26, // ampersand
    /* 0x27 */ 0x27, // quoteright
    /* 0x28 */ 0x28, // parenleft
    /* 0x29 */ 0x29, // parenright
    /* 0x2a */ 0x2a, // asterisk
    /* 0x2b */ 0x2b, // plus
    /* 0x2c */ 0x2c, // comma
    /* 0x2d */ 0x2d, // hyphen
    /* 0x2e */ 0x2e, // period
    /* 0x2f */ 0x2f, // slash
    /* 0x30 */ 0x30, // zero
    /* 0x31 */ 0x31, // one
    /* 0x32 */ 0x32, // two
    /* 0x33 */ 0x33, // three
    /* 0x34 */ 0x34, // four
    /* 0x35 */ 0x35, // five
    /* 0x36 */ 0x36, // six
    /* 0x37 */ 0x37, // seven
    /* 0x38 */ 0x38, // eight
    /* 0x39 */ 0x39, // nine
    /* 0x3a */ 0x3a, // colon
    /* 0x3b */ 0x3b, // semicolon
    /* 0x3c */ 0x3c, // exclamdown
    /* 0x3d */ 0x3d, // equal
    /* 0x3e */ 0x3e, // questiondown
    /* 0x3f */ 0x3f, // question
    /* 0x40 */ 0x40, // at
    /* 0x41 */ 0x41, // A
    /* 0x42 */ 0x42, // B
    /* 0x43 */ 0x43, // C
    /* 0x44 */ 0x44, // D
    /* 0x45 */ 0x45, // E
    /* 0x46 */ 0x46, // F
    /* 0x47 */ 0x47, // G
    /* 0x48 */ 0x48, // H
    /* 0x49 */ 0x49, // I
    /* 0x4a */ 0x4a, // J
    /* 0x4b */ 0x4b, // K
    /* 0x4c */ 0x4c, // L
    /* 0x4d */ 0x4d, // M
    /* 0x4e */ 0x4e, // N
    /* 0x4f */ 0x4f, // O
    /* 0x50 */ 0x50, // P
    /* 0x51 */ 0x51, // Q
    /* 0x52 */ 0x52, // R
    /* 0x53 */ 0x53, // S
    /* 0x54 */ 0x54, // T
    /* 0x55 */ 0x55, // U
    /* 0x56 */ 0x56, // V
    /* 0x57 */ 0x57, // W
    /* 0x58 */ 0x58, // X
    /* 0x59 */ 0x59, // Y
    /* 0x5a */ 0x5a, // Z
    /* 0x5b */ 0x5b, // bracketleft
    /* 0x5c */ 0x5c, // quotedblleft
    /* 0x5d */ 0x5d, // bracketright
    /* 0x5e */ 0x5e, // circumflex
    /* 0x5f */ 0x5f, // dotaccent
    /* 0x60 */ 0x60, // quoteleft
    /* 0x61 */ 0x61, // a
    /* 0x62 */ 0x62, // b
    /* 0x63 */ 0x63, // c
    /* 0x64 */ 0x64, // d
    /* 0x65 */ 0x65, // e
    /* 0x66 */ 0x66, // f
    /* 0x67 */ 0x67, // g
    /* 0x68 */ 0x68, // h
    /* 0x69 */ 0x69, // i
    /* 0x6a */ 0x6a, // j
    /* 0x6b */ 0x6b, // k
    /* 0x6c */ 0x6c, // l
    /* 0x6d */ 0x6d, // m
    /* 0x6e */ 0x6e, // n
    /* 0x6f */ 0x6f, // o
    /* 0x70 */ 0x70, // p
    /* 0x71 */ 0x71, // q
    /* 0x72 */ 0x72, // r
    /* 0x73 */ 0x73, // s
    /* 0x74 */ 0x74, // t
    /* 0x75 */ 0x75, // u
    /* 0x76 */ 0x76, // v
    /* 0x77 */ 0x77, // w
    /* 0x78 */ 0x78, // x
    /* 0x79 */ 0x79, // y
    /* 0x7a */ 0x7a, // z
    /* 0x7b */ 0x7c, // endash
    /* 0x7c */ 0x7b, // emdash
    /* 0x7d */ 0x7d, // hungarumlaut
    /* 0x7e */ 0x7e, // tilde
    /* 0x7f */ 0xc4, // dieresis
  };

static ComputerModernShaper::PlainChar cmsMap[] =
  {
    { 0x007B, 0x66 },  // LEFT CURLY BRACKET
    { 0x007D, 0x67 },  // RIGHT CURLY BRACKET
    { 0x00A7, 0x78 },  // SECTION SIGN
    { 0x00AC, 0x3A },  // NOT SIGN
    { 0x00B1, 0x06 },  // PLUS-MINUS SIGN
    { 0x00B6, 0x7B },  // PILCROW SIGN
    { 0x00B7, 0x01 },  // MIDDLE DOT
    { 0x00D7, 0x02 },  // MULTIPLICATION SIGN
    { 0x00F7, 0x04 },  // DIVISION SIGN
    { 0x0338, 0x36 },  // COMBINING LONG SOLIDUS OVERLAY
    { 0x2020, 0x79 },  // DAGGER
    { 0x2021, 0x7A },  // DOUBLE DAGGER
    { 0x2032, 0x30 },  // PRIME
    { 0x210B, 0x48 },  // Script capital H
    { 0x2110, 0x49 },  // Script capital I
    { 0x2111, 0x3D },  // Fraktur capital I
    { 0x2112, 0x4C },  // Script capital L
    { 0x211B, 0x52 },  // SCRIPT CAPITAL R, &Rscr;
    { 0x211C, 0x3C },  // Fraktur capital R
    { 0x212C, 0x42 },  // Script capital B
    { 0x2130, 0x45 },  // Script capital E
    { 0x2131, 0x46 },  // Script capital F
    { 0x2133, 0x4D },  // Script capital M
    { 0x2135, 0x40 },  // Alef symbol
    { 0x2190, 0x20 },
    { 0x2191, 0x22 },  // UPWARDS ARROW
    { 0x2192, 0x21 },
    { 0x2193, 0x23 },  // DOWNWARDS ARROW
    { 0x2194, 0x24 },  // LEFT RIGHT ARROW
    { 0x2195, 0x6C },  // UP DOWN ARROW
    { 0x2196, 0x2D },  // NORTH WEST ARROW
    { 0x2197, 0x25 },  // NORTH EAST ARROW
    { 0x2198, 0x26 },  // SOUTH EAST ARROW
    { 0x2199, 0x2E },  // SOUTH WEST ARROW
    { 0x21D0, 0x28 },
    { 0x21D1, 0x2A },
    { 0x21D2, 0x29 },
    { 0x21D3, 0x2B },
    { 0x21D4, 0x2C },  // LEFT RIGHT DOUBLE ARROW
    { 0x21D5, 0x6D },  // UP DOWN DOUBLE ARROW
    { 0x2200, 0x38 },  // FOR ALL
    { 0x2203, 0x39 },  // THERE EXISTS
    { 0x2205, 0x3B },  // EMPTY SET
    { 0x2207, 0x72 },  // NABLA
    { 0x2208, 0x32 },  // ELEMENT OF
    { 0x220B, 0x33 },  // CONTAINS AS MEMBER
    { 0x2210, 0x71 },  // N-ARY COPRODUCT
    { 0x2212, 0x00 },  // Minus sign
    { 0x2213, 0x07 },  // MINUS-OR-PLUS SIGN
    { 0x2216, 0x6E },  // SET MINUS
    { 0x2218, 0x0E },  // RING OPERATOR
    { 0x2219, 0x0F },  // BULLET OPERATOR
    { 0x221A, 0x70 },  // SQUARE ROOT
    { 0x221D, 0x2F },  // PROPORTIONAL TO
    { 0x221E, 0x31 },  // INFINITY
    { 0x2223, 0x6A },  // DIVIDES
    { 0x2225, 0x6B },  // PARALLEL TO
    { 0x2227, 0x5E },  // Logical and
    { 0x2228, 0x5F },  // Logical or
    { 0x2229, 0x5C },  // Intersection
    { 0x222A, 0x5B },  // Union
    { 0x222B, 0x73 },  // INTEGRAL
    { 0x223C, 0x18 },  // TILDE OPERATOR
    { 0x2240, 0x6F },  // WREATH PRODUCT
    { 0x2242, 0x27 },  // MINUS TILDE
    { 0x2243, 0x10 },  // ASYMPTOTICALLY EQUAL TO
    { 0x2245, 0x19 },  // APPROXIMATELY EQUAL TO
    { 0x224D, 0x11 },  // EQUIVALENT TO
    { 0x2264, 0x14 },  // LESS-THAN OR EQUAL TO
    { 0x2265, 0x15 },  // GREATER-THAN OR EQUAL TO
    { 0x226A, 0x1C },  // Much less-than
    { 0x226B, 0x1D },  // Much greater-than
    { 0x227A, 0x1E },  // PRECEDES
    { 0x227B, 0x1F },  // SUCCEEDS
    { 0x227C, 0x16 },  // PRECEDES OR EQUAL TO
    { 0x227D, 0x17 },  // SUCCEEDS OR EQUAL TO
    { 0x2282, 0x1A },  // SUBSET OF
    { 0x2283, 0x1B },  // SUPERSET OF
    { 0x2286, 0x12 },  // SUBSET OF OR EQUAL TO
    { 0x2287, 0x13 },  // SUPERSET OF OR EQUAL TO
    { 0x228E, 0x5D },  // Multiset union
    { 0x2291, 0x76 },  // SQUARE IMAGE OF OR EQUAL TO
    { 0x2292, 0x77 },  // SQUARE ORIGINAL OF OR EQUAL TO
    { 0x2293, 0x75 },  // SQUARE CAP
    { 0x2294, 0x74 },  // SQUARE CUP
    { 0x2295, 0x08 },  // CIRCLED PLUS
    { 0x2296, 0x09 },  // CIRCLED MINUS
    { 0x2297, 0x0A },  // CIRCLED TIMES
    { 0x2298, 0x0B },  // CIRCLED DIVISION SLASH
    { 0x2299, 0x0C },  // CIRCLED DOT OPERATOR
    { 0x22A2, 0x60 },  // RIGHT TACK
    { 0x22A3, 0x61 },  // LEFT TACK
    { 0x22A4, 0x3E },  // DOWN TACK
    { 0x22A5, 0x3F },  // UP TACK
    { 0x22C4, 0x05 },  // DIAMOND OPERATOR
    { 0x22C6, 0x03 },  // STAR OPERATOR
    { 0x2308, 0x64 },  // LEFT CEILING
    { 0x2309, 0x65 },  // RIGHT CEILING
    { 0x230A, 0x62 },  // LEFT FLOOR
    { 0x230B, 0x63 },  // RIGHT FLOOR
    { 0x2329, 0x68 },  // LEFT-POINTING ANGLE BRACKET
    { 0x232A, 0x69 },  // RIGHT-POINTING ANGLE BRACKET
    { 0x25B3, 0x34 },  // WHITE UP-POINTING TRIANGLE
    { 0x25BD, 0x35 },  // WHITE DOWN-POINTING TRIANGLE
    { 0x25CB, 0x0D },  // WHITE CIRCLE
    { 0x2660, 0x7F },  // BLACK SPADE SUIT
    { 0x2661, 0x7E },  // WHITE HEART SUIT
    { 0x2662, 0x7D },  // WHITE DIAMOND SUIT
    { 0x2663, 0x7C },  // BLACK CLUB SUIT
    { 0x1D49C, 0x41 },  // MATHEMATICAL SCRIPT CAPITAL A, &Ascr;
    { 0x1D49E, 0x43 },  // MATHEMATICAL SCRIPT CAPITAL C, &Cscr;
    { 0x1D49F, 0x44 },  // MATHEMATICAL SCRIPT CAPITAL D, &Dscr;
    { 0x1D4A2, 0x47 },  // MATHEMATICAL SCRIPT CAPITAL G, &Gscr;
    { 0x1D4A5, 0x4A },  // MATHEMATICAL SCRIPT CAPITAL J, &Jscr;
    { 0x1D4A6, 0x4B },  // MATHEMATICAL SCRIPT CAPITAL K, &Kscr;
    { 0x1D4A9, 0x4E },  // MATHEMATICAL SCRIPT CAPITAL N, &Nscr;
    { 0x1D4AA, 0x4F },  // MATHEMATICAL SCRIPT CAPITAL O, &Oscr;
    { 0x1D4AB, 0x50 },  // MATHEMATICAL SCRIPT CAPITAL P, &Pscr;
    { 0x1D4AC, 0x51 },  // MATHEMATICAL SCRIPT CAPITAL Q, &Qscr;
    { 0x1D4AE, 0x53 },  // MATHEMATICAL SCRIPT CAPITAL S, &Sscr;
    { 0x1D4AF, 0x54 },  // MATHEMATICAL SCRIPT CAPITAL T, &Tscr;
    { 0x1D4B0, 0x55 },  // MATHEMATICAL SCRIPT CAPITAL U, &Uscr;
    { 0x1D4B1, 0x56 },  // MATHEMATICAL SCRIPT CAPITAL V, &Vscr;
    { 0x1D4B2, 0x57 },  // MATHEMATICAL SCRIPT CAPITAL W, &Wscr;
    { 0x1D4B3, 0x58 },  // MATHEMATICAL SCRIPT CAPITAL X, &Xscr;
    { 0x1D4B4, 0x59 },  // MATHEMATICAL SCRIPT CAPITAL Y, &Yscr;
    { 0x1D4B5, 0x5A },  // MATHEMATICAL SCRIPT CAPITAL Z, &Zscr;

    { 0x0000, 0x00 }
  };

static UChar8 cmsTTFMap[] =
  {
    /* 0x00 */ 0xa1, // minus
    /* 0x01 */ 0xa2, // periodcentered
    /* 0x02 */ 0xa3, // multiply
    /* 0x03 */ 0xa4, // asteriskmath
    /* 0x04 */ 0xa5, // divide
    /* 0x05 */ 0xa6, // diamondmath
    /* 0x06 */ 0xa7, // plusminus
    /* 0x07 */ 0xa8, // minusplus
    /* 0x08 */ 0xa9, // circleplus
    /* 0x09 */ 0xaa, // circleminus
    /* 0x0a */ 0xad, // circlemultiply
    /* 0x0b */ 0xae, // circledivide
    /* 0x0c */ 0xaf, // circledot
    /* 0x0d */ 0xb0, // circlecopyrt
    /* 0x0e */ 0xb1, // openbullet
    /* 0x0f */ 0xb2, // bullet
    /* 0x10 */ 0xb3, // equivasymptotic
    /* 0x11 */ 0xb4, // equivalence
    /* 0x12 */ 0xb5, // reflexsubset
    /* 0x13 */ 0xb6, // reflexsuperset
    /* 0x14 */ 0xb7, // lessequal
    /* 0x15 */ 0xb8, // greaterequal
    /* 0x16 */ 0xb9, // precedesequal
    /* 0x17 */ 0xba, // followsequal
    /* 0x18 */ 0xbb, // similar
    /* 0x19 */ 0xbc, // approxequal
    /* 0x1a */ 0xbd, // propersubset
    /* 0x1b */ 0xbe, // propersuperset
    /* 0x1c */ 0xbf, // lessmuch
    /* 0x1d */ 0xc0, // greatermuch
    /* 0x1e */ 0xc1, // precedes
    /* 0x1f */ 0xc2, // follows
    /* 0x20 */ 0xc3, // arrowleft
    /* 0x21 */ 0x21, // arrowright
    /* 0x22 */ 0x22, // arrowup
    /* 0x23 */ 0x23, // arrowdown
    /* 0x24 */ 0x24, // arrowboth
    /* 0x25 */ 0x25, // arrownortheast
    /* 0x26 */ 0x26, // arrowsoutheast
    /* 0x27 */ 0x27, // similarequal
    /* 0x28 */ 0x28, // arrowdblleft
    /* 0x29 */ 0x29, // arrowdblright
    /* 0x2a */ 0x2a, // arrowdblup
    /* 0x2b */ 0x2b, // arrowdbldown
    /* 0x2c */ 0x2c, // arrowdblboth
    /* 0x2d */ 0x2d, // arrownorthwest
    /* 0x2e */ 0x2e, // arrowsouthwest
    /* 0x2f */ 0x2f, // proportional
    /* 0x30 */ 0x30, // prime
    /* 0x31 */ 0x31, // infinity
    /* 0x32 */ 0x32, // element
    /* 0x33 */ 0x33, // owner
    /* 0x34 */ 0x34, // triangle
    /* 0x35 */ 0x35, // triangleinv
    /* 0x36 */ 0x36, // negationslash
    /* 0x37 */ 0x37, // mapsto
    /* 0x38 */ 0x38, // universal
    /* 0x39 */ 0x39, // existential
    /* 0x3a */ 0x3a, // logicalnot
    /* 0x3b */ 0x3b, // emptyset
    /* 0x3c */ 0x3c, // Rfractur
    /* 0x3d */ 0x3d, // Ifractur
    /* 0x3e */ 0x3e, // latticetop
    /* 0x3f */ 0x3f, // perpendicular
    /* 0x40 */ 0x40, // aleph
    /* 0x41 */ 0x41, // A
    /* 0x42 */ 0x42, // B
    /* 0x43 */ 0x43, // C
    /* 0x44 */ 0x44, // D
    /* 0x45 */ 0x45, // E
    /* 0x46 */ 0x46, // F
    /* 0x47 */ 0x47, // G
    /* 0x48 */ 0x48, // H
    /* 0x49 */ 0x49, // I
    /* 0x4a */ 0x4a, // J
    /* 0x4b */ 0x4b, // K
    /* 0x4c */ 0x4c, // L
    /* 0x4d */ 0x4d, // M
    /* 0x4e */ 0x4e, // N
    /* 0x4f */ 0x4f, // O
    /* 0x50 */ 0x50, // P
    /* 0x51 */ 0x51, // Q
    /* 0x52 */ 0x52, // R
    /* 0x53 */ 0x53, // S
    /* 0x54 */ 0x54, // T
    /* 0x55 */ 0x55, // U
    /* 0x56 */ 0x56, // V
    /* 0x57 */ 0x57, // W
    /* 0x58 */ 0x58, // X
    /* 0x59 */ 0x59, // Y
    /* 0x5a */ 0x5a, // Z
    /* 0x5b */ 0x5b, // union
    /* 0x5c */ 0x5c, // intersection
    /* 0x5d */ 0x5d, // unionmulti
    /* 0x5e */ 0x5e, // logicaland
    /* 0x5f */ 0x5f, // logicalor
    /* 0x60 */ 0x60, // turnstileleft
    /* 0x61 */ 0x61, // turnstileright
    /* 0x62 */ 0x62, // floorleft
    /* 0x63 */ 0x63, // floorright
    /* 0x64 */ 0x64, // ceilingleft
    /* 0x65 */ 0x65, // ceilingright
    /* 0x66 */ 0x66, // braceleft
    /* 0x67 */ 0x67, // braceright
    /* 0x68 */ 0x68, // angbracketleft
    /* 0x69 */ 0x69, // angbracketright
    /* 0x6a */ 0x6a, // bar
    /* 0x6b */ 0x6b, // bardbl
    /* 0x6c */ 0x6c, // arrowbothv
    /* 0x6d */ 0x6d, // arrowdblbothv
    /* 0x6e */ 0x6e, // backslash
    /* 0x6f */ 0x6f, // wreathproduct
    /* 0x70 */ 0x70, // radical
    /* 0x71 */ 0x71, // coproduct
    /* 0x72 */ 0x72, // nabla
    /* 0x73 */ 0x73, // integral
    /* 0x74 */ 0x74, // unionsq
    /* 0x75 */ 0x75, // intersectionsq
    /* 0x76 */ 0x76, // subsetsqequal
    /* 0x77 */ 0x77, // supersetsqequal
    /* 0x78 */ 0x78, // section
    /* 0x79 */ 0x79, // dagger
    /* 0x7a */ 0x7a, // daggerdbl
    /* 0x7b */ 0x7b, // paragraph
    /* 0x7c */ 0x7c, // club
    /* 0x7d */ 0x7d, // diamond
    /* 0x7e */ 0x7e, // heart
    /* 0x7f */ 0xc4, // spade
  };

static ComputerModernShaper::HStretchyChar hMap[] =
  {
    //        N            L            G            R
    { 0x2190, {CMS, 0x20}, {CMS, 0x20}, {CMS, 0x00}, {NIL, 0x00} },
    { 0x2192, {CMS, 0x21}, {NIL, 0x00}, {CMS, 0x00}, {CMS, 0x21} },
    { 0x2194, {CMS, 0x24}, {CMS, 0x20}, {CMS, 0x00}, {CMS, 0x21} },
    { 0x21a6, {NIL, 0x00}, {CMS, 0x37}, {CMS, 0x00}, {CMS, 0x21} },
    { 0x21a9, {NIL, 0x00}, {CMS, 0x20}, {CMS, 0x00}, {CMM, 0x2D} },
    { 0x21aa, {NIL, 0x00}, {CMM, 0x2C}, {CMS, 0x00}, {CMS, 0x21} },
    { 0x21bc, {CMM, 0x28}, {NIL, 0x00}, {CMS, 0x00}, {NIL, 0x00} },
    { 0x21bd, {CMM, 0x29}, {NIL, 0x00}, {CMS, 0x00}, {NIL, 0x00} },
    { 0x21c0, {CMM, 0x2A}, {NIL, 0x00}, {CMS, 0x00}, {NIL, 0x00} },
    { 0x21c1, {CMM, 0x2B}, {NIL, 0x00}, {CMS, 0x00}, {NIL, 0x00} },
    { 0x21d0, {CMS, 0x28}, {CMS, 0x28}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x21d4, {CMS, 0x2C}, {CMS, 0x28}, {NIL, 0x00}, {CMS, 0x29} },
    { 0x21d2, {CMS, 0x29}, {NIL, 0x00}, {NIL, 0x00}, {CMS, 0x29} },

    { 0x0000, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }
  };

static ComputerModernShaper::VStretchyChar vMap[] =
  {
      //        N            N1           N2           N3           N4             T            G            M            B
    { 0x0028, { {CMR, 0x28}, {CME, 0x00}, {CME, 0x10}, {CME, 0x12}, {CME, 0x20} }, {CME, 0x30}, {CME, 0x42}, {NIL, 0x00}, {CME, 0x40} },
    { 0x0029, { {CMR, 0x29}, {CME, 0x01}, {CME, 0x11}, {CME, 0x13}, {CME, 0x21} }, {CME, 0x31}, {CME, 0x43}, {NIL, 0x00}, {CME, 0x41} },
    { 0x002F, { {CMR, 0x2F}, {CME, 0x0E}, {CME, 0x2E}, {CME, 0x1E}, {CME, 0x2C} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x005B, { {CMR, 0x5B}, {CME, 0x02}, {CME, 0x68}, {CME, 0x14}, {CME, 0x22} }, {CME, 0x32}, {CME, 0x36}, {NIL, 0x00}, {CME, 0x34} },
    { 0x005D, { {CMR, 0x5D}, {CME, 0x03}, {CME, 0x69}, {CME, 0x15}, {CME, 0x23} }, {CME, 0x33}, {CME, 0x37}, {NIL, 0x00}, {CME, 0x35} },
    { 0x007B, { {CMS, 0x66}, {CME, 0x08}, {CME, 0x6E}, {CME, 0x1A}, {CME, 0x28} }, {CME, 0x38}, {CME, 0x3E}, {CME, 0x3C}, {CME, 0x3A} },
    { 0x007D, { {CMS, 0x67}, {CME, 0x09}, {CME, 0x6F}, {CME, 0x1B}, {CME, 0x29} }, {CME, 0x39}, {CME, 0x3E}, {CME, 0x3D}, {CME, 0x3B} },
    { 0x2191, { {CMS, 0x22}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {CME, 0x78}, {CME, 0x3F}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2193, { {CMS, 0x23}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {CME, 0x3F}, {NIL, 0x00}, {CME, 0x79} },
    { 0x2195, { {CMS, 0x6C}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {CME, 0x78}, {CME, 0x3F}, {NIL, 0x00}, {CME, 0x79} },
    { 0x21d1, { {CMS, 0x2a}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {CME, 0x7E}, {CME, 0x77}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x21d3, { {CMS, 0x2b}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {CME, 0x77}, {NIL, 0x00}, {CME, 0x7F} },
    { 0x21d5, { {CMS, 0x6D}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {CME, 0x7E}, {CME, 0x77}, {NIL, 0x00}, {CME, 0x7F} },
    { 0x220F, { {NIL, 0x00}, {CME, 0x51}, {CME, 0x59}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2210, { {CMS, 0x71}, {CME, 0x60}, {CME, 0x61}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2211, { {NIL, 0x00}, {CME, 0x50}, {CME, 0x58}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2216, { {CMS, 0x6E}, {CME, 0x0F}, {CME, 0x2F}, {CME, 0x1F}, {CME, 0x2D} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x221A, { {CMS, 0x70}, {CME, 0x70}, {CME, 0x71}, {CME, 0x72}, {CME, 0x73} }, {CME, 0x76}, {CME, 0x75}, {NIL, 0x00}, {CME, 0x74} },
    { 0x2223, { {CMS, 0x6A}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {CME, 0x0C}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2225, { {CMS, 0x6B}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {CME, 0x0D}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2227, { {CMS, 0x5E}, {CME, 0x56}, {CME, 0x5E}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2228, { {CMS, 0x5F}, {CME, 0x57}, {CME, 0x5F}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x222B, { {CMS, 0x73}, {CME, 0x52}, {CME, 0x5A}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x222E, { {NIL, 0x00}, {CME, 0x48}, {CME, 0x49}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x228E, { {CMS, 0x5D}, {CME, 0x55}, {CME, 0x5D}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2294, { {CMS, 0x74}, {CME, 0x46}, {CME, 0x47}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2295, { {CMS, 0x08}, {CME, 0x4C}, {CME, 0x4D}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2297, { {CMS, 0x0A}, {CME, 0x4E}, {CME, 0x4F}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2299, { {CMS, 0x0C}, {CME, 0x4A}, {CME, 0x4B}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x22C2, { {NIL, 0x00}, {CME, 0x54}, {CME, 0x5C}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x22C3, { {NIL, 0x00}, {CME, 0x53}, {CME, 0x5B}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2308, { {CMS, 0x64}, {CME, 0x06}, {CME, 0x6C}, {CME, 0x18}, {CME, 0x26} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2309, { {CMS, 0x65}, {CME, 0x07}, {CME, 0x6D}, {CME, 0x19}, {CME, 0x27} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x230A, { {CMS, 0x62}, {CME, 0x04}, {CME, 0x6A}, {CME, 0x16}, {CME, 0x24} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x230B, { {CMS, 0x63}, {CME, 0x05}, {CME, 0x6B}, {CME, 0x17}, {CME, 0x25} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x2329, { {CMS, 0x68}, {CME, 0x0A}, {CME, 0x44}, {CME, 0x1C}, {CME, 0x2A} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },
    { 0x232A, { {CMS, 0x69}, {CME, 0x0B}, {CME, 0x45}, {CME, 0x1D}, {CME, 0x2B} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} },

    { 0x0000, { {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00}, {NIL, 0x00} }
  };

static ComputerModernShaper::HBigChar hbMap[] =
  {
    { 0x02DC, { {CME, 0x65}, {CME, 0x66}, {CME, 0x67} } },
    { 0x0302, { {CME, 0x62}, {CME, 0x63}, {CME, 0x64} } },
  };

#define OVER_BRACE  0xfe37
#define UNDER_BRACE 0xfe38
#define OVER_BRACE_INDEX OVER_BRACE
#define UNDER_BRACE_INDEX UNDER_BRACE

static UChar8 cmexTTFMap[] =
  {
    /* 0x00 */ 0xa1, // parenleftbig
    /* 0x01 */ 0xa2, // parenrightbig
    /* 0x02 */ 0xa3, // bracketleftbig
    /* 0x03 */ 0xa4, // bracketrightbig
    /* 0x04 */ 0xa5, // floorleftbig
    /* 0x05 */ 0xa6, // floorrightbig
    /* 0x06 */ 0xa7, // ceilingleftbig
    /* 0x07 */ 0xa8, // ceilingrightbig
    /* 0x08 */ 0xa9, // braceleftbig
    /* 0x09 */ 0xaa, // bracerightbig
    /* 0x0a */ 0xad, // angbracketleftbig
    /* 0x0b */ 0xae, // angbracketrightbig
    /* 0x0c */ 0xaf, // vextendsingle
    /* 0x0d */ 0xb0, // vextenddouble
    /* 0x0e */ 0xb1, // slashbig
    /* 0x0f */ 0xb2, // backslashbig
    /* 0x10 */ 0xb3, // parenleftBig
    /* 0x11 */ 0xb4, // parenrightBig
    /* 0x12 */ 0xb5, // parenleftbigg
    /* 0x13 */ 0xb6, // parenrightbigg
    /* 0x14 */ 0xb7, // bracketleftbigg
    /* 0x15 */ 0xb8, // bracketrightbigg
    /* 0x16 */ 0xb9, // floorleftbigg
    /* 0x17 */ 0xba, // floorrightbigg
    /* 0x18 */ 0xbb, // ceilingleftbigg
    /* 0x19 */ 0xbc, // ceilingrightbigg
    /* 0x1a */ 0xbd, // braceleftbigg
    /* 0x1b */ 0xbe, // bracerightbigg
    /* 0x1c */ 0xbf, // angbracketleftbigg
    /* 0x1d */ 0xc0, // angbracketrightbigg
    /* 0x1e */ 0xc1, // slashbigg
    /* 0x1f */ 0xc2, // backslashbigg
    /* 0x20 */ 0xc3, // parenleftBigg
    /* 0x21 */ 0x21, // parenrightBigg
    /* 0x22 */ 0x22, // bracketleftBigg
    /* 0x23 */ 0x23, // bracketrightBigg
    /* 0x24 */ 0x24, // floorleftBigg
    /* 0x25 */ 0x25, // floorrightBigg
    /* 0x26 */ 0x26, // ceilingleftBigg
    /* 0x27 */ 0x27, // ceilingrightBigg
    /* 0x28 */ 0x28, // braceleftBigg
    /* 0x29 */ 0x29, // bracerightBigg
    /* 0x2a */ 0x2a, // angbracketleftBigg
    /* 0x2b */ 0x2b, // angbracketrightBigg
    /* 0x2c */ 0x2c, // slashBigg
    /* 0x2d */ 0x2d, // backslashBigg
    /* 0x2e */ 0x2e, // slashBig
    /* 0x2f */ 0x2f, // backslashBig
    /* 0x30 */ 0x30, // parenlefttp
    /* 0x31 */ 0x31, // parenrighttp
    /* 0x32 */ 0x32, // bracketlefttp
    /* 0x33 */ 0x33, // bracketrighttp
    /* 0x34 */ 0x34, // bracketleftbt
    /* 0x35 */ 0x35, // bracketrightbt
    /* 0x36 */ 0x36, // bracketleftex
    /* 0x37 */ 0x37, // bracketrightex
    /* 0x38 */ 0x38, // bracelefttp
    /* 0x39 */ 0x39, // bracerighttp
    /* 0x3a */ 0x3a, // braceleftbt
    /* 0x3b */ 0x3b, // bracerightbt
    /* 0x3c */ 0x3c, // braceleftmid
    /* 0x3d */ 0x3d, // bracerightmid
    /* 0x3e */ 0x3e, // braceex
    /* 0x3f */ 0x3f, // arrowvertex
    /* 0x40 */ 0x40, // parenleftbt
    /* 0x41 */ 0x41, // parenrightbt
    /* 0x42 */ 0x42, // parenleftex
    /* 0x43 */ 0x43, // parenrightex
    /* 0x44 */ 0x44, // angbracketleftBig
    /* 0x45 */ 0x45, // angbracketrightBig
    /* 0x46 */ 0x46, // unionsqtext
    /* 0x47 */ 0x47, // unionsqdisplay
    /* 0x48 */ 0x48, // contintegraltext
    /* 0x49 */ 0x49, // contintegraldisplay
    /* 0x4a */ 0x4a, // circledottext
    /* 0x4b */ 0x4b, // circledotdisplay
    /* 0x4c */ 0x4c, // circleplustext
    /* 0x4d */ 0x4d, // circleplusdisplay
    /* 0x4e */ 0x4e, // circlemultiplytext
    /* 0x4f */ 0x4f, // circlemultiplydisplay
    /* 0x50 */ 0x50, // summationtext
    /* 0x51 */ 0x51, // producttext
    /* 0x52 */ 0x52, // integraltext
    /* 0x53 */ 0x53, // uniontext
    /* 0x54 */ 0x54, // intersectiontext
    /* 0x55 */ 0x55, // unionmultitext
    /* 0x56 */ 0x56, // logicalandtext
    /* 0x57 */ 0x57, // logicalortext
    /* 0x58 */ 0x58, // summationdisplay
    /* 0x59 */ 0x59, // productdisplay
    /* 0x5a */ 0x5a, // integraldisplay
    /* 0x5b */ 0x5b, // uniondisplay
    /* 0x5c */ 0x5c, // intersectiondisplay
    /* 0x5d */ 0x5d, // unionmultidisplay
    /* 0x5e */ 0x5e, // logicalanddisplay
    /* 0x5f */ 0x5f, // logicalordisplay
    /* 0x60 */ 0x60, // coproducttext
    /* 0x61 */ 0x61, // coproductdisplay
    /* 0x62 */ 0x62, // hatwide
    /* 0x63 */ 0x63, // hatwider
    /* 0x64 */ 0x64, // hatwidest
    /* 0x65 */ 0x65, // tildewide
    /* 0x66 */ 0x66, // tildewider
    /* 0x67 */ 0x67, // tildewidest
    /* 0x68 */ 0x68, // bracketleftBig
    /* 0x69 */ 0x69, // bracketrightBig
    /* 0x6a */ 0x6a, // floorleftBig
    /* 0x6b */ 0x6b, // floorrightBig
    /* 0x6c */ 0x6c, // ceilingleftBig
    /* 0x6d */ 0x6d, // ceilingrightBig
    /* 0x6e */ 0x6e, // braceleftBig
    /* 0x6f */ 0x6f, // bracerightBig
    /* 0x70 */ 0x70, // radicalbig
    /* 0x71 */ 0x71, // radicalBig
    /* 0x72 */ 0x72, // radicalbigg
    /* 0x73 */ 0x73, // radicalBigg
    /* 0x74 */ 0x74, // radicalbt
    /* 0x75 */ 0x75, // radicalvertex
    /* 0x76 */ 0x76, // radicaltp
    /* 0x77 */ 0x77, // arrowvertexdbl
    /* 0x78 */ 0x78, // arrowtp
    /* 0x79 */ 0x79, // arrowbt
    /* 0x7a */ 0x7a, // bracehtipdownleft
    /* 0x7b */ 0x7b, // bracehtipdownright
    /* 0x7c */ 0x7c, // bracehtipupleft
    /* 0x7d */ 0x7d, // bracehtipupright
    /* 0x7e */ 0x7e, // arrowdbltp
    /* 0x7f */ 0xc4, // arrowdblbt
  };

struct FontDesc
{
  MathVariant variant;
  ComputerModernFamily::FontNameId name;
};

static FontDesc variantDesc[] =
  {
    { NORMAL_VARIANT, ComputerModernFamily::FN_CMR },
    { BOLD_VARIANT, ComputerModernFamily::FN_CMB },
    { ITALIC_VARIANT, ComputerModernFamily::FN_CMTI },
    { BOLD_ITALIC_VARIANT, ComputerModernFamily::FN_CMBXTI },
    { SANS_SERIF_VARIANT, ComputerModernFamily::FN_CMSS },
    { BOLD_SANS_SERIF_VARIANT, ComputerModernFamily::FN_CMSSBX },
    { SANS_SERIF_ITALIC_VARIANT, ComputerModernFamily::FN_CMSSI },
    { MONOSPACE_VARIANT, ComputerModernFamily::FN_CMTT }
  };

inline UChar8
makeFontId(ComputerModernFamily::FontEncId id, MathVariant variant = NORMAL_VARIANT)
{
  assert(ComputerModernFamily::validFontEncId(id));
  assert(validMathVariant(variant));
  return id | ((variant - NORMAL_VARIANT) << 4);
}

inline ComputerModernFamily::FontEncId
encIdOfFontId(UChar8 id)
{ return ComputerModernFamily::FontEncId(id & 0x0f); }

inline MathVariant
variantOfFontId(UChar8 id)
{ return MathVariant((id >> 4) + NORMAL_VARIANT); }

UChar8
ComputerModernShaper::toTTFGlyphIndex(ComputerModernFamily::FontEncId name, UChar8 index)
{
  UChar8* map = 0;
  switch (name)
    {
    case ComputerModernFamily::FE_CMR:
      assert(index < sizeof(cmrTTFMap));
      map = cmrTTFMap;
      break;
    case ComputerModernFamily::FE_CMMI:
      assert(index < sizeof(cmmTTFMap));
      map = cmmTTFMap;
      break;
    case ComputerModernFamily::FE_CMSY:
      assert(index < sizeof(cmsTTFMap));
      map = cmsTTFMap;
      break;
    case ComputerModernFamily::FE_CMEX:
      assert(index < sizeof(cmexTTFMap));
      map = cmexTTFMap;
      break;
    default:
      assert(false);
      break;
    }
  return map[index];
}

ComputerModernShaper::ComputerModernShaper(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : postShapingMode(POST_SHAPING_NEVER), family(ComputerModernFamily::create(l, conf))
{ }

ComputerModernShaper::~ComputerModernShaper()
{ }

bool
ComputerModernShaper::setPostShapingMode(const String& m)
{
  if (m == "never")
    setPostShapingMode(POST_SHAPING_NEVER);
  else if (m == "always")
    setPostShapingMode(POST_SHAPING_ALWAYS);
  else if (m == "math")
    setPostShapingMode(POST_SHAPING_MATH);
  else if (m == "text")
    setPostShapingMode(POST_SHAPING_TEXT);
  else
    return false;
  return true;
}

void
ComputerModernShaper::registerShaper(const SmartPtr<ShaperManager>& sm, unsigned shaperId)
{
  assert(sm);

  for (unsigned j = 0; j < sizeof(variantDesc) / sizeof(FontDesc); j++)
    for (unsigned i = 0; cmrMap[i].ch; i++)
      {
	const Char32 vch = mapMathVariant(variantDesc[j].variant, cmrMap[i].ch);
	if (family->fontEnabled(variantDesc[j].name)
	    && (variantDesc[j].variant == NORMAL_VARIANT || vch != cmrMap[i].ch))
	  sm->registerChar(vch,
			   GlyphSpec(shaperId,
				     makeFontId(ComputerModernFamily::encIdOfFontNameId(variantDesc[j].name),
						variantDesc[j].variant),
				     cmrMap[i].index));
      }

  for (unsigned i = 0; cmmMap[i].ch; i++)
    {
      const Char32 ch = cmmMap[i].ch;
      const Char32 ch1 = mapMathVariant(ITALIC_VARIANT, ch);
      if (family->fontEnabled(ComputerModernFamily::FN_CMMI))
	{
	  if (ch1 != ch)
	    sm->registerChar(ch1, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_CMMI, ITALIC_VARIANT), cmmMap[i].index));
	  else
	    sm->registerChar(ch, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_CMMI, NORMAL_VARIANT), cmmMap[i].index));
	}

      const Char32 ch2 = mapMathVariant(BOLD_VARIANT, ch);
      if (ch2 != ch && family->fontEnabled(ComputerModernFamily::FN_CMMIB))
	sm->registerChar(ch2, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_CMMI, BOLD_VARIANT), cmmMap[i].index));

      const Char32 ch3 = mapMathVariant(BOLD_ITALIC_VARIANT, ch);
      if (ch3 != ch && ch3 != ch && family->fontEnabled(ComputerModernFamily::FN_CMMIB))
	sm->registerChar(ch3, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_CMMI, BOLD_ITALIC_VARIANT), cmmMap[i].index));
    }

  for (unsigned i = 0; cmsMap[i].ch; i++)
    {
      const Char32 ch = cmsMap[i].ch;
      const Char32 vch = mapMathVariant(BOLD_VARIANT, ch);

      if (family->fontEnabled(ComputerModernFamily::FN_CMSY))
	sm->registerChar(ch, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_CMSY), cmsMap[i].index));

      if (family->fontEnabled(ComputerModernFamily::FN_CMBSY) && vch != ch)
	sm->registerChar(vch, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_CMSY, BOLD_VARIANT), cmsMap[i].index));
    }

  for (unsigned i = 0; vMap[i].ch; i++)
    sm->registerStretchyChar(vMap[i].ch, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_V_STRETCHY), i));

  for (unsigned i = 0; hMap[i].ch; i++)
    sm->registerStretchyChar(hMap[i].ch, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_H_STRETCHY), i));

  sm->registerStretchyChar(UNDER_BRACE, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_H_BRACE), UNDER_BRACE_INDEX));
  sm->registerStretchyChar(OVER_BRACE, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_H_BRACE), OVER_BRACE_INDEX));

  for (unsigned i = 0; i < sizeof(hbMap) / sizeof(HBigChar); i++)
    sm->registerStretchyChar(hbMap[i].ch, GlyphSpec(shaperId, makeFontId(ComputerModernFamily::FE_H_BIG), i));
}

void
ComputerModernShaper::unregisterShaper(const SmartPtr<ShaperManager>&, unsigned)
{
  // nothing to do???
}

void
ComputerModernShaper::shape(ShapingContext& context) const
{
  for (unsigned n = context.chunkSize(); n > 0; n--)
    {
      AreaRef res;
      const ComputerModernFamily::FontEncId encId = encIdOfFontId(context.getSpec().getFontId());
      switch (encId)
	{
	case ComputerModernFamily::FE_H_BIG:
	  assert(false);
	  break;
	case ComputerModernFamily::FE_H_BRACE:
	  res = shapeHorizontalBrace(context);
	  break;
	case ComputerModernFamily::FE_H_STRETCHY:
	  res = shapeStretchyCharH(context);
	  break;
	case ComputerModernFamily::FE_V_STRETCHY:
	  res = shapeStretchyCharV(context);
	  break;
	default:
	  res = shapeChar(context, encId);
	  break;
	}

      if (!res) break;

      context.pushArea(1, res);

      switch (getPostShapingMode())
	{
	case POST_SHAPING_ALWAYS:
	  postShape(context);
	  break;
	case POST_SHAPING_MATH:
	  if (context.inMathMode()) postShape(context);
	  break;
	case POST_SHAPING_TEXT:
	  if (!context.inMathMode()) postShape(context);
	  break;
	case POST_SHAPING_NEVER:
	  break;
	default:
	  assert(false);
	}
    }
}

void
ComputerModernShaper::postShape(ShapingContext&) const
{ /* do nothing */ }

AreaRef
ComputerModernShaper::shapeChar(const ShapingContext& context, ComputerModernFamily::FontEncId encId) const
{
  const MathVariant hardVariant = variantOfFontId(context.getSpec().getFontId());
  const MathVariant softVariant = context.getMathVariant();
  const MathVariant variant = (hardVariant != NORMAL_VARIANT) ? hardVariant : softVariant;
  return getGlyphArea(variant, encId, context.getSpec().getGlyphId(), context.getSize());
}

AreaRef
ComputerModernShaper::shapeStretchyCharV(const ShapingContext& context) const
{
  const MathVariant variant = context.getMathVariant();
  const SmartPtr<AreaFactory> factory = context.getFactory();
  const scaled size = context.getSize();
  const scaled span = context.getVSpan() - (1 * size) / 10; // use tex formula
  const VStretchyChar& charSpec = vMap[context.getSpec().getGlyphId()];

  AreaRef normal = 0;
  for (unsigned i = 0; i < 5; i++)
    if (AreaRef tryNormal = getGlyphArea(variant, charSpec.normal[i], size))
      {
	if (tryNormal->box().verticalExtent() >= span)
	  return tryNormal;
	else
	  normal = tryNormal;
      }

  AreaRef top = getGlyphArea(variant, charSpec.top, size);
  AreaRef glue = getGlyphArea(variant, charSpec.glue, size);
  AreaRef middle = getGlyphArea(variant, charSpec.middle, size);
  AreaRef bottom = getGlyphArea(variant, charSpec.bottom, size);

  return composeStretchyCharV(factory, normal, top, glue, middle, bottom, span);
}

AreaRef
ComputerModernShaper::shapeStretchyCharH(const ShapingContext& context) const
{
  const MathVariant variant = context.getMathVariant();
  const SmartPtr<AreaFactory> factory = context.getFactory();
  const scaled size = context.getSize();
  const scaled span = context.getHSpan() - (1 * size) / 10; // use tex formula also for H?
  const HStretchyChar& charSpec = hMap[context.getSpec().getGlyphId()];

  AreaRef normal = getGlyphArea(variant, charSpec.normal, size);
  AreaRef left = getGlyphArea(variant, charSpec.left, size);
  AreaRef glue = getGlyphArea(variant, charSpec.glue, size);
  AreaRef right = getGlyphArea(variant, charSpec.right, size);

  return composeStretchyCharH(factory, normal, left, glue, right, span);
}

AreaRef
ComputerModernShaper::shapeHorizontalBrace(const ShapingContext& context) const
{
  const MathVariant variant = context.getMathVariant();
  const SmartPtr<AreaFactory> factory = context.getFactory();
  const scaled size = context.getSize();
  const scaled span = context.getHSpan() - (1 * size) / 10;
  
  AreaRef tipDL = getGlyphArea(variant, ComputerModernFamily::FE_CMEX, 0x7a, size);
  AreaRef tipDR = getGlyphArea(variant, ComputerModernFamily::FE_CMEX, 0x7b, size);
  AreaRef tipUL = getGlyphArea(variant, ComputerModernFamily::FE_CMEX, 0x7c, size);
  AreaRef tipUR = getGlyphArea(variant, ComputerModernFamily::FE_CMEX, 0x7d, size);
  
  const scaled thickness = tipDL->box().height;
  const scaled fixedWidth = tipDL->box().width + tipDR->box().width + tipUL->box().width + tipUR->box().width;
  const scaled barWidth = std::max(scaled::zero(), span - fixedWidth);

  std::vector<AreaRef> barC;
  barC.reserve(2);
  barC.push_back(factory->verticalSpace(thickness, 0));
  barC.push_back(factory->horizontalSpace(barWidth / 2));

  const AreaRef bar =
    (barWidth > scaled::zero())
    ? factory->ink(factory->horizontalArray(barC))
    : 0;

  std::vector<AreaRef> c;
  c.reserve(7);
  c.push_back(factory->verticalSpace(3 * thickness, 2 * thickness));
  switch (context.getSpec().getGlyphId())
    {
    case UNDER_BRACE_INDEX:
      c.push_back(tipUL);
      if (bar) c.push_back(bar);
      c.push_back(tipDR);
      c.push_back(tipDL);
      if (bar) c.push_back(bar);
      c.push_back(tipUR);
      break;
    case OVER_BRACE_INDEX:
      c.push_back(tipDL);
      if (bar) c.push_back(bar);
      c.push_back(tipUR);
      c.push_back(tipUL);
      if (bar) c.push_back(bar);
      c.push_back(tipDR);
      break;
    default:
      assert(false);
    }

  return factory->horizontalArray(c);
}

AreaRef
ComputerModernShaper::shapeBigCharH(const ShapingContext& context) const
{
  const MathVariant variant = context.getMathVariant();
  const SmartPtr<AreaFactory> factory = context.getFactory();
  const scaled size = context.getSize();
  const scaled span = context.getHSpan() - (1 * size) / 10;
  
  AreaRef res;
  for (unsigned i = 0; i < sizeof(hbMap[0].big) / sizeof(GlyphSpec); i++)
    {
      res = getGlyphArea(variant, hbMap[i].big[i], size);
      if (res && res->box().width >= span)
	return res;
    }
  
  return res;
}

AreaRef
ComputerModernShaper::getGlyphArea(MathVariant variant, const GlyphIndex& glyph, const scaled& size) const
{
  if (glyph.valid())
    return getGlyphArea(variant, ComputerModernFamily::FontEncId(glyph.fontEnc), glyph.index, size);
  else
    return 0;
}

AreaRef
ComputerModernShaper::getGlyphArea(MathVariant variant, ComputerModernFamily::FontEncId encId, UChar8 index, const scaled& size) const
{
  scaled finalSize = size;
  ComputerModernFamily::FontSizeId designSize;
  const ComputerModernFamily::FontNameId fontNameId = family->findFont(variant, encId, finalSize, designSize);
  if (fontNameId != ComputerModernFamily::FN_NIL)
    return getGlyphArea(fontNameId, designSize, index, finalSize.toInt());
  else
    return 0;
}

