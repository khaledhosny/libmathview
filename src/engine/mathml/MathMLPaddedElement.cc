// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "defs.h"
#include "ValueConversion.hh"
#include "MathMLValueConversion.hh"
#include "MathMLPaddedElement.hh"
#include "MathMLOperatorElement.hh"
#include "Variant.hh"
#include "MathMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLPaddedElement::MathMLPaddedElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLNormalizingContainerElement(context)
{ }

MathMLPaddedElement::~MathMLPaddedElement()
{ }

AreaRef
MathMLPaddedElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      if (AreaRef childArea = getChild() ? getChild()->format(ctxt) : 0)
	{
	  const BoundingBox childBox = childArea->box();
	  const scaled lspace = evalLengthDimension(ctxt, GET_ATTRIBUTE_VALUE(MathML, Padded, lspace), T_LSPACE, scaled::zero(), childBox);
	  const scaled width = evalLengthDimension(ctxt, GET_ATTRIBUTE_VALUE(MathML, Padded, width), T_WIDTH, childBox.width, childBox);
	  const scaled height = evalLengthDimension(ctxt, GET_ATTRIBUTE_VALUE(MathML, Padded, height), T_HEIGHT, childBox.height, childBox);
	  const scaled depth = evalLengthDimension(ctxt, GET_ATTRIBUTE_VALUE(MathML, Padded, depth), T_DEPTH, childBox.depth, childBox);

	  AreaRef res;
	  if (lspace != scaled::zero())
	    {
	      std::vector<AreaRef> h;
	      h.reserve(2);
	      h.push_back(ctxt.MGD()->getFactory()->horizontalSpace(lspace));
	      h.push_back(childArea);
	      res = ctxt.MGD()->getFactory()->horizontalArray(h);
	    }
	  else
	    res = childArea;

	  res = ctxt.MGD()->getFactory()->box(res, BoundingBox(width, height, depth));
	  res = formatEmbellishment(this, ctxt, res);
	  res = ctxt.MGD()->wrapper(ctxt, res);

	  setArea(res);
	}
      else
	setArea(0);

      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLPaddedElement::parseLengthDimension(const FormattingContext& ctxt,
					  const SmartPtr<Value>& value,
					  LengthDimension& dim,
					  TokenId pseudoUnitId)
{
  dim.valid = false;

  if (!value) return;

  SmartPtr<ValueSequence> seq = ToSequence(value);
  assert(seq);
  assert(seq->getSize() == 3);

  if (SmartPtr<Value> v = seq->getValue(0))
    if (IsTokenId(v))
      switch (ToTokenId(v))
	{
	case T__PLUS:  dim.sign = +1; break;
	case T__MINUS: dim.sign = -1; break;
	default: assert(false); break;
	}
    else
      dim.sign = 0;
  else
    assert(false);
  
  if (SmartPtr<Value> v = seq->getValue(1))
    dim.number = ToNumber(v);
  else
    assert(false);

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
		dim.unit = ctxt.MGD()->evaluate(ctxt, Length(1.0, unitId), scaled::zero());
	      else
		dim.unit = ctxt.MGD()->evaluate(ctxt, toLength(v, ctxt), scaled::zero());
	    }
	}
    }
  
  dim.valid = true;
}

scaled
MathMLPaddedElement::evalLengthDimension(const FormattingContext& ctxt,
					 const SmartPtr<Value>& value,
					 TokenId pseudoUnitId,
					 const scaled& orig,
					 const BoundingBox& b)
{
  LengthDimension dim;
  parseLengthDimension(ctxt, value, dim, pseudoUnitId);

  if (!dim.valid) return orig;
  
  float f = dim.number;
  if (dim.percentage) f *= 0.01f;

  scaled res;

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
MathMLPaddedElement::getCoreOperator()
{
  return getChild() ? getChild()->getCoreOperator() : 0;
}
