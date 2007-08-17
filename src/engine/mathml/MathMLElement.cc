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

#include "AttributeSet.hh"
#include "MathMLElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLNamespaceContext.hh"
#include "ValueConversion.hh"
#include "Variant.hh"

MathMLElement::MathMLElement(const SmartPtr<MathMLNamespaceContext>& context) : Element(context)
{ }

MathMLElement::~MathMLElement()
{ }

bool
MathMLElement::IsSpaceLike() const
{
  return false;
}

SmartPtr<MathMLOperatorElement>
MathMLElement::getCoreOperator()
{
  return 0;
}

SmartPtr<MathMLOperatorElement>
MathMLElement::getCoreOperatorTop()
{
  if (SmartPtr<MathMLOperatorElement> coreOp = getCoreOperator())
    {
      SmartPtr<MathMLElement> parent = getParent<MathMLElement>();
      if (!parent || parent->getCoreOperator() != coreOp)
	return coreOp;
    }
  return 0;
}

SmartPtr<MathMLNamespaceContext>
MathMLElement::getMathMLNamespaceContext() const
{ return smart_cast<MathMLNamespaceContext>(getNamespaceContext()); }

