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

#include "FormattingContext.hh"
#include "Globals.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLRowElement.hh"
#include "RenderingEnvironment.hh"
#include "ValueConversion.hh"
#include "operatorAux.hh"
#include "scaledConv.hh"
#include "traverseAux.hh"

MathMLOperatorElement::MathMLOperatorElement()
{
  Init();
}

#if defined(HAVE_GMETADOM)
MathMLOperatorElement::MathMLOperatorElement(const DOM::Element& node)
  : MathMLTokenElement(node)
{
  Init();
}
#endif

void
MathMLOperatorElement::Init()
{
  defaults = NULL;

  fence = separator = stretchy = symmetric = infiniteMaxSize = accent = movableLimits = 0;
  forcedFence = forcedSeparator = forcedSymmetric = 0;
}

MathMLOperatorElement::~MathMLOperatorElement()
{
}

const AttributeSignature*
MathMLOperatorElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_FORM,      	  operatorFormParser, 	 NULL,             NULL },
    { ATTR_FENCE,     	  booleanParser,      	 "false",          NULL },
    { ATTR_SEPARATOR, 	  booleanParser,      	 "false",          NULL },
    { ATTR_LSPACE,    	  spaceParser,    	 "thickmathspace", NULL },
    { ATTR_RSPACE,    	  spaceParser,    	 "thickmathspace", NULL },
#ifdef ENABLE_EXTENSIONS
    { ATTR_TSPACE,        numberUnitParser,      "0ex",            NULL },
    { ATTR_BSPACE,        numberUnitParser,      "0ex",            NULL },
#endif // ENABLE_EXTENSIONS
    { ATTR_STRETCHY,  	  booleanParser,      	 "false",          NULL },
    { ATTR_SYMMETRIC, 	  booleanParser,      	 "true",           NULL },
    { ATTR_MAXSIZE,   	  operatorMaxSizeParser, "infinity",       NULL },
    { ATTR_MINSIZE,   	  operatorMinSizeParser, "1",              NULL },
    { ATTR_LARGEOP,       booleanParser,         "false",          NULL },
    { ATTR_MOVABLELIMITS, booleanParser,         "false",          NULL },
    { ATTR_ACCENT,        booleanParser,         "false",          NULL },
    { ATTR_NOTVALID,      NULL,                  NULL,             NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLTokenElement::GetAttributeSignature(id);

  return signature;
}

#if 0
void
MathMLOperatorElement::Normalize(const SmartPtr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if 0
      if (!eOp)
	{
	  SmartPtr<MathMLElement> op = MathMLEmbellishedOperatorElement::create(this);
	  assert(op);
	  eOp = smart_cast<MathMLEmbellishedOperatorElement>(op);
	}
      assert(eOp);

      // now we have to substitute the root of the embellished operator
      // with the embellished operator object just created
      assert(is_a<MathMLContainerElement>(GetParent()));
      SmartPtr<MathMLContainerElement> pContainer = smart_cast<MathMLContainerElement>(GetParent());
      assert(pContainer);
      pContainer->Replace(this, eOp);
      eOp->SetChild(this);
#if defined(HAVE_GMETADOM)
      doc->setFormattingNode(GetDOMElement(), eOp);
#endif
#endif

      MathMLTokenElement::Normalize(doc);
    }
}
#endif

void
MathMLOperatorElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute())
    {
      axis = env.GetAxis();

      if (SmartPtr<Value> value = GetAttributeValue(ATTR_FORM, env, false))
	form = ToFormId(value);
      else
	form = InferOperatorForm();

      const MathMLAttributeList* prefix  = 0;
      const MathMLAttributeList* infix   = 0;
      const MathMLAttributeList* postfix = 0;

      String operatorName = GetRawContent();
      Globals::dictionary.Search(operatorName, &prefix, &infix, &postfix);

      if      (form == OP_FORM_PREFIX && prefix) defaults = prefix;
      else if (form == OP_FORM_INFIX && infix) defaults = infix;
      else if (form == OP_FORM_POSTFIX && postfix) defaults = postfix;
      else if (infix) defaults = infix;
      else if (postfix) defaults = postfix;
      else if (prefix) defaults = prefix;
      else defaults = 0;

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_FENCE, env))
	if (!ForcedFence()) fence = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_SEPARATOR, env))
	if (!ForcedSeparator()) separator = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_LSPACE, env))
	{
	  SmartPtr<Value> resValue = Resolve(value, env);
	  if (env.GetScriptLevel() <= 0)
	    lSpace = env.ToScaledPoints(ToNumberUnit(resValue));
	  else
	    lSpace = 0;
	}
      else
	assert(IMPOSSIBLE);

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_RSPACE, env))
	{
	  SmartPtr<Value> resValue = Resolve(value, env);
	  if (env.GetScriptLevel() <= 0)
	    rSpace = env.ToScaledPoints(ToNumberUnit(resValue));
	  else
	    rSpace = 0;
	}
      else
	assert(IMPOSSIBLE);

#ifdef ENABLE_EXTENSIONS
      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_TSPACE, env))
	tSpace = env.ToScaledPoints(ToNumberUnit(value));

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_BSPACE, env))
	bSpace = env.ToScaledPoints(ToNumberUnit(value));
#endif // ENABLE_EXTENSIONS

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_STRETCHY, env))
	stretchy = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_SYMMETRIC, env))
	if (!ForcedSymmetric()) symmetric = ToBoolean(value) ? 1 : 0;

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_MAXSIZE, env))
	if (ToKeywordId(value) == KW_INFINITY)
	  infiniteMaxSize = 1;
	else
	  {
	    infiniteMaxSize = 0;
	    ParseLimitValue(value, env, maxMultiplier, maxSize);
	  }

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_MINSIZE, env))
	ParseLimitValue(value, env, minMultiplier, minSize);
      else
	assert(IMPOSSIBLE);

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_MOVABLELIMITS, env))
	movableLimits = ToBoolean(value) ? 1 : 0;
      else
	assert(IMPOSSIBLE);

      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_ACCENT, env))
	accent = ToBoolean(value) ? 1 : 0;
      else
	assert(IMPOSSIBLE);

      bool largeOp = false;
      if (SmartPtr<Value> value = GetOperatorAttributeValue(ATTR_LARGEOP, env))
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
MathMLOperatorElement::GetOperatorAttributeValue(AttributeId id,
						 const RenderingEnvironment& env) const
{
  //printf("`%s': searching for attribute `%s'\n", NameOfTagId(IsA()), NameOfAttributeId(id));

  // 1st attempt, the attribute may be set for the current operator
  if (SmartPtr<Value> value = GetAttributeValue(id, env, false))
    return value;

  if (defaults)
    {
      // no, it is not explicitly set, but this operator has an entry in
      // the operator dictionary, so let's see if the attribute has a
      // default value
      if (const MathMLAttribute* attribute = defaults->GetAttribute(id))
	{
	  const AttributeSignature* aSignature = GetAttributeSignature(id);
	  assert(aSignature);
	  if (SmartPtr<Value> value = attribute->GetParsedValue(aSignature))
	    return value;
	}

      //if (value != NULL) printf("found in dictionary\n");
    }

  // if the attribute hasn't still a value, then take its default
  // for the mo element
  if (SmartPtr<Value> value = GetAttributeValue(id))
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
