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

#include "FormattingContext.hh"
#include "Globals.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLRowElement.hh"
#include "RenderingEnvironment.hh"
#include "ValueConversion.hh"
#include "operatorAux.hh"
#include "scaledConv.hh"
#include "traverseAux.hh"

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
  if (DirtyAttribute() || DirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, Operator, form);
      REFINE_ATTRIBUTE(context, Operator, fence);
      REFINE_ATTRIBUTE(context, Operator, separator);
      REFINE_ATTRIBUTE(context, Operator, lspace);
      REFINE_ATTRIBUTE(context, Operator, rspace);
      REFINE_ATTRIBUTE(context, Operator, stretchy);
      REFINE_ATTRIBUTE(context, Operator, symmetric);
      REFINE_ATTRIBUTE(context, Operator, maxsize);
      REFINE_ATTRIBUTE(context, Operator, minsize);
      REFINE_ATTRIBUTE(context, Operator, largeop);
      REFINE_ATTRIBUTE(context, Operator, movablelimits);
      REFINE_ATTRIBUTE(context, Operator, accent);
      MathMLTokenElement::refine(context);
    }
}

void
MathMLOperatorElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute())
    {
      axis = env.GetAxis();

      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Operator, form))
	form = ToFormId(value);
      else
	form = InferOperatorForm();

      SmartPtr<MathMLAttributeList> prefix;
      SmartPtr<MathMLAttributeList> infix;
      SmartPtr<MathMLAttributeList> postfix;

      String operatorName = GetRawContent();
      Globals::dictionary.Search(operatorName, prefix, infix, postfix);

      if      (form == OP_FORM_PREFIX && prefix) defaults = prefix;
      else if (form == OP_FORM_INFIX && infix) defaults = infix;
      else if (form == OP_FORM_POSTFIX && postfix) defaults = postfix;
      else if (infix) defaults = infix;
      else if (postfix) defaults = postfix;
      else if (prefix) defaults = prefix;
      else defaults = 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, fence))
	if (!ForcedFence()) fence = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, separator))
	if (!ForcedSeparator()) separator = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, lspace))
	{
	  SmartPtr<Value> resValue = Resolve(value, env);
	  if (env.GetScriptLevel() <= 0)
	    lSpace = env.ToScaledPoints(ToNumberUnit(resValue));
	  else
	    lSpace = 0;
	}
      else
	assert(IMPOSSIBLE);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, rspace))
	{
	  SmartPtr<Value> resValue = Resolve(value, env);
	  if (env.GetScriptLevel() <= 0)
	    rSpace = env.ToScaledPoints(ToNumberUnit(resValue));
	  else
	    rSpace = 0;
	}
      else
	assert(IMPOSSIBLE);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, stretchy))
	stretchy = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, symmetric))
	if (!ForcedSymmetric()) symmetric = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, maxsize))
	if (ToKeywordId(value) == KW_INFINITY)
	  infiniteMaxSize = 1;
	else
	  {
	    infiniteMaxSize = 0;
	    ParseLimitValue(value, env, maxMultiplier, maxSize);
	  }

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, minsize))
	ParseLimitValue(value, env, minMultiplier, minSize);
      else
	assert(IMPOSSIBLE);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, movablelimits))
	movableLimits = ToBoolean(value) ? 1 : 0;
      else
	assert(IMPOSSIBLE);

      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, accent))
	accent = ToBoolean(value) ? 1 : 0;
      else
	assert(IMPOSSIBLE);

      bool largeOp = false;
      if (SmartPtr<Value> value = GET_OPERATOR_ATTRIBUTE_VALUE(Operator, largeop))
	largeOp = ToBoolean(value);

      MathMLTokenElement::Setup(env);

#if 0
      if (GetSize() == 1 && largeOp && env.GetDisplayStyle())
	{
	  // WARNING: the fact that I'm using a local variable is probably due
	  // to a GCC bug. If the method is called directly the compiler
	  // reports an ambiguous overloading
          SmartPtr<MathMLTextNode> child = GetChild(0);
	  if (SmartPtr<MathMLCharNode> sNode = smart_cast<MathMLCharNode>(child))
	    if (sNode->IsStretchyChar()) sNode->SetDefaultLargeGlyph(true);
	}
