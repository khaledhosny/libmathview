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

#include "Globals.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLRowElement.hh"
#include "ValueConversion.hh"
#include "scaledConv.hh"
#include "traverseAux.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "AttributeList.hh"
#include "MathMLAttributeSignatures.hh"

#define GET_OPERATOR_ATTRIBUTE_VALUE(ns,el,name,def) getOperatorAttributeValue(ATTRIBUTE_SIGNATURE(ns,el,name),def)

MathMLOperatorElement::MathMLOperatorElement(const SmartPtr<MathMLView>& view)
  : MathMLTokenElement(view)
{
  fence = separator = stretchy = symmetric = infiniteMaxSize = accent = movableLimits = 0;
  forcedFence = forcedSeparator = forcedSymmetric = 0;
}

MathMLOperatorElement::~MathMLOperatorElement()
{
}

void
MathMLOperatorElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, Operator, form);
      REFINE_ATTRIBUTE(context, MathML, Operator, fence);
      REFINE_ATTRIBUTE(context, MathML, Operator, separator);
      REFINE_ATTRIBUTE(context, MathML, Operator, lspace);
      REFINE_ATTRIBUTE(context, MathML, Operator, rspace);
      REFINE_ATTRIBUTE(context, MathML, Operator, stretchy);
      REFINE_ATTRIBUTE(context, MathML, Operator, symmetric);
      REFINE_ATTRIBUTE(context, MathML, Operator, maxsize);
      REFINE_ATTRIBUTE(context, MathML, Operator, minsize);
      REFINE_ATTRIBUTE(context, MathML, Operator, largeop);
      REFINE_ATTRIBUTE(context, MathML, Operator, movablelimits);
      REFINE_ATTRIBUTE(context, MathML, Operator, accent);
      MathMLTokenElement::refine(context);
    }
}

#include "scaledAux.hh"
#include "BoundingBoxAux.hh"

AreaRef
MathMLOperatorElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      
      TokenId form = T__NOTVALID;
      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, Operator, form))
	form = ToTokenId(value);
      else
	form = inferOperatorForm();

      SmartPtr<AttributeList> prefix;
      SmartPtr<AttributeList> infix;
      SmartPtr<AttributeList> postfix;

      String operatorName = GetRawContent();
      Globals::dictionary.Search(operatorName, prefix, infix, postfix);

      SmartPtr<AttributeList> defaults;
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
	  SmartPtr<Value> resValue = Resolve(value, ctxt);
	  if (ctxt.getScriptLevel() <= 0)
	    lSpace = ctxt.getDevice()->evaluate(ctxt, ToLength(resValue), scaled::zero());
	  else
	    lSpace = scaled::zero();
	}
      else
	assert(false);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(MathML, Operator, rspace, defaults))
	{
	  SmartPtr<Value> resValue = Resolve(value, ctxt);
	  if (ctxt.getScriptLevel() <= 0)
	    rSpace = ctxt.getDevice()->evaluate(ctxt, ToLength(resValue), scaled::zero());
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

	  std::cerr << "FOUND STRETCHY OP" << std::endl;

	  // before stretchying the operator need to be formatted at
	  // least once
	  AreaRef minArea = getArea();
	  assert(minArea);

	  const BoundingBox minBox = minArea->box();

	  std::cerr << "minimum area bounding box = " << minBox << std::endl;

	  const scaled axis = ctxt.getDevice()->axis(ctxt);
	  const scaled height = ctxt.getStretchToHeight() - axis;
	  const scaled depth = ctxt.getStretchToDepth() + axis;

	  // Here we have to calculate the desired size of the stretchable symbol.
	  // If symmetric == true the we have to stretch to cover the maximum among
	  // height and depth, otherwise we just stretch to ascent + descent
	  scaled v = std::max(scaled::zero(), symmetric ? (2 * std::max(height, depth)) : (height + depth));
	  scaled h = std::max(scaled::zero(), ctxt.getStretchToWidth());

	  std::cerr << "desired V = " << v << " H = " << h << std::endl;

	  // ...however, there may be some contraints over the size of the stretchable
	  // operator. 
	  const scaled minV = minBox.height + minBox.depth;
	  const scaled minH = minBox.width;
	  
	  std::cerr << "minV = " << minV << " minH = " << minH << std::endl;
	  std::cerr << "minSize = " << minSize << " maxMult = " << minMultiplier << std::endl;

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

	  std::cerr << "after min constraint v = " << v << " h = " << h << std::endl;
	  std::cerr << "maxSize = " << maxSize << " maxMult = " << maxMultiplier << std::endl;

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

	  std::cerr << "after max constraint v = " << v << " h = " << h << std::endl;

	  ctxt.setStretchV(v);
	  ctxt.setStretchH(h);
	  
	  std::cerr << "stretch by V = " << v << " H = " << h << std::endl;

	  res = MathMLTokenElement::format(ctxt);
	  
	  BoundingBox opBox = res->box();

	  if (height + depth > scaled::zero())
	    {
	      const scaled aHeight = (symmetric ? v / 2 : (v * height.toFloat()) / (height + depth).toFloat()) + axis;
	      const scaled aDepth = (symmetric ? v / 2 : (v * depth.toFloat()) / (height + depth).toFloat()) - axis;
	      
	      const scaled sh = (aHeight - aDepth - opBox.height + opBox.depth) / 2;

	      res = ctxt.getDevice()->getFactory()->shift(res, sh);
	    }
	}
      else
	res = MathMLTokenElement::format(ctxt);

      // the wrapper?
      res = formatEmbellishment(this, ctxt, res);

      ctxt.pop();

      setArea(res);

      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLOperatorElement::parseLimitValue(const SmartPtr<Value>& value,
				       const MathFormattingContext& ctxt,
				       float& multiplier,
				       scaled& size)
{
  assert(value);

  if (IsTokenId(value))
    { // it must be a named math space
      SmartPtr<Value> resValue = Resolve(value, ctxt);
      multiplier = -1;
      size = ctxt.getDevice()->evaluate(ctxt, ToLength(resValue), scaled::zero());
    }
  else if (IsNumber(value))
    multiplier = std::max(EPSILON, ToNumber(value));
  else
    {
      assert(IsLength(value));
      Length siz = ToLength(value);
      
      if (siz.type == Length::PERCENTAGE_UNIT)
	{
	  Globals::logger(LOG_WARNING, "percentage value specified in maxsize attribute (mo) (ignored)");
	  multiplier = std::max(EPSILON, siz.value);
	} 
      else
	{
	  multiplier = -1;
	  size = ctxt.getDevice()->evaluate(ctxt, siz, scaled::zero());
	}
    }
}

SmartPtr<Value>
MathMLOperatorElement::getOperatorAttributeValue(const AttributeSignature& signature,
						 const SmartPtr<AttributeList>& defaults) const
{
  // 1st attempt, the attribute may be set for the current operator
  if (SmartPtr<Value> value = getAttributeValueNoDefault(signature))
    return value;

  if (defaults)
    {
      // no, it is not explicitly set, but this operator has an entry in
      // the operator dictionary, so let's see if the attribute has a
      // default value
      if (SmartPtr<Value> value = defaults->getValue(ATTRIBUTE_ID_OF_SIGNATURE(signature)))
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
  SmartPtr<MathMLElement> elem = eOp->GetParent();
  assert(elem);

  TokenId res = T_INFIX;

  if (elem->IsA() == T_MROW)
    {
      SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(elem);
      assert(row);
      res = row->GetOperatorForm(eOp);
    }

  return res;
}

SmartPtr<MathMLOperatorElement>
MathMLOperatorElement::getCoreOperator()
{
  return this;
}
