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

#include <math.h>

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

  // the following initialization is really important for
  // subclasses of MathMLCharNode overriding the Setup method
  fChar.font = NULL;
  fChar.charMap = NULL;
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

  if (env->charMapper.FontifyChar(fChar, env->GetFontAttributes(), ch)) {
    assert(fChar.font != NULL);
    assert(fChar.charMap != NULL);

#ifdef DEBUG
    MathEngine::logger(LOG_DEBUG, "successful layout for U+%04x simple index %02x", ch, fChar.nch);
#endif // DEBUG
  }

  FontifiedChar sChar;

  if (env->charMapper.FontifyStretchyChar(sChar, env->GetFontAttributes(), ch)) {
    assert(sChar.font != NULL);
    assert(sChar.charMap != NULL);
    
#ifdef DEBUG
    MathEngine::logger(LOG_DEBUG, "successful stretchy layout for U+%04x simple index %02x", ch, sChar.nch);
#endif // DEBUG
    
    layout = new StretchyCharLayout;
    layout->sChar = sChar;
    layout->simple = NULLCHAR;
    layout->n = 0;
  }
  
  if (fChar.font == NULL && layout == NULL) {
    // no glyph found
    scaled sppex = env->GetScaledPointsPerEx();
    box.Set(sppex, (2 * sppex) / 3, sppex / 3);
  }
}

void
MathMLCharNode::SetDefaultLargeGlyph(bool large)
{
  if (!IsStretchyFontified()) return;
  assert(layout != NULL);
  assert(layout->sChar.font != NULL);
  assert(layout->sChar.charMap != NULL);
#ifdef DEBUG
  MathEngine::logger(LOG_DEBUG, "before setting large was %x", layout->sChar.nch);
#endif // DEBUG
  layout->sChar.nch = layout->sChar.charMap->Map(ch, large);
  fChar = layout->sChar;
#ifdef DEBUG
  MathEngine::logger(LOG_DEBUG, "char %x with large %d set to %x", ch, large, layout->sChar.nch);
#endif // DEBUG
}

