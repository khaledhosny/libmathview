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

#include "Layout.hh"
#include "StringUnicode.hh"
#include "MathMLCharNode.hh"
#include "MathMLStringNode.hh"
#include "MathMLStringLitElement.hh"

MathMLStringLitElement::MathMLStringLitElement(mDOMNodeRef node) :
  MathMLTokenElement(node, TAG_MS)
{
  lQuote = rQuote = NULL;
  setupDone = false;
}

MathMLStringLitElement::~MathMLStringLitElement()
{
}

const AttributeSignature*
MathMLStringLitElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_LQUOTE,   NULL, new StringC("\"") },
    { ATTR_RQUOTE,   NULL, new StringC("\"") },
    { ATTR_NOTVALID, NULL, NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLTokenElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLStringLitElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  const String* s = NULL;

  if (setupDone) {
    MathMLFrame* frame = content.RemoveFirst();
    delete frame;
    frame = content.RemoveLast();
    delete frame;
  }

  s = GetAttribute(ATTR_LQUOTE, env);
  assert(s != NULL);
  if (s->GetLength() == 1) lQuote = new MathMLCharNode(s->GetChar(0));
  else if (s->GetLength() > 1) lQuote = new MathMLStringNode(s->Clone());
  assert(lQuote != NULL);
  lQuote->SetParent(this);
  content.AddFirst(lQuote);

  s = GetAttribute(ATTR_RQUOTE, env);
  assert(s != NULL);
  if (s->GetLength() == 1) rQuote = new MathMLCharNode(s->GetChar(0));
  else if (s->GetLength() > 0) rQuote = new MathMLStringNode(s->Clone());
  assert(rQuote != NULL);
  rQuote->SetParent(this);
  content.AddLast(rQuote);

  MathMLTokenElement::Setup(env);

  setupDone = true;
}
