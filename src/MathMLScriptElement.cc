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
#include <stddef.h>

#include "traverseAux.hh"
#include "MathMLDummyElement.hh"
#include "MathMLScriptElement.hh"
#include "RenderingEnvironment.hh"

MathMLScriptElement::MathMLScriptElement(mDOMNodeRef node, TagId id) :
  MathMLContainerElement(node, id)
{
  subScript = superScript = NULL;
}

MathMLScriptElement::~MathMLScriptElement()
{
}

const AttributeSignature*
MathMLScriptElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature subSig[] = {
    { ATTR_SUBSCRIPTSHIFT, numberUnitParser, NULL, NULL },
    { ATTR_NOTVALID,       NULL,             NULL, NULL }
  };

  static AttributeSignature supSig[] = {
    { ATTR_SUPERSCRIPTSHIFT, numberUnitParser, NULL, NULL },
    { ATTR_NOTVALID,         NULL,             NULL, NULL }
  };

  const AttributeSignature* signature = NULL;
  if (IsA() == TAG_MSUB || IsA() == TAG_MSUBSUP || IsA() == TAG_MMULTISCRIPTS)
    signature = GetAttributeSignatureAux(id, subSig);
  if ((IsA() == TAG_MSUP || IsA() == TAG_MSUBSUP || IsA() == TAG_MMULTISCRIPTS) && signature == NULL)
    signature = GetAttributeSignatureAux(id, supSig);

  if (signature == NULL) signature = MathMLContainerElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLScriptElement::Normalize()
{
  unsigned n = (IsA() == TAG_MSUBSUP) ? 3 : 2;

  while (content.GetSize() > n) {
    MathMLElement* elem = content.RemoveLast();
    delete elem;
  }

  while (content.GetSize() < n) {
    MathMLElement* mdummy = new MathMLDummyElement();
    mdummy->SetParent(this);
    content.Append(mdummy);
  }

  // BEWARE: normalization has to be done here, since it may
  // change the content!!!
  MathMLContainerElement::Normalize();

  base = content.GetFirst();
  assert(base != NULL);

  if (IsA() == TAG_MSUB) subScript = content.GetLast();
  else if (IsA() == TAG_MSUP) superScript = content.GetLast();
  else {
    subScript   = content.Get(1);
    superScript = content.Get(2);
  }
}

void
MathMLScriptElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  ScriptSetup(env);

  assert(base != NULL);
  base->Setup(env);

  env->Push();
  env->AddScriptLevel(1);
  env->SetDisplayStyle(false);

  const Value* value = NULL;

  if (subScript != NULL) {
    subScript->Setup(env);

    value = GetAttributeValue(ATTR_SUBSCRIPTSHIFT, env, false);
    if (value != NULL) {
      assert(value->IsNumberUnit());

      UnitValue unitValue = value->ToNumberUnit();
      assert(!unitValue.IsPercentage());

      subMinShift = env->ToScaledPoints(unitValue);

      delete value;
    }
  }

  if (superScript != NULL) {
    superScript->Setup(env);

    value = GetAttributeValue(ATTR_SUPERSCRIPTSHIFT, env, false);
    if (value != NULL) {
      assert(value->IsNumberUnit());
      
      UnitValue unitValue = value->ToNumberUnit();
      assert(!unitValue.IsPercentage());

      superMinShift = env->ToScaledPoints(unitValue);

      delete value;
    }
  }

  env->Drop();
}

void
MathMLScriptElement::DoBoxedLayout(LayoutId id, BreakId bid, scaled maxWidth)
{
  if (!HasDirtyLayout(id, maxWidth)) return;

  assert(base != NULL);

  base->DoBoxedLayout(id, BREAK_NO, maxWidth / 2);
  if (subScript != NULL) subScript->DoBoxedLayout(id, BREAK_NO, maxWidth / 2);
  if (superScript != NULL) superScript->DoBoxedLayout(id, BREAK_NO, maxWidth / 2);

  const MathMLElement* rel = findRightmostChild(base);
  assert(rel != NULL);

  const BoundingBox& baseBox = base->GetBoundingBox();
  BoundingBox relBox = rel->GetBoundingBox();
  relBox.rBearing = baseBox.rBearing;
  relBox.width = baseBox.width;

  BoundingBox subScriptBox;
  BoundingBox superScriptBox;

  subScriptBox.Null();
  if (subScript != NULL) subScriptBox = subScript->GetBoundingBox();

  superScriptBox.Null();
  if (superScript != NULL) superScriptBox = superScript->GetBoundingBox();

  DoScriptLayout(relBox, subScriptBox, superScriptBox, subShiftX, subShiftY, superShiftX, superShiftY);

  box = baseBox;

  box.width = scaledMax(box.width,
			scaledMax(superShiftX + superScriptBox.width,
				  subShiftX + subScriptBox.width));
  box.rBearing = scaledMax(box.rBearing,
			   scaledMax(superShiftX + superScriptBox.rBearing,
				     subShiftX + subScriptBox.rBearing));

  if (subScript != NULL) {
    box.ascent   = scaledMax(box.ascent, subScriptBox.ascent - subShiftY);
    box.tAscent  = scaledMax(box.tAscent, subScriptBox.tAscent - subShiftY);
    box.descent  = scaledMax(box.descent, subScriptBox.descent + subShiftY);
    box.tDescent = scaledMax(box.tDescent, subScriptBox.tDescent + subShiftY);
  }

  if (superScript != NULL) {
    box.ascent   = scaledMax(box.ascent, superScriptBox.ascent + superShiftY);
    box.tAscent  = scaledMax(box.tAscent, superScriptBox.tAscent + superShiftY);
    box.descent  = scaledMax(box.descent, superScriptBox.descent - superShiftY);
    box.tDescent = scaledMax(box.tDescent, superScriptBox.tDescent - superShiftY);
  }

  ConfirmLayout(id);

  ResetDirtyLayout(id, maxWidth);
}

void
MathMLScriptElement::SetPosition(scaled x, scaled y)
{
  assert(base != NULL);

  position.x = x;
  position.y = y;

  base->SetPosition(x, y);

  if (subScript != NULL)
    subScript->SetPosition(x + subShiftX, y + subShiftY);

  if (superScript != NULL)
    superScript->SetPosition(x + superShiftX, y - superShiftY);
}
