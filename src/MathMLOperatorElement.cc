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
#include <stdio.h>

#include "Iterator.hh"
#include "MathEngine.hh"
#include "operatorAux.hh"
#include "traverseAux.hh"
#include "ValueSequence.hh"
#include "StringFactory.hh"
#include "StringUnicode.hh"
#include "MathMLCharNode.hh"
#include "ValueConversion.hh"
#include "MathMLStringNode.hh"
#include "MathMLRowElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLEmbellishedOperatorElement.hh"

MathMLOperatorElement::MathMLOperatorElement(mDOMNodeRef node) :
  MathMLTokenElement(node, TAG_MO)
{
  eOp = NULL;
  defaults = NULL;
  nameFactory = new StringFactory;
  operatorName = NULL;

  fence = separator = stretchy = symmetric = infiniteMaxSize = accent = movableLimits = 0;
  forcedFence = forcedSeparator = forcedSymmetric = 0;
}

MathMLOperatorElement::~MathMLOperatorElement()
{
  delete operatorName;
}

const AttributeSignature*
MathMLOperatorElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_FORM,      	  operatorFormParser, 	 NULL,                          NULL },
    { ATTR_FENCE,     	  booleanParser,      	 new StringC("false"),          NULL },
    { ATTR_SEPARATOR, 	  booleanParser,      	 new StringC("false"),          NULL },
    { ATTR_LSPACE,    	  spaceParser,    	 new StringC("thickmathspace"), NULL },
    { ATTR_RSPACE,    	  spaceParser,    	 new StringC("thickmathspace"), NULL },
#ifdef ENABLE_EXTENSIONS
    { ATTR_TSPACE,        numberUnitParser,      new StringC("0ex"),            NULL },
    { ATTR_BSPACE,        numberUnitParser,      new StringC("0ex"),            NULL },