#endif

      ResetDirtyAttribute();
    }
}

void
MathMLOperatorElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      MathMLTokenElement::DoLayout(ctxt);
      DoEmbellishmentLayout(this, box);
      if (ctxt.GetLayoutType() == LAYOUT_MIN) minBox = box;
      ResetDirtyLayout(ctxt);
    }
}

void
MathMLOperatorElement::VerticalStretchTo(const scaled& ascent, const scaled& descent, bool strict)
{
  assert(stretchy);

  scaled height = ascent - axis;
  scaled depth = descent + axis;

  scaled desiredSize = 0;

  // Here we have to calculate the desired size of the stretchable symbol.
  // If symmetric == true the we have to stretch to cover the maximum among
  // height and depth, otherwise we just stretch to ascent + descent
  desiredSize = symmetric ? (2 * std::max(height, depth)) : (height + depth);

  // actually a slightly smaller fence is usually enough when symmetric is true
  Globals::logger(LOG_DEBUG, "request for stretch to %d...", sp2ipx(desiredSize));
  if (true || symmetric)
    desiredSize = std::max(desiredSize - pt2sp(5), ((desiredSize * 901) / 1000));
  Globals::logger(LOG_DEBUG, "%d will be enough!", sp2ipx(desiredSize));

  desiredSize = std::max(scaled(0), desiredSize);

  // ...however, there may be some contraints over the size of the stretchable
  // operator. adjustedSize will be the final allowed size for the operator
  scaled minHeight = minBox.verticalExtent();
  Globals::logger(LOG_DEBUG, "the minimum height is %d", sp2ipx(minHeight));

  scaled adjustedSize = desiredSize;

  if (minMultiplier > 0)
    adjustedSize = std::max(adjustedSize, minHeight * minMultiplier);
  else
    adjustedSize = std::max(adjustedSize, minSize);

  if (!infiniteMaxSize) {
    if (maxMultiplier > 0)
      adjustedSize = std::min(adjustedSize, minHeight * maxMultiplier);
    else
      adjustedSize = std::min(adjustedSize, maxSize);
  }

  adjustedSize = std::max(scaled(0), adjustedSize);

#if 0
  assert(GetSize() == 1);
  if (SmartPtr<MathMLCharNode> cNode = smart_cast<MathMLCharNode>(GetChild(0)))
    {
      if (!cNode->IsStretchyChar())
	{
	  Globals::logger(LOG_WARNING, "character `U+%04x' could not be stretched", cNode->GetChar());
	  return;
	}
    }

  SmartPtr<MathMLCharNode> sNode = smart_cast<MathMLCharNode>(GetChild(0));
  assert(sNode);

  scaled adjustedHeight = 0;
  scaled adjustedDepth = 0;

  if (symmetric)
    {
      adjustedHeight = adjustedSize / 2;
      adjustedDepth = adjustedSize / 2;
    }
  else
    {
      adjustedHeight = height * (adjustedSize.toFloat() / desiredSize.toFloat());
      adjustedDepth = depth * (adjustedSize.toFloat() / desiredSize.toFloat());
    }

  Globals::logger(LOG_DEBUG, "adjusted stretchy size %d", sp2ipx(adjustedSize));

  sNode->DoVerticalStretchyLayout(adjustedHeight, adjustedDepth, axis, strict);

  // since the bounding box may have changed, we force dirtyLayout to true, so that
  // a DoBoxedLayout done on this operator will have effect
  SetDirtyLayout();
#endif
}

