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

#ifndef __String_hh__
#define __String_hh__

#include <string>

#include "gmv_defines.h"
#include "Char.hh"

typedef std::basic_string<Char> String;
typedef std::basic_string<Char8> UTF8String;
//typedef std::basic_string<Char16> UTF16String;
typedef std::basic_string<Char32> UCS4String;

inline GMV_MathView_EXPORT bool isXmlSpace(char ch) { return ch == 0x09 || ch == 0x0a || ch == 0x0d || ch == 0x20; }

GMV_MathView_EXPORT String trimSpacesLeft(const String&);
GMV_MathView_EXPORT String trimSpacesRight(const String&);
GMV_MathView_EXPORT String collapseSpaces(const String&);
GMV_MathView_EXPORT String deleteSpaces(const String&);
GMV_MathView_EXPORT String toLowerCase(const String&);

GMV_MathView_EXPORT UTF8String UTF8StringOfUCS4String(const UCS4String&);
GMV_MathView_EXPORT UCS4String UCS4StringOfUTF8String(const UTF8String&);
#if 0
GMV_MathView_EXPORT UTF16String UTF16StringOfUCS4String(const UCS4String&);
GMV_MathView_EXPORT UCS4String UCS4StringOfUTF16String(const UTF16String&);
#endif

#if CHAR8 == 1
inline GMV_MathView_EXPORT String StringOfUCS4String(const UCS4String& s) { return UTF8StringOfUCS4String(s); }
inline GMV_MathView_EXPORT UCS4String UCS4StringOfString(const String& s) { return UCS4StringOfUTF8String(s); }
#elif CHAR16 == 1
#error "UTF16 encoding is not supported"
#if 0
inline GMV_MathView_EXPORT String StringOfUCS4String(const UCS4String& s) { return UTF16StringOfUCS4String(s); }
inline GMV_MathView_EXPORT UCS4String UCS4StringOfString(const String& s) { return UCS4StringOfUTF16String(s); }
#endif
#elif CHAR32 == 1
inline GMV_MathView_EXPORT String StringOfUCS4String(const UCS4String& s) { return s; }
inline GMV_MathView_EXPORT UCS4String UCS4StringOfString(const String& s) { return s; }
#else
#error "could not define string conversion functions"
#endif

#endif // __String_hh__
