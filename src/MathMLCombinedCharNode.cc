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

#include "CharMapper.hh"
#include "MathEngine.hh"
#include "MathMLElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLCombinedCharNode.hh"

MathMLCombinedCharNode::MathMLCombinedCharNode(Char c, Char cc) :
  MathMLCharNode(c)
{
  cch = cc;
}

MathMLCombinedCharNode::~MathMLCombinedCharNode()
{
}

void
MathMLCombinedCharNode::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  MathMLCharNode::Setup(env);
  assert(fChar.charMap != NULL);

  if (!env->charMapper.FontifyChar(cChar, env->GetFontAttributes(), cch)) {
    cChar.font = NULL;
    cChar.charMap = NULL;
  }

  if (cChar.font == NULL)
    MathEngine::logger(LOG_WARNING, "not able to render combining char `U+%04x'", cch);
  else if (cChar.font != fChar.font)
    MathEngine::logger(LOG_WARNING, "base char `U+%04x' and combining char `U+%04x' use different fonts", ch, cch);
}

void
MathMLCombinedCharNode::DoLayout()
{
  MathMLCharNode::DoLayout();

  if (cChar.font != NULL && cChar.charMap != NULL) {
    BoundingBox cBox;
    cChar.GetBoundingBox(cBox);

#if 0
    if (cChar.font == fChar.font)
      shiftX = cChar.font->GetKerning(cChar.nch, fChar.nch);
    else
#endif
      shiftX = 0;

    if (isCombiningOverlay(cch))
      shiftY = 0;
    else
      shiftY = box.ascent + cBox.descent + cChar.font->GetLineThickness();

    charBox.ascent = scaledMax(box.ascent, cBox.ascent + shiftY);
    charBox.descent = scaledMax(box.descent, cBox.descent - shiftY);
    charBox.tAscent = scaledMax(box.tAscent, cBox.tAscent + shiftY);
    charBox.tDescent = scaledMax(box.tDescent, cBox.tDescent - shiftY);
    charBox.width = scaledMax(box.width, cBox.width + scaledAbs(shiftX));
    charBox.lBearing = scaledMin(box.lBearing, cBox.lBearing + shiftX);
    charBox.rBearing = scaledMax(box.rBearing, cBox.rBearing + shiftX);
    box = charBox;
  }
}

void
MathMLCombinedCharNode::Render(const DrawingArea& area)
{
  MathMLCharNode::Render(area);

  if (cChar.font != NULL) {
    assert(GetParent() != NULL);
    const GraphicsContext* gc = GetParent()->GetForegroundGC();

    area.DrawChar(gc, cChar.font, GetX() + shiftX, GetY() - shiftY, cChar.nch);
  }
}
