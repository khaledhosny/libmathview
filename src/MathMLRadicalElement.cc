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

#include "stringAux.hh"
#include "MathEngine.hh"
#include "StringUnicode.hh"
#include "EntitiesTable.hh"
#include "MathMLCharNode.hh"
#include "MathMLRowElement.hh"
#include "MathMLDummyElement.hh"
#include "MathMLRadicalElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"

MathMLRadicalElement::MathMLRadicalElement(mDOMNodeRef node, TagId id) :
  MathMLNormalizingContainerElement(node, id)
{
  assert(id == TAG_MSQRT || id == TAG_MROOT);
  radical = NULL;
}

MathMLRadicalElement::~MathMLRadicalElement()
{
  delete radical;
}

void
MathMLRadicalElement::Normalize()
{
  if (IsA() == TAG_MSQRT)
    MathMLNormalizingContainerElement::Normalize();
  else {
    assert(IsA() == TAG_MROOT);

    while (content.GetSize() < 2) {
      MathMLElement* mdummy = new MathMLDummyElement();
      Append(mdummy);
    }
    
    while (content.GetSize() > 2) {
      MathMLElement* elem = content.RemoveLast();
      delete elem;
    }

    MathMLContainerElement::Normalize();
  }

  String* rad = MathEngine::entitiesTable.GetEntityContent(DOM_CONST_STRING("Sqrt"));
  assert(rad != NULL);
  if (radical != NULL) delete radical;
  radical = new MathMLCharNode(rad->GetChar(0));
  radical->SetParent(this);
  delete rad;
}

void
MathMLRadicalElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  spacing       = env->ToScaledPoints(env->GetMathSpace(MATH_SPACE_MEDIUM));
  color         = env->GetColor();
  background    = env->GetBackgroundColor();
  lineThickness = env->GetRuleThickness();

  MathMLElement* base = content.GetFirst();
  assert(base != NULL);
  base->Setup(env);

  assert(radical != NULL);
  radical->Setup(env);

  if (IsA() == TAG_MROOT) {
    MathMLElement* script = content.GetLast();
    assert(script != NULL);
    
    env->Push();
    env->SetDisplayStyle(false);
    env->AddScriptLevel(2);
    script->Setup(env);
    env->Drop();
  }

}

void
MathMLRadicalElement::DoBoxedLayout(LayoutId id, BreakId, scaled availWidth)
{
  if (!HasDirtyLayout(id, availWidth)) return;

  MathMLElement* base = content.GetFirst();
  assert(base != NULL);

  base->DoBoxedLayout(id, BREAK_NO, availWidth / content.GetSize());
  box = base->GetBoundingBox();

  assert(radical != NULL);
  radical->DoLayout();
  radical->DoVerticalStretchyLayout(box.ascent + lineThickness, box.descent, 0, false);
  const BoundingBox& radBox = radical->GetBoundingBox();

  box.width += radBox.width;
  box.rBearing += radBox.width;
  box.ascent = scaledMax(box.ascent + spacing, radBox.ascent);
  box.tAscent = scaledMax(box.tAscent, box.ascent);
  box.descent = scaledMax(box.descent, radBox.descent);
  box.tDescent = scaledMax(box.tDescent, box.descent);

  if (IsA() == TAG_MROOT) {
    MathMLElement* script = content.GetLast();
    assert(script != NULL);

    script->DoBoxedLayout(id, BREAK_NO, availWidth / 2);
    const BoundingBox& scriptBox = script->GetBoundingBox();

    box.width += scriptBox.width;

    if (box.GetHeight() / 2 < scriptBox.GetHeight()) {
      box.ascent += scriptBox.GetHeight() - box.GetHeight() / 2;
      box.tAscent += scriptBox.descent + scriptBox.tAscent - box.GetHeight() / 2;
    }
  }

  ConfirmLayout(id);

  ResetDirtyLayout(id, availWidth);
}

void
MathMLRadicalElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  MathMLElement* base = content.GetFirst();
  assert(base != NULL);

  assert(radical != NULL);
  const BoundingBox& radBox = radical->GetBoundingBox();

  if (IsA() == TAG_MROOT) {
    MathMLElement* script = content.GetLast();
    assert(script != NULL);

    const BoundingBox& baseBox   = base->GetBoundingBox();
    const BoundingBox& scriptBox = script->GetBoundingBox();

    script->SetPosition(x, y + (baseBox.GetHeight() / 2 - baseBox.ascent) - scriptBox.descent);
    radical->SetPosition(x + scriptBox.width, y);
    base->SetPosition(x + scriptBox.width + radBox.width, y);
  } else {
    radical->SetPosition(x, y - box.ascent + radBox.ascent);
    base->SetPosition(x + radBox.width, y);
  }
}

void
MathMLRadicalElement::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;

  MathMLContainerElement::Render(area);

  if (fGC[IsSelected()] == NULL) {
    GraphicsContextValues values;
    values.foreground = IsSelected() ? area.GetSelectionForeground() : color;
    values.background = IsSelected() ? area.GetSelectionBackground() : background;
    values.lineWidth = lineThickness;
    fGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND | GC_MASK_LINE_WIDTH);
  }

  assert(radical != NULL);
  radical->SetDirty();
  radical->Render(area);

  MathMLElement* base = content.GetFirst();
  assert(base != NULL);

  const BoundingBox& radBox = radical->GetBoundingBox();

  area.MoveTo(radical->GetX() + radBox.width, radical->GetY() - radBox.ascent + lineThickness / 2);
  area.DrawLineToDelta(fGC[IsSelected()], base->GetBoundingBox().width, 0);

  ResetDirty();
}

bool
MathMLRadicalElement::IsExpanding() const
{
  MathMLElement* base = content.GetFirst();
  assert(base != NULL);
  return base->IsExpanding();
}

scaled
MathMLRadicalElement::GetLeftEdge() const
{
  assert(radical != NULL);
  return scaledMin(MathMLContainerElement::GetLeftEdge(), radical->GetLeftEdge());
}
