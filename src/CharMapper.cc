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
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "minidom.h"
#include "Iterator.hh"
#include "stringAux.hh"
#include "MathEngine.hh"
#include "CharMapper.hh"
#include "FontManager.hh"
#include "EntitiesTable.hh"
#include "MathMLParseFile.hh"

// CharMapper: this class is used to map a Unicode char and a FontAttributes structures
// into a native char index and a native font to draw that char.
// Information on the native fonts available in the system and the actual mapping
// of the glyphs inside them is given externally by means of a configuration XML file.

// some auxiliary functions defined below

static Char parseCode(mDOMNodeRef);

// CharMapper, default constructor
CharMapper::CharMapper(FontManager& fm) : fontManager(fm)
{
}

CharMapper::~CharMapper()
{
  for (Iterator<FontDescriptor*> i(fonts); i.More(); i.Next()) {
    assert(i() != NULL);
    // TODO: more to be deleted...
    delete i();
  }

  for (Iterator<FontMap*> i(maps); i.More(); i.Next()) {
    assert(i() != NULL);
    // TODO: more to be deleted...
    delete i();
  }
}

StretchId
CharMapper::GetStretch(Char ch) const
{
  for (Iterator<FontMap*> fontMap(maps); fontMap.More(); fontMap.Next()) {
    assert(fontMap() != NULL);
    StretchId id = fontMap()->GetStretch(ch);
    if (id != STRETCH_NO) return id;
  }

  return STRETCH_NO;
}

const AFont*
CharMapper::GetFont(const FontAttributes& fa) const
{
  const AFont* font = fontManager.GetFont(fa, NULL);
  if (font != NULL) fontManager.MarkAsUsed(font);
  return font;
}

bool
CharMapper::FontifyChar(FontifiedChar& fMap, const FontAttributes& fa, Char ch) const
{
  bool res = FontifyCharAux(fMap, fa, ch, false);
  if (res) return true;

  if (isPlain(ch)) 
    MathEngine::logger(LOG_WARNING, "could not find a suitable font for `%c = U+%04x'", ch, ch);
  else
    MathEngine::logger(LOG_WARNING, "could not find a suitable font for `U+%04x'", ch);

  res = FontifyCharAux(fMap, fa, '?', false);
  if (!res) {
    MathEngine::logger(LOG_ERROR, "fatal: could not find default fonts, maybe the font configuration is wrong");
    exit(1);
  }

  return false;
}

bool
CharMapper::FontifyStretchyChar(FontifiedChar& fMap, const FontAttributes& fa, Char ch) const
{
  return FontifyCharAux(fMap, fa, ch, true);
}

// FontifyCharAux: this is the main method for this class. It takes a Unicode
// char ch and a FontAttributes structures and tries to fontify the character, i.e.
// it finds a font whose attributes are as close as possible to FA and which is
// capable of render CH. Moreover, it returns a mapping function, i.e. a conversion
// function which takes CH and return an index to the corresponding glyph within
// the selected font.
// If stretchy == TRUE then try to find a font suitable for stretchying the
// given char, even if this could imply a little mismatch between the font
// and the requested attributes
bool
CharMapper::FontifyCharAux(FontifiedChar& fMap, const FontAttributes& fa, Char ch, bool stretchy) const
{
  unsigned bestEval = 0;
  const AFont* bestFont = NULL;
  const CharMap* bestCharMap = NULL;

  FontAttributes myfa(fa);

  do {
    for (Iterator<FontDescriptor*> i(fonts); i.More(); i.Next()) {
      assert(i() != NULL);

      if (i()->fontMap != NULL) {
	unsigned eval = i()->attributes.Compare(myfa);
	const CharMap* charMap = i()->fontMap->GetCharMap(ch, stretchy);

	if (charMap != NULL && eval > bestEval) {
	  const AFont* font = fontManager.GetFont(myfa, &i()->extraAttributes);
	  if (font != NULL) {
	    bestEval = eval;
	    bestFont = font;
	    bestCharMap = charMap;
	  }
	}
      }
    }
  } while (bestFont == NULL && myfa.DownGrade());

  if (bestFont == NULL || bestCharMap == NULL) {
    fMap.charMap = NULL;
    fMap.font = NULL;
    fMap.nch = NULLCHAR;

    return false;
  }

  fontManager.MarkAsUsed(bestFont);

  fMap.charMap = bestCharMap;
  fMap.font = bestFont;
  fMap.nch = bestCharMap->Map(ch);

  return true;
}

StretchId
CharMapper::FontMap::GetStretch(Char ch) const
{
  for (Iterator<CharMap*> charMap(single[CHAR_HASH(ch)]); charMap.More(); charMap.Next()) {
    assert(charMap() != NULL);
    if (charMap()->MapsStretchyChar(ch)) return charMap()->GetStretch();
  }

  return STRETCH_NO;
}

