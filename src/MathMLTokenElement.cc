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
#include <wctype.h>

#include "AFont.hh"
#include "Layout.hh"
#include "frameAux.hh"
#include "Iterator.hh"
#include "stringAux.hh"
#include "MathEngine.hh"
#include "ShapeFactory.hh"
#include "allocTextNode.hh"
#include "StringUnicode.hh"
#include "MathMLMarkNode.hh"
#include "MathMLTextNode.hh"
#include "ValueConversion.hh"
#include "MathMLSpaceNode.hh"
#include "MathMLStringNode.hh"
#include "MathMLTokenElement.hh"
#include "RenderingEnvironment.hh"

#define LINK_COLOR RGB(50, 100, 139)

MathMLTokenElement::MathMLTokenElement(mDOMNodeRef node, TagId t) : MathMLElement(node, t)
{
  rawContentLength = 0;
}

MathMLTokenElement::~MathMLTokenElement()
{
  Free();
}

void
MathMLTokenElement::Free()
{
  for (Iterator<MathMLTextNode*> i(content); i.More(); i.Next()) delete i();
}

const AttributeSignature*
MathMLTokenElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_FONTSIZE,   numberUnitParser, NULL },
    { ATTR_FONTWEIGHT, fontWeightParser, NULL },
    { ATTR_FONTSTYLE,  fontStyleParser,  new StringC("normal") },
    { ATTR_FONTFAMILY, stringParser,     NULL },
    { ATTR_COLOR,      colorParser,      NULL },
    { ATTR_NOTVALID,   NULL,             NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLTokenElement::Append(const String* s)
{
  assert(s != NULL);

  if (s->GetLength() == 0) return;

  rawContentLength += s->GetLength();

  MathMLTextNode* last = NULL;
  if (content.GetSize() > 0 &&
      content.GetLast() != NULL &&
      content.GetLast()->IsText()) {
    last = TO_TEXT(content.GetLast());
    assert(last != NULL);
  }

  unsigned i = 0;
  while (i < s->GetLength()) {
    MathMLTextNode* node = NULL;

    int spacing;
    BreakId bid;
    unsigned len = isNonMarkingChar(*s, i, &spacing, &bid);
    if (len > 0) {
      if (last != NULL && last->GetBreakability() < BREAK_YES) {
	last->AddSpacing(spacing);
	last->AddBreakability(bid);
      } else node = new MathMLSpaceNode(spacing, bid);
      i += len;
    } else if (isPlain(s->GetChar(i))) {
      unsigned start = i;
      while (i < s->GetLength() && isPlain(s->GetChar(i)) && !iswspace(s->GetChar(i))) i++;
      assert(start < i);
      
      const String* sText = allocString(*s, start, i - start);
      node = allocTextNode(&sText);
    } else if (!isModifier(s->GetChar(i))) {
      node = allocCharNode(s->GetChar(i));
      i++;
    } else {
      MathEngine::logger(LOG_WARNING, "ignoring modifier char U+%04x", s->GetChar(i));
      i++;
    }
    
    if (node != NULL) {
      Append(node);
      last = node;
    }
  }
}

void
MathMLTokenElement::Append(MathMLTextNode* node)
{
  assert(node != NULL);
  node->SetParent(this);
  content.Append(node);
}

void
MathMLTokenElement::Setup(RenderingEnvironment* env)
{
  env->Push();

  if (IsA() == TAG_MTEXT || IsA() == TAG_MN) {
    env->SetFontMode(FONT_MODE_TEXT);
  }

  const Value* value = NULL;

  value = GetAttributeValue(ATTR_FONTSIZE, NULL, false);
  if (value != NULL) env->SetFontSize(value->ToNumberUnit());

  value = GetAttributeValue(ATTR_FONTFAMILY, NULL, false);
  if (value != NULL) env->SetFontFamily(value->ToString());

  value = GetAttributeValue(ATTR_FONTWEIGHT, NULL, false);
  if (value != NULL) env->SetFontWeight(ToFontWeightId(value));

  value = GetAttributeValue(ATTR_FONTSTYLE, NULL, false);
  if (value != NULL) env->SetFontStyle(ToFontStyleId(value));
  else if (IsA() == TAG_MI) {
    if (rawContentLength == 1) env->SetFontStyle(FONT_STYLE_ITALIC);
    else {
      env->SetFontStyle(FONT_STYLE_NORMAL);
      env->SetFontMode(FONT_MODE_TEXT);
    }
  }

  value = GetAttributeValue(ATTR_COLOR, NULL, false);
  if (value != NULL) env->SetColor(ToRGB(value));
  else if (HasLink()) env->SetColor(LINK_COLOR);

  color      = env->GetColor();
  background = env->GetBackgroundColor();
  sppm       = env->GetScaledPointsPerEm();

  for (Iterator<MathMLTextNode*> p(content); p.More(); p.Next()) {
    assert(p() != NULL);
    p()->Setup(env);
  }

  env->Drop();
}

void
MathMLTokenElement::DoLayout(LayoutId id, Layout& layout)
{
  scaled oldAscent;
  scaled oldDescent;
  layout.GetLineStrut(oldAscent, oldDescent);
  //layout.SetMinimumLineStrut(font->GetAscent(), font->GetDescent());

  Iterator<MathMLTextNode*> i(content);
  while (i.More()) {
    assert(i() != NULL);
    MathMLTextNode* text = i();
    assert(text != NULL);

    scaled spacing = (sppm * text->GetSpacing()) / 18;
    BreakId breakability = text->GetBreakability();
    
    // the breakability after the token will be set by the
    // surrounding context
    if (i.IsLast()) breakability = BREAK_NO;
    else if (breakability == BREAK_AUTO) breakability = BREAK_GOOD;
    
    // ok, we do the actual layout of the chunk only if we are
    // doing minimum layout. In all the other cases the layout is exactly the same
    // as the minimum layout, that we have previously done,
    // so we save some work.
    if (id == LAYOUT_MIN) text->DoLayout();
    
    if (!text->IsSpace())
      layout.Append(text, spacing, breakability);
    else {
      layout.SetLastBreakability(breakability);
      layout.Append(spacing, breakability);
    }

    i.Next();
  }
  
  layout.SetLineStrut(oldAscent, oldDescent);
}

void
MathMLTokenElement::Freeze()
{
  if (HasLayout()) MathMLElement::Freeze();
  else {
    if (shape != NULL) delete shape;
    ShapeFactory shapeFactory;
    for (Iterator<MathMLTextNode*> i(content); i.More(); i.Next()) {
      assert(i() != NULL);

      Rectangle* rect = new Rectangle;
      getFrameBoundingBox(i(), LAYOUT_AUTO).ToRectangle(i()->GetX(), i()->GetY(), *rect);
      shapeFactory.Add(rect);
      if (i()->IsLast()) shapeFactory.SetNewRow();
    }
    shape = shapeFactory.GetShape();
  }
}

void
MathMLTokenElement::Render(const DrawingArea& area)
{
  if (!HasDirtyChildren()) return;

  RenderBackground(area);

  if (fGC[IsSelected()] == NULL) {
    GraphicsContextValues values;

    values.foreground = IsSelected() ? area.GetSelectionForeground() : color;
    values.background = IsSelected() ? area.GetSelectionBackground() : background;
    fGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
  }

  for (Iterator<MathMLTextNode*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);
    i()->Render(area);
  }

  //area.DrawRectangle(fGC[0], *shape);

  ResetDirty();
}