#endif // ENABLE_EXTENSIONS
    { ATTR_STRETCHY,  	  booleanParser,      	 new StringC("false"),          NULL },
    { ATTR_SYMMETRIC, 	  booleanParser,      	 new StringC("true"),           NULL },
    { ATTR_MAXSIZE,   	  operatorMaxSizeParser, new StringC("infinity"),       NULL },
    { ATTR_MINSIZE,   	  operatorMinSizeParser, new StringC("1"),              NULL },
    { ATTR_LARGEOP,       booleanParser,         new StringC("false"),          NULL },
    { ATTR_MOVABLELIMITS, booleanParser,         new StringC("false"),          NULL },
    { ATTR_ACCENT,        booleanParser,         new StringC("false"),          NULL },
    { ATTR_NOTVALID,      NULL,                  NULL,                          NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLTokenElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLOperatorElement::Append(const String* s)
{
  assert(s != NULL);
  assert(nameFactory != NULL);

  if (s->GetLength() == 0) return;

  nameFactory->Append(s);
  MathMLTokenElement::Append(s);
}

void
MathMLOperatorElement::Normalize()
{
  MathMLElement* root = findEmbellishedOperatorRoot(this);
  assert(root != NULL);

  MathMLElement* p = root->GetParent();
  assert(p != NULL);

  // at the normalization step it is assumed that the content of this
  // operator is fixed. The StringFactory is packed into a single string
  // and no further Append is allowed
  assert(nameFactory != NULL);
  operatorName = nameFactory->Pack();
  delete nameFactory;
  nameFactory = NULL;

  MathMLEmbellishedOperatorElement* op = new MathMLEmbellishedOperatorElement(this);
  op->SetParent(p);
  op->content.Append(root);
  eOp = op;

  // now we have to substitute the root of the embellished operator
  // with the embellished operator object just created
  assert(p->IsContainer());
  MathMLContainerElement* pContainer = TO_CONTAINER(p);
  assert(pContainer != NULL);
  pContainer->content.Substitute(root, eOp);
  // Important!!! The root of the embellished operator now has
  // the "embellishment" as parent
  root->SetParent(eOp);

  MathMLTokenElement::Normalize();
}

void
MathMLOperatorElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  const Value* value = NULL;
  const Value* resValue = NULL;

  axis = env->GetAxis();

  value = GetAttributeValue(ATTR_FORM, env, false);
  if (value != NULL) form = ToFormId(value);
  else form = InferOperatorForm();
  delete value;

  const MathMLAttributeList* prefix  = NULL;
  const MathMLAttributeList* infix   = NULL;
  const MathMLAttributeList* postfix = NULL;
  
  if (operatorName != NULL)
    MathEngine::dictionary.Search(operatorName, &prefix, &infix, &postfix);

  if      (form == OP_FORM_PREFIX && prefix != NULL) defaults = prefix;
  else if (form == OP_FORM_INFIX && infix != NULL) defaults = infix;
  else if (form == OP_FORM_POSTFIX && postfix != NULL) defaults = postfix;
  else if (infix != NULL) defaults = infix;
  else if (postfix != NULL) defaults = postfix;
  else if (prefix != NULL) defaults = prefix;
  else defaults = NULL;

  value = GetOperatorAttributeValue(ATTR_FENCE, env);
  assert(value != NULL && value->IsBoolean());
  if (!ForcedFence()) fence = value->ToBoolean() ? 1 : 0;
  delete value;

  value = GetOperatorAttributeValue(ATTR_SEPARATOR, env);
  assert(value != NULL && value->IsBoolean());
  if (!ForcedSeparator()) separator = value->ToBoolean() ? 1 : 0;
  delete value;

  value = GetOperatorAttributeValue(ATTR_LSPACE, env);
  assert(value != NULL);
  resValue = Resolve(value, env);
  assert(resValue != NULL && resValue->IsNumberUnit());
  lSpace = env->ToScaledPoints(resValue->ToNumberUnit());
  delete resValue;
  delete value;

  value = GetOperatorAttributeValue(ATTR_RSPACE, env);
  assert(value != NULL);
  resValue = Resolve(value, env);
  assert(resValue != NULL && resValue->IsNumberUnit());
  rSpace = env->ToScaledPoints(resValue->ToNumberUnit());
  delete resValue;
  delete value;

#ifdef ENABLE_EXTENSIONS
  value = GetOperatorAttributeValue(ATTR_TSPACE, env);
  assert(value != NULL && value->IsNumberUnit());
  tSpace = env->ToScaledPoints(value->ToNumberUnit());
  delete value;

  value = GetOperatorAttributeValue(ATTR_BSPACE, env);
  assert(value != NULL && value->IsNumberUnit());
  bSpace = env->ToScaledPoints(value->ToNumberUnit());
  delete value;
#endif // ENABLE_EXTENSIONS

  value = GetOperatorAttributeValue(ATTR_STRETCHY, env);
  assert(value != NULL && value->IsBoolean());
  stretchy = value->ToBoolean() ? 1 : 0;
  delete value;

  value = GetOperatorAttributeValue(ATTR_SYMMETRIC, env);
  assert(value != NULL && value->IsBoolean());
  if (!ForcedSymmetric()) symmetric = value->ToBoolean() ? 1 : 0;
  delete value;

  value = GetOperatorAttributeValue(ATTR_MAXSIZE, env);
  assert(value != NULL);
  if (value->IsKeyword(KW_INFINITY)) infiniteMaxSize = 1;
  else {
    infiniteMaxSize = 0;
    ParseLimitValue(value, env, maxMultiplier, maxSize);
  }
  delete value;

  value = GetOperatorAttributeValue(ATTR_MINSIZE, env);
  assert(value != NULL);
  ParseLimitValue(value, env, minMultiplier, minSize);
  delete value;

  value = GetOperatorAttributeValue(ATTR_MOVABLELIMITS, env);
  assert(value != NULL && value->IsBoolean());
  movableLimits = value->ToBoolean() ? 1 : 0;
  delete value;

  value = GetOperatorAttributeValue(ATTR_ACCENT, env);
  assert(value != NULL && value->IsBoolean());
  accent = value->ToBoolean() ? 1 : 0;
  delete value;

  value = GetOperatorAttributeValue(ATTR_LARGEOP, env);
  assert(value != NULL && value->IsBoolean());
  bool largeOp = value->ToBoolean();
  delete value;

  MathMLTokenElement::Setup(env);

  if (content.GetSize() == 1 && largeOp && env->GetDisplayStyle()) {
    assert(content.GetFirst() != NULL);
    if (content.GetFirst()->IsStretchyChar()) {
      MathMLCharNode* sNode = TO_CHAR(content.GetFirst());
      assert(sNode != NULL);
      sNode->SetDefaultLargeGlyph(true);
    }
  }
}

