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

#include <cassert>

#include "ValueConversion.hh"
#include "StringUnicode.hh"
#include "ValueConversion.hh"
#include "MathMLPaddedElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"
#include "Variant.hh"

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
MathMLPaddedElement::Normalize(const SmartPtr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
      MathMLNormalizingContainerElement::Normalize(doc);
      if (SmartPtr<MathMLOperatorElement> coreOp = GetCoreOperator())
	{
	  SmartPtr<MathMLEmbellishedOperatorElement> eOp = coreOp->GetEmbellishment();
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

      if (SmartPtr<Value> value = GetAttributeValue(ATTR_WIDTH, false))
	ParseLengthDimension(env, value, width, KW_WIDTH);

      if (SmartPtr<Value> value = GetAttributeValue(ATTR_LSPACE))
	ParseLengthDimension(env, value, lSpace, KW_LSPACE);

      if (SmartPtr<Value> value = GetAttributeValue(ATTR_HEIGHT, false))
	ParseLengthDimension(env, value, height, KW_HEIGHT);

      if (SmartPtr<Value> value = GetAttributeValue(ATTR_DEPTH, false))
	ParseLengthDimension(env, value, depth, KW_DEPTH);

      MathMLNormalizingContainerElement::Setup(env);
      ResetDirtyAttribute();
    }
}

void
MathMLPaddedElement::ParseLengthDimension(RenderingEnvironment& env,
					  const SmartPtr<Value>& value,
					  LengthDimension& dim,
					  KeywordId pseudoUnitId)
{
  assert(value);

  SmartPtr<ValueSequence> seq = ToSequence(value);
  assert(seq);
  assert(seq->getSize() == 3);

  if (SmartPtr<Value> v = seq->getValue(0))
    switch (ToKeywordId(v))
      {
      case KW_PLUS:  dim.sign = +1; break;
      case KW_MINUS: dim.sign = -1; break;
      default: dim.sign = 0; break;
      }
  else
    assert(IMPOSSIBLE);
  
  if (SmartPtr<Value> v = seq->getValue(1))
    dim.number = ToNumber(v);
  else
    assert(IMPOSSIBLE);
  
  if (SmartPtr<Value> v = seq->getValue(2))
    {
      if (SmartPtr<ValueSequence> vSeq = ToSequence(v))
	{
	  assert(vSeq->getSize() == 2);
	  dim.percentage = true;
	  v = vSeq->getValue(1);
	}
      else
	dim.percentage = false;

      if (IsEmpty(v))
	{
	  dim.pseudo = true;
	  dim.pseudoUnitId = pseudoUnitId;
	} 
      else
	{
	  assert(IsKeyword(v));

	  KeywordId id = ToKeywordId(v);
	  if (id == KW_WIDTH ||
	      id == KW_LSPACE ||
	      id == KW_HEIGHT ||
	      id == KW_DEPTH)
	    {
	      dim.pseudo = true;
	      dim.pseudoUnitId = id;
	    } 
	  else
	    {
	      dim.pseudo = false;

	      UnitId unitId = ToUnitId(v);
	      if (unitId != UNIT_NOTVALID)
		dim.unit = env.ToScaledPoints(UnitValue(1.0, unitId));
	      else
		{
		  MathSpaceId spaceId = ToNamedSpaceId(v);
		  dim.unit = env.ToScaledPoints(env.GetMathSpace(spaceId));
		}
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
      box.set(lSpaceE + EvalLengthDimension(elemBox.width, width, elemBox),
	      EvalLengthDimension(elemBox.height, height, elemBox),
	      EvalLengthDimension(elemBox.depth, depth, elemBox));

      DoEmbellishmentLayout(this, box);

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLPaddedElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (GetChild()) GetChild()->SetPosition(x + lSpaceE, y);
}

scaled
MathMLPaddedElement::EvalLengthDimension(const scaled& orig,
					 const LengthDimension& dim,
					 const BoundingBox& b) const
{
  if (!dim.valid) return orig;
  
  float f = dim.number;
  if (dim.percentage) f *= 0.01;

  scaled res = 0;

  if (dim.pseudo) {
    switch (dim.pseudoUnitId) {
    case KW_WIDTH: res = b.width * f; break;
    case KW_LSPACE: break; // LUCA: BoundingBox does not have a lspace length!!!
    case KW_HEIGHT: res = b.height * f; break;
    case KW_DEPTH: res = b.depth * f; break;
    default:
      assert(false);
      break;
    }
  } else {
    res = dim.unit * f;
  }

  if      (dim.sign == -1) return orig - res;
  else if (dim.sign == +1) return orig + res;
  else return res;
}

#if 0
void
MathMLPaddedElement::SetDirty(const Rectangle* rect)
{
  assert(child);
  child->SetDirty(rect);
}
#endif

SmartPtr<MathMLOperatorElement>
MathMLPaddedElement::GetCoreOperator()
{
  if (GetChild()) return GetChild()->GetCoreOperator();
  else return 0;
}
