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

#include "MathEngine.hh"
#include "operatorAux.hh"
#include "traverseAux.hh"
#include "MathMLCharNode.hh"
#include "MathMLDummyElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLUnderOverElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLEmbellishedOperatorElement.hh"

MathMLUnderOverElement::MathMLUnderOverElement(mDOMNodeRef node, TagId id) :
  MathMLContainerElement(node, id)
{
  assert(id == TAG_MUNDER || id == TAG_MOVER || id == TAG_MUNDEROVER);
  underScript = overScript = NULL;
}

MathMLUnderOverElement::~MathMLUnderOverElement()
{
}

const AttributeSignature*
MathMLUnderOverElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature underSig[] = {
    { ATTR_ACCENTUNDER, booleanParser, NULL, NULL },
    { ATTR_NOTVALID,    NULL,          NULL, NULL }
  };

  static AttributeSignature overSig[] = {
    { ATTR_ACCENT,      booleanParser, NULL, NULL },
    { ATTR_NOTVALID,    NULL,          NULL, NULL }
  };

  const AttributeSignature* signature = NULL;
  if (IsA() == TAG_MUNDER || IsA() == TAG_MUNDEROVER)
    signature = GetAttributeSignatureAux(id, underSig);
  if (signature == NULL && (IsA() == TAG_MOVER || IsA() == TAG_MUNDEROVER))
    signature = GetAttributeSignatureAux(id, overSig);    
  if (signature == NULL)
    signature = MathMLContainerElement::GetAttributeSignature(id);
  
  return signature;
}

void
MathMLUnderOverElement::Normalize()
{
  unsigned n = (IsA() == TAG_MUNDEROVER) ? 3 : 2;

  while (content.GetSize() > n) {
    MathMLElement* elem = content.RemoveLast();
    delete elem;
  }

  while (content.GetSize() < n) {
    MathMLElement* mdummy = new MathMLDummyElement();
    Append(mdummy);
  }

  // BEWARE: normalization has to be done here, since it may
  // change the content!!!
  MathMLContainerElement::Normalize();

  base = content.GetFirst();
  assert(base != NULL);

  if (IsA() == TAG_MUNDER) underScript = content.GetLast();
  else if (IsA() == TAG_MOVER) overScript = content.GetLast();
  else {
    underScript = content.Get(1);
    overScript  = content.Get(2);
  }
}

void
MathMLUnderOverElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);
  assert(base != NULL);

  bool displayStyle = env->GetDisplayStyle();

  ScriptSetup(env);

  scaled smallSpacing = ruleThickness;
  scaled bigSpacing   = 3 * ruleThickness;

  base->Setup(env);
  const MathMLOperatorElement* op = findCoreOperator(base);

  scriptize = op != NULL && !displayStyle && op->HasMovableLimits();

  env->Push();
  env->SetDisplayStyle(false);

  accentUnder = false;
  underSpacing = 0;
  if (underScript != NULL) {
    if (!scriptize) {
      const Value* value = GetAttributeValue(ATTR_ACCENTUNDER, env, false);
      if (value != NULL) accentUnder = value->ToBoolean();
      else {
	const MathMLOperatorElement* op = findCoreOperator(underScript);
	if (op != NULL) {
	  underScript->Setup(env);
	  accentUnder = op->IsAccent();
	}
      }
    }

    if (accentUnder) underSpacing = smallSpacing;
    else {
      env->AddScriptLevel(1);
      underSpacing = displayStyle ? bigSpacing : smallSpacing;
    }
    underScript->Setup(env);
  }

  env->Drop();
  env->Push();
  env->SetDisplayStyle(false);

  accent = false;
  overSpacing = 0;
  if (overScript != NULL) {
    if (!scriptize) {
      const Value* value = GetAttributeValue(ATTR_ACCENT, env, false);
      if (value != NULL) accent = value->ToBoolean();
      else {
	const MathMLOperatorElement* op = findCoreOperator(overScript);
	if (op != NULL) {
	  overScript->Setup(env);
	  accent = op->IsAccent();
	}
      }
    }

    if (accent) {
      overSpacing = smallSpacing;
    } else {
      env->AddScriptLevel(1);
      overSpacing = displayStyle ? bigSpacing : smallSpacing;
    }
    overScript->Setup(env);
  }
  
  env->Drop();
}