const CharMap*
CharMapper::FontMap::GetCharMap(Char ch, bool stretchy) const
{
  const CharMap* m = NULL;

  for (Iterator<CharMap*> charMap(single[CHAR_HASH(ch)]); m == NULL && charMap.More(); charMap.Next()) {
    assert(charMap() != NULL);
    if ((!stretchy && charMap()->MapsSimpleChar(ch)) ||
	(stretchy && charMap()->MapsStretchyChar(ch))) m = charMap();
  }

  if (m == NULL && !stretchy) {
    for (Iterator<CharMap*> charMap(multi); m == NULL && charMap.More(); charMap.Next()) {
      assert(charMap() != NULL);
      if (charMap()->MapsChar(ch)) m = charMap();
    }
  }

  return m;
}

// Load: loads a configuration file
bool
CharMapper::Load(const char* fileName)
{
  assert(fileName != NULL);

  mDOMDocRef doc = MathMLParseFile(fileName, false);
  if (doc == NULL) return false;

  mDOMNodeRef root = mdom_doc_get_root_node(doc);
  if (root == NULL || mdom_node_get_name(root) == NULL) {
    mdom_doc_free(doc);
    return false;
  }

  if (mdom_string_eq(mdom_node_get_name(root), DOM_CONST_STRING("font-configuration")))
    ParseFontConfiguration(root);
  
  mdom_doc_free(doc);

  return true;
}

void
CharMapper::ParseFontConfiguration(mDOMNodeRef node)
{
  assert(node != NULL);
  // a conf file is made of a single <font-configuration> element

  for (mDOMNodeRef p = mdom_node_get_first_child(node); p != NULL; p = mdom_node_get_next_sibling(p)) {
    // every child of <font-configuration> must be a particular font map
    if      (mdom_string_eq(mdom_node_get_name(p), DOM_CONST_STRING("font"))) ParseFont(p);
    else if (mdom_string_eq(mdom_node_get_name(p), DOM_CONST_STRING("map"))) ParseMap(p);
  }

  PatchConfiguration();
}

void
CharMapper::ParseFont(mDOMNodeRef node)
{
  assert(node != NULL);

  FontDescriptor* desc = new FontDescriptor;
  desc->fontMapId = NULL;
  desc->fontMap = NULL;

  for (mDOMAttrRef attr = mdom_node_get_first_attribute(node);
       attr != NULL;
       attr = mdom_attr_get_next_sibling(attr)) {
    mDOMConstStringRef name = mdom_attr_get_name(attr);
    mDOMStringRef value = mdom_attr_get_value(attr);

    if (value != NULL) {
      if        (mdom_string_eq(name, DOM_CONST_STRING("family"))) {
	desc->attributes.family = C_CONST_STRING(value);
	value = NULL;
      } else if (mdom_string_eq(name, DOM_CONST_STRING("style"))) {
	if      (mdom_string_eq(value, DOM_CONST_STRING("normal")))
	  desc->attributes.style = FONT_STYLE_NORMAL;
	else if (mdom_string_eq(value, DOM_CONST_STRING("italic")))
	  desc->attributes.style = FONT_STYLE_ITALIC;
      } else if (mdom_string_eq(name, DOM_CONST_STRING("weight"))) {
	if (mdom_string_eq(value, DOM_CONST_STRING("normal")))
	  desc->attributes.weight = FONT_WEIGHT_NORMAL;
	else if (mdom_string_eq(value, DOM_CONST_STRING("bold")))
	  desc->attributes.weight = FONT_WEIGHT_BOLD;
      } else if (mdom_string_eq(name, DOM_CONST_STRING("map"))) {
	desc->fontMapId = C_CONST_STRING(value);
	value = NULL;
      } else if (mdom_string_eq(name, DOM_CONST_STRING("mode"))) {
	if      (mdom_string_eq(value, DOM_CONST_STRING("text")))
	  desc->attributes.mode = FONT_MODE_TEXT;
	else if (mdom_string_eq(value, DOM_CONST_STRING("math")))
	  desc->attributes.mode = FONT_MODE_MATH;
      } else if (mdom_string_eq(name, DOM_CONST_STRING("size"))) {
	assert(NOT_IMPLEMENTED);
      } else {
	mDOMStringRef cName = mdom_string_dup(name);
	desc->extraAttributes.AddProperty(C_CONST_STRING(cName), C_CONST_STRING(value));
	value = NULL;
      }

      if (value != NULL) mdom_string_free(value);
    }
  }
  
  if (desc->fontMapId == NULL && desc->attributes.HasFamily())
    desc->fontMapId = strdup(desc->attributes.family);

  if (desc->fontMapId != NULL) fonts.Append(desc);
  else delete desc;
}

