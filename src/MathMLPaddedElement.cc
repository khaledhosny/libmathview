// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "defs.h"
#include "ValueConversion.hh"
#include "ValueConversion.hh"
#include "MathMLPaddedElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"
#include "Variant.hh"

MathMLPaddedElement::MathMLPaddedElement(const SmartPtr<class MathMLView>& view)
  : MathMLNormalizingContainerElement(view)
{
}

MathMLPaddedElement::~MathMLPaddedElement()
{
}

void
MathMLPaddedElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, Padded, width);
      REFINE_ATTRIBUTE(context, Padded, lspace);
      REFINE_ATTRIBUTE(context, Padded, height);
      REFINE_ATTRIBUTE(context, Padded, depth);
      MathMLNormalizingContainerElement::refine(context);
    }
}

void
MathMLPaddedElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      width.valid = lSpace.valid = height.valid = depth.valid = false;

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Padded, width))
	ParseLengthDimension(env, value, width, T_WIDTH);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Padded, lspace))
	ParseLengthDimension(env, value, lSpace, T_LSPACE);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Padded, height))
	ParseLengthDimension(env, value, height, T_HEIGHT);

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Padded, depth))
	ParseLengthDimension(env, value, depth, T_DEPTH);

      MathMLNormalizingContainerElement::Setup(env);
      ResetDirtyAttribute();
    }
}

void
MathMLPaddedElement::ParseLengthDimension(RenderingEnvironment& env,
					  const SmartPtr<Value>& value,
					  LengthDimension& dim,
					  TokenId pseudoUnitId)
{
  assert(value);

  SmartPtr<ValueSequence> seq = ToSequence(value);
  assert(seq);
  assert(seq->getSize() == 3);

  if (SmartPtr<Value> v = seq->getValue(0))
    switch (ToTokenId(v))
      {
      case T__PLUS:  dim.sign = +1; break;
      case T__MINUS: dim.sign = -1; break;
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
	  assert(IsTokenId(v));

	  TokenId id = ToTokenId(v);
	  if (id == T_WIDTH ||
	      id == T_LSPACE ||
	      id == T_HEIGHT ||
	      id == T_DEPTH)
	    {
	      dim.pseudo = true;
	      dim.pseudoUnitId = id;
	    } 
	  else
	    {
	      dim.pseudo = false;

	      Length::Unit unitId = toUnitId(v);
	      if (unitId != Length::UNDEFINED_UNIT)
		dim.unit = env.ToScaledPoints(Length(1.0, unitId));
	      else
		{
		  RenderingEnvironment::MathSpaceId spaceId = RenderingEnvironment::mathSpaceIdOfTokenId(ToTokenId(v));
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
  if (dim.percentage) f *= 0.01f;

  scaled res = 0;

  if (dim.pseudo)
    {
    switch (dim.pseudoUnitId)
      {
      case T_WIDTH: res = b.width * f; break;
      case T_LSPACE: break; // LUCA: BoundingBox does not have a lspace length!!!
      case T_HEIGHT: res = b.height * f; break;
      case T_DEPTH: res = b.depth * f; break;
      default:
	assert(false);
	break;
      }
    } 
  else
    {
      res = dim.unit * f;
    }

  if      (dim.sign == -1) return orig - res;
  else if (dim.sign == +1) return orig + res;
  else return res;
}

SmartPtr<MathMLOperatorElement>
MathMLPaddedElement::GetCoreOperator()
{
  if (GetChild()) return GetChild()->GetCoreOperator();
  else return 0;
}
