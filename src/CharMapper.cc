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
#include <gdome.h>

#include "gdomeAux.h"
#include "Iterator.hh"
#include "stringAux.hh"
#include "MathEngine.hh"
#include "CharMapper.hh"
#include "FontManager.hh"
#include "StringUnicode.hh"
#include "AttributeParser.hh"
#include "MathMLParseFile.hh"

#ifdef ENABLE_PROFILE
unsigned CharMapper::alnumChars = 0;
unsigned CharMapper::chars = 0;
#endif // ENABLE_PROFILE

// CharMapper: this class is used to map a Unicode char and a FontAttributes structures
// into a native char index and a native font to draw that char.
// Information on the native fonts available in the system and the actual mapping
// of the glyphs inside them is given externally by means of a configuration XML file.

// some auxiliary functions defined below

static Char parseCode(GdomeElement*);

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
  unsigned bestEval = 0;
  const AFont* bestFont = NULL;
  FontDescriptor* bestDesc = NULL;

  FontAttributes myfa(fa);

  do {
    for (Iterator<FontDescriptor*> i(fonts); i.More() && bestEval > 0; i.Next()) {
      assert(i() != NULL);

      if (i()->fontMap != NULL) {
	unsigned eval = i()->attributes.Compare(myfa);
	if (eval < bestEval && fontManager.IsAvailable(myfa, &i()->extraAttributes)) {
	  bestEval = eval;
	  bestDesc = i();
	}
      }
    }

    if (bestDesc != NULL) bestFont = fontManager.GetFont(myfa, &bestDesc->extraAttributes);
  } while (bestFont == NULL && myfa.DownGrade());

  if (bestFont != NULL) fontManager.MarkAsUsed(bestFont);

  return bestFont;
}

bool
CharMapper::FontifyChar(FontifiedChar& fMap, const FontAttributes& fa, Char ch) const
{
#ifdef ENABLE_PROFILE
  chars++;
  if (isalnum(ch)) alnumChars++;
#endif // ENABLE_PROFILE
  
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
  unsigned bestEval = UINT_MAX;
  const AFont* bestFont = NULL;
  const CharMap* bestCharMap = NULL;
  FontDescriptor* bestDesc = NULL;

  FontAttributes myfa(fa);
#if 0
  MathEngine::logger(LOG_DEBUG, "requested font attributes:");
  myfa.Dump();
#endif

  do {
    for (Iterator<FontDescriptor*> i(fonts); i.More() && bestEval > 0; i.Next()) {
      assert(i() != NULL);

      // NOTE: the order of the following tests is very important for
      // performances. Basically, there are 3 tests to be done:
      // 1) whether this font can render the requested char
      // 2) whether this font is better than a previously found font
      // 3) whether this font is available with the current font manager
      // The order for the tests should be choosen with the most-likely-to-fail
      // test first, and so on for the other. This must be weighted in order to consider
      // the computational cost for performing the test. For example, the
      // charMap is a very cheap test and it is likely to fail on a pretty wide range
      // of fonts, especially for mathematical symbols. By contrast, `eval' has
      // a complex procedure to be computed, but once a good font is found, then all
      // the other are not considered any more. Finally, the availability of the
      // font is a call to a virtual function.
      if (i()->fontMap != NULL) {
	const CharMap* charMap = i()->fontMap->GetCharMap(ch, stretchy);
	if (charMap != NULL) {
	  unsigned eval = i()->attributes.Compare(myfa);
	  if (eval < bestEval && fontManager.IsAvailable(myfa, &i()->extraAttributes)) {
	    bestEval = eval;
	    bestCharMap = charMap;
	    bestDesc = i();
	  }
	}
      }
    }

    if (bestDesc != NULL) bestFont = fontManager.GetFont(myfa, &bestDesc->extraAttributes);
  } while (bestFont == NULL && myfa.DownGrade());

  //MathEngine::logger(LOG_DEBUG, "attributes:");
  //myfa.Dump();

  if (bestFont == NULL || bestCharMap == NULL) {
    fMap.charMap = NULL;
    fMap.font = NULL;
    fMap.nch = NULLCHAR;

    return false;
  }

  // with the current implementation, the following operation seems to be
  // useless, since only needed fonts are actually loaded.
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

  GdomeException exc;
  GdomeDocument* doc = MathMLParseFile(fileName, false);
  if (doc == NULL) return false;

  GdomeElement* root = gdome_doc_documentElement(doc, &exc);
  if (root == NULL) {
    gdome_doc_unref(doc, &exc);
    return false;
  }

  if (gdome_n_name_is(GDOME_N(root), "font-configuration"))
    ParseFontConfiguration(root);
  
  gdome_doc_unref(doc, &exc);

  return true;
}

