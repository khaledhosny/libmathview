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

#include "DOM.hh"

typedef DOM::UTF8String String;
typedef DOM::UCS4String UCS4String;
typedef UCS4String::value_type Char;

inline bool isXmlSpace(Char ch) { return ch == 0x09 || ch == 0x0a || ch == 0x0d || ch == 0x20; }

inline UCS4String toUCS4String(const String& s) { return DOM::GdomeString(s); }
inline String fromUCS4String(const UCS4String& s) { return DOM::GdomeString(s); }

inline String fromDOMString(const DOM::GdomeString& s) { return s; }
inline DOM::GdomeString toDOMString(const String& s) { return s; }

String trimSpacesLeft(const String&);
String trimSpacesRight(const String&);
String collapseSpaces(const String&);
String deleteSpaces(const String&);
String toLowerCase(const String&);

struct StringHash : public std::unary_function<String, size_t>
{
  size_t operator()(const String&) const;
};

//typedef equal_to<String, String, bool> StringEq;
//struct StringEq : public equal_to<String, String, bool> { };

struct StringEq : public std::binary_function<String, String, bool>
{
  bool operator()(const String& s1, const String& s2) const
  { return s1 == s2; }
};

#endif // __String_hh__