void
MathMLOperatorElement::VerticalStretchTo(scaled ascent, scaled descent, bool strict)
{
  assert(stretchy);

  scaled height = ascent - axis;
  scaled depth = descent + axis;

  scaled desiredSize = 0;

  // Here we have to calculate the desired size of the stretchable symbol.
  // If symmetric == true the we have to stretch to cover the maximum among
  // height and depth, otherwise we just stretch to ascent + descent
  desiredSize = symmetric ? (2 * scaledMax(height, depth)) : (height + depth);

  // actually a slightly smaller fence is usually enough when symmetric is true
  MathEngine::logger(LOG_DEBUG, "request for stretch to %d...", sp2ipx(desiredSize));
  if (true || symmetric)
    desiredSize = scaledMax(desiredSize - pt2sp(5), ((desiredSize * 901) / 1000));
  MathEngine::logger(LOG_DEBUG, "%d will be enough!", sp2ipx(desiredSize));

  desiredSize = scaledMax(SP_EPSILON, desiredSize);

  // ...however, there may be some contraints over the size of the stretchable
  // operator. adjustedSize will be the final allowed size for the operator
  scaled minHeight = GetMinBoundingBox().GetHeight();
  MathEngine::logger(LOG_DEBUG, "the minimum height is %d", sp2ipx(minHeight));

  scaled adjustedSize = desiredSize;

  if (minMultiplier > 0)
    adjustedSize = scaledMax(adjustedSize, float2sp(minHeight * minMultiplier));
  else
    adjustedSize = scaledMax(adjustedSize, minSize);

  if (!infiniteMaxSize) {
    if (maxMultiplier > 0)
      adjustedSize = scaledMin(adjustedSize, float2sp(minHeight * maxMultiplier));
    else
      adjustedSize = scaledMin(adjustedSize, maxSize);
  }

  adjustedSize = scaledMax(0, adjustedSize);

  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);
  if (!content.GetFirst()->IsStretchyChar()) {
    MathEngine::logger(LOG_WARNING, "character `U+%04x' could not be stretched", operatorName->GetChar(0));
    return;
  }

  MathMLCharNode* sNode = TO_CHAR(content.GetFirst());
  assert(sNode != NULL);

  scaled adjustedHeight = 0;
  scaled adjustedDepth = 0;

  if (symmetric) {
    adjustedHeight = adjustedSize / 2;
    adjustedDepth = adjustedSize / 2;
  } else {
    adjustedHeight = scaledProp(height, adjustedSize, desiredSize);
    adjustedDepth = scaledProp(depth, adjustedSize, desiredSize);
  }

  MathEngine::logger(LOG_DEBUG, "adjusted stretchy size %d", sp2ipx(adjustedSize));

  sNode->DoVerticalStretchyLayout(adjustedHeight, adjustedDepth, axis, strict);

  // since the bounding box may have changed, we force dirtyLayout to true, so that
  // a DoBoxedLayout done on this operator will have effect
  SetDirtyLayout();
}

void
MathMLOperatorElement::HorizontalStretchTo(scaled width, bool strict)
{
  assert(stretchy);

  scaled desiredSize = width;

  desiredSize = scaledMax(SP_EPSILON, desiredSize);

  // ...however, there may be some contraints over the size of the stretchable
  // operator. adjustedSize will be the final allowed size for the operator
  scaled minWidth = GetMinBoundingBox().width;
  scaled adjustedSize = desiredSize;

  if (minMultiplier > 0)
    adjustedSize = scaledMax(adjustedSize, float2sp(minWidth * minMultiplier));
  else
    adjustedSize = scaledMax(adjustedSize, minSize);

  if (!infiniteMaxSize) {
    if (maxMultiplier > 0)
      adjustedSize = scaledMin(adjustedSize, float2sp(minWidth * maxMultiplier));
    else
      adjustedSize = scaledMin(adjustedSize, minSize);
  }

  adjustedSize = scaledMax(0, adjustedSize);

  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);
  if (!content.GetFirst()->IsStretchyChar()) {
    MathEngine::logger(LOG_WARNING, "character `U+%04x' could not be stretched", operatorName->GetChar(0));
    return;
  }

  MathMLCharNode* sNode = TO_CHAR(content.GetFirst());
  assert(sNode != NULL);

  // now we do the stretchy layout. fontAttributes will be used to find the
  // proper font
  sNode->DoHorizontalStretchyLayout(adjustedSize, strict);

  // since the bounding box may have changed, we force dirtyLayout to true, so that
  // a DoBoxedLayout done on this operator will have effect
  SetDirtyLayout();
}

