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

#include "MathMLMarkNode.hh"
#include "MathMLTokenElement.hh"
#include "MathMLAlignMarkElement.hh"
#include "MathMLAlignGroupElement.hh"
#include "FormattingContext.hh"

MathMLAlignGroupElement::MathMLAlignGroupElement()
{
  Init();
}

#if defined(HAVE_GMETADOM)
MathMLAlignGroupElement::MathMLAlignGroupElement(const DOM::Element& node)
  : MathMLElement(node)
{
  Init();
}
#endif

void
MathMLAlignGroupElement::Init()
{
  width = 0;
}

MathMLAlignGroupElement::~MathMLAlignGroupElement()
{
}

void
MathMLAlignGroupElement::DoLayout(const FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      box.Set(width, 0, 0);
      ResetDirtyLayout(ctxt);
    }
}

void
MathMLAlignGroupElement::SetDecimalPoint(const Ptr<class MathMLTokenElement>& token)
{
  assert(token);
  assert(!decimalPoint);
  decimalPoint = token;
}

void
MathMLAlignGroupElement::SetAlignmentMark(const Ptr<class MathMLMarkNode>& mark)
{
  assert(mark);
  assert(!alignMarkNode);
  alignMarkNode = mark;
}

void
MathMLAlignGroupElement::SetAlignmentMark(const Ptr<class MathMLAlignMarkElement>& mark)
{
  assert(mark);
  assert(!alignMarkElement);
  alignMarkElement = mark;
}

void
MathMLAlignGroupElement::Normalize(const Ptr<class MathMLDocument>&)
{
  if (DirtyStructure()) ResetDirtyStructure();
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