void
CharMapper::ParseMap(mDOMNodeRef node)
{
  assert(node != NULL);

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING("id"));
  if (value == NULL) return;

  FontMap* fontMap = new FontMap;
  fontMap->id = C_CONST_STRING(value);

  for (mDOMNodeRef p = mdom_node_get_first_child(node);
       p != NULL;
       p = mdom_node_get_next_sibling(p)) {
    mDOMConstStringRef name = mdom_node_get_name(p);
    if      (mdom_string_eq(name, DOM_CONST_STRING("range"))) ParseRange(p, fontMap);
    else if (mdom_string_eq(name, DOM_CONST_STRING("multi"))) ParseMulti(p, fontMap);
    else if (mdom_string_eq(name, DOM_CONST_STRING("single"))) ParseSingle(p, fontMap);
    else if (mdom_string_eq(name, DOM_CONST_STRING("stretchy"))) ParseStretchy(p, fontMap);
  }

  maps.Append(fontMap);
}

void
CharMapper::ParseRange(mDOMNodeRef node, FontMap* fontMap)
{
  assert(node != NULL);
  assert(fontMap != NULL);

  CharMap* charMap = new CharMap;
  charMap->type = CHAR_MAP_RANGE;

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING("first"));
  if (value == NULL) {
    delete charMap;
    return;
  }
  charMap->range.first = strtol(C_CONST_STRING(value), NULL, 0);
  mdom_string_free(value);

  value = mdom_node_get_attribute(node, DOM_CONST_STRING("last"));
  if (value == NULL) {
    delete charMap;
    return;
  }
  charMap->range.last = strtol(C_CONST_STRING(value), NULL, 0);
  mdom_string_free(value);

  value = mdom_node_get_attribute(node, DOM_CONST_STRING("offset"));
  if (value == NULL) {
    delete charMap;
    return;
  }
  charMap->range.offset = strtol(C_CONST_STRING(value), NULL, 0);
  mdom_string_free(value);

  if (charMap->range.last < charMap->range.first) {
    delete charMap;
    return;
  }

  fontMap->multi.Append(charMap);
}

void
CharMapper::ParseMulti(mDOMNodeRef node, FontMap* fontMap)
{
  assert(node != NULL);
  assert(fontMap != NULL);

  CharMap* charMap = new CharMap;
  charMap->type = CHAR_MAP_MULTI;

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING("first"));
  if (value == NULL) {
    delete charMap;
    return;
  }
  charMap->multi.first = strtol(C_CONST_STRING(value), NULL, 0);
  mdom_string_free(value);

  value = mdom_node_get_attribute(node, DOM_CONST_STRING("last"));
  if (value == NULL) {
    delete charMap;
    return;
  }
  charMap->multi.last = strtol(C_CONST_STRING(value), NULL, 0);
  mdom_string_free(value);

  if (charMap->multi.last < charMap->multi.first) {
    delete charMap;
    return;
  }

  value = mdom_node_get_attribute(node, DOM_CONST_STRING("index"));
  if (value == NULL) {
    delete charMap;
    return;
  }
  charMap->multi.index = new char[charMap->multi.last - charMap->multi.first + 1];

  const char* ptr = C_CONST_STRING(value);
  for (Char ch = charMap->multi.first; ch < charMap->multi.last; ch++) {
    char* newPtr;
    charMap->multi.index[ch - charMap->multi.first] = strtol(ptr, &newPtr, 0);
    ptr = newPtr;
  }
  mdom_string_free(value);

  fontMap->multi.Append(charMap);
}

void
CharMapper::ParseSingle(mDOMNodeRef node, FontMap* fontMap)
{
  assert(node != NULL);
  assert(fontMap != NULL);

  CharMap* charMap = new CharMap;
  charMap->type = CHAR_MAP_SINGLE;

  charMap->single.code = parseCode(node);
  if (charMap->single.code == 0) {
    delete charMap;
    return;
  }

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING("index"));
  if (value == NULL) {
    delete charMap;
    return;
  }
  charMap->single.index = strtol(C_CONST_STRING(value), NULL, 0);
  mdom_string_free(value);

  fontMap->single[CHAR_HASH(charMap->single.code)].Append(charMap);
}

