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

#include <config.h>

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
  do i--; while (i != s.begin() && isXmlSpace(*i));
  if (i != s.end())
    return String(s.begin(), i + 1);
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

size_t
StringHash::operator()(const String& s) const
{
  size_t h = 0;
  for (String::const_iterator i = s.begin(); i != s.end(); i++)
    {
      h = (h << 4) + *i;
      if (size_t g = h & 0xf0000000)
	{
	  h = h ^ (g >> 24);
	  h = h ^ g;
	}
    }

  return h;
}

