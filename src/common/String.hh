// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef __String_hh__
#define __String_hh__

#include <functional>
#include <string>

#include "Char.hh"

typedef std::basic_string<Char> String;
typedef std::basic_string<Char8> UTF8String;
//typedef std::basic_string<Char16> UTF16String;
typedef std::basic_string<Char32> UCS4String;

inline bool isXmlSpace(char ch) { return ch == 0x09 || ch == 0x0a || ch == 0x0d || ch == 0x20; }

String trimSpacesLeft(const String&);
String trimSpacesRight(const String&);
String collapseSpaces(const String&);
String deleteSpaces(const String&);
String toLowerCase(const String&);

UTF8String UTF8StringOfUCS4String(const UCS4String&);
UCS4String UCS4StringOfUTF8String(const UTF8String&);
#if 0
UTF16String UTF16StringOfUCS4String(const UCS4String&);
UCS4String UCS4StringOfUTF16String(const UTF16String&);
#endif

#if CHAR8 == 1
inline String StringOfUCS4String(const UCS4String& s) { return UTF8StringOfUCS4String(s); }
inline UCS4String UCS4StringOfString(const String& s) { return UCS4StringOfUTF8String(s); }
#elif CHAR16 == 1
#error "UTF16 encoding is not supported"
#if 0
inline String StringOfUCS4String(const UCS4String& s) { return UTF16StringOfUCS4String(s); }
inline UCS4String UCS4StringOfString(const String& s) { return UCS4StringOfUTF16String(s); }
#endif
#elif CHAR32 == 1
inline String StringOfUCS4String(const UCS4String& s) { return s; }
inline UCS4String UCS4StringOfString(const String& s) { return s; }
#else
#error "could not define string conversion functions"
#endif

struct StringHash : public std::unary_function<String, size_t>
{
  size_t operator()(const String&) const;
};

struct StringEq : public std::binary_function<String, String, bool>
{
  bool operator()(const String& s1, const String& s2) const
  { return s1 == s2; }
};

#endif // __String_hh__
