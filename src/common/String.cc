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

#include <glib.h>
#include <cctype>

#include "String.hh"

String
trimSpacesLeft(const String& s)
{
  String::const_iterator i = s.begin();
  while (i != s.end() && isXmlSpace(*i)) i++;
  if (i != s.begin())
    return String(i, s.end());
  else
    return s;
}

String
trimSpacesRight(const String& s)
{
  String::const_iterator i = s.end();
  while (i != s.begin() && isXmlSpace(*(i - 1))) i--;
  if (i != s.end())
    return String(s.begin(), i);
  else
    return s;
}

String
collapseSpaces(const String& s)
{
  String res;
  res.reserve(s.length());

  String::const_iterator i = s.begin();
  while (i != s.end())
    if (isXmlSpace(*i))
      {
	res.push_back(' ');
	do i++; while (i != s.end() && isXmlSpace(*i));
      }
    else
      {
	res.push_back(*i);
	i++;
      }
  return res;
}

String
deleteSpaces(const String& s)
{
  String res;
  res.reserve(s.length());
  for (String::const_iterator i = s.begin(); i != s.end(); i++)
    if (!isXmlSpace(*i))
      res.push_back(*i);
  return res;
}

String
toLowerCase(const String& s)
{
  String res;
  res.reserve(s.length());
  for (String::const_iterator i = s.begin(); i != s.end(); i++)
    res.push_back(tolower(*i));
  return res;
}

template <typename DEST_CHAR, typename SOURCE_CHAR, typename DEST_STRING, typename SOURCE_STRING, 
	  DEST_CHAR* (*f)(const SOURCE_CHAR*, glong, glong*, glong*, GError**)>
DEST_STRING
DESTofSOURCE(const SOURCE_STRING& s)
{
  g_assert(sizeof(DEST_CHAR) == sizeof(typename DEST_STRING::value_type));
  g_assert(sizeof(SOURCE_CHAR) == sizeof(typename SOURCE_STRING::value_type));
  glong length;
  DEST_CHAR* destBuffer = f((const SOURCE_CHAR*) s.data(), s.length(), NULL, &length, NULL);
  DEST_STRING res((const typename DEST_STRING::value_type*) destBuffer, length);
  g_free(destBuffer);
  return res;
}

UTF8String
UTF8StringOfUCS4String(const UCS4String& s)
{ return DESTofSOURCE<gchar, gunichar, UTF8String, UCS4String, &g_ucs4_to_utf8>(s); }

UCS4String
UCS4StringOfUTF8String(const UTF8String& s)
{ return DESTofSOURCE<gunichar, gchar, UCS4String, UTF8String, &g_utf8_to_ucs4>(s); }

#if 0
UTF16String
UTF16StringOfUCS4String(const UCS4String& s)
{ return DESTofSOURCE<gunichar2, gunichar, UTF16String, UCS4String, &g_ucs4_to_utf16>(s); }

UCS4String
UCS4StringOfUTF16String(const UTF16String& s)
{ return DESTofSOURCE<gunichar, gunichar2, UCS4String, UTF16String, &g_utf16_to_ucs4>(s); }
#endif
