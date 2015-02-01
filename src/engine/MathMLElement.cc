// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

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
  return nullptr;
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
  return nullptr;
}

SmartPtr<MathMLNamespaceContext>
MathMLElement::getMathMLNamespaceContext() const
{ return smart_cast<MathMLNamespaceContext>(getNamespaceContext()); }

