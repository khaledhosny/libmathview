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

#include "Globals.hh"
#include "scaledConv.hh"
#include "ValueConversion.hh"
#include "RenderingEnvironment.hh"

#include "AreaFactory.hh"
#include "ShaperManager.hh"

RenderingEnvironment::RenderingEnvironment(const SmartPtr<AreaFactory>& af,
					   const SmartPtr<ShaperManager>& sm)
  : areaFactory(af), shaperManager(sm)
{
  AttributeLevel* top = new AttributeLevel;
  top->displayStyle = true;

  top->scriptLevel = 0;
  top->scriptMinSize.set(6.0, Length::PT_UNIT);
  top->scriptSizeMultiplier = 0.71;

  top->size.set(Globals::configuration.GetFontSize(), Length::PT_UNIT);

  top->color = Globals::configuration.GetForeground();
  top->background = Globals::configuration.GetBackground();
  top->transparentBackground = true;

  for (unsigned i = 0; i < 7; i++)
    {
      top->mathSpace[MATH_SPACE_NEGATIVEVERYVERYTHICK + i].set(-(7 - i) / 18.0, Length::EM_UNIT);
      top->mathSpace[MATH_SPACE_VERYVERYTHIN + i].set((i + 1) / 18.0, Length::EM_UNIT);
    }

  level.push_front(top);
}

RenderingEnvironment::~RenderingEnvironment()
{
  while (!level.empty()) Drop();
}

void
RenderingEnvironment::Push()
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  AttributeLevel* newLevel = new AttributeLevel;

  *newLevel = *top; // inherit previous environment

  level.push_front(newLevel);
}

void
RenderingEnvironment::Drop()
{
  assert(level.size() > 0);
  AttributeLevel* top = level.front();
  assert(top != 0);
  delete top;
  level.pop_front();
}

void
RenderingEnvironment::SetDisplayStyle(bool b)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  top->displayStyle = b;
}

bool
RenderingEnvironment::GetDisplayStyle(void) const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->displayStyle;
}

void
RenderingEnvironment::SetScriptMinSize(const Length& size)
{
  assert(!level.empty());
  assert(!size.type == Length::PERCENTAGE_UNIT);

  AttributeLevel* top = level.front();
  assert(top != NULL);

  // WARNING: we admit for scriptminsize a value of kind 1em
  // that is, the minimum font size is relative to the current font
  // EM size.
  top->scriptMinSize = size;
}

void
RenderingEnvironment::SetScriptSizeMultiplier(float mult)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  top->scriptSizeMultiplier = mult;
}

void
RenderingEnvironment::SetScriptLevel(int l)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  assert(l >= 0);
  AddScriptLevel(l - top->scriptLevel);
}

void
RenderingEnvironment::AddScriptLevel(int delta)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  if (delta == 0) return; // no change

  float multiplier = pow(top->scriptSizeMultiplier, static_cast<float>(delta));
  top->scriptLevel += delta;

  Length newFontSize(top->size.value * multiplier, top->size.type);

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
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->scriptLevel;
}

void
RenderingEnvironment::SetFontSize(const Length& size)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  // WARNING: a fontSize attribute is to be converted if
  // it's expressed in terms of em ex or %
  switch (size.type)
    {
    case Length::EM_UNIT:
      top->size.set((GetScaledPointsPerEm() * size.value).toFloat(), Length::PT_UNIT);
      break;
    case Length::EX_UNIT:
      top->size.set((GetScaledPointsPerEx() * size.value).toFloat(), Length::PT_UNIT);
      break;
    case Length::PERCENTAGE_UNIT:
      top->size.set(top->size.value * size.value, top->size.type);
      break;
    default:
      top->size = size;
      break;
    }
}

const Length&
RenderingEnvironment::GetFontSize() const
{
  assert(!level.empty());
  AttributeLevel* top = level.front();
  assert(top);
  return top->size;
}

void
RenderingEnvironment::SetColor(const RGBColor& c)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();

  assert(top != NULL);
  top->color = c;
}

void
RenderingEnvironment::SetBackgroundColor(const RGBColor& c)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  top->background = c;
  top->transparentBackground = false;
}

RGBColor
RenderingEnvironment::GetColor() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->color;
}

RGBColor
RenderingEnvironment::GetBackgroundColor() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->background;
}

