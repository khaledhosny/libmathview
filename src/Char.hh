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

#ifndef Char_hh
#define Char_hh

#include <stddef.h>

#include "keyword.hh"

#if defined(HAVE_GMETADOM)

#include "gmetadom.hh"

typedef DOM::Char8  Char8;
typedef DOM::Char16 Char16;
typedef DOM::Char32 Char32;

#else

#include <glib.h>

typedef guint8  Char8;
typedef guint16 Char16;
typedef guint32 Char32;

#endif

typedef Char32 Char;

inline bool isPlain(Char ch) { return ch < 0x80; }
inline bool isUnicode16(Char ch) { return ch < 0x8000; }
inline bool isUnicode32(Char) { return true; }

Char        getBiggestChar(const Char*, unsigned);
bool 	    isPlain(const char*);
bool 	    isPlain(const char*, unsigned);
bool 	    isPlain(const Char*, unsigned);
bool 	    isPlain(const class String&, unsigned, unsigned);

inline bool isXmlSpace(Char ch) { return ch == 0x09 || ch == 0x0a || ch == 0x0d || ch == 0x20; }

inline bool isVariant(Char ch) { return ch == 0xfe00; }
inline bool isCombining(Char ch) { return (ch >= 0x0300 && ch <= 0x0362) || (ch >= 0x20d0 && ch <= 0x20e8); }
bool        isCombiningOverlay(Char ch);
bool        isCombiningBelow(Char ch);
inline bool isCombiningAbove(Char ch) { return isCombining(ch) && !isCombiningOverlay(ch) && !isCombiningBelow(ch); }

unsigned    isNonMarkingChar(Char, int* = 0, BreakId* = 0);
unsigned    isNonMarkingChar(Char, Char, int* = 0, BreakId* = 0);
unsigned    isNonMarkingChar(const class String&, int* = 0, BreakId* = 0);
unsigned    isNonMarkingChar(const class String&, unsigned, int* = 0, BreakId* = 0);

inline bool isUpperCaseGreek(Char ch) { return ch >= 0x0391 && ch <= 0x03a9; }
inline bool isIntegral(Char ch) { return ch >= 0x222b && ch <= 0x2233; }

#endif // Char_hh
