// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
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
#include <hb.h>

#include <vector>

#include "Area.hh"
#include "AreaFactory.hh"
#include "MathShaper.hh"
#include "ShapingContext.hh"

MathShaper::MathShaper(const hb_font_t* font)
  : m_font(font)
{ }

MathShaper::~MathShaper()
{ }

void
MathShaper::shape(ShapingContext& context) const
{
  hb_font_t* font = const_cast<hb_font_t*>(m_font);
  hb_face_t* face = hb_font_get_face(font);
  int upem = hb_face_get_upem(face);

  std::u32string source = context.getSource();
  hb_buffer_t* buffer = hb_buffer_create();

  hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
  hb_buffer_set_script(buffer, hb_script_from_string("Math", -1));
  hb_buffer_add_utf32(buffer, (const uint32_t*) source.data(), source.length(), 0, source.length());

  unsigned scriptLevel = context.getScriptLevel();
  if (scriptLevel > 0)
    {
      hb_feature_t features[] = {
        { HB_TAG('s','s','t','y'), scriptLevel, 0, (unsigned)-1 },
        { 0, 0, 0, 0 }
      };
      hb_shape(font, buffer, features, 1);
    }
  else
    {
      hb_shape(font, buffer, nullptr, 0);
    }

  unsigned len = hb_buffer_get_length(buffer);
  hb_glyph_info_t* glyphs = hb_buffer_get_glyph_infos(buffer, nullptr);

  const SmartPtr<AreaFactory> factory = context.getFactory();
  AreaRef area;

  if (len == 1)
    {
      hb_codepoint_t glyphId = glyphs[0].codepoint;
      AreaRef glyphArea = getGlyphArea(glyphId, context.getSize());

      if (glyphArea->box().verticalExtent() < context.getVSpan())
        {
          scaled span = (context.getVSpan() * upem).getValue() / context.getSize().getValue();
          hb_ot_shape_math_stretchy(font, buffer, false, span.toInt());
          unsigned stretchyLen = hb_buffer_get_length(buffer);
          hb_glyph_info_t* stretchyGlyphs = hb_buffer_get_glyph_infos(buffer, nullptr);
          hb_glyph_position_t* stretchyPositions = hb_buffer_get_glyph_positions(buffer, nullptr);
          std::vector<AreaRef> areas;
          for (unsigned int i = 0; i < stretchyLen; i++)
            {
              hb_codepoint_t id = stretchyGlyphs[i].codepoint;
              hb_position_t advance = stretchyPositions[i].y_advance;
              areas.push_back(getGlyphArea(id, context.getSize()));
            }
          area = factory->verticalArray(areas, 0);
        }
      else if (glyphArea->box().horizontalExtent() < context.getHSpan())
        {
          scaled span = (context.getHSpan() * upem).getValue() / context.getSize().getValue();
          hb_ot_shape_math_stretchy(font, buffer, true, span.toInt());
          unsigned stretchyLen = hb_buffer_get_length(buffer);
          hb_glyph_info_t * stretchyGlyphs = hb_buffer_get_glyph_infos(buffer, nullptr);
          hb_glyph_position_t* stretchyPositions = hb_buffer_get_glyph_positions(buffer, nullptr);
          std::vector<AreaRef> areas;
          for (unsigned int i = 0; i < stretchyLen; i++)
            {
              hb_codepoint_t id = stretchyGlyphs[i].codepoint;
              hb_position_t advance = stretchyPositions[i].x_advance;
              areas.push_back(getGlyphArea(id, context.getSize()));
            }
          area = factory->horizontalArray(areas);
        }
      else
        {
          area = glyphArea;
        }
    }
  else
    {
      std::vector<AreaRef> areas;
      for (unsigned int i = 0; i < len; i++)
        {
          hb_codepoint_t id = glyphs[i].codepoint;
          AreaRef area = getGlyphArea(id, context.getSize());
          areas.push_back(area);
        }
      area = factory->horizontalArray(areas);
    }

  context.pushArea(source.length(), area);

  hb_buffer_destroy(buffer);
}

bool
MathShaper::shapeCombiningChar(const ShapingContext&) const
{
  return false;
}

bool
MathShaper::computeCombiningCharOffsetsAbove(const AreaRef& base,
                                             const AreaRef& script,
                                             scaled& dx,
                                             scaled& dy) const
{
  //default value of dx and dy
  dx = (base->box().width - script->box().width) / 2;
  dy = base->box().height + script->box().depth;

  return true;
}

bool
MathShaper::computeCombiningCharOffsetsBelow(const AreaRef& base,
                                             const AreaRef& script,
                                             scaled& dxUnder) const
{
  dxUnder = (base->box().width - script->box().width) / 2;

  return true;
}
