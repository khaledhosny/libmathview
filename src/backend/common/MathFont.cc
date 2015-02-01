// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include "MathFont.hh"

MathFont::MathFont(const hb_font_t* font)
{
  hb_face_t* face = hb_font_get_face(const_cast<hb_font_t*>(font));
  m_table = hb_face_reference_table(const_cast<hb_face_t*>(face), HB_TAG('M','A','T','H'));
}

MathFont::~MathFont()
{
  hb_blob_destroy(const_cast<hb_blob_t*>(m_table));
}

SmartPtr<MathFont>
MathFont::create(const hb_font_t* font)
{
  return new MathFont(font);
}

int
MathFont::getConstant(MathConstant constant) const
{
  int value = 0;
  const char* tableData = hb_blob_get_data(const_cast<hb_blob_t*>(m_table), nullptr);
  if (tableData)
    {
      const uint16_t* constants = (const uint16_t*)(tableData + SWAP(((const MathTableHeader*)tableData)->mathConstants));
      if (constant < firstMathValueRecord)
        {
          /* it's a simple 16-bit value */
          value =  SWAP(constants[constant]);
        }
      else if (constant <= lastMathValueRecord)
        {
          const MathValueRecord* valueRecords = (const MathValueRecord*)
              ((char*)constants + firstMathValueRecord * sizeof(uint16_t) - firstMathValueRecord * sizeof(MathValueRecord));
          value =  SWAP(valueRecords[constant].value);
        }
      else if (constant <= lastMathConstant)
        {
          value =  SWAP(constants[constant + (lastMathValueRecord - firstMathValueRecord + 1)]);
        }
    }

  return value;
}

static int32_t getCoverage(const Coverage* coverage, GlyphID g)
{
  if (SWAP(coverage->format) == 1)
    {
      const CoverageFormat1 *table = (const CoverageFormat1 *) coverage;
      for (int i = 0; i < SWAP(table->glyphCount); i++)
        {
        if (SWAP(table->glyphArray[i]) == g)
          return i;
        }
    }
  else if (SWAP(coverage->format) == 2)
    {
      const CoverageFormat2 *table = (const CoverageFormat2 *) coverage;
      for (int i = 0; i < SWAP(table->rangeCount); i++)
        {
          if (SWAP(table->rangeArray[i].start) <= g && SWAP(table->rangeArray[i].end) >= g)
            return SWAP(table->rangeArray[i].startCoverageIndex) + (g - SWAP(table->rangeArray[i].start));
        }
    }

  return -1;
}

unsigned
MathFont::getVariant(int glyph, scaled size, bool horiz)
{
  int variant = glyph;

  const char* tableData = hb_blob_get_data(const_cast<hb_blob_t*>(m_table), nullptr);
  if (tableData)
    {
      uint16_t offset = SWAP(((const MathTableHeader*)tableData)->mathVariants);
      if (offset != 0)
        {
          const MathVariants* variants = (const MathVariants*)(tableData + offset);

          if (horiz)
            offset = SWAP(variants->horizGlyphCoverage);
          else
            offset = SWAP(variants->vertGlyphCoverage);

          if (offset != 0)
            {
              const Coverage* coverage = (const Coverage*)(((const char*)variants) + offset);

              int32_t index = getCoverage(coverage, glyph);
              if (index >= 0)
                {
                  if (horiz)
                    index += SWAP(variants->vertGlyphCount);
                  const MathGlyphConstruction* construction = (const MathGlyphConstruction*)(((const char*)variants)
                                                              + SWAP(variants->vertGlyphConstruction[index]));
                  for (int i = 0; i < SWAP(construction->variantCount); i++)
                    {
                      variant = SWAP(construction->mathGlyphVariantRecord[i].variantGlyph);
                      int adv = SWAP(construction->mathGlyphVariantRecord[i].advanceMeasurement);
                      if (adv > size.toInt())
                        break;
                    }
                }
            }
        }
    }

  return variant;
}
