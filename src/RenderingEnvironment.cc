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
#include "StringTokenizer.hh"
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
  top->scriptMinSize.Set(6.0, UNIT_PT);
  top->scriptSizeMultiplier = 0.71;

  top->fontAttributes.family = "serif";
  top->fontAttributes.size.Set(Globals::configuration.GetFontSize(), UNIT_PT);
  top->fontAttributes.weight = FONT_WEIGHT_NORMAL;
  top->fontAttributes.style  = FONT_STYLE_NORMAL;

  top->color = Globals::configuration.GetForeground();
  top->background = Globals::configuration.GetBackground();
  top->transparentBackground = true;

  top->doc = 0;

  for (unsigned i = 1; i <= 7; i++)
    top->mathSpace[i - 1].Set(i / 18.0, UNIT_EM);

  top->defaults = NULL;

  level.push_front(top);
}

RenderingEnvironment::~RenderingEnvironment()
{
  while (!level.empty()) Drop();
}

void
RenderingEnvironment::Push(const SmartPtr<MathMLAttributeList>& aList)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  AttributeLevel* newLevel = new AttributeLevel;

  *newLevel = *top; // inherit previous environment
  newLevel->defaults = aList;

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

#if 0
SmartPtr<MathMLAttribute>
RenderingEnvironment::GetAttribute(AttributeId id) const
{
  for (std::list<AttributeLevel*>::const_iterator i = level.begin();
       i != level.end();
       i++)
    {
      AttributeLevel* thisLevel = *i;
      assert(thisLevel != 0);

      if (thisLevel->defaults)
	if (SmartPtr<MathMLAttribute> attribute = thisLevel->defaults->get(id))
	  return attribute;
    }

  return 0;
}
#endif

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
RenderingEnvironment::SetScriptMinSize(const UnitValue& size)
{
  assert(!level.empty());
  assert(!size.IsPercentage());

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

  UnitValue newFontSize(top->fontAttributes.size.GetValue() * multiplier,
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
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->scriptLevel;
}

void
RenderingEnvironment::SetFontFamily(const char* family)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  assert(family != NULL);
  top->fontAttributes.family = family;
}

void
RenderingEnvironment::SetFontFamily(const String& family)
{
  SetFontFamily(family.c_str());
}

void
RenderingEnvironment::SetFontSize(const UnitValue& size)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  // WARNING: a fontSize attribute is to be converted if
  // it's expressed in terms of em ex or %
  switch (size.GetUnitId()) {
  case UNIT_EM:
    top->fontAttributes.size.Set((GetScaledPointsPerEm() * size.GetValue()).toFloat(), UNIT_PT);
    break;
  case UNIT_EX:
    top->fontAttributes.size.Set((GetScaledPointsPerEx() * size.GetValue()).toFloat(), UNIT_PT);
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
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);
  top->fontAttributes.weight = weight;
}

void
RenderingEnvironment::SetFontStyle(FontStyleId style)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  top->fontAttributes.style = style;
}

const FontAttributes&
RenderingEnvironment::GetFontAttributes() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->fontAttributes;
}

void
RenderingEnvironment::SetColor(RGBValue c)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();

  assert(top != NULL);
  top->color = c;
}

void
RenderingEnvironment::SetBackgroundColor(RGBValue c)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  top->background = c;
  top->transparentBackground = false;
}

RGBValue
RenderingEnvironment::GetColor() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->color;
}

RGBValue
RenderingEnvironment::GetBackgroundColor() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->background;
}

void
RenderingEnvironment::SetMathSpace(MathSpaceId id,
				   const UnitValue& value)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  assert(id != MATH_SPACE_NOTVALID);
  assert(id != MATH_SPACE_LAST);
  assert(!value.IsPercentage());

  top->mathSpace[id] = value;
}

const UnitValue&
RenderingEnvironment::GetMathSpace(MathSpaceId id) const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  assert(id != MATH_SPACE_NOTVALID);
  assert(id != MATH_SPACE_LAST);

  return top->mathSpace[id];
}

void
RenderingEnvironment::SetDocument(const SmartPtr<MathMLDocument>& doc)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  top->doc = doc;
}

SmartPtr<MathMLDocument>
RenderingEnvironment::GetDocument() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->doc;
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

  assert(top->fontAttributes.HasSize());
  assert(top->fontAttributes.size.IsAbsolute());
  return top->fontAttributes.size.ToScaledPoints();
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

  assert(top->fontAttributes.HasSize());
  assert(top->fontAttributes.size.IsAbsolute());
  return top->fontAttributes.size.ToScaledPoints() * (2.0 / 3.0);
}

scaled
RenderingEnvironment::ToScaledPoints(const UnitValue& value) const
{
  assert(!value.IsPercentage());
  return value.ToScaledPoints(GetScaledPointsPerEm(), GetScaledPointsPerEx());
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
  scaled s = top->fontAttributes.size.ToScaledPoints() * 0.04;
  return s;
#else
  scaled s = std::min(px2sp(1), top->fontAttributes.size.ToScaledPoints() * 0.1);
  return s;
#endif
}

void
RenderingEnvironment::SetFontMode(FontModeId mode)
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  top->fontAttributes.mode = mode;
}

FontModeId
RenderingEnvironment::GetFontMode() const
{
  assert(!level.empty());

  AttributeLevel* top = level.front();
  assert(top != NULL);

  return top->fontAttributes.mode;
}
