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

#include "AttributeSignature.hh"

SmartPtr<Value>
AttributeSignature::getDefaultValue() const
{
  if (!defaultValue && defaultUnparsedValue)
    defaultValue = parseValue(defaultUnparsedValue);

  return defaultValue;
}

SmartPtr<Value>
AttributeSignature::parseValue(const String& v) const
{
  assert(parser);
  UCS4String s = UCS4StringOfString(v);
  UCS4String::const_iterator next;
  return parser(s.begin(), s.end(), next);
}
