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

#include "MathMLAlignGroupElement.hh"

MathMLAlignGroupElement::MathMLAlignGroupElement(mDOMNodeRef node) :
  MathMLElement(node, TAG_MALIGNGROUP)
{
  width = 0;
  decimalPoint = NULL;
  alignMarkNode = NULL;
  alignMarkElement = NULL;
}

MathMLAlignGroupElement::~MathMLAlignGroupElement()
{
}

void
MathMLAlignGroupElement::DoBoxedLayout(LayoutId id, BreakId, scaled availWidth)
{
  if (!HasDirtyLayout(id, availWidth)) return;
  box.Set(width, 0, 0);
  ConfirmLayout(id);
  ResetDirtyLayout(id, availWidth);
}

void
MathMLAlignGroupElement::SetDecimalPoint(class MathMLTokenElement* token)
{
  assert(token != NULL);
  assert(decimalPoint == NULL);
  decimalPoint = token;
}

void
MathMLAlignGroupElement::SetAlignmentMark(class MathMLMarkNode* mark)
{
  assert(mark != NULL);
  assert(alignMarkNode == NULL);
  alignMarkNode = mark;
}

void
MathMLAlignGroupElement::SetAlignmentMark(class MathMLAlignMarkElement* mark)
{
  assert(mark != NULL);
  assert(alignMarkElement == NULL);
  alignMarkElement = mark;
}

bool
MathMLAlignGroupElement::IsSpaceLike() const
{
  return true;
}

void
MathMLAlignGroupElement::SetWidth(scaled w)
{
  width = w;
}
