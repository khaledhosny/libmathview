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
#include <math.h>   // for pow(...)
#include <string.h> // for strncpy(...)
#include <stdlib.h> // for exit()

#include "Iterator.hh"
#include "CharMapper.hh"
#include "MathEngine.hh"
#include "StringTokenizer.hh"
#include "ValueConversion.hh"
#include "RenderingEnvironment.hh"


RenderingEnvironment::RenderingEnvironment(CharMapper& cm) : charMapper(cm)
{
  AttributeLevel* top = new AttributeLevel;
  top->displayStyle = true;

  top->scriptLevel = 0;
  top->scriptMinSize.Set(6.0, UNIT_PT);
  top->scriptSizeMultiplier = 0.71;

  top->fontAttributes.family = "serif";
  top->fontAttributes.size.Set(MathEngine::configuration.GetFontSize(), UNIT_PT);
  top->fontAttributes.weight = FONT_WEIGHT_NORMAL;
  top->fontAttributes.style  = FONT_STYLE_NORMAL;

  top->color = MathEngine::configuration.GetForeground();
  top->background = MathEngine::configuration.GetBackground();
  top->transparentBackground = true;

  for (unsigned i = 1; i <= 7; i++)
    top->mathSpace[i - 1].Set(i / 18.0, UNIT_EM);

  top->defaults = NULL;

  level.Push(top);
}

RenderingEnvironment::~RenderingEnvironment()
{
}

void
RenderingEnvironment::Push(const MathMLAttributeList* aList)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  AttributeLevel* newLevel = new AttributeLevel;

  *newLevel = *top; // inherit previous environment
  newLevel->defaults = aList;

  level.Push(newLevel);
}

void
RenderingEnvironment::Drop()
{
  assert(level.GetSize() > 1);
  level.Drop();
}

const MathMLAttribute*
RenderingEnvironment::GetAttribute(AttributeId id) const
{
  for (Iterator<AttributeLevel*> i(level); i.More(); i.Next()) {
    AttributeLevel* thisLevel = i();
    assert(thisLevel != NULL);

    if (thisLevel->defaults != NULL) {
      const MathMLAttribute* attribute = thisLevel->defaults->GetAttribute(id);
      if (attribute != NULL) return attribute;
    }
  }

  return NULL;
}

void
RenderingEnvironment::SetDisplayStyle(bool b)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  top->displayStyle = b;
}

bool
RenderingEnvironment::GetDisplayStyle(void) const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  return top->displayStyle;
}

void
RenderingEnvironment::SetScriptMinSize(const UnitValue& size)
{
  assert(!level.IsEmpty());
  assert(!size.IsPercentage());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  // WARNING: we admit for scriptminsize a value of kind 1em
  // that is, the minimum font size is relative to the current font
  // EM size.
  top->scriptMinSize = size;
}

void
RenderingEnvironment::SetScriptSizeMultiplier(float mult)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  top->scriptSizeMultiplier = mult;
}

void
RenderingEnvironment::SetScriptLevel(int l)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  assert(l >= 0);
  AddScriptLevel(l - top->scriptLevel);
}

void
RenderingEnvironment::AddScriptLevel(int delta)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  if (delta == 0) return; // no change

  float multiplier = pow(top->scriptSizeMultiplier, static_cast<float>(delta));
  top->scriptLevel += delta;

  UnitValue newFontSize;
  newFontSize.Set(top->fontAttributes.size.GetValue() * multiplier,
		  top->fontAttributes.size.GetUnitId());

  // WARNING: if scriptMinSize cannot be em or ex, than the
  // following test can be implemented much more efficiently, because
  // we don't need to convert em or ex to scaled points. 
  if (ToScaledPoints(newFontSize) < ToScaledPoints(top->scriptMinSize))
    newFontSize = top->scriptMinSize;

  SetFontSize(newFontSize);
}

int
RenderingEnvironment::GetScriptLevel() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  return top->scriptLevel;
}

void
RenderingEnvironment::SetFontFamily(const char* family)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  assert(family != NULL);
  top->fontAttributes.family = family;
}

void
RenderingEnvironment::SetFontFamily(const String* family)
{
  assert(family != NULL);
  SetFontFamily(family->ToStaticC());
}

