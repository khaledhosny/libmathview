// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

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
    Char8 index;
    int value;
  };

  struct Ligature
  {
    Char8 index;
    Char8 mode;
    Char8 result;
  };

  struct Character
  {
    Char8 index;
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

  BoundingBox getGlyphBoundingBox(Char8) const;
  scaled getGlyphItalicCorrection(Char8) const;
  bool getGlyphKerning(Char8, Char8, scaled&) const;
  bool getGlyphLigature(Char8, Char8, Char8&, Char8&) const;

  float getScale(const scaled&) const;

protected:
  static scaled scaledOfFIX(int);
  const Character& getCharacter(Char8) const;

private:
  const String name;
  const Font* font;
  const Dimension* dimension;
  const Character* character;
};

#endif // __TFM_hh__