void
CharMapper::ParseFontConfiguration(GdomeElement* node)
{
  assert(node != NULL);
  // a conf file is made of a single <font-configuration> element

  GdomeException exc;

  for (GdomeNode* p = gdome_el_firstChild(node, &exc);
       p != NULL;
       p = gdome_n_nextSibling_unref(p)) {
    // every child of <font-configuration> must be a particular font map
    if (gdome_n_name_is(p, "font")) ParseFont(GDOME_EL(p));
    else if (gdome_n_name_is(p, "map")) ParseMap(GDOME_EL(p));
  }

  // this is to link fonts to maps, since they can occur in any order within the
  // font configuration file
  PatchConfiguration();
}

void
CharMapper::ParseFont(GdomeElement* node)
{
  assert(node != NULL);

  FontDescriptor* desc = new FontDescriptor;
  desc->fontMapId = NULL;
  desc->fontMap = NULL;

  GdomeException exc;
  GdomeNamedNodeMap* attributes = gdome_el_attributes(node, &exc);

  for (unsigned i = 0; i < gdome_nnm_length(attributes, &exc); i++) {
    GdomeAttr* attr = GDOME_A(gdome_nnm_item(attributes, i, &exc));

    GdomeDOMString* name = gdome_a_name(attr, &exc);
    GdomeDOMString* value = gdome_a_value(attr, &exc);

    assert(name != NULL);
    assert(value != NULL);

    if (gdome_str_equal_c(name, "family")) {
      gdome_str_ref(value);
      desc->attributes.family = gdome_str_c(value);
    } else if (gdome_str_equal_c(name, "style")) {
      if      (gdome_str_equal_c(value, "normal"))
	desc->attributes.style = FONT_STYLE_NORMAL;
      else if (gdome_str_equal_c(value, "italic"))
	desc->attributes.style = FONT_STYLE_ITALIC;
    } else if (gdome_str_equal_c(name, "weight")) {
      if (gdome_str_equal_c(value, "normal"))
	desc->attributes.weight = FONT_WEIGHT_NORMAL;
      else if (gdome_str_equal_c(value, "bold"))
	desc->attributes.weight = FONT_WEIGHT_BOLD;
    } else if (gdome_str_equal_c(name, "map")) {
      gdome_str_ref(value);
      desc->fontMapId = gdome_str_c(value);
    } else if (gdome_str_equal_c(name, "mode")) {
      if      (gdome_str_equal_c(value, "text"))
	desc->attributes.mode = FONT_MODE_TEXT;
      else if (gdome_str_equal_c(value, "math"))
	desc->attributes.mode = FONT_MODE_MATH;
    } else if (gdome_str_equal_c(name, "size")) {
      StringC sName(gdome_str_c(value));
      StringTokenizer st(sName);
      const Value* v = numberUnitParser(st);
      if (v != NULL) {
	desc->attributes.size = v->ToNumberUnit();
	delete v;
      }
    } else {
      gdome_str_ref(name);
      gdome_str_ref(value);
      desc->extraAttributes.AddProperty(gdome_str_c(name), gdome_str_c(value));
    }

    gdome_str_unref(name);
    gdome_str_unref(value);
  }

  gdome_nnm_unref(attributes, &exc);

  if (desc->fontMapId != NULL) fonts.Append(desc);
  else delete desc;
}

