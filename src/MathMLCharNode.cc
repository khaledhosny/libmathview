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

#include "MathEngine.hh"
#include "CharMapper.hh"
#include "MathMLElement.hh"
#include "MathMLCharNode.hh"
#include "RenderingEnvironment.hh"

MathMLCharNode::MathMLCharNode(Char c)
{
  ch = c;
  // The basic idea is that a stretchable char behaves exactly as an unstretchable
  // one as long as the layout is not allocated.
  layout = NULL;
}

MathMLCharNode::~MathMLCharNode()
{
  delete layout;
}

void
MathMLCharNode::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  delete layout;
  layout = NULL;

  fChar.font = NULL;
  fChar.charMap = NULL;
  env->charMapper.FontifyChar(fChar, env->GetFontAttributes(), ch);
  assert(fChar.font != NULL);
  assert(fChar.charMap != NULL);

  FontifiedChar sChar;
  sChar.font = NULL;
  sChar.charMap = NULL;
  env->charMapper.FontifyStretchyChar(sChar, env->GetFontAttributes(), ch);
  if (sChar.font != NULL && sChar.charMap != NULL) {
    layout = new StretchyCharLayout;
    layout->sChar = sChar;
    layout->simple = NULLCHAR;
    layout->n = 0;
  }
}

void
MathMLCharNode::SetDefaultLargeGlyph(bool large)
{
  if (!IsStretchyFontified()) return;
  assert(layout != NULL);
  assert(layout->sChar.font != NULL);
  assert(layout->sChar.charMap != NULL);
  layout->sChar.nch = layout->sChar.charMap->Map(ch, large);
}

void
MathMLCharNode::DoLayout()
{
  assert(IsFontified());
  fChar.GetBoundingBox(charBox);
  box = charBox;

  if (box.descent > box.ascent && fChar.charMap->GetStretch() != STRETCH_NO) {
    // BEWARE!
    // vertical stretchy char may have a meaningless bounding box. For example,
    // stretchy chars inside cmex font (for TeX) all have a (quasi) zero ascent.
    // we infer the nominal height of the font by computing the height
    // of the smallest summation sign, the ascent is given by the smallest
    // accent sign

    BoundingBox aBox;
    fChar.font->CharBox(98, aBox); // an accent
    scaled fontAscent  = aBox.ascent;

    fChar.font->CharBox(80, aBox); // a sum
    scaled fontHeight  = aBox.GetHeight();

    scaled fontDescent = fontHeight - fontAscent;
    scaled delta = (fontHeight - charBox.GetHeight()) / 2;

    box.ascent = fontAscent - delta;
    box.descent = fontDescent - delta;

    delta = (fontHeight - charBox.GetTotalHeight()) / 2;
    box.tAscent = fontAscent - delta;
    box.tDescent = fontDescent - delta;
  }
}

void
MathMLCharNode::DoVerticalStretchyLayout(scaled height, scaled depth, scaled axis, bool strict)
{
  if (!IsStretchyFontified()) return;
  if (layout->sChar.charMap->GetStretch() != STRETCH_VERTICAL) return;

  DoVerticalStretchyLayoutAux(height + depth, strict);

  scaled charHeight = charBox.GetHeight();
  scaled delta = (height + depth - charHeight) / 2;
  box.Set(charBox.width,
	  height + axis - delta, depth - axis - delta,
	  charBox.lBearing, charBox.rBearing);
}

