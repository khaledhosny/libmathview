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

#include "defs.h" // for EPSILON
#include "AreaFactory.hh"
#include "AbstractLogger.hh"
#include "NamespaceContext.hh"
#include "View.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLOperatorDictionary.hh"
#include "MathMLRowElement.hh"
#include "MathMLValueConversion.hh"
#include "traverseAux.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "AttributeSet.hh"
#include "MathMLAttributeSignatures.hh"

#define GET_OPERATOR_ATTRIBUTE_VALUE(ns,el,name,def) getOperatorAttributeValue(ATTRIBUTE_SIGNATURE(ns,el,name),def)

MathMLOperatorElement::MathMLOperatorElement(const SmartPtr<MathMLNamespaceContext>& context)
  : MathMLTokenElement(context)
{
  fence = separator = stretchy = symmetric = infiniteMaxSize = accent = movableLimits = 0;
  forcedFence = forcedSeparator = forcedSymmetric = 0;
}

MathMLOperatorElement::~MathMLOperatorElement()
{ }

// #include <iostream>
// #include "scaledAux.hh"
// #include "BoundingBoxAux.hh"

AreaRef
MathMLOperatorElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      
      TokenId form = T__NOTVALID;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Operator, form))
	form = ToTokenId(value);
      else
	form = inferOperatorForm();

      SmartPtr<AttributeSet> prefix;
      SmartPtr<AttributeSet> infix;
      SmartPtr<AttributeSet> postfix;

      String operatorName = GetRawContent();
      if (SmartPtr<MathMLOperatorDictionary> dictionary = getNamespaceContext()->getView()->getOperatorDictionary())
	dictionary->search(operatorName, prefix, infix, postfix);

      SmartPtr<AttributeSet> defaults;
      if      (form == T_PREFIX && prefix) defaults = prefix;
      else if (form == T_INFIX && infix) defaults = infix;
      else if (form == T_POSTFIX && postfix) defaults = postfix;
      else if (infix) defaults = infix;
      else if (postfix) defaults = postfix;
      else if (prefix) defaults = prefix;
      else defaults = 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, fence, defaults))
	fence = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, separator, defaults))
	separator = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, lspace, defaults))
	{
	  if (ctxt.getScriptLevel() <= 0)
	    lSpace = ctxt.MGD()->evaluate(ctxt, resolveLength(ctxt, value), scaled::zero());
	  else
	    lSpace = scaled::zero();
	}
      else
	assert(false);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, rspace, defaults))
	{
	  if (ctxt.getScriptLevel() <= 0)
	    rSpace = ctxt.MGD()->evaluate(ctxt, resolveLength(ctxt, value), scaled::zero());
	  else
	    rSpace = scaled::zero();
	}
      else
	assert(false);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, stretchy, defaults))
	stretchy = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, symmetric, defaults))
	symmetric = ToBoolean(value) ? 1 : 0;

      float maxMultiplier = 0.0f;
      scaled maxSize;
      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, maxsize, defaults))
	if (ToTokenId(value) == T_INFINITY)
	  maxSize = scaled::max();
	else
	  parseLimitValue(value, ctxt, maxMultiplier, maxSize);

      float minMultiplier = 0.0f;
      scaled minSize;
      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, minsize, defaults))
	parseLimitValue(value, ctxt, minMultiplier, minSize);
      else
	assert(false);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, movablelimits, defaults))
	movableLimits = ToBoolean(value) ? 1 : 0;
      else
	assert(false);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, accent, defaults))
	accent = ToBoolean(value) ? 1 : 0;
      else
	assert(false);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, largeop, defaults))
	largeOp = ToBoolean(value) ? 1 : 0;

      AreaRef res;
      if (stretchy && this == ctxt.getStretchOperator())
	{
	  // is it a good place to handle largeOp here???

	  //std::cerr << "FOUND STRETCHY OP" << std::endl;

	  // before stretchying the operator need to be formatted at
	  // least once
	  AreaRef minArea = getArea();
	  assert(minArea);

	  const BoundingBox minBox = minArea->box();

	  const scaled axis = ctxt.MGD()->axis(ctxt);
	  const scaled height = ctxt.getStretchToHeight() - axis;
	  const scaled depth = ctxt.getStretchToDepth() + axis;

	  // std::cerr << "minimum area bounding box = " << minBox << " height = " << height + axis << " axis = " << axis << std::endl;

	  // Here we have to calculate the desired size of the stretchable symbol.
	  // If symmetric == true the we have to stretch to cover the maximum among
	  // height and depth, otherwise we just stretch to ascent + descent
	  scaled v = std::max(scaled::zero(), symmetric ? (2 * std::max(height, depth)) : (height + depth));
	  scaled h = std::max(scaled::zero(), ctxt.getStretchToWidth());

	  // std::cerr << "desired V = " << v << " H = " << h << std::endl;

	  // ...however, there may be some contraints over the size of the stretchable
	  // operator. 
	  const scaled minV = minBox.height + minBox.depth;
	  const scaled minH = minBox.width;
	  
	  // std::cerr << "minV = " << minV << " minH = " << minH << std::endl;
	  // std::cerr << "minSize = " << minSize << " maxMult = " << minMultiplier << std::endl;

	  if (minMultiplier > 0)
	    {
	      v = std::max(v, minV * minMultiplier);
	      h = std::max(h, minH * minMultiplier);
	    }
	  else
	    {
	      v = std::max(v, minSize);
	      h = std::max(h, minSize);
	    }

	  // std::cerr << "after min constraint v = " << v << " h = " << h << std::endl;
	  // std::cerr << "maxSize = " << maxSize << " maxMult = " << maxMultiplier << std::endl;

	  if (maxMultiplier > 0)
	    {
	      v = std::min(v, minV * maxMultiplier);
	      h = std::min(h, minH * maxMultiplier);
	    }
	  else
	    {
	      v = std::min(v, maxSize);
	      h = std::min(h, maxSize);
	    }

	  // std::cerr << "after max constraint v = " << v << " h = " << h << std::endl;

	  ctxt.setStretchV(v);
	  ctxt.setStretchH(h);
	  
	  // std::cerr << "stretch by V = " << v << " H = " << h << std::endl;

	  res = formatAux(ctxt);
	  
	  BoundingBox opBox = res->box();

	  if (height + depth > scaled::zero())
	    {
	      const scaled aHeight = (symmetric ? v / 2 : (v * height.toFloat()) / (height + depth).toFloat()) + axis;
	      const scaled aDepth = (symmetric ? v / 2 : (v * depth.toFloat()) / (height + depth).toFloat()) - axis;
	      
	      const scaled sh = (aHeight - aDepth - opBox.height + opBox.depth) / 2;

	      res = ctxt.MGD()->getFactory()->shift(res, sh);
	    }
	}
      else
	res = formatAux(ctxt);

      //std::cerr << "formatting operator, is top? " << (getCoreOperatorTop() != 0) << " has dirty something" << dirtyAttribute() << dirtyLayout() << std::endl;

      res = formatEmbellishment(this, ctxt, res);
      setArea(ctxt.MGD()->wrapper(ctxt, res));

      ctxt.pop();

      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLOperatorElement::parseLimitValue(const SmartPtr<Value>& value,
				       const FormattingContext& ctxt,
				       float& multiplier,
				       scaled& size)
{
  assert(value);

  if (IsTokenId(value))
    { // it must be a named math space
      multiplier = -1;
      size = ctxt.MGD()->evaluate(ctxt, resolveLength(ctxt, value), scaled::zero());
    }
  else if (IsNumber(value))
    multiplier = std::max(EPSILON, ToNumber(value));
  else
    {
      assert(IsLength(value));
      Length siz = ToLength(value);
      
      if (siz.type == Length::PERCENTAGE_UNIT)
	{
	  getLogger()->out(LOG_WARNING, "percentage value specified in maxsize attribute (mo) (ignored)");
	  multiplier = std::max(EPSILON, siz.value);
	} 
      else
	{
	  multiplier = -1;
	  size = ctxt.MGD()->evaluate(ctxt, siz, scaled::zero());
	}
    }
}

