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

#include <cassert>

#include "HashMap.hh"
#include "token.hh"

struct Entry
{
  TokenId id;
  const char* literal;
};

static Entry token[] =
  {
#include "token.def"
    { T__NOTVALID, 0 }
  };

typedef HASH_MAP_NS::hash_map<String,TokenId,StringHash,StringEq> Map;
static Map map;

TokenId
tokenIdOfString(const char* s)
{
  assert(s);
  return tokenIdOfString(String(s));
}

TokenId
tokenIdOfString(const String& s)
{
  static bool initialized = false;
  if (!initialized)
    {
      for (unsigned i = 1; token[i].literal; i++)
	map[String(token[i].literal)] = token[i].id;
      initialized = true;
    }

  Map::iterator p = map.find(s);
  return (p != map.end()) ? (*p).second : T__NOTVALID;
}

const char*
stringOfTokenId(TokenId id)
{
  assert(id >= 0 && id < sizeof(token) / sizeof(Entry));
  return token[id].literal;
}