void
MathMLUnderOverElement::DoBoxedLayout(LayoutId id, BreakId, scaled maxWidth)
{
  if (!HasDirtyLayout(id, maxWidth)) return;

  assert(base != NULL);

  scaled overClearance = 0;
  scaled underClearance = 0;

  if (scriptize) {
    base->DoBoxedLayout(id, BREAK_NO, maxWidth / 2);
    if (overScript != NULL) overScript->DoBoxedLayout(id, BREAK_NO, maxWidth / 2);
    if (underScript != NULL) underScript->DoBoxedLayout(id, BREAK_NO, maxWidth / 2);

    const BoundingBox& baseBox = base->GetBoundingBox();
    BoundingBox underBox;
    BoundingBox overBox;
    
    if (underScript != NULL) underBox = underScript->GetBoundingBox();
    else underBox.Null();

    if (overScript != NULL) overBox = overScript->GetBoundingBox();
    else overBox.Null();

    DoScriptLayout(baseBox, underBox, overBox, underShiftX, underShiftY, overShiftX, overShiftY);
    underClearance = overClearance = 0;

    baseShiftX = 0;
  } else {    
    if (id != LAYOUT_AUTO) {
      base->DoBoxedLayout(id, BREAK_NO);
      if (underScript != NULL) underScript->DoBoxedLayout(id, BREAK_NO);
      if (overScript != NULL) overScript->DoBoxedLayout(id, BREAK_NO);
    } else {
      unsigned nOp    = 0;
      unsigned nOther = 0;

      scaled wOp      = 0;
      scaled wOther   = 0;

      MathMLOperatorElement* baseOp  = findStretchyOperator(base, STRETCH_HORIZONTAL);
      MathMLOperatorElement* underOp = findStretchyOperator(underScript, STRETCH_HORIZONTAL);
      MathMLOperatorElement* overOp  = findStretchyOperator(overScript, STRETCH_HORIZONTAL);

      MathEngine::logger(LOG_DEBUG, "stretchy: %p %p %p", baseOp, underOp, overOp);

      if (baseOp == NULL) base->DoBoxedLayout(id, BREAK_NO, maxWidth);
      if (underScript != NULL && underOp == NULL) underScript->DoBoxedLayout(id, BREAK_NO, maxWidth);
      if (overScript != NULL && overOp == NULL) overScript->DoBoxedLayout(id, BREAK_NO, maxWidth);

      if (baseOp == NULL) {
	wOther = base->GetBoundingBox().width;
	nOther++;
      } else {
	wOp = base->GetMinBoundingBox().width;
	nOp++;
      }

      if (underScript != NULL) {
	if (underOp == NULL) {
	  wOther = scaledMax(wOther, underScript->GetBoundingBox().width);
	  nOther++;
	} else {
	  wOp = scaledMax(wOp, underScript->GetMinBoundingBox().width);
	  nOp++;
	}
      }

      if (overScript != NULL) {
	if (overOp == NULL) {
	  wOther = scaledMax(wOther, overScript->GetBoundingBox().width);
	  nOther++;
	} else {
	  wOp = scaledMax(wOp, overScript->GetMinBoundingBox().width);
	  nOp++;
	}
      }

      if (nOp > 0) {
	scaled w = (nOther == 0) ? wOp : wOther;

	if (baseOp != NULL) baseOp->HorizontalStretchTo(w);
	if (underOp != NULL) underOp->HorizontalStretchTo(w);
	if (overOp != NULL) overOp->HorizontalStretchTo(w);
      }

      if (baseOp != NULL) base->DoBoxedLayout(id, BREAK_NO);
      if (underScript != NULL && underOp != NULL) underScript->DoBoxedLayout(id, BREAK_NO);
      if (overScript != NULL && overOp != NULL) overScript->DoBoxedLayout(id, BREAK_NO);
    }

    const BoundingBox& baseBox = base->GetBoundingBox();
    const MathMLCharNode* bChar = base->GetCharNode();

    if (underScript != NULL) {
      const MathMLCharNode* cChar = underScript->GetCharNode();

      if (accentUnder &&
	  bChar != NULL && cChar != NULL &&
	  isCombiningBelow(cChar->GetChar()) &&
	  bChar->CombineWith(cChar, underShiftX, underShiftY)) {
	MathEngine::logger(LOG_DEBUG, "this is the special handling for U+%04X used as accent under U+%04X",
			   cChar->GetChar(), bChar->GetChar());

	underShiftY = -underShiftY;

	if (underScript->IsEmbellishedOperator()) {
	  MathMLEmbellishedOperatorElement* eOp = TO_EMBELLISHED_OPERATOR(underScript);
	  assert(eOp != NULL);
	  MathMLOperatorElement* coreOp = eOp->GetCoreOperator();
	  assert(coreOp != NULL);
	  underShiftY += coreOp->GetTopPadding();
	}
      } else {
	const BoundingBox& scriptBox = underScript->GetBoundingBox();

	underShiftX = (baseBox.width - scriptBox.width) / 2;
	underShiftY = baseBox.descent + underSpacing + scriptBox.ascent;
	underClearance = ruleThickness;
      }
    }

    if (overScript != NULL) {
      const MathMLCharNode* cChar = overScript->GetCharNode();

      if (accent &&
	  bChar != NULL && cChar != NULL &&
	  isCombiningAbove(cChar->GetChar()) &&
	  bChar->CombineWith(cChar, overShiftX, overShiftY)) {
	MathEngine::logger(LOG_DEBUG, "this is the special handling for U+%04X used as accent over U+%04X",
			   cChar->GetChar(), bChar->GetChar());

	if (overScript->IsEmbellishedOperator()) {
	  MathMLEmbellishedOperatorElement* eOp = TO_EMBELLISHED_OPERATOR(overScript);
	  assert(eOp != NULL);
	  MathMLOperatorElement* coreOp = eOp->GetCoreOperator();
	  assert(coreOp != NULL);
	  MathEngine::logger(LOG_DEBUG, "the accent will get en extra spacing of %d", sp2ipx(coreOp->GetBottomPadding()));
	  overShiftY += coreOp->GetBottomPadding();
	}
      } else {
	const BoundingBox& scriptBox = overScript->GetBoundingBox();

	overShiftX = (baseBox.width - scriptBox.width) / 2 + scaledMax(0, baseBox.rBearing - baseBox.width);
	overShiftY = baseBox.ascent + overSpacing + scriptBox.descent;
	overClearance = ruleThickness;
      }
    }

    baseShiftX = scaledMax(0, - scaledMin(overShiftX, underShiftX));
  }

  overShiftX += baseShiftX;
  underShiftX += baseShiftX;

  box = base->GetBoundingBox();
  box.width += baseShiftX;
  box.lBearing += baseShiftX;

  if (underScript != NULL) {
    const BoundingBox& scriptBox = underScript->GetBoundingBox();

    box.width = scaledMax(box.width, underShiftX + scriptBox.width);
    box.rBearing = scaledMax(box.rBearing, underShiftX + scriptBox.rBearing);
    box.lBearing = scaledMin(box.lBearing, underShiftX + scriptBox.lBearing);
    box.ascent   = scaledMax(box.ascent, scriptBox.ascent - underShiftY);
    box.tAscent  = scaledMax(box.tAscent, scriptBox.tAscent - underShiftY);
    box.descent  = scaledMax(box.descent, scriptBox.descent + underShiftY);
    box.tDescent = scaledMax(box.tDescent, scriptBox.tDescent + underShiftY);
    box.descent += underClearance;
  }

  if (overScript != NULL) {
    const BoundingBox& scriptBox = overScript->GetBoundingBox();

    box.width = scaledMax(box.width, overShiftX + scriptBox.width);
    box.rBearing = scaledMax(box.rBearing, overShiftX + scriptBox.rBearing);
    box.lBearing = scaledMin(box.lBearing, overShiftX + scriptBox.lBearing);
    box.ascent   = scaledMax(box.ascent, scriptBox.ascent + overShiftY);
    box.tAscent  = scaledMax(box.tAscent, scriptBox.tAscent + overShiftY);
    box.descent  = scaledMax(box.descent, scriptBox.descent - overShiftY);
    box.tDescent = scaledMax(box.tDescent, scriptBox.tDescent - overShiftY);
    box.ascent += overClearance;
  }
  
  ConfirmLayout(id);

  ResetDirtyLayout(id, maxWidth);
}

void
MathMLUnderOverElement::SetPosition(scaled x, scaled y)
{
  assert(base != NULL);

  position.x = x;
  position.y = y;

  base->SetPosition(x + baseShiftX, y);

  if (underScript != NULL)
    underScript->SetPosition(x + underShiftX, y + underShiftY);

  if (overScript != NULL)
    overScript->SetPosition(x + overShiftX, y - overShiftY);
}

bool
MathMLUnderOverElement::IsExpanding() const
{
  assert(base != NULL);
  return base->IsExpanding();
}
