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
  cChar = new MathMLCharNode(cc);
}

MathMLCombinedCharNode::~MathMLCombinedCharNode()
{
  delete cChar;
  cChar = NULL;
}

void
MathMLCombinedCharNode::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  MathMLCharNode::Setup(env);

  env->Push();
  env->SetFontMode(FONT_MODE_ANY);
  env->SetFontStyle(FONT_STYLE_NORMAL);

  assert(cChar != NULL);
  // this is really ugly, but in some sense is also true...
  cChar->SetParent(GetParent());
  cChar->Setup(env);

  if (cChar->GetFont() != fChar.font)
    MathEngine::logger(LOG_WARNING, "base char `U+%04x' and combining char `U+%04x' use different fonts", ch, cChar->GetChar());

  env->Drop();
}

void
MathMLCombinedCharNode::DoLayout()
{
  MathMLCharNode::DoLayout();
  assert(cChar != NULL);
  cChar->DoLayout();

  if (IsFontified() && cChar->IsFontified()) {
    const BoundingBox& cBox = cChar->GetBoundingBox();

    bool res = CombineWith(cChar, shiftX, shiftY);
    assert(res);

    box.ascent = scaledMax(charBox.ascent, cBox.ascent + shiftY);
    box.descent = scaledMax(charBox.descent, cBox.descent - shiftY);
    box.tAscent = scaledMax(charBox.tAscent, cBox.tAscent + shiftY);
    box.tDescent = scaledMax(charBox.tDescent, cBox.tDescent - shiftY);
    box.width = scaledMax(charBox.width, cBox.width + scaledAbs(shiftX));
    box.lBearing = scaledMin(charBox.lBearing, cBox.lBearing + shiftX);
    box.rBearing = scaledMax(charBox.rBearing, cBox.rBearing + shiftX);
  }
}

void
MathMLCombinedCharNode::SetPosition(scaled x, scaled y)
{
  MathMLCharNode::SetPosition(x, y);
  assert(cChar != NULL);
  cChar->SetPosition(x + shiftX, y - shiftY);
}

void
MathMLCombinedCharNode::SetDirty(const Rectangle* rect)
{
  MathMLCharNode::SetDirty(rect);
  assert(cChar != NULL);
  cChar->SetDirty(rect);
}

void
MathMLCombinedCharNode::Render(const DrawingArea& area)
{
  MathMLCharNode::Render(area);
  assert(cChar != NULL);
  if (cChar->IsFontified()) cChar->Render(area);
}

bool
MathMLCombinedCharNode::IsCombinedChar() const
{
  return true;
}
