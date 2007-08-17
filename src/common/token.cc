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

#include "HashMap.hh"
#include "StringHash.hh"
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