void
MathMLOperatorElement::ParseLimitValue(const Value* value,
				       const RenderingEnvironment* env,
				       float& multiplier,
				       scaled& size)
{
  assert(value != NULL);
  assert(env != NULL);

  if (value->IsKeyword()) { // it must be a named math space
    const Value* resValue = Resolve(value, env);
    assert(resValue->IsNumberUnit());
    multiplier = -1;
    size = env->ToScaledPoints(resValue->ToNumberUnit());
    delete resValue;
  } else {
    assert(value->IsSequence());
    const ValueSequence* seq = value->ToValueSequence();
    const Value* v = seq->GetFirstValue();
    const Value* unitV = seq->GetLastValue();
    assert(v != NULL);
    assert(v->IsNumber());
    assert(unitV != NULL);
    
    if (unitV->IsEmpty()) {      
      multiplier = floatMax(EPSILON, v->ToNumber());
    } else {
      assert(unitV->IsKeyword());
      multiplier = -1;

      UnitValue siz;
      siz.Set(v->ToNumber(), ToUnitId(unitV));

      if (siz.IsPercentage()) {
	MathEngine::logger(LOG_WARNING, "percentage value specified in maxsize attribute (mo) (ignored)");
	multiplier = floatMax(EPSILON, siz.GetValue());
      } else {
	multiplier = -1;
	size = env->ToScaledPoints(siz);
      }
    }
  }
}

const Value*
MathMLOperatorElement::GetOperatorAttributeValue(AttributeId id,
						 const RenderingEnvironment* env) const
{
  assert(env != NULL);

  //printf("`%s': searching for attribute `%s'\n", NameOfTagId(IsA()), NameOfAttributeId(id));

  // 1st attempt, the attribute may be set for the current operator
  const Value* value = GetAttributeValue(id, NULL, false);

  //if (value != NULL) printf("found directly\n");

  if (value == NULL && defaults != NULL) {
    // no, it is not explicitly set, but this operator has an entry in
    // the operator dictionary, so let's see if the attribute has a
    // default value
    const MathMLAttribute* attribute = defaults->GetAttribute(id);
    if (attribute != NULL) {
      const AttributeSignature* aSignature = GetAttributeSignature(id);
      assert(aSignature != NULL);
      value = attribute->GetParsedValue(aSignature);
    }

    //if (value != NULL) printf("found in dictionary\n");
  }

  // if the attribute hasn't still a value, then take its default
  // from the environment or the default for the mo element
  if (value == NULL) value = GetAttributeValue(id, env);
  assert(value != NULL);

  return value;
}

OperatorFormId
MathMLOperatorElement::InferOperatorForm() const
{
  assert(eOp != NULL);

  MathMLElement* elem = eOp->GetParent();
  assert(elem != NULL);

  OperatorFormId res = OP_FORM_INFIX;

  if (elem->IsA() == TAG_MROW) {
    MathMLRowElement* row = TO_ROW(elem);
    assert(row != NULL);

    unsigned rowLength = 0;
    unsigned position  = 0;
    for (Iterator<MathMLElement*> i(row->content); i.More(); i.Next()) {
      const MathMLElement* p = i();
      assert(p != NULL);

      if (!p->IsSpaceLike()) {
	if (p == eOp) position = rowLength;
	rowLength++;
      }
    }
    
    if (rowLength > 1) {
      if (position == 0) res = OP_FORM_PREFIX;
      else if (position == rowLength - 1) res = OP_FORM_POSTFIX;
    }
  }

  return res;
}

StretchId
MathMLOperatorElement::GetStretch() const
{
  if (!IsStretchy()) return STRETCH_NO;

  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);

  if (!content.GetFirst()->IsStretchyChar()) return STRETCH_NO;

  MathMLCharNode* sChar = TO_CHAR(content.GetFirst());
  assert(sChar != NULL);

  return sChar->GetStretch();
}

bool
MathMLOperatorElement::IsOperator() const
{
  return true;
}

bool
MathMLOperatorElement::IsBreakable() const
{
  return false;
}

