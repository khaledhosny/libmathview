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
#include <hb-ft.h>

#include <vector>

#include "Area.hh"
#include "MathShaper.hh"
#include "ShapingContext.hh"

enum class VariationSelector : Char32 {
  ONE = 0XFE00
};

MathShaper::MathShaper(const hb_font_t* font)
  : m_font(font)
{
  m_mathfont = MathFont::create(font);
}

MathShaper::~MathShaper()
{ }

unsigned
MathShaper::shapeChar(Char32 ch, Char32 vs) const
{
  hb_codepoint_t glyph;
  hb_font_get_glyph(const_cast<hb_font_t*>(m_font), ch, vs, &glyph);
  return glyph;
}

void
MathShaper::shape(ShapingContext& context) const
{
  hb_face_t* face = hb_font_get_face(const_cast<hb_font_t*>(m_font));
  int upem = hb_face_get_upem(face);
  for (unsigned i = context.chunkSize(); i > 0;)
    {
      unsigned glyphId, variantId, n;
      if (context.nextChar() == static_cast<Char32>(VariationSelector::ONE))
        {
          glyphId = shapeChar(context.thisChar(), context.nextChar());
          n = 2;
        }
      else
        {
          glyphId = shapeChar(context.thisChar());
          n = 1;
        }

      AreaRef glyphArea = getGlyphArea(glyphId, context.getSize());
      variantId = glyphId;

      if (glyphArea->box().verticalExtent() < context.getVSpan())
        {
          scaled span = (context.getVSpan() * upem).getValue() / context.getSize().getValue();
          variantId = m_mathfont->getVariant(variantId, span, false);
        }
      if (glyphArea->box().horizontalExtent() < context.getHSpan())
        {
          scaled span = (context.getHSpan() * upem).getValue() / context.getSize().getValue();
          variantId = m_mathfont->getVariant(variantId, span, true);
        }

      if (variantId != glyphId)
        glyphArea = getGlyphArea(variantId, context.getSize());

      context.pushArea(n, glyphArea);
      i -= n;
    }
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
