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
#include "StringUnicode.hh"
#include "ValueConversion.hh"
#include "MathMLDummyElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLFractionElement.hh"

MathMLFractionElement::MathMLFractionElement(mDOMNodeRef node) : MathMLContainerElement(node, TAG_MFRAC)
{
}

MathMLFractionElement::~MathMLFractionElement()
{
}

const AttributeSignature*
MathMLFractionElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_LINETHICKNESS, lineThicknessParser, new StringC("1"),      NULL },
    { ATTR_NUMALIGN,      fracAlignParser,     new StringC("center"), NULL },
    { ATTR_DENOMALIGN,    fracAlignParser,     new StringC("center"), NULL },
    { ATTR_BEVELLED,      booleanParser,       new StringC("false"),  NULL },
    { ATTR_NOTVALID,      NULL,                NULL,                  NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLContainerElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLFractionElement::Normalize()
{
  while (content.GetSize() > 2) {
    MathMLElement* elem = content.RemoveLast();
    if (elem != NULL) delete elem;
  }

  while (content.GetSize() < 2) {
    MathMLElement* mdummy = new MathMLDummyElement;
    mdummy->SetParent(this);
    content.Append(mdummy);
  }

  MathMLContainerElement::Normalize();
}

void
MathMLFractionElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  const Value* value = NULL;

#ifdef TEXISH_MATHML
  defaultRuleThickness = env->GetRuleThickness();
#else
  scaled defaultRuleThickness = env->GetRuleThickness();
#endif // TEXISH_MATHML

  value = GetAttributeValue(ATTR_LINETHICKNESS, env, true);
  if (value != NULL) {
    if (value->IsKeyword()) {
      switch (value->ToKeyword()) {
      case KW_THIN:
	lineThickness = defaultRuleThickness / 2;
	break;
      case KW_MEDIUM:
	lineThickness = defaultRuleThickness;
	break;
      case KW_THICK:
	lineThickness = defaultRuleThickness * 2;
	break;
      default:
	assert(IMPOSSIBLE);
	break;
      }
    } else {
      assert(value->IsSequence());
      const Value* number = value->Get(0);
      const Value* unit   = value->Get(1);

      assert(number != NULL);
      assert(unit != NULL);

      if (unit->IsEmpty()) lineThickness = defaultRuleThickness * roundFloat(number->ToNumber());
      else {
	assert(unit->IsKeyword());
	UnitId unitId = ToUnitId(unit);
	if (unitId == UNIT_PERCENTAGE) {
	  MathEngine::logger(LOG_WARNING, "line thickness given as percentage in `mfrac' element (taking default)");
	  lineThickness = defaultRuleThickness;
	} else {
	  UnitValue unitValue;
	  unitValue.Set(number->ToNumber(), unitId);
	  lineThickness = env->ToScaledPoints(unitValue);
	}
      }
    }

    lineThickness = scaledMax(0, lineThickness);
  }

  delete value;

  value = GetAttributeValue(ATTR_NUMALIGN, env, true);
  if (value != NULL) {
    numAlign = ToFractionAlignId(value);
    delete value;
  }

  value = GetAttributeValue(ATTR_DENOMALIGN, env, true);
  if (value != NULL) {
    denomAlign = ToFractionAlignId(value);
    delete value;
  }

  value = GetAttributeValue(ATTR_BEVELLED, env, true);
  if (value != NULL) {
    bevelled = value->ToBoolean();
    delete value;
  }

  color = env->GetColor();

  axis = env->GetAxis();

  displayStyle = env->GetDisplayStyle();

#ifdef TEXISH_MATHML
  if (displayStyle) {
    numMinShift = float2sp(sp2float(env->GetFontAttributes().size.ToScaledPoints()) * 0.676508);
    denomMinShift = float2sp(sp2float(env->GetFontAttributes().size.ToScaledPoints()) * 0.685951);
  } else {
    numMinShift = float2sp(sp2float(env->GetFontAttributes().size.ToScaledPoints()) * (lineThickness > 0 ? 0.393732 : 0.443731));
    denomMinShift = float2sp(sp2float(env->GetFontAttributes().size.ToScaledPoints()) * 0.344841);
  }
#else
  minShift = env->GetScaledPointsPerEx();
#endif // TEXISH_MATHML

  env->Push();
  if (!displayStyle) env->AddScriptLevel(1);
  else env->SetDisplayStyle(false);
  MathMLContainerElement::Setup(env);
  env->Drop();
}