void
MathMLCharNode::DoLayout()
{
  if (!IsFontified()) return;

  fChar.GetBoundingBox(charBox);
  box = charBox;

#ifdef DEBUG
  MathEngine::logger(LOG_DEBUG, "done char layout for %x resulting in %d height", fChar.nch, sp2ipx(box.GetHeight()));
#endif // DEBUG

#if 1
  if (box.descent > box.ascent && fChar.charMap->GetStretch() != STRETCH_NO) {
    MathEngine::logger(LOG_DEBUG, "WARNING Texish code here");
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
#endif
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
  layout->simple = NULLCHAR;

  // first of all let's see if the small, unstretchable char is enough
  fChar.GetBoundingBox(charBox);
  if (scaledGeq(charBox.GetHeight(), desiredSize)) return;

  // next let's see if there is some single large char large enough
  for (unsigned i = 0; i < MAX_SIMPLE_CHARS && nch[i] != NULLCHAR; i++) {
    layout->simple = nch[i];
#if 0
    MathEngine::logger(LOG_DEBUG, "trying simple char %x for desire %d", layout->simple, sp2ipx(desiredSize));
#endif
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
#ifdef DEBUG
    MathEngine::logger(LOG_DEBUG, "rendering stretchy char U+%04X with simple %02x and n %d", ch, layout->simple, layout->n);
#endif // DEBUG
    if (layout->sChar.charMap->GetStretch() == STRETCH_VERTICAL)
      RenderVerticalStretchyChar(area, gc, GetX(), GetY() + box.descent);
    else
      RenderHorizontalStretchyChar(area, gc, GetX(), GetY());
  } else if (IsFontified()) {
    area.DrawChar(gc, fChar.font, GetX(), GetY() + box.descent - charBox.descent, fChar.nch);
  } else {
    // no glyph available
    if (MathEngine::DrawMissingCharacter())
      RenderMissingCharacter(area, gc);
  }

  // area.DrawBoundingBox(gc, GetX(), GetY(), box);

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
#ifdef DEBUG
    MathEngine::logger(LOG_DEBUG, "rendering vertical stretchy char %x", layout->simple);
#endif // DEBUG
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

  unsigned i;

  if (nch[SC_MIDDLE] != NULLCHAR) {  
    assert((nRepeat % 2) == 0);

    for (i = 0; i < nRepeat / 2; i++) {
      y -= layout->box[SC_REPEAT].descent;
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      y -= layout->box[SC_REPEAT].ascent;
    }

    y -= layout->box[SC_MIDDLE].descent;
    area.DrawChar(gc, font, x, y, nch[SC_MIDDLE]);
    y -= layout->box[SC_MIDDLE].ascent;

    for (i = 0; i < nRepeat / 2; i++) {
      y -= layout->box[SC_REPEAT].descent;
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      y -= layout->box[SC_REPEAT].ascent;
    }
  } else {
    for (i = 0; i < nRepeat; i++) {
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
  unsigned i = 0;

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

    for (i = 0; i < nRepeat / 2; i++) {
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      x += layout->box[SC_REPEAT].width;
    }

    area.DrawChar(gc, font, x, y, nch[SC_MIDDLE]);
    x += layout->box[SC_MIDDLE].width;

    for (i = 0; i < nRepeat / 2; i++) {
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      x += layout->box[SC_REPEAT].width;
    }
  } else {
    for (i = 0; i < nRepeat; i++) {
      area.DrawChar(gc, font, x, y, nch[SC_REPEAT]);
      x += layout->box[SC_REPEAT].width;
    }
  }

  if (nch[SC_LAST] != NULLCHAR) {
    area.DrawChar(gc, font, x, y, nch[SC_LAST]);
  }
}

void
MathMLCharNode::RenderMissingCharacter(const DrawingArea& area, const GraphicsContext* gc)
{
  assert(gc != NULL);
  area.MoveTo(GetX(), GetY());
  area.DrawLineToDelta(gc, 0, box.descent / 2);
  area.DrawLineToDelta(gc, box.width, 0);
  area.DrawLineToDelta(gc, 0, - box.descent / 2);
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

bool
MathMLCharNode::CombineWith(const MathMLCharNode* cChar, scaled& shiftX, scaled& shiftY) const
{
  assert(cChar != NULL);
  if (!IsFontified() || cChar->IsCombinedChar() || !cChar->IsFontified()) return false;
  if (!isCombining(cChar->GetChar())) return false;

  Char cch = cChar->GetChar();
  const AFont* cFont = cChar->GetFont();
  assert(cFont != NULL);

  const BoundingBox& cBox = cChar->GetBoundingBox();

  if (isCombiningOverlay(cch)) {
    shiftX = box.lBearing - cBox.lBearing + (box.rBearing - box.lBearing - cBox.rBearing + cBox.lBearing) / 2;
    shiftY = 0;
  } else if (isCombiningBelow(cch)) {
    shiftX = 0;
    shiftY = - box.descent - cBox.ascent;
  } else {
    /*
    printf("italic angle of the base char is %f %f\n",
	   fChar.font->GetItalicAngle(),
	   cFont->GetItalicAngle()
	   );
    */

    //shiftY = box.ascent + cBox.descent + cChar.font->GetLineThickness();
    // the following computation assumes that the accent is taken from a TeX font
    // and that that font has a valid glyph for x as position 'x'
    shiftY = box.ascent - cFont->GetEx();
    shiftY = scaledMax(shiftY, box.ascent + cBox.descent);

    float ia = (M_PI * (90 + fChar.font->GetItalicAngle())) / 180;
    scaled correction = pt2sp(sp2pt(shiftY) * cos(ia));
    shiftX = correction + (box.width - cBox.width) / 2;
#if 0
    float ia = (M_PI * (90 + fChar.font->GetItalicAngle())) / 180;

    scaled correction = (shiftY > 0) ? float2sp(sp2float(box.ascent) * cos(ia)) : 0;

    shiftX = correction + (box.width - cBox.rBearing + cBox.lBearing) / 2 - cBox.lBearing;
    //printf("%04x ic: %f height: %f\n", GetChar(), scaledMax(0, box.rBearing - box.width), box.ascent);
#endif
  }

  return true;
}
