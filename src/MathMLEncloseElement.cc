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

#include "StringUnicode.hh"
#include "RenderingEnvironment.hh"
#include "MathMLEncloseElement.hh"
#include "MathMLRadicalElement.hh"

#if defined(HAVE_MINIDOM)
MathMLEncloseElement::MathMLEncloseElement(mDOMNodeRef node)
#elif defined(HAVE_GMETADOM)
MathMLEncloseElement::MathMLEncloseElement(const GMetaDOM::Element& node)
#endif
  : MathMLNormalizingContainerElement(node, TAG_MENCLOSE)
{
  normalized = false;
}

MathMLEncloseElement::~MathMLEncloseElement()
{
}

const AttributeSignature*
MathMLEncloseElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_NOTATION, notationParser, new StringC("longdiv"), NULL },
    { ATTR_NOTVALID, NULL,           NULL,                   NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLEncloseElement::NormalizeRadicalElement()
{
  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);

  MathMLElement* child = content.RemoveFirst();

  MathMLContainerElement* sqrt = new MathMLRadicalElement(NULL, TAG_MSQRT);
  sqrt->content.Append(child);
  child->SetParent(sqrt);
  sqrt->SetParent(this);
  sqrt->Normalize();

  content.Append(sqrt);
}

void
MathMLEncloseElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  const Value* value = GetAttributeValue(ATTR_NOTATION, env);
  assert(value != NULL);
  if (value->IsKeyword(KW_LONGDIV)) notation = NOTATION_LONGDIV;
  else if (value->IsKeyword(KW_ACTUARIAL)) notation = NOTATION_ACTUARIAL;
  else if (value->IsKeyword(KW_RADICAL)) notation = NOTATION_RADICAL;
  else assert(IMPOSSIBLE);
  delete value;

  spacing = env->ToScaledPoints(env->GetMathSpace(MATH_SPACE_MEDIUM));
  lineThickness = env->GetRuleThickness();
  color = env->GetColor();

  if (!normalized) {
    if (notation == NOTATION_RADICAL) NormalizeRadicalElement();
    normalized = true;
  }

  MathMLContainerElement::Setup(env);
}

void
MathMLEncloseElement::DoBoxedLayout(LayoutId id, BreakId, scaled availWidth)
{
  if (!HasDirtyLayout(id, availWidth)) return;

  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);

  MathMLNormalizingContainerElement::DoBoxedLayout(id, BREAK_NO, availWidth);
  box = content.GetFirst()->GetBoundingBox();

  if (notation != NOTATION_RADICAL) {
    box = content.GetFirst()->GetBoundingBox();
    box.ascent += spacing + lineThickness;
    box.width += spacing + lineThickness;
  }

  ConfirmLayout(id);

  ResetDirtyLayout(id, availWidth);
}

void
MathMLEncloseElement::SetPosition(scaled x, scaled y)
{
  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);

  position.x = x;
  position.y = y;

  if (notation == NOTATION_RADICAL)
    content.GetFirst()->SetPosition(x, y);
  else {
    if (notation == NOTATION_LONGDIV)
      content.GetFirst()->SetPosition(x + spacing + lineThickness, y);
    else
      content.GetFirst()->SetPosition(x, y);
  }
}

void
MathMLEncloseElement::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;

  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);

  MathMLNormalizingContainerElement::Render(area);

  if (fGC[IsSelected()] == NULL) {
    GraphicsContextValues values;
    values.foreground = IsSelected() ? area.GetSelectionForeground() : color;
    fGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND);
  }

  if (notation == NOTATION_LONGDIV) {
    area.MoveTo(GetX() + lineThickness / 2, GetY() + box.descent);
    area.DrawLineTo(fGC[IsSelected()], GetX() + lineThickness / 2, GetY() - box.ascent + lineThickness / 2);
    area.DrawLineTo(fGC[IsSelected()], GetX() + box.width, GetY() - box.ascent + lineThickness / 2);
  } else if (notation == NOTATION_ACTUARIAL) {
    area.MoveTo(GetX(), GetY() - box.ascent + lineThickness / 2);
    area.DrawLineTo(fGC[IsSelected()], GetX() + box.width - lineThickness / 2, GetY() - box.ascent + lineThickness / 2);
    area.DrawLineTo(fGC[IsSelected()], GetX() + box.width - lineThickness / 2, GetY() + box.descent);
  }

  ResetDirty();
}