void
MathMLFractionElement::DoBoxedLayout(LayoutId id, BreakId, scaled maxWidth)
{
  if (!HasDirtyLayout(id, maxWidth)) return;

  MathMLElement* num   = content.GetFirst();
  MathMLElement* denom = content.GetLast();
  assert(num != NULL && denom != NULL);

  if (bevelled) {
    // the fraction is bevelled
    num->DoBoxedLayout(id, BREAK_NO, maxWidth / 3);
    denom->DoBoxedLayout(id, BREAK_NO, maxWidth / 3);

    const BoundingBox& numBox   = num->GetBoundingBox();
    const BoundingBox& denomBox = denom->GetBoundingBox();

    scaled barVert = scaledMax(numBox.GetHeight(), denomBox.GetHeight());
    scaled barHoriz = barVert / 2;

    box.Set(barHoriz + 2 * lineThickness, 0, 0);
    box.Append(numBox);
    box.Append(denomBox);
  } else {
    num->DoBoxedLayout(id, BREAK_NO, maxWidth);
    denom->DoBoxedLayout(id, BREAK_NO, maxWidth);

    const BoundingBox& numBox   = num->GetBoundingBox();
    const BoundingBox& denomBox = denom->GetBoundingBox();

#ifdef TEXISH_MATHML
    scaled u = numMinShift;
    scaled v = denomMinShift;
#else
    scaled u = minShift;
    scaled v = minShift;
#endif // TEXISH_MATHML

    if (lineThickness < EPSILON) {
#ifdef TEXISH_MATHML
      scaled psi = (displayStyle ? 7 : 3) * defaultRuleThickness;
#else
      scaled psi = displayStyle ? 3 * lineThickness : lineThickness;
#endif // TEXISH_MATHML
      scaled phi = (u - numBox.descent) - (denomBox.ascent - v);

      if (psi < phi) {
	u += (phi - psi) / 2;
	v += (phi - psi) / 2;
      }
    } else {
      scaled phi = displayStyle ? 3 * lineThickness : lineThickness;

      scaled diff = phi - ((u - numBox.descent) - (axis + lineThickness / 2));
      if (diff > 0) u += diff;

      diff = phi - ((axis - lineThickness / 2) - (denomBox.ascent - v));
      if (diff > 0) v += diff;
    }

    numShift   = u;
    denomShift = v;
    
    box.Set(scaledMax(numBox.width, denomBox.width),
	    numShift + numBox.ascent,
	    denomShift + denomBox.descent);
    box.tAscent = numShift + numBox.tAscent;
    box.tDescent = denomShift + denomBox.tDescent;
    box.rBearing = scaledMax(numBox.rBearing, denomBox.rBearing);
    box.width = scaledMax(box.width, box.rBearing);
  }

  ConfirmLayout(id);

  ResetDirtyLayout(id, maxWidth);
}

void
MathMLFractionElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  MathMLElement* num   = content.GetFirst();
  MathMLElement* denom = content.GetLast();
  assert(num != NULL && denom != NULL);

  const BoundingBox& box      = GetBoundingBox();
  const BoundingBox& numBox   = num->GetBoundingBox();
  const BoundingBox& denomBox = denom->GetBoundingBox();

  if (bevelled) {
    scaled barVert = scaledMax(numBox.GetHeight(), denomBox.GetHeight());
    scaled barHoriz = barVert / 2;

    num->SetPosition(x, y);
    denom->SetPosition(x + numBox.width + barHoriz + 2 * lineThickness, y);
  } else {
    scaled numXOffset = 0;
    switch (numAlign) {
    case FRAC_ALIGN_CENTER:
      numXOffset = (box.width - scaledMax(numBox.width, numBox.rBearing)) / 2;
      break;
    case FRAC_ALIGN_RIGHT:
      numXOffset = box.width - numBox.width;
      break;
    default:
      numXOffset = 0;
      break;
    }

    scaled denomXOffset = 0;
    switch (denomAlign) {
    case FRAC_ALIGN_CENTER:
      denomXOffset = (box.width - denomBox.width) / 2 - scaledMax(0, denomBox.rBearing - denomBox.width);
      break;
    case FRAC_ALIGN_RIGHT:
      denomXOffset = box.width - denomBox.width;
      break;
    default:
      denomXOffset = 0;
      break;
    }

    num->SetPosition(x + numXOffset, y - numShift);
    denom->SetPosition(x + denomXOffset, y + denomShift);
  }
}

void
MathMLFractionElement::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;

  MathMLContainerElement::Render(area);

  if (fGC[IsSelected()] == NULL) {
    GraphicsContextValues values;
    values.foreground = IsSelected() ? area.GetSelectionForeground() : color;
    values.lineWidth = lineThickness;
    fGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_LINE_WIDTH);
  }

  if (lineThickness > SP_EPSILON) {
    if (bevelled) {
      MathMLElement* num   = content.GetFirst();
      MathMLElement* denom = content.GetLast();
      assert(num != NULL && denom != NULL);

      const BoundingBox& numBox   = num->GetBoundingBox();
      const BoundingBox& denomBox = denom->GetBoundingBox();

      scaled barVert = scaledMax(numBox.GetHeight(), denomBox.GetHeight());
      scaled barHoriz = barVert / 2;

      area.DrawLine(fGC[IsSelected()],
		    GetX() + numBox.width + lineThickness,
		    GetY() + scaledMax(numBox.descent, denomBox.descent),
		    GetX() + numBox.width + lineThickness + barHoriz,
		    GetY() - scaledMax(numBox.ascent, denomBox.ascent));
    } else {
      area.DrawLine(fGC[IsSelected()], GetX(), GetY() - axis,
		    GetX() + box.width, GetY() - axis);
    }
  }

  ResetDirty();
}

bool
MathMLFractionElement::IsExpanding() const
{
  MathMLElement* num   = content.GetFirst();
  MathMLElement* denom = content.GetLast();
  assert(num != NULL && denom != NULL);

  if (num->IsExpanding()) return true;
  if (denom->IsExpanding()) return true;

  return false;
}
