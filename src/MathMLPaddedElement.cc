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

#include "ValueSequence.hh"
#include "StringUnicode.hh"
#include "ValueConversion.hh"
#include "MathMLPaddedElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"

MathMLPaddedElement::MathMLPaddedElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLPaddedElement::MathMLPaddedElement(const DOM::Element& node)
  : MathMLNormalizingContainerElement(node)
{
}
#endif

MathMLPaddedElement::~MathMLPaddedElement()
{
}

const AttributeSignature*
MathMLPaddedElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] =
  {
    { ATTR_WIDTH,    paddedWidthParser, NULL,               NULL },
    { ATTR_LSPACE,   paddedValueParser, new StringC("0em"), NULL },
    { ATTR_HEIGHT,   paddedValueParser, NULL,               NULL },
    { ATTR_DEPTH,    paddedValueParser, NULL,               NULL },
    { ATTR_NOTVALID, NULL,              NULL,               NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);  
  if (signature == NULL) signature = MathMLNormalizingContainerElement::GetAttributeSignature(id);

  return signature;
}

#if 0
void
MathMLPaddedElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
      MathMLNormalizingContainerElement::Normalize(doc);
      if (Ptr<MathMLOperatorElement> coreOp = GetCoreOperator())
	{
	  Ptr<MathMLEmbellishedOperatorElement> eOp = coreOp->GetEmbellishment();
	  assert(eOp && eOp->GetParent() == this);
	  eOp->Lift(doc);
	}
      ResetDirtyStructure();
    }
}
#endif

void
MathMLPaddedElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      width.valid = lSpace.valid = height.valid = depth.valid = false;

      const Value* value = GetAttributeValue(ATTR_WIDTH, false);
      if (value != NULL) ParseLengthDimension(env, value, width, KW_WIDTH);
      delete value;

      value = GetAttributeValue(ATTR_LSPACE);
      if (value != NULL) ParseLengthDimension(env, value, lSpace, KW_LSPACE);
      delete value;

      value = GetAttributeValue(ATTR_HEIGHT, false);
      if (value != NULL) ParseLengthDimension(env, value, height, KW_HEIGHT);
      delete value;

      value = GetAttributeValue(ATTR_DEPTH, false);
      if (value != NULL) ParseLengthDimension(env, value, depth, KW_DEPTH);
      delete value;

      MathMLNormalizingContainerElement::Setup(env);
      ResetDirtyAttribute();
    }
}

void
MathMLPaddedElement::ParseLengthDimension(RenderingEnvironment& env,
					  const Value* value,
					  LengthDimension& dim,
					  KeywordId pseudoUnitId)
{
  assert(value != NULL);

  assert(value->IsSequence());
  const ValueSequence* seq = value->ToValueSequence();
  assert(seq != NULL);
  assert(seq->GetSize() == 3);

  const Value* v = NULL;

  v = seq->GetFirstValue();
  assert(v != NULL);
  if      (v->IsKeyword(KW_PLUS)) dim.sign = +1;
  else if (v->IsKeyword(KW_MINUS)) dim.sign = -1;
  else dim.sign = 0;

  v = seq->GetValue(1);
  assert(v != NULL);
  assert(v->IsNumber());
  dim.number = v->ToNumber();

  v = seq->GetLastValue();
  assert(v != NULL);
  if (v->IsSequence()) {
    dim.percentage = true;

    const ValueSequence* seq = v->ToValueSequence();
    assert(seq != NULL);
    assert(seq->GetSize() == 2);
    
    v = seq->GetLastValue();
  } else
    dim.percentage = false;

  if (v->IsEmpty()) {
    dim.pseudo = true;
    dim.pseudoUnitId = pseudoUnitId;
  } else {
    assert(v->IsKeyword());

    KeywordId id = v->ToKeyword();
    if (id == KW_WIDTH ||
	id == KW_LSPACE ||
	id == KW_HEIGHT ||
	id == KW_DEPTH) {
      dim.pseudo = true;
      dim.pseudoUnitId = id;
    } else {
      dim.pseudo = false;

      UnitId unitId = ToUnitId(v);
      if (unitId != UNIT_NOTVALID) {
	UnitValue unitValue;
	unitValue.Set(1.0, unitId);
	dim.unit = env.ToScaledPoints(unitValue);
      } else {
	MathSpaceId spaceId = ToNamedSpaceId(v);
	dim.unit = env.ToScaledPoints(env.GetMathSpace(spaceId));
      }
    }
  }

  dim.valid = true;
}

void
MathMLPaddedElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      assert(child);
      child->DoLayout(ctxt);
      const BoundingBox& elemBox = child->GetBoundingBox();

      lSpaceE = EvalLengthDimension(0, lSpace, elemBox);
      box.Set(lSpaceE + EvalLengthDimension(elemBox.width, width, elemBox),
	      EvalLengthDimension(elemBox.ascent, height, elemBox),
	      EvalLengthDimension(elemBox.descent, depth, elemBox));

      DoEmbellishmentLayout(this, box);

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLPaddedElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (GetChild()) GetChild()->SetPosition(x + lSpaceE, y);
}

scaled
MathMLPaddedElement::EvalLengthDimension(scaled orig,
					 const LengthDimension& dim,
					 const BoundingBox& b) const
{
  if (!dim.valid) return orig;
  
  float res = dim.number;

  if (dim.percentage) res *= 0.01;

  if (dim.pseudo) {
    switch (dim.pseudoUnitId) {
    case KW_WIDTH: res *= b.width; break;
    case KW_LSPACE: res *= 0; break; // LUCA: BoundingBox does not have a lspace length!!!
    case KW_HEIGHT: res *= b.ascent; break;
    case KW_DEPTH: res *= b.descent; break;
    default:
      assert(IMPOSSIBLE);
      break;
    }
  } else {
    res *= dim.unit;
  }

  if      (dim.sign == -1) return orig - float2sp(res);
  else if (dim.sign == +1) return orig + float2sp(res);
  else return float2sp(res);
}

#if 0
void
MathMLPaddedElement::SetDirty(const Rectangle* rect)
{
  assert(child);
  child->SetDirty(rect);
}
#endif

Ptr<MathMLOperatorElement>
MathMLPaddedElement::GetCoreOperator()
{
  if (GetChild()) return GetChild()->GetCoreOperator();
  else return 0;
}