void
MathMLTokenElement::GetLinearBoundingBox(BoundingBox& b) const
{
  b.Null();
  for (Iterator<MathMLTextNode*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);
    const BoundingBox& textBox = i()->GetBoundingBox();
    b.Append(textBox);
  }
}

bool
MathMLTokenElement::IsToken() const
{
  return true;
}

bool
MathMLTokenElement::IsBreakable() const
{
  return true;
}

BreakId
MathMLTokenElement::GetBreakability() const
{
  // we have to skip some empty elements (malignmark) and get
  // the breakability of the last text node

  Iterator<MathMLTextNode*> text(content);
  text.ResetLast();
  while (text.More()) {
    assert(text() != NULL);
    if (!text()->IsMark()) break;
    text.Prev();
  }

  return text.More() ? text()->GetBreakability() : BREAK_AUTO;
}

void
MathMLTokenElement::SetDirty(const Rectangle* rect)
{
  assert(IsShaped());

  dirtyBackground =
    (GetParent() != NULL && (GetParent()->IsSelected() != IsSelected())) ? 1 : 0;

  if (IsDirty()) return;
  if (rect != NULL && !shape->Overlaps(*rect)) return;

  dirty = 1;
  SetDirtyChildren();

  for (Iterator<MathMLTextNode*> text(content); text.More(); text.Next()) {
    assert(text() != NULL);
    text()->SetDirty(rect);
  }
}

scaled
MathMLTokenElement::GetLeftEdge() const
{
  scaled edge = 0;

  for (Iterator<MathMLTextNode*> text(content); text.More(); text.Next()) {
    assert(text() !=  NULL);
    if (text.IsFirst()) edge = text()->GetLeftEdge();
    else edge = scaledMin(edge, text()->GetLeftEdge());
  }

  return edge;
}

scaled
MathMLTokenElement::GetRightEdge() const
{
  scaled edge = 0;

  for (Iterator<MathMLTextNode*> text(content); text.More(); text.Next()) {
    assert(text() !=  NULL);
    if (text.IsFirst()) edge = text()->GetRightEdge();
    else edge = scaledMax(edge, text()->GetRightEdge());
  }

  return edge;
}

scaled
MathMLTokenElement::GetDecimalPointEdge() const
{
  for (Iterator<MathMLTextNode*> text(content); text.More(); text.Next()) {
    assert(text() != NULL);
    if (text()->HasDecimalPoint()) return text()->GetDecimalPointEdge();
  }

  return GetRightEdge();
}

bool
MathMLTokenElement::IsNonMarking() const
{
  for (Iterator<MathMLTextNode*> text(content); text.More(); text.Next()) {
    assert(text() != NULL);
    if (!text()->IsSpace()) return false;
  }

  return true;
}