void
MathMLCharNode::DoVerticalStretchyLayoutAux(scaled desiredSize, bool)
{
  assert(layout != NULL);
  assert(layout->sChar.font != NULL);
  assert(layout->sChar.charMap != NULL);

  const AFont* font = layout->sChar.font;
  const char* nch = layout->sChar.charMap->stretchy.simple;

  layout->n = 0;

  // first of all let's see if there is some single char large enough
  for (unsigned i = 0; i < MAX_SIMPLE_CHARS && nch[i] != NULLCHAR; i++) {
    layout->simple = nch[i];
    font->CharBox(layout->simple, charBox);
    if (scaledGeq(charBox.GetHeight(), desiredSize)) return;
  }
  
  // if there is no compound char then we return immediately, the largest
  // single char is the best possible
  nch = layout->sChar.charMap->stretchy.compound;
  if (nch[SC_FIRST] == NULLCHAR
      && nch[SC_MIDDLE] == NULLCHAR
      && nch[SC_LAST] == NULLCHAR
      && nch[SC_REPEAT] == NULLCHAR) return;

  // we didn't succeeded, so we look for a suitable compound char
  layout->simple = NULLCHAR;

  scaled size  = 0; // total size of the stretched char
  scaled width = 0; // total width of the stretched char
  scaled lBearing = 0; // FIXME: we have to compute the minimum left-bearing
  scaled rBearing = 0;

  if (nch[SC_FIRST] != NULLCHAR) {
    font->CharBox(nch[SC_FIRST], layout->box[SC_FIRST]);
    size += layout->box[SC_FIRST].GetHeight();
    width = scaledMax(width, layout->box[SC_FIRST].width);
    lBearing = scaledMin(lBearing, layout->box[SC_FIRST].lBearing);
    rBearing = scaledMax(rBearing, layout->box[SC_FIRST].rBearing);
    layout->n++;
  }

  if (nch[SC_LAST] != NULLCHAR) {
    font->CharBox(nch[SC_LAST], layout->box[SC_LAST]);
    size += layout->box[SC_LAST].GetHeight();
    width = scaledMax(width, layout->box[SC_LAST].width);
    lBearing = scaledMin(lBearing, layout->box[SC_LAST].lBearing);
    rBearing = scaledMax(rBearing, layout->box[SC_LAST].rBearing);
    layout->n++;
  }

  if (nch[SC_MIDDLE] != NULLCHAR) {
    font->CharBox(nch[SC_MIDDLE], layout->box[SC_MIDDLE]);
    size += layout->box[SC_MIDDLE].GetHeight();
    width = scaledMax(width, layout->box[SC_MIDDLE].width);
    lBearing = scaledMin(lBearing, layout->box[SC_MIDDLE].lBearing);
    rBearing = scaledMax(rBearing, layout->box[SC_MIDDLE].rBearing);
    layout->n++;
  }

  if (scaledGeq(size, desiredSize)) {
    if      (nch[SC_LAST] != NULLCHAR) charBox = layout->box[SC_LAST];
    else if (nch[SC_MIDDLE] != NULLCHAR) charBox = layout->box[SC_MIDDLE];
    else charBox = layout->box[SC_FIRST];

    charBox.ascent += size - charBox.GetHeight();
    charBox.width = width;
    charBox.lBearing = lBearing;
    charBox.rBearing = rBearing;

    return;
  }

  if (nch[SC_REPEAT] != NULLCHAR)
    font->CharBox(nch[SC_REPEAT], layout->box[SC_REPEAT]);

  if (nch[SC_REPEAT] != NULLCHAR && layout->box[SC_REPEAT].GetHeight() > SP_EPSILON) {
    width = scaledMax(width, layout->box[SC_REPEAT].width);
    lBearing = scaledMin(lBearing, layout->box[SC_REPEAT].lBearing);
    rBearing = scaledMax(rBearing, layout->box[SC_REPEAT].rBearing);
    while (desiredSize - size > SP_EPSILON) {
      unsigned step = (nch[SC_MIDDLE] == NULLCHAR) ? 1 : 2;
      layout->n += step;
      size += step * layout->box[SC_REPEAT].GetHeight();
    }
  }

  if      (nch[SC_LAST] != NULLCHAR) charBox = layout->box[SC_LAST];
  else if (nch[SC_REPEAT] != NULLCHAR) charBox = layout->box[SC_REPEAT];
  else charBox = layout->box[SC_FIRST];

  charBox.ascent += size - charBox.GetHeight();
  charBox.width = width;
  charBox.lBearing = lBearing;
  charBox.rBearing = rBearing;
}

void
MathMLCharNode::DoHorizontalStretchyLayout(scaled desiredSize, bool strict)
{
  if (!IsStretchyFontified()) return;
  if (layout->sChar.charMap->GetStretch() != STRETCH_HORIZONTAL) return;

  DoHorizontalStretchyLayoutAux(desiredSize, strict);
  box = charBox;
}