SmartPtr<Value>
MathMLOperatorElement::getOperatorAttributeValue(const AttributeSignature& signature,
						 const SmartPtr<AttributeSet>& defaults) const
{
  // 1st attempt, the attribute may be set for the current operator
  if (SmartPtr<Value> value = getAttributeValueNoDefault(signature))
    return value;

  if (defaults)
    {
      // no, it is not explicitly set, but this operator has an entry in
      // the operator dictionary, so let's see if the attribute has a
      // default value
      if (SmartPtr<Attribute> attr = defaults->get(ATTRIBUTE_ID_OF_SIGNATURE(signature)))
	if (SmartPtr<Value> value = attr->getValue())
	  return value;
    }

  // if the attribute hasn't still a value, then take its default
  // for the mo element
  if (SmartPtr<Value> value = signature.getDefaultValue())
    return value;
  
  assert(false);
  return 0;
}

TokenId
MathMLOperatorElement::inferOperatorForm()
{
  SmartPtr<MathMLElement> eOp = findEmbellishedOperatorRoot(this);
  assert(eOp);
  
  if (SmartPtr<MathMLRowElement> row = eOp->getParent<MathMLRowElement>())
    return row->GetOperatorForm(eOp);
  else
    return T_INFIX;
}

SmartPtr<MathMLOperatorElement>
MathMLOperatorElement::getCoreOperator()
{
  return this;
}