void
CharMapper::ParseStretchy(mDOMNodeRef node, FontMap* fontMap)
{
  assert(node != NULL);
  assert(fontMap != NULL);

  CharMap* charMap = new CharMap;
  charMap->type = CHAR_MAP_STRETCHY;

  for (unsigned i = 0; i < MAX_SIMPLE_CHARS; i++) charMap->stretchy.simple[i] = NULLCHAR;
  for (unsigned i = 0; i < SC_REPEAT + 1; i++) charMap->stretchy.compound[i] = NULLCHAR;

  charMap->stretchy.code = parseCode(node);
  if (charMap->stretchy.code == 0) {
    delete charMap;
    return;
  }

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING("direction"));
  if (value != NULL)
    if      (mdom_string_eq(value, DOM_CONST_STRING("horizontal")))
      charMap->stretchy.direction = STRETCH_HORIZONTAL;
    else if (mdom_string_eq(value, DOM_CONST_STRING("vertical")))
      charMap->stretchy.direction = STRETCH_VERTICAL;
    else if (mdom_string_eq(value, DOM_CONST_STRING("both")))
      charMap->stretchy.direction = STRETCH_BOTH;
    else
      charMap->stretchy.direction = STRETCH_NO;
  else
    charMap->stretchy.direction = STRETCH_NO;

  mdom_string_free(value);

  for (mDOMNodeRef p = mdom_node_get_first_child(node);
       p != NULL;
       p = mdom_node_get_next_sibling(p)) {
    mDOMConstStringRef name = mdom_node_get_name(p);
    if      (mdom_string_eq(name, DOM_CONST_STRING("simple"))) ParseStretchySimple(p, charMap);
    else if (mdom_string_eq(name, DOM_CONST_STRING("compound"))) ParseStretchyCompound(p, charMap);
  }

  fontMap->single[CHAR_HASH(charMap->stretchy.code)].Append(charMap);
}

void
CharMapper::ParseStretchySimple(mDOMNodeRef node, CharMap* charMap)
{
  assert(node != NULL);
  assert(charMap != NULL);

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING("index"));
  if (value == NULL) return;

  const char* ptr = C_CONST_STRING(value);
  for (unsigned i = 0; i < MAX_SIMPLE_CHARS && ptr != NULL && *ptr != '\0'; i++) {
    char* newPtr;
    if (i < 4) charMap->stretchy.simple[i] = strtol(ptr, &newPtr, 0);
    ptr = newPtr;
  }

  mdom_string_free(value);
}

void
CharMapper::ParseStretchyCompound(mDOMNodeRef node, CharMap* charMap)
{
  assert(node != NULL);
  assert(charMap != NULL);

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING("index"));
  if (value == NULL) return;

  const char* ptr = C_CONST_STRING(value);
  for (unsigned i = 0; i < SC_REPEAT + 1 && ptr != NULL && *ptr != '\0'; i++) {
    char* newPtr;
    if (i < SC_REPEAT + 1) charMap->stretchy.compound[i] = strtol(ptr, &newPtr, 0);
    ptr = newPtr;
  }

  mdom_string_free(value);
}

void
CharMapper::PatchConfiguration()
{
  // this method is to patch the font configuration, giving each
  // fontMap its mapping
  for (Iterator<FontDescriptor*> i(fonts); i.More(); i.Next()) {
    assert(i() != NULL);
    assert(i()->fontMapId != NULL);
    i()->fontMap = SearchMapping(i()->fontMapId);
  }
}

const CharMapper::FontMap*
CharMapper::SearchMapping(const char* id) const
{
  assert(id != NULL);

  for (Iterator<FontMap*> i(maps); i.More(); i.Next()) {
    assert(i() != NULL);
    assert(i()->id != NULL);
    if (!strcmp(i()->id, id)) return i();
  }

  return NULL;
}

/////////////////////////
// auxiliary functions //
/////////////////////////

static Char
parseCode(mDOMNodeRef node)
{
  assert(node != NULL);

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING("code"));
  if (value != NULL) {
    Char ch = 0;

    if (*value == '\0') ch = 0;
    else if (*value == '0' && tolower(*(value + 1)) == 'x') ch = strtol(C_CONST_STRING(value), NULL, 0);
    else if (isPlain(*value) && *(value + 1) == '\0') ch = *value;
    else MathEngine::logger(LOG_WARNING, "UTF8 character(s) inside font configuration file (ignored)");
    mdom_string_free(value);

    return ch;
  }

  value = mdom_node_get_attribute(node, DOM_CONST_STRING("name"));
  if (value != NULL) {
    String* s = MathEngine::entitiesTable.GetEntityContent(value);
    
    Char ch = 0;
    
    if (s != NULL && s->GetLength() > 0) {
      assert(s != NULL);
      ch = s->GetChar(0);
      delete s;
    } else
      if (s == NULL) MathEngine::logger(LOG_WARNING, "unknown entity `%s' in font configuration file (ignored)", value);

    mdom_string_free(value);

    return ch;
  }

  return 0;
}
