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

#include "Globals.hh"
#include "ChildList.hh"
#include "StringUnicode.hh"
#include "ValueConversion.hh"
#include "MathMLDummyElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLFractionElement.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"

MathMLFractionElement::MathMLFractionElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLFractionElement::MathMLFractionElement(const DOM::Element& node)
  : MathMLContainerElement(node)
{
}
#endif

MathMLFractionElement::~MathMLFractionElement()
{
  SetNumerator(0);
  SetDenominator(0);
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
MathMLFractionElement::SetNumerator(const Ptr<MathMLElement>& elem)
{
  if (elem != numerator)
    {
      if (numerator) numerator->Unlink();
      if (elem) elem->Link(this);
      numerator = elem;
      SetDirtyLayout();
    }
}

void
MathMLFractionElement::SetDenominator(const Ptr<MathMLElement>& elem)
{
  if (elem != denominator)
    {
      if (denominator) denominator->Unlink();
      if (elem) elem->Link(this);
      denominator = elem;
      SetDirtyLayout();
    }
}

void
MathMLFractionElement::Replace(const Ptr<MathMLElement>& oldElem, const Ptr<MathMLElement>& newElem)
{
  assert(oldElem);
  if (oldElem == numerator) SetNumerator(newElem);
  else if (oldElem == denominator) SetDenominator(newElem);
  else assert(false);
}

void
MathMLFractionElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  assert(IsA() == TAG_MFRAC);
	  ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
	  unsigned n = children.get_length();

	  if (n > 0)
	    SetNumerator(doc->getFormattingNode(children.item(0)));
	  else if (!numerator || !is_a<MathMLDummyElement>(numerator))
	    SetNumerator(MathMLDummyElement::create());

	  if (n > 1)
	    SetDenominator(doc->getFormattingNode(children.item(1)));
	  else if (!denominator || !is_a<MathMLDummyElement>(denominator))
	    SetDenominator(MathMLDummyElement::create());
	}
#endif

      if (numerator) numerator->Normalize(doc);
      if (denominator) denominator->Normalize(doc);

      ResetDirtyStructure();
    }
}

void
MathMLFractionElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      color = env.GetColor();
      background = env.GetBackgroundColor();

      const Value* value = NULL;

#ifdef TEXISH_MATHML
      defaultRuleThickness = env.GetRuleThickness();
#else
      scaled defaultRuleThickness = env.GetRuleThickness();
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

	  if (unit->IsEmpty())
	    lineThickness = defaultRuleThickness * roundFloat(number->ToNumber());
	  else
	    {
	      assert(unit->IsKeyword());
	      UnitId unitId = ToUnitId(unit);
	      if (unitId == UNIT_PERCENTAGE)
		lineThickness = defaultRuleThickness * number->ToNumber() / 100;
	      else
		{
		  UnitValue unitValue;
		  unitValue.Set(number->ToNumber(), unitId);
		  lineThickness = env.ToScaledPoints(unitValue);
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

      color = env.GetColor();

      axis = env.GetAxis();

      displayStyle = env.GetDisplayStyle();

#ifdef TEXISH_MATHML
      if (displayStyle) {
	numMinShift = float2sp(sp2float(env.GetFontAttributes().size.ToScaledPoints()) * 0.676508);
	denomMinShift = float2sp(sp2float(env.GetFontAttributes().size.ToScaledPoints()) * 0.685951);
      } else {
	numMinShift = float2sp(sp2float(env.GetFontAttributes().size.ToScaledPoints()) * (lineThickness > 0 ? 0.393732 : 0.443731));
	denomMinShift = float2sp(sp2float(env.GetFontAttributes().size.ToScaledPoints()) * 0.344841);
      }
#else
      minShift = env.GetScaledPointsPerEx();
#endif // TEXISH_MATHML

      env.Push();
      if (!displayStyle) env.AddScriptLevel(1);
      else env.SetDisplayStyle(false);

      if (numerator) numerator->Setup(env);
      if (denominator) denominator->Setup(env);

      env.Drop();
      ResetDirtyAttribute();
    }
}