void
MathMLOperatorElement::HorizontalStretchTo(const scaled& width, bool strict)
{
  assert(stretchy);

  scaled desiredSize = width;

  desiredSize = std::max(scaled(0), desiredSize);

  // ...however, there may be some contraints over the size of the stretchable
  // operator. adjustedSize will be the final allowed size for the operator
  scaled minWidth = minBox.width;
  scaled adjustedSize = desiredSize;

  if (minMultiplier > 0)
    adjustedSize = std::max(adjustedSize, minWidth * minMultiplier);
  else
    adjustedSize = std::max(adjustedSize, minSize);

  if (!infiniteMaxSize) {
    if (maxMultiplier > 0)
      adjustedSize = std::min(adjustedSize, minWidth * maxMultiplier);
    else
      adjustedSize = std::min(adjustedSize, minSize);
  }

  adjustedSize = std::max(scaled(0), adjustedSize);

#if 0
  assert(GetSize() == 1);
  if (SmartPtr<MathMLCharNode> cNode = smart_cast<MathMLCharNode>(GetChild(0)))
    {
      if (!cNode->IsStretchyChar())
	{
	  Globals::logger(LOG_WARNING, "character `U+%04x' could not be stretched", cNode->GetChar());
	  return;
	}
    }

  SmartPtr<MathMLCharNode> sNode = smart_cast<MathMLCharNode>(GetChild(0));
  assert(sNode);

  // now we do the stretchy layout. fontAttributes will be used to find the
  // proper font
  sNode->DoHorizontalStretchyLayout(adjustedSize, strict);

  // since the bounding box may have changed, we force dirtyLayout to true, so that
  // a DoBoxedLayout done on this operator will have effect
  SetDirtyLayout();
#endif
}

void
MathMLOperatorElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  SetContentPosition(x, y);
}

void
MathMLOperatorElement::ParseLimitValue(const SmartPtr<Value>& value,
				       const RenderingEnvironment& env,
				       float& multiplier,
				       scaled& size)
{
  assert(value);

  if (IsKeyword(value))
    { // it must be a named math space
      SmartPtr<Value> resValue = Resolve(value, env);
      multiplier = -1;
      size = env.ToScaledPoints(ToNumberUnit(resValue));
    }
  else
    {
      SmartPtr<ValueSequence> seq = ToSequence(value);
      assert(seq);
      assert(seq->getSize() == 2);
      SmartPtr<Value> v = seq->getValue(0);
      SmartPtr<Value> unitV = seq->getValue(1);
      assert(v);
      assert(unitV);

      if (IsEmpty(unitV)) 
	multiplier = std::max(EPSILON, ToNumber(v));
      else
	{
	  assert(IsKeyword(unitV));
	  multiplier = -1;

	  UnitValue siz(ToNumber(v), ToUnitId(unitV));

	  if (siz.IsPercentage())
	    {
	      Globals::logger(LOG_WARNING, "percentage value specified in maxsize attribute (mo) (ignored)");
	      multiplier = std::max(EPSILON, siz.GetValue());
	    } 
	  else
	    {
	      multiplier = -1;
	      size = env.ToScaledPoints(siz);
	    }
	}
    }
}

SmartPtr<Value>
MathMLOperatorElement::getOperatorAttributeValue(const MathMLAttributeSignature& signature) const
{
  //printf("`%s': searching for attribute `%s'\n", NameOfTagId(IsA()), NameOfAttributeId(id));

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

OperatorFormId
MathMLOperatorElement::InferOperatorForm()
{
  SmartPtr<MathMLElement> eOp = findEmbellishedOperatorRoot(this);
  assert(eOp);
  SmartPtr<MathMLElement> elem = eOp->GetParent();
  assert(elem);

  OperatorFormId res = OP_FORM_INFIX;

  if (elem->IsA() == TAG_MROW)
    {
      SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(elem);
      assert(row);
      res = row->GetOperatorForm(eOp);
    }

  return res;
}

StretchId
MathMLOperatorElement::GetStretch() const
{
#if 0
  if (!IsStretchy()) return STRETCH_NO;

  //assert(GetSize() == 1);
  if (!is_a<MathMLCharNode>(GetChild(0))) return STRETCH_NO;
  SmartPtr<MathMLCharNode> sChar = smart_cast<MathMLCharNode>(GetChild(0));
  assert(sChar);

  if (!sChar->IsStretchyChar()) return STRETCH_NO;

  return sChar->GetStretch();
#endif
  return STRETCH_NO;
}

SmartPtr<MathMLOperatorElement>
MathMLOperatorElement::GetCoreOperator()
{
  return this;
}