void
RenderingEnvironment::SetFontSize(const UnitValue& size)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  // WARNING: a fontSize attribute is to be converted if
  // it's expressed in terms of em ex or %
  switch (size.GetUnitId()) {
  case UNIT_EM:
    {
      float sppm = GetScaledPointsPerEm();
      top->fontAttributes.size.Set((size.GetValue() * sppm) / SCALED_POINTS_PER_PT, UNIT_PT);
    }
    break;
  case UNIT_EX:
    {
      float sppx = GetScaledPointsPerEx();
      top->fontAttributes.size.Set((size.GetValue() * sppx) / SCALED_POINTS_PER_PT, UNIT_PT);
    }
    break;
  case UNIT_PERCENTAGE:
    top->fontAttributes.size.Set(top->fontAttributes.size.GetValue() * size.GetValue(),
				 top->fontAttributes.size.GetUnitId());
    break;
  default:
    top->fontAttributes.size = size;
    break;
  }
}

void
RenderingEnvironment::SetFontWeight(FontWeightId weight)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);
  top->fontAttributes.weight = weight;
}

void
RenderingEnvironment::SetFontStyle(FontStyleId style)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  top->fontAttributes.style = style;
}

const FontAttributes&
RenderingEnvironment::GetFontAttributes() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  return top->fontAttributes;
}

void
RenderingEnvironment::SetColor(RGBValue c)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();

  assert(top != NULL);
  top->color = c;
}

void
RenderingEnvironment::SetBackgroundColor(RGBValue c)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  top->background = c;
  top->transparentBackground = false;
}

RGBValue
RenderingEnvironment::GetColor() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  return top->color;
}

RGBValue
RenderingEnvironment::GetBackgroundColor() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  return top->background;
}

void
RenderingEnvironment::SetMathSpace(MathSpaceId id,
				   const UnitValue& value)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  assert(id != MATH_SPACE_NOTVALID);
  assert(id != MATH_SPACE_LAST);
  assert(!value.IsPercentage());

  top->mathSpace[id] = value;
}

const
UnitValue& RenderingEnvironment::GetMathSpace(MathSpaceId id) const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  assert(id != MATH_SPACE_NOTVALID);
  assert(id != MATH_SPACE_LAST);

  return top->mathSpace[id];
}

scaled
RenderingEnvironment::GetScaledPointsPerEm() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  FontifiedChar fChar;
  if (!charMapper.FontifyChar(fChar, top->fontAttributes, 'M')) {
    MathEngine::logger(LOG_ERROR, "fatal: could not find default fonts, maybe the font configuration is wrong");
    exit(1);
  }
  assert(fChar.font != NULL);

  return fChar.font->GetEm();
}

scaled
RenderingEnvironment::GetScaledPointsPerEx() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  FontifiedChar fChar;
  if (!charMapper.FontifyChar(fChar, top->fontAttributes, 'x')) {
    MathEngine::logger(LOG_ERROR, "fatal: could not find default fonts, maybe the font configuration is wrong");
    exit(1);
  }
  assert(fChar.font != NULL);

  return fChar.font->GetEx();
}

scaled
RenderingEnvironment::ToScaledPoints(const UnitValue& value) const
{
  assert(!value.IsPercentage());

  float sppm = GetScaledPointsPerEm();
  float sppx = GetScaledPointsPerEx();

  return value.ToScaledPoints(sppm, sppx);
}

scaled
RenderingEnvironment::GetAxis() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  FontifiedChar fChar;
  if (!charMapper.FontifyChar(fChar, top->fontAttributes, '=')) {
    MathEngine::logger(LOG_ERROR, "fatal: could not find default fonts, maybe the font configuration is wrong");
    exit(1);
  }
  assert(fChar.font != NULL);

  BoundingBox eqBox;
  fChar.GetBoundingBox(eqBox);

  return eqBox.ascent - (eqBox.ascent + eqBox.descent) / 2;
}

scaled
RenderingEnvironment::GetRuleThickness() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  // don't know if this is the correct heuristics
  scaled s = float2sp(sp2float(top->fontAttributes.size.ToScaledPoints()) * 0.04);
  return s;

#if 0
  const FontifiedChar* fChar = charMapper.FontifyChar(top->fontAttributes, '-');
  assert(fChar != NULL);

  BoundingBox minusBox;
  fChar->GetBoundingBox(minusBox);
  delete fChar;

  return scaledMax(SCALED_POINTS_PER_PX, minusBox.ascent + minusBox.descent);
#endif
}

void
RenderingEnvironment::SetFontMode(FontModeId mode)
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  top->fontAttributes.mode = mode;
}

FontModeId
RenderingEnvironment::GetFontMode() const
{
  assert(!level.IsEmpty());

  AttributeLevel* top = level.Top();
  assert(top != NULL);

  return top->fontAttributes.mode;
}
