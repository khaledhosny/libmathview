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

#ifndef CharMapper_hh
#define CharMapper_hh

#if defined(HAVE_MINIDOM)
#include "minidom.h"
#elif defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "CharMap.hh"
#include "FontAttributes.hh"

#define CHAR_MAP_HASH_BITS       4
#define CHAR_MAP_HASH_TABLE_SIZE (1 << CHAR_MAP_HASH_BITS)
#define CHAR_MAP_HASH_MASK       (CHAR_MAP_HASH_TABLE_SIZE - 1)
#define CHAR_HASH(ch)            ((ch) & CHAR_MAP_HASH_MASK)

class CharMapper
{
public:
  CharMapper(class FontManager&);
  ~CharMapper();

  bool Load(const char*);
  StretchId GetStretch(Char) const;
  const AFont* GetFont(const FontAttributes&) const;
  bool FontifyChar(FontifiedChar&, const FontAttributes&, Char) const;
  bool FontifyStretchyChar(FontifiedChar&, const FontAttributes&, Char) const;

#ifdef ENABLE_PROFILE
  static unsigned GetChars(void) { return chars; }
  static unsigned GetAlnumChars(void) { return alnumChars; }
#endif // ENABLE_PROFILE

private:
  struct FontMap {
    const char* id;
    Container<CharMap*> multi; // actually, RANGE and MULTI maps
    Container<CharMap*> single[CHAR_MAP_HASH_TABLE_SIZE]; // actually, SINGLE and STRETCHY

    StretchId GetStretch(Char) const;
    const CharMap* GetCharMap(Char, bool = false) const;
  };

  struct FontDescriptor {
    FontAttributes      attributes;
    ExtraFontAttributes extraAttributes;

    const char*         fontMapId;
    const FontMap*      fontMap;
  };

  bool FontifyCharAux(FontifiedChar&, const FontAttributes&, Char, bool = false) const;

#if defined(HAVE_MINIDOM)
  void ParseFontConfiguration(mDOMNodeRef);
  void ParseFont(mDOMNodeRef);
  void ParseMap(mDOMNodeRef);
  void ParseRange(mDOMNodeRef, FontMap*);
  void ParseMulti(mDOMNodeRef, FontMap*);
  void ParseSingle(mDOMNodeRef, FontMap*);
  void ParseStretchy(mDOMNodeRef, FontMap*);
  void ParseStretchySimple(mDOMNodeRef, CharMap*);
  void ParseStretchyCompound(mDOMNodeRef, CharMap*);
#elif defined(HAVE_GMETADOM)
  void ParseFontConfiguration(const GMetaDOM::Element&);
  void ParseFont(const GMetaDOM::Element&);
  void ParseMap(const GMetaDOM::Element&);
  void ParseRange(const GMetaDOM::Element&, FontMap*);
  void ParseMulti(const GMetaDOM::Element&, FontMap*);
  void ParseSingle(const GMetaDOM::Element&, FontMap*);
  void ParseStretchy(const GMetaDOM::Element&, FontMap*);
  void ParseStretchySimple(const GMetaDOM::Element&, CharMap*);
  void ParseStretchyCompound(const GMetaDOM::Element&, CharMap*);
#endif // HAVE_GMETADOM

  void PatchConfiguration(void);
  const FontMap* SearchMapping(const char*) const;

  Container<FontDescriptor*> fonts;
  Container<FontMap*>        maps;

  class FontManager& fontManager;

#ifdef ENABLE_PROFILE
  static unsigned alnumChars;
  static unsigned chars;
#endif // ENABLE_PROFILE
};

#endif // CharMapper_hh
 
