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

#include "MathMLDummyElement.hh"
#include "MathMLLabeledTableRowElement.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLTableElement.hh"
#include "MathMLTableRowElement.hh"
#include "MathMLNamespaceContext.hh"
#include "ValueConversion.hh"

MathMLLabeledTableRowElement::MathMLLabeledTableRowElement(const SmartPtr<MathMLNamespaceContext>& context)
  : MathMLTableRowElement(context)
{ }

MathMLLabeledTableRowElement::~MathMLLabeledTableRowElement()
{ }

void
MathMLLabeledTableRowElement::setFlagDown(Flags f)
{
  MathMLLinearContainerElement::setFlagDown(f);
  label.setFlagDown(f);
}

void
MathMLLabeledTableRowElement::resetFlagDown(Flags f)
{
  MathMLLinearContainerElement::resetFlagDown(f);
  label.resetFlagDown(f);
}

