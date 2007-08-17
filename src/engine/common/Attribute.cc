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

#include "Attribute.hh"
#include "AttributeSignature.hh"

Attribute::Attribute(const AttributeSignature& sig, const String& v)
  : signature(sig), unparsedValue(v)
{ }

Attribute::~Attribute()
{ }

SmartPtr<Value>
Attribute::getValue() const
{
  if (!value)
    {
      value = signature.parseValue(unparsedValue);
      if (!value)
	{
	  // issue warning
	  value = signature.getDefaultValue();
	}
    }

  return value;
}

bool
Attribute::equal(const SmartPtr<Attribute>& attribute) const
{
  return attribute->signature.name == signature.name && attribute->value == value;
}