void
CharMapper::ParseMap(GdomeElement* node)
{
  assert(node != NULL);

  if (!gdome_el_hasAttribute_c(node, "id")) {
    MathEngine::logger(LOG_WARNING, "there is a map without `id' attribute in font configuration file (skipped)");
    return;
  }

  GdomeDOMString* value = gdome_el_getAttribute_c(node, "id");
  assert(value != NULL);

  FontMap* fontMap = new FontMap;
  fontMap->id = gdome_str_c(value);

  GdomeException exc;

  for (GdomeNode* p = gdome_el_firstChild(node, &exc);
       p != NULL;
       p = gdome_n_nextSibling_unref(p)) {
    if      (gdome_n_name_is(p, "range")) ParseRange(GDOME_EL(p), fontMap);
    else if (gdome_n_name_is(p, "multi")) ParseMulti(GDOME_EL(p), fontMap);
    else if (gdome_n_name_is(p, "single")) ParseSingle(GDOME_EL(p), fontMap);
    else if (gdome_n_name_is(p, "stretchy")) ParseStretchy(GDOME_EL(p), fontMap);
    else if (!gdome_n_isBlank(p)) {
      GdomeDOMString* name = gdome_n_nodeName(p, &exc);
      assert(name != NULL);

      MathEngine::logger(LOG_WARNING, "unrecognized element `%s' in font configuration (ignored)",
			 gdome_str_c(name));

      gdome_str_unref(name);
    }
  }

  maps.Append(fontMap);
}

void
CharMapper::ParseRange(GdomeElement* node, FontMap* fontMap)
{
  assert(node != NULL);
  assert(fontMap != NULL);

  CharMap* charMap = new CharMap;
  charMap->type = CHAR_MAP_RANGE;

  if (!gdome_el_hasAttribute_c(node, "first")) {
    MathEngine::logger(LOG_WARNING, "range map has no mandatory `first' attribute");
    delete charMap;
    return;
  }
  
  GdomeDOMString* value = gdome_el_getAttribute_c(node, "first");
  charMap->range.first = strtol(gdome_str_c(value), NULL, 0);
  gdome_str_unref(value);

  if (!gdome_el_hasAttribute_c(node, "last")) {
    MathEngine::logger(LOG_WARNING, "range map has no mandatory `last' attribute");
    delete charMap;
    return;
  }

  value = gdome_el_getAttribute_c(node, "last");
  charMap->range.last = strtol(gdome_str_c(value), NULL, 0);
  gdome_str_unref(value);

  if (!gdome_el_hasAttribute_c(node, "offset")) {
    MathEngine::logger(LOG_WARNING, "range map has no mandatory `offset' attribute");
    delete charMap;
    return;
  }

  value = gdome_el_getAttribute_c(node, "offset");
  charMap->range.offset = strtol(gdome_str_c(value), NULL, 0);
  gdome_str_unref(value);

  if (charMap->range.last < charMap->range.first) {
    MathEngine::logger(LOG_WARNING, "range map has inconsistent first-last entries");
    delete charMap;
    return;
  }

  fontMap->multi.Append(charMap);
}

void
CharMapper::ParseMulti(GdomeElement* node, FontMap* fontMap)
{
  assert(node != NULL);
  assert(fontMap != NULL);

  CharMap* charMap = new CharMap;
  charMap->type = CHAR_MAP_MULTI;

  if (!gdome_el_hasAttribute_c(node, "first")) {
    MathEngine::logger(LOG_WARNING, "multi map has no mandatory `first' attribute");
    delete charMap;
    return;
  }

  GdomeDOMString* value = gdome_el_getAttribute_c(node, "first");
  charMap->multi.first = strtol(gdome_str_c(value), NULL, 0);
  gdome_str_unref(value);

  if (!gdome_el_hasAttribute_c(node, "last")) {
    MathEngine::logger(LOG_WARNING, "multi map has no mandatory `last' attribute");
    delete charMap;
    return;
  }

  value = gdome_el_getAttribute_c(node, "last");
  charMap->multi.last = strtol(gdome_str_c(value), NULL, 0);
  gdome_str_unref(value);

  if (charMap->multi.last < charMap->multi.first) {
    MathEngine::logger(LOG_WARNING, "multi map has inconsistent first-last entries");
    delete charMap;
    return;
  }

  if (!gdome_el_hasAttribute_c(node, "index")) {
    MathEngine::logger(LOG_WARNING, "multi map has no mandatory `index' attribute");
    delete charMap;
    return;
  }
  charMap->multi.index = new char[charMap->multi.last - charMap->multi.first + 1];

  value = gdome_el_getAttribute_c(node, "index");
  const char* ptr = gdome_str_c(value);
  for (Char ch = charMap->multi.first; ch < charMap->multi.last; ch++) {
    char* newPtr;
    charMap->multi.index[ch - charMap->multi.first] = strtol(ptr, &newPtr, 0);
    ptr = newPtr;
  }
  gdome_str_unref(value);

  fontMap->multi.Append(charMap);
}

