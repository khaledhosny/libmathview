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

#include <config.h>

#include <string.h>

#include "Value.hh"
#include "keyword.hh"

#include "keyword.def"
#include "attribute.def"
#include "tag.def"

void
InitKeywordValues()
{
  for (unsigned i = 1; sKeywordToken[i].name != NULL; i++)
    sKeywordToken[i].value = new Value(sKeywordToken[i].id);
}

KeywordId
KeywordIdOfName(const char* s)
{
  unsigned i;
  for (i = 1; sKeywordToken[i].name != 0 && strcmp(s, sKeywordToken[i].name); i++) ;
  return sKeywordToken[i].name != 0 ? sKeywordToken[i].id : KW_NOTVALID;
}

const char*
NameOfKeywordId(KeywordId id)
{
  return sKeywordToken[id].name;
}

const char*
NameOfUnitId(UnitId id)
{
  static const char* name[] = {
    "not-valid-unit-id",
    "zero",
    "em", "ex", "px", "in", "cm", "mm", "pt", "pc", "%"
  };

  return name[id + 1];
}

const Value*
ValueOfKeywordId(KeywordId id)
{
  return sKeywordToken[id].value;
}

AttributeId
AttributeIdOfName(const char* prefix, const char* s)
{
  AttributeId id = AttributeIdOfName(s);
  // TODO: check the namespace!
  return id;
}

AttributeId
AttributeIdOfName(const char* s)
{
  unsigned i;
  for (i = 1; sAttributeToken[i].name != 0 && strcmp(s, sAttributeToken[i].name); i++) ;
  return sAttributeToken[i].name != 0 ? sAttributeToken[i].id : ATTR_NOTVALID;
}

const char*
NameOfAttributeId(AttributeId id)
{
  return sAttributeToken[id].name;
}

TagId
TagIdOfName(const char* s)
{
  unsigned i;
  for (i = 1; sTagToken[i].name != 0 && strcmp(s, sTagToken[i].name); i++) ;
  return sTagToken[i].name != 0 ? sTagToken[i].id : TAG_NOTVALID;
}

const char*
NameOfTagId(TagId id)
{
  return sTagToken[id].name;
}
