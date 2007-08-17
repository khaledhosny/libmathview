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

#include "TFM.hh"

scaled
TFM::scaledOfFIX(int v)
{ return scaled(v >> (UNITY_SHIFT - scaled::getPrecision()), true); }

TFM::TFM(const String& _name, const Font* _font, const Dimension* _dimension, const Character* _character)
  : name(_name), font(_font), dimension(_dimension), character(_character)
{
  assert(font);
  assert(dimension);
  assert(character);
  assert(UNITY_SHIFT >= scaled::getPrecision());
  assert(sizeof(int) >= 4);
}

TFM::~TFM()
{ }

scaled
TFM::getDesignSize() const
{ return scaledOfFIX(font->designSize); }

String
TFM::getDimensionName(unsigned index) const
{
  assert(index >= 1 && index <= font->nDimensions);
  assert(dimension[index - 1].index == index);
  return dimension[index - 1].name;
#if 0
  for (int i = 0; i < font->nDimensions; i++)
    if (dimension[i].index == index)
      return dimension[i].name;
  return "";
#endif
}

scaled
TFM::getDimension(unsigned index) const
{
  assert(index >= 1 && index <= font->nDimensions);
  assert(dimension[index - 1].index == index);
  return scaledOfFIX(dimension[index - 1].value);
#if 0
  for (unsigned i = 0; i < font->nDimensions; i++)
    if (dimension[i].index == index)
      return scaledOfFIX(dimension[i].value);
  return scaled();
#endif
}

scaled
TFM::getDimension(const String& name) const
{
  for (unsigned i = 0; i < font->nDimensions; i++)
    if (name == dimension[i].name)
      return scaledOfFIX(dimension[i].value);
  return scaled();
}

const TFM::Character&
TFM::getCharacter(UChar8 index) const
{
  assert(index < font->nCharacters);
  assert(character[index].index == index);
  return character[index];
}

BoundingBox
TFM::getGlyphBoundingBox(UChar8 index) const
{
  const Character& c = getCharacter(index);
  return BoundingBox(scaledOfFIX(c.width), scaledOfFIX(c.height), scaledOfFIX(c.depth));
}

scaled
TFM::getGlyphItalicCorrection(UChar8 index) const
{ return scaledOfFIX(getCharacter(index).italicCorrection); }

bool
TFM::getGlyphKerning(UChar8 index1, UChar8 index2, scaled& result) const
{
  const Character& c = getCharacter(index1);
  for (unsigned i = 0; i < c.nKernings; i++)
    if (c.kerning[i].index == index2)
      {
	result = scaledOfFIX(c.kerning[i].value);
	return true;
      }
  return false;
}

bool
TFM::getGlyphLigature(UChar8 index1, UChar8 index2, UChar8& result, UChar8& mode) const
{
  const Character& c = getCharacter(index1);
  for (unsigned i = 0; i < c.nLigatures; i++)
    if (c.ligature[i].index == index2)
      {
	result = c.ligature[i].result;
	mode = c.ligature[i].mode;
	return true;
      }
  return false;
}

float
TFM::getScale(const scaled& size) const
{ return size.toFloat(); }
