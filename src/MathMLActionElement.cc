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
#include "StringUnicode.hh"
#include "AttributeParser.hh"
#include "MathMLActionElement.hh"

MathMLActionElement::MathMLActionElement(mDOMNodeRef node) : MathMLContainerElement(node, TAG_MACTION)
{
  selection = 0;
}

MathMLActionElement::~MathMLActionElement()
{
}

const AttributeSignature*
MathMLActionElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_ACTIONTYPE, NULL,          NULL },
    { ATTR_SELECTION,  integerParser, new StringC("1") },
    { ATTR_NOTVALID,   NULL,          NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLContainerElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLActionElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  const String* sValue = GetAttribute(ATTR_ACTIONTYPE, env, false);
  if (sValue != NULL)
    MathEngine::logger(LOG_WARNING, "action `%s' is not supported (ignored)", sValue->ToStaticC());
  else
    MathEngine::logger(LOG_WARNING, "no action specified for `maction' element");

  const Value* value = GetAttributeValue(ATTR_SELECTION, env);
  if (value != NULL) {
    selection = value->ToInteger() - 1;
    if (selection >= content.GetSize()) selection = content.GetSize() - 1;
  }

  MathMLContainerElement::Setup(env);
}

void
MathMLActionElement::DoBoxedLayout(LayoutId id, BreakId bid, scaled availWidth)
{
  MathMLElement* elem = GetSelectedElement();

  if (elem != NULL) {
    elem->DoBoxedLayout(id, bid, availWidth);
    box = elem->GetBoundingBox();
  } else
    box.Null();

  ConfirmLayout(id);
}

void
MathMLActionElement::DoLayout(LayoutId id, Layout& layout)
{
  MathMLElement* elem = GetSelectedElement();
  if (elem != NULL) elem->DoLayout(id, layout);
}

void
MathMLActionElement::DoStretchyLayout()
{
  MathMLElement* elem = GetSelectedElement();
  if (elem != NULL) elem->DoStretchyLayout();
}

void
MathMLActionElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  MathMLElement* elem = GetSelectedElement();
  if (elem != NULL) elem->SetPosition(x, y);
}

void
MathMLActionElement::Render(const DrawingArea& area)
{
  MathMLElement* elem = GetSelectedElement();
  if (elem != NULL) elem->Render(area);
}

void
MathMLActionElement::SetDirty(const Rectangle* rect)
{
  MathMLElement* elem = GetSelectedElement();
  if (elem != NULL) {
    elem->SetDirty(rect);
    dirty = elem->IsDirty();
  }
}

bool
MathMLActionElement::IsBreakable() const
{
  MathMLElement* elem = GetSelectedElement();
  return (elem != NULL) ? elem->IsBreakable() : false;
}

bool
MathMLActionElement::IsExpanding() const
{
  MathMLElement* elem = GetSelectedElement();
  return (elem != NULL) ? elem->IsExpanding() : false;
}

MathMLElement*
MathMLActionElement::GetSelectedElement() const
{
  return (selection < content.GetSize()) ? content.Get(selection) : NULL;
}

BreakId
MathMLActionElement::GetBreakability() const
{
  MathMLElement* elem = GetSelectedElement();
  return (elem != NULL) ? elem->GetBreakability() : BREAK_AUTO;
}

scaled
MathMLActionElement::GetLeftEdge() const
{
  MathMLElement* elem = GetSelectedElement();
  return (elem != NULL) ? elem->GetLeftEdge() : GetX();
}

scaled
MathMLActionElement::GetRightEdge() const
{
  MathMLElement* elem = GetSelectedElement();
  return (elem != NULL) ? elem->GetRightEdge() : GetX();
}