void
CharMapper::ParseSingle(GdomeElement* node, FontMap* fontMap)
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

  if (!gdome_el_hasAttribute_c(node, "index")) {
    MathEngine::logger(LOG_WARNING, "single map has no mandatory `index' attribute");
    delete charMap;
    return;
  }

  GdomeDOMString* value = gdome_el_getAttribute_c(node, "index");
  charMap->single.index = strtol(gdome_str_c(value), NULL, 0);
  gdome_str_unref(value);

  fontMap->single[CHAR_HASH(charMap->single.code)].Append(charMap);
}

void
CharMapper::ParseStretchy(GdomeElement* node, FontMap* fontMap)
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

  if (gdome_el_hasAttribute_c(node, "direction")) {
    GdomeDOMString* value = gdome_el_getAttribute_c(node, "direction");
    assert(value != NULL);

    if      (gdome_str_equal_c(value, "horizontal"))
      charMap->stretchy.direction = STRETCH_HORIZONTAL;
    else if (gdome_str_equal_c(value, "vertical"))
      charMap->stretchy.direction = STRETCH_VERTICAL;
    else if (gdome_str_equal_c(value, "both"))
      charMap->stretchy.direction = STRETCH_BOTH;
    else
      charMap->stretchy.direction = STRETCH_NO;

    gdome_str_unref(value);
  } else
    charMap->stretchy.direction = STRETCH_NO;

  GdomeException exc;

  for (GdomeNode* p = gdome_el_firstChild(node, &exc);
       p != NULL;
       p = gdome_n_nextSibling_unref(p)) {
    if      (gdome_n_name_is(p, "simple")) ParseStretchySimple(GDOME_EL(p), charMap);
    else if (gdome_n_name_is(p, "compound")) ParseStretchyCompound(GDOME_EL(p), charMap);
  }

  fontMap->single[CHAR_HASH(charMap->stretchy.code)].Append(charMap);
}

void
CharMapper::ParseStretchySimple(GdomeElement* node, CharMap* charMap)
{
  assert(node != NULL);
  assert(charMap != NULL);

  if (!gdome_el_hasAttribute_c(node, "index")) return;

  GdomeDOMString* value = gdome_el_getAttribute_c(node, "index");
  assert(value != NULL);

  const char* ptr = gdome_str_c(value);
  for (unsigned i = 0; i < MAX_SIMPLE_CHARS && ptr != NULL && *ptr != '\0'; i++) {
    char* newPtr;
    if (i < 4) charMap->stretchy.simple[i] = strtol(ptr, &newPtr, 0);
    ptr = newPtr;
  }

  gdome_str_unref(value);
}

void
CharMapper::ParseStretchyCompound(GdomeElement* node, CharMap* charMap)
{
  assert(node != NULL);
  assert(charMap != NULL);

  if (!gdome_el_hasAttribute_c(node, "index")) return;

  GdomeDOMString* value = gdome_el_getAttribute_c(node, "index");
  assert(value != NULL);

  const char* ptr = gdome_str_c(value);
  for (unsigned i = 0; i < SC_REPEAT + 1 && ptr != NULL && *ptr != '\0'; i++) {
    char* newPtr;
    if (i < SC_REPEAT + 1) charMap->stretchy.compound[i] = strtol(ptr, &newPtr, 0);
    ptr = newPtr;
  }

  gdome_str_unref(value);
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
parseCode(GdomeElement* node)
{
  assert(node != NULL);

  if (gdome_el_hasAttribute_c(node, "code")) {
    GdomeDOMString* value = gdome_el_getAttribute_c(node, "code");
    assert(value != NULL);

    const char* c_value = gdome_str_c(value);
    assert(c_value != NULL);

    Char ch = 0;

    if (*c_value == '\0') ch = 0;
    else if (*c_value == '0' && tolower(*(c_value + 1)) == 'x') ch = strtol(c_value, NULL, 0);
    else if (isPlain(*c_value) && *(c_value + 1) == '\0') ch = *c_value;
    else MathEngine::logger(LOG_WARNING, "UTF8 character(s) inside font configuration file (ignored)");
    gdome_str_unref(value);

    return ch;
  }

  return 0;
}