void
MathMLCharNode::DoHorizontalStretchyLayoutAux(scaled desiredSize, bool)
{
  assert(layout != NULL);
  assert(layout->sChar.font != NULL);
  assert(layout->sChar.charMap != NULL);

  const AFont* font = layout->sChar.font;
  const char* nch = layout->sChar.charMap->stretchy.simple;

  layout->n = 0;

  // first of all let's see if there is some single char large enough
  for (unsigned i = 0; i < MAX_SIMPLE_CHARS && nch[i] != NULLCHAR; i++) {
    layout->simple = nch[i];
    font->CharBox(layout->simple, charBox);
    if (scaledGeq(charBox.width, desiredSize)) return;
  }

  nch = layout->sChar.charMap->stretchy.compound;
  if (nch[SC_FIRST] == NULLCHAR
      && nch[SC_MIDDLE] == NULLCHAR
      && nch[SC_LAST] == NULLCHAR
      && nch[SC_REPEAT] == NULLCHAR) return;

  layout->simple = NULLCHAR;
  charBox.Null();

  if (nch[SC_FIRST] != NULLCHAR) {
    font->CharBox(nch[SC_FIRST], layout->box[SC_FIRST]);
    charBox.Append(layout->box[SC_FIRST]);
    layout->n++;
  }

  if (nch[SC_LAST] != NULLCHAR) {
    font->CharBox(nch[SC_LAST], layout->box[SC_LAST]);
    charBox.Append(layout->box[SC_LAST]);
    layout->n++;
  }

  if (nch[SC_MIDDLE] != NULLCHAR) {
    font->CharBox(nch[SC_MIDDLE], layout->box[SC_MIDDLE]);
    charBox.Append(layout->box[SC_MIDDLE]);
    layout->n++;
  }

  if (scaledGeq(charBox.width, desiredSize)) return;

  if (nch[SC_REPEAT] != NULLCHAR)
    font->CharBox(nch[SC_REPEAT], layout->box[SC_REPEAT]);

  if (nch[SC_REPEAT] != NULLCHAR && layout->box[SC_REPEAT].width > SP_EPSILON) {
    while (desiredSize - charBox.width > SP_EPSILON) {
      unsigned step = (nch[SC_MIDDLE] == NULLCHAR) ? 1 : 2;
      layout->n += step;
      charBox.Append(layout->box[SC_REPEAT]);
      if (step == 2)
	charBox.Append(layout->box[SC_REPEAT]);
    }
  }
}

void
MathMLCharNode::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;

  assert(GetParent() != NULL);
  const GraphicsContext* gc = GetParent()->GetForegroundGC();

  if (IsStretchyFontified() && (layout->simple != NULLCHAR || layout->n > 0)) {
    if (layout->sChar.charMap->GetStretch() == STRETCH_VERTICAL)
      RenderVerticalStretchyChar(area, gc, GetX(), GetY() + box.descent);
    else
      RenderHorizontalStretchyChar(area, gc, GetX(), GetY());
  } else {
    area.DrawChar(gc, fChar.font, GetX(), GetY() + box.descent - charBox.descent, fChar.nch);
  }

#if 0
  box.Dump();
  putchar('\n');
  area.DrawBoundingBox(gc, GetX(), GetY(), box, true);
#endif

  ResetDirty();
}

