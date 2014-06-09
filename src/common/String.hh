// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __String_hh__
#define __String_hh__

#include <cstdint>
#include <string>

typedef char Char;
typedef uint32_t Char32;

typedef std::basic_string<Char> String;
typedef std::basic_string<Char32> UCS4String;

inline bool isXmlSpace(char ch) { return ch == 0x09 || ch == 0x0a || ch == 0x0d || ch == 0x20; }

String trimSpacesLeft(const String&);
String trimSpacesRight(const String&);
String collapseSpaces(const String&);
String deleteSpaces(const String&);
String toLowerCase(const String&);

String StringOfUCS4String(const UCS4String&);
UCS4String UCS4StringOfString(const String&);

#endif // __String_hh__