void
MathMLFractionElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      assert(numerator && denominator);
      numerator->DoLayout(ctxt);
      denominator->DoLayout(ctxt);

      const BoundingBox& numBox   = numerator->GetBoundingBox();
      const BoundingBox& denomBox = denominator->GetBoundingBox();

      if (bevelled) {
	scaled barVert = scaledMax(numBox.GetHeight(), denomBox.GetHeight());
	scaled barHoriz = barVert / 2;

	box.Set(barHoriz + 2 * lineThickness, 0, 0);
	box.Append(numBox);
	box.Append(denomBox);
      } else {
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
	box.rBearing = scaledMax(numBox.rBearing, denomBox.rBearing);
	box.width = scaledMax(box.width, box.rBearing);
      }

      DoEmbellishmentLayout(this, box);

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLFractionElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  SetEmbellishmentPosition(this, x, y);

  assert(numerator && denominator);

  const BoundingBox& box      = GetBoundingBox();
  const BoundingBox& numBox   = numerator->GetBoundingBox();
  const BoundingBox& denomBox = denominator->GetBoundingBox();

  if (bevelled) {
    scaled barVert = scaledMax(numBox.GetHeight(), denomBox.GetHeight());
    scaled barHoriz = barVert / 2;

    numerator->SetPosition(x, y);
    denominator->SetPosition(x + numBox.width + barHoriz + 2 * lineThickness, y);
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

    numerator->SetPosition(x + numXOffset, y - numShift);
    denominator->SetPosition(x + denomXOffset, y + denomShift);
  }
}

void
MathMLFractionElement::Render(const DrawingArea& area)
{
  if (Dirty())
    {
      if (fGC[Selected()] == NULL)
	{
	  GraphicsContextValues values;
	  values.foreground = Selected() ? area.GetSelectionForeground() : color;
	  values.lineWidth = lineThickness;
	  fGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_LINE_WIDTH);
	}

      RenderBackground(area);

      assert(numerator && denominator);
      numerator->Render(area);
      denominator->Render(area);

      if (lineThickness > SP_EPSILON)
	{
	  if (bevelled)
	    {
	      const BoundingBox& numBox   = numerator->GetBoundingBox();
	      const BoundingBox& denomBox = denominator->GetBoundingBox();

	      scaled barVert = scaledMax(numBox.GetHeight(), denomBox.GetHeight());
	      scaled barHoriz = barVert / 2;

	      area.DrawLine(fGC[Selected()],
			    GetX() + numBox.width + lineThickness,
			    GetY() + scaledMax(numBox.descent, denomBox.descent),
			    GetX() + numBox.width + lineThickness + barHoriz,
			    GetY() - scaledMax(numBox.ascent, denomBox.ascent));
	    }
	  else
	    {
	      area.DrawLine(fGC[Selected()], GetX(), GetY() - axis,
			    GetX() + box.width, GetY() - axis);
	    }
	}

      ResetDirty();
    }
}

#if 0
void
MathMLFractionElement::SetDirty(const Rectangle* rect)
{
  if (!IsDirty() && !HasDirtyChildren())
    {
      MathMLElement::SetDirty(rect);
      if (numerator) numerator->SetDirty(rect);
      if (denominator) denominator->SetDirty(rect);
    }
}

void
MathMLFractionElement::SetDirtyLayout(bool children)
{
  MathMLElement::SetDirtyLayout(children);
  if (children)
    {
      if (numerator) numerator->SetDirtyLayout(children);
      if (denominator) denominator->SetDirtyLayout(children);
    }
}

void
MathMLFractionElement::SetSelected()
{
  MathMLContainerElement::SetSelected();
  if (numerator) numerator->SetSelected();
  if (denominator) denominator->SetSelected();
}

void
MathMLFractionElement::ResetSelected()
{
  MathMLContainerElement::ResetSelected();
  if (numerator) numerator->ResetSelected();
  if (denominator) denominator->ResetSelected();
}
#endif

void
MathMLFractionElement::SetFlagDown(Flags f)
{
  MathMLElement::SetFlagDown(f);
  if (numerator) numerator->SetFlagDown(f);
  if (denominator) denominator->SetFlagDown(f);
}

void
MathMLFractionElement::ResetFlagDown(Flags f)
{
  MathMLElement::ResetFlagDown(f);
  if (numerator) numerator->ResetFlagDown(f);
  if (denominator) denominator->ResetFlagDown(f);
}

scaled
MathMLFractionElement::GetLeftEdge() const
{
  return 0;
}

scaled
MathMLFractionElement::GetRightEdge() const
{
  return box.width;
}

void
MathMLFractionElement::ReleaseGCs()
{
  MathMLElement::ReleaseGCs();
  if (numerator) numerator->ReleaseGCs();
  if (denominator) denominator->ReleaseGCs();
}

Ptr<MathMLElement>
MathMLFractionElement::Inside(scaled x, scaled y)
{
  if (!IsInside(x, y)) return 0;

  Ptr<MathMLElement> inside = 0;
  if (numerator && (inside = numerator->Inside(x, y))) return inside;
  if (denominator && (inside = denominator->Inside(x, y))) return inside;

  return this;
}

Ptr<MathMLOperatorElement>
MathMLFractionElement::GetCoreOperator()
{
  if (numerator) return numerator->GetCoreOperator();
  else return 0;
}