void
RenderingEnvironment::SetMathSpace(MathSpaceId id, const Length& value)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  assert(id >= MATH_SPACE_NEGATIVEVERYVERYTHICK &&
	 id <= MATH_SPACE_VERYVERYTHICK);
  assert(!value.type == Length::PERCENTAGE_UNIT);

  top->mathSpace[id] = value;
}

RenderingEnvironment::MathSpaceId
RenderingEnvironment::mathSpaceIdOfTokenId(TokenId id)
{
  switch (id)
    {
    case T_NEGATIVEVERYVERYTHICKMATHSPACE: return MATH_SPACE_NEGATIVEVERYVERYTHICK;
    case T_NEGATIVEVERYTHICKMATHSPACE: return MATH_SPACE_NEGATIVEVERYTHICK;
    case T_NEGATIVETHICKMATHSPACE: return MATH_SPACE_NEGATIVETHICK;
    case T_NEGATIVEMEDIUMMATHSPACE: return MATH_SPACE_NEGATIVEMEDIUM;
    case T_NEGATIVETHINMATHSPACE: return MATH_SPACE_NEGATIVETHIN;
    case T_NEGATIVEVERYTHINMATHSPACE: return MATH_SPACE_NEGATIVEVERYTHIN;
    case T_NEGATIVEVERYVERYTHINMATHSPACE: return MATH_SPACE_NEGATIVEVERYVERYTHIN;
    case T_VERYVERYTHINMATHSPACE: return MATH_SPACE_VERYVERYTHIN;
    case T_VERYTHINMATHSPACE: return MATH_SPACE_VERYTHIN;
    case T_THINMATHSPACE: return MATH_SPACE_THIN;
    case T_MEDIUMMATHSPACE: return MATH_SPACE_MEDIUM;
    case T_THICKMATHSPACE: return MATH_SPACE_THICK;
    case T_VERYTHICKMATHSPACE: return MATH_SPACE_VERYTHICK;
    case T_VERYVERYTHICKMATHSPACE: return MATH_SPACE_VERYVERYTHICK;
    default: assert(false);
    }
}

const Length&
RenderingEnvironment::GetMathSpace(MathSpaceId id) const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  assert(id >= MATH_SPACE_NEGATIVEVERYVERYTHICK &&
	 id <= MATH_SPACE_VERYVERYTHICK);

  return top->mathSpace[id];
}

scaled
RenderingEnvironment::GetScaledPointsPerEm() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

#if 0
  FontifiedChar fChar;
  if (charMapper.FontifyChar(fChar, top->fontAttributes, 'M')) {
    assert(fChar.font != NULL);
    return fChar.font->GetEm();
  }
#endif

  return ToScaledPoints(GetFontSize());
}

scaled
RenderingEnvironment::GetScaledPointsPerEx() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

#if 0
  FontifiedChar fChar;
  if (charMapper.FontifyChar(fChar, top->fontAttributes, 'x')) {
    assert(fChar.font != NULL);
    return fChar.font->GetEx();
  }
#endif

  return ToScaledPoints(GetFontSize()) * (2.0 / 3.0);
}

scaled
RenderingEnvironment::ToScaledPoints(const Length& length) const
{
  switch (length.type)
    {
    case Length::EM_UNIT: return GetScaledPointsPerEm() * length.value;
    case Length::EX_UNIT: return GetScaledPointsPerEx() * length.value;
    default: return ::ToScaledPoints(length);
    }
}

scaled
RenderingEnvironment::GetAxis() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

#if 0
  FontifiedChar fChar;
  if (charMapper.FontifyChar(fChar, top->fontAttributes, '=')) {
    assert(fChar.font != NULL);

    BoundingBox eqBox;
    fChar.GetBoundingBox(eqBox);

    return eqBox.height - eqBox.verticalExtent() / 2;
  }
#endif

  return GetScaledPointsPerEx() / 2;
}

scaled
RenderingEnvironment::GetRuleThickness() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

#ifdef TEXISH_MATHML
  // don't know if this is the correct heuristics
  scaled s = ToScaledPoints(top->fontAttributes.size) * 0.04;
  return s;
#else
  scaled s = std::min(px2sp(1), ToScaledPoints(top->size) * 0.1);
  return s;
#endif
}

