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

#include "operatorAux.hh"
#include "MathMLDummyElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLUnderOverElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLEmbellishedOperatorElement.hh"

MathMLUnderOverElement::MathMLUnderOverElement(mDOMNodeRef node, TagId id) :
  MathMLContainerElement(node, id)
{
  assert(id == TAG_MUNDER || id == TAG_MOVER || id == TAG_MUNDEROVER);

  base = underScript = overScript = NULL;
}

MathMLUnderOverElement::~MathMLUnderOverElement()
{
}

const AttributeSignature*
MathMLUnderOverElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature underSig[] = {
    { ATTR_ACCENTUNDER, booleanParser, NULL },
    { ATTR_NOTVALID,    NULL,          NULL }
  };

  static AttributeSignature overSig[] = {
    { ATTR_ACCENT,      booleanParser, NULL },
    { ATTR_NOTVALID,    NULL,          NULL }
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

  scaled smallSpacing = env->ToScaledPoints(env->GetMathSpace(MATH_SPACE_VERYVERYTHIN));
  scaled bigSpacing   = env->ToScaledPoints(env->GetMathSpace(MATH_SPACE_MEDIUM));
  scriptSpacing       = env->ToScaledPoints(env->GetMathSpace(MATH_SPACE_THIN));
  background          = env->GetBackgroundColor();

  base->Setup(env);

  const MathMLOperatorElement* op = getCoreOperator(base);

  if (op != NULL) {
    scriptize = (env->GetDisplayStyle() == false) && op->HasMovableLimits();
  } else
    scriptize = false;

  env->Push();
  env->SetDisplayStyle(false);

  accentUnder = false;
  underSpacing = 0;
  if (underScript != NULL) {
    if (!scriptize) {
      const Value* value = GetAttributeValue(ATTR_ACCENTUNDER, env, false);
      if (value != NULL) accentUnder = value->ToBoolean();
      else {
	const MathMLOperatorElement* op = getCoreOperator(underScript);
	if (op != NULL) {
	  underScript->Setup(env);
	  accentUnder = op->IsAccent();
	}
      }
    }

    if (accentUnder) underSpacing = smallSpacing;
    else {
      env->AddScriptLevel(1);
      underSpacing = bigSpacing;
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
	const MathMLOperatorElement* op = getCoreOperator(overScript);
	if (op != NULL) {
	  overScript->Setup(env);
	  accent = op->IsAccent();
	}
      }
    }

    if (accent) overSpacing = smallSpacing;
    else {
      env->AddScriptLevel(1);
      overSpacing = bigSpacing;
    }
    overScript->Setup(env);
  }
  
  env->Drop();
}

void
MathMLUnderOverElement::DoBoxedLayout(LayoutId id, BreakId bid, scaled maxWidth)
{
  assert(base != NULL);

  if (scriptize) {
    base->DoBoxedLayout(id, BREAK_NO);
    box = base->GetBoundingBox();

    BoundingBox underBox;
    BoundingBox overBox;

    underBox.Null();
    if (underScript != NULL) {
      underScript->DoBoxedLayout(id, BREAK_NO);
      underBox = underScript->GetBoundingBox();
    }

    overBox.Null();
    if (overScript != NULL) {
      overScript->DoBoxedLayout(id, BREAK_NO);
      overBox = overScript->GetBoundingBox();
    }

    scaled baseHeight = box.GetHeight();

    box.width += scriptSpacing + scaledMax(underBox.width, overBox.width);

    underShift = box.descent - underBox.descent;
    overShift  = box.ascent - overBox.ascent;    
  
    if (underBox.GetHeight() + overBox.GetHeight() > baseHeight) {
      scaled more = (underBox.GetHeight() + overBox.GetHeight() - baseHeight) / 2;
      box.ascent += more;
      box.descent += more;
      underShift += more;
      overShift += more;
    }
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

      MathMLOperatorElement* baseOp  = getStretchyOperator(base, STRETCH_HORIZONTAL);
      MathMLOperatorElement* underOp = getStretchyOperator(underScript, STRETCH_HORIZONTAL);
      MathMLOperatorElement* overOp  = getStretchyOperator(overScript, STRETCH_HORIZONTAL);

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

    box = base->GetBoundingBox();

    if (underScript != NULL) {
      const BoundingBox& scriptBox = underScript->GetBoundingBox();

      box.width = scaledMax(box.width, scriptBox.width);
      box.descent += scriptBox.GetHeight() + underSpacing;
    }

    if (overScript != NULL) {
      const BoundingBox& scriptBox = overScript->GetBoundingBox();

      box.width = scaledMax(box.width, scriptBox.width);
      box.ascent += scriptBox.GetHeight() + overSpacing;
    }
  }

  ConfirmLayout(id);
}

void
MathMLUnderOverElement::SetPosition(scaled x, scaled y)
{
  assert(base != NULL);

  position.x = x;
  position.y = y;

  const BoundingBox& box     = GetBoundingBox();
  const BoundingBox& baseBox = base->GetBoundingBox();

  if (scriptize) {
    base->SetPosition(x, y);

    if (underScript != NULL)
      underScript->SetPosition(x + baseBox.width + scriptSpacing, y + underShift);

    if (overScript != NULL)
      overScript->SetPosition(x + baseBox.width + scriptSpacing, y - overShift);
  } else {
    base->SetPosition(x + (box.width - baseBox.width) / 2, y);

    if (underScript != NULL) {
      const BoundingBox& scriptBox = underScript->GetBoundingBox();
      underScript->SetPosition(x + (box.width - scriptBox.width) / 2,
			       y + baseBox.descent + underSpacing + scriptBox.ascent);
    }

    if (overScript != NULL) {
      const BoundingBox& scriptBox = overScript->GetBoundingBox();
      overScript->SetPosition(x + (box.width - scriptBox.width) / 2,
			      y - baseBox.ascent - overSpacing - scriptBox.descent);
    }      
  }
}

bool
MathMLUnderOverElement::IsExpanding() const
{
  assert(base != NULL);
  return base->IsExpanding();
}
