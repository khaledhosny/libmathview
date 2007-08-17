// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "MathMLMarkNode.hh"
#include "MathMLTokenElement.hh"
#include "MathMLAlignMarkElement.hh"
#include "MathMLAlignGroupElement.hh"

MathMLAlignGroupElement::MathMLAlignGroupElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLElement(context)
{ }

MathMLAlignGroupElement::~MathMLAlignGroupElement()
{ }

void
MathMLAlignGroupElement::SetDecimalPoint(const SmartPtr<class MathMLTokenElement>& token)
{
  assert(token);
  assert(!decimalPoint);
  decimalPoint = token;
}

void
MathMLAlignGroupElement::SetAlignmentMark(const SmartPtr<class MathMLMarkNode>& mark)
{
  assert(mark);
  assert(!alignMarkNode);
  alignMarkNode = mark;
}

void
MathMLAlignGroupElement::SetAlignmentMark(const SmartPtr<class MathMLAlignMarkElement>& mark)
{
  assert(mark);
  assert(!alignMarkElement);
  alignMarkElement = mark;
}

bool
MathMLAlignGroupElement::IsSpaceLike() const
{ return true; }

void
MathMLAlignGroupElement::SetWidth(const scaled& w)
{ width = w; }