void
MathMLCharNode::RenderVerticalStretchyChar(const DrawingArea& area,
					   const GraphicsContext* gc, scaled x, scaled y)
{
  assert(gc != NULL);
  assert(layout != NULL);
  assert(layout->sChar.font != NULL);
  assert(layout->sChar.charMap != NULL);

  const AFont* font = layout->sChar.font;

  if (layout->simple != NULLCHAR) {
    y -= charBox.descent;
    area.DrawChar(gc, font, x, y, layout->simple);
    return;
  }

  const char* nch = layout->sChar.charMap->stretchy.compound;

  unsigned nRepeat = layout->n;
  assert(nRepeat > 0);

  if (nch[SC_FIRST] != NULLCHAR) nRepeat--;
  if (nch[SC_LAST] != NULLCHAR) nRepeat--;
  if (nch[SC_MIDDLE] != NULLCHAR && nRepeat > 0) nRepeat--;

  if (nch[SC_LAST] != NULLCHAR) {
    y -= layout->box[SC_LAST].descent;
    area.DrawChar(gc, font, x, y, nch[SC_LAST]);
    y -= layout->box[SC_LAST].ascent;
  }

  if (nch[SC_MIDDLE] != NULLCHAR) {  
    assert((nRepeat % 2) == 0);

    for (unsigned i = 0; i < nRepeat / 2; i++) {
      y -= layout->box[SC_REPEAT].descent;
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      y -= layout->box[SC_REPEAT].ascent;
    }

    y -= layout->box[SC_MIDDLE].descent;
    area.DrawChar(gc, font, x, y, nch[SC_MIDDLE]);
    y -= layout->box[SC_MIDDLE].ascent;

    for (unsigned i = 0; i < nRepeat / 2; i++) {
      y -= layout->box[SC_REPEAT].descent;
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      y -= layout->box[SC_REPEAT].ascent;
    }
  } else {
    for (unsigned i = 0; i < nRepeat; i++) {
      y -= layout->box[SC_REPEAT].descent;
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      y -= layout->box[SC_REPEAT].ascent;
    }
  }

  if (nch[SC_FIRST] != NULLCHAR) {
    y -= layout->box[SC_FIRST].descent;
    area.DrawChar(gc, font, x, y, nch[SC_FIRST]);
  }
}

void
MathMLCharNode::RenderHorizontalStretchyChar(const DrawingArea& area,
					     const GraphicsContext* gc, scaled x, scaled y)
{
  assert(gc != NULL);
  assert(layout != NULL);
  assert(layout->sChar.font != NULL);
  assert(layout->sChar.charMap != NULL);

  const AFont* font = layout->sChar.font;

  if (layout->n == 0) {
    assert(layout->simple != NULLCHAR);
    area.DrawChar(gc, font, x, y, layout->simple);
    return;
  }

  const char* nch = layout->sChar.charMap->stretchy.compound;

  unsigned nRepeat = layout->n;
  assert(nRepeat > 0);

  if (nch[SC_FIRST] != NULLCHAR) nRepeat--;
  if (nch[SC_LAST] != NULLCHAR) nRepeat--;
  if (nch[SC_MIDDLE] != NULLCHAR && nRepeat > 0) nRepeat--;

  if (nch[SC_FIRST] != NULLCHAR) {
    area.DrawChar(gc, font, x, y, nch[SC_FIRST]);
    x += layout->box[SC_FIRST].width;
  }

  if (nch[SC_MIDDLE] != NULLCHAR) {  
    assert((nRepeat % 2) == 0);

    for (unsigned i = 0; i < nRepeat / 2; i++) {
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      x += layout->box[SC_REPEAT].width;
    }

    area.DrawChar(gc, font, x, y, nch[SC_MIDDLE]);
    x += layout->box[SC_MIDDLE].width;

    for (unsigned i = 0; i < nRepeat / 2; i++) {
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      x += layout->box[SC_REPEAT].width;
    }
  } else {
    for (unsigned i = 0; i < nRepeat; i++) {
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      x += layout->box[SC_REPEAT].width;
    }
  }

  if (nch[SC_LAST] != NULLCHAR) {
    area.DrawChar(gc, font, x, y, nch[SC_LAST]);
  }
}

bool
MathMLCharNode::IsChar() const
{
  return true;
}

bool
MathMLCharNode::IsFontified() const
{
  return fChar.font != NULL && fChar.charMap != NULL;
}

bool
MathMLCharNode::IsStretchyChar() const
{
  return layout != NULL;
}

bool
MathMLCharNode::IsStretchyFontified() const
{
  return IsStretchyChar()
    && layout->sChar.font != NULL
    && layout->sChar.charMap != NULL;
}

bool
MathMLCharNode::HasDecimalPoint() const
{
  return GetChar() == '.';
}

scaled
MathMLCharNode::GetDecimalPointEdge() const
{
  return GetX() + ((GetChar() == '.') ? 0 : box.width);
}

StretchId
MathMLCharNode::GetStretch() const
{
  if (!IsStretchyFontified()) return STRETCH_NO;
  else return layout->sChar.charMap->GetStretch();
}
