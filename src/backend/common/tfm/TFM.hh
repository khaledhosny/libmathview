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

#ifndef __TFM_hh__
#define __TFM_hh__

#include "Object.hh"
#include "SmartPtr.hh"
#include "scaled.hh"
#include "Char.hh"
#include "String.hh"
#include "BoundingBox.hh"

class TFM : public Object
{
public:
  enum { UNITY_SHIFT = 20 };

  struct Font
  {
    const char* family;
    unsigned char face;
    const char* codingScheme;
    int designSize;
    int checksum;
    unsigned int nDimensions;
    unsigned int nCharacters;
  };

  struct Dimension
  {
    unsigned index;
    const char* name;
    int value;
  };

  struct Kerning
  {
    UChar8 index;
    int value;
  };

  struct Ligature
  {
    UChar8 index;
    UChar8 mode;
    UChar8 result;
  };

  struct Character
  {
    UChar8 index;
    int width;
    int height;
    int depth;
    int italicCorrection;
    unsigned char nKernings;
    const Kerning* kerning;
    unsigned char nLigatures;
    const Ligature* ligature;
  };

protected:
  TFM(const String&, const Font*, const Dimension*, const Character*);
  virtual ~TFM();

public:
  static SmartPtr<TFM>
  create(const String& _name, const Font* _font, const Dimension* _dimension, const Character* _character)
  { return new TFM(_name, _font, _dimension, _character); }

  String getName(void) const { return name; }
  String getFamily(void) const { return font->family; }
  unsigned char getFace(void) const { return font->face; }
  String getCodingScheme(void) const { return font->codingScheme; }
  scaled getDesignSize(void) const;

  String getDimensionName(unsigned) const;
  scaled getDimension(unsigned) const;
  scaled getDimension(const String&) const;

  BoundingBox getGlyphBoundingBox(UChar8) const;
  scaled getGlyphItalicCorrection(UChar8) const;
  bool getGlyphKerning(UChar8, UChar8, scaled&) const;
  bool getGlyphLigature(UChar8, UChar8, UChar8&, UChar8&) const;

  float getScale(const scaled&) const;

protected:
  static scaled scaledOfFIX(int);
  const Character& getCharacter(UChar8) const;

private:
  const String name;
  const Font* font;
  const Dimension* dimension;
  const Character* character;
};

#endif // __TFM_hh__

