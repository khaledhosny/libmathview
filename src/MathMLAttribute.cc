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

#include "AttributeParser.hh"
#include "MathMLAttribute.hh"
#include "AttributeSignature.hh"

MathMLAttribute::MathMLAttribute(AttributeId i, const String* v)
{
  id = i;
  value = v;
  parsedValue = NULL;
}

MathMLAttribute::~MathMLAttribute()
{
  delete value;
  delete parsedValue;
}

const Value*
MathMLAttribute::GetParsedValue(const AttributeSignature* aSignature) const
{
  if (parsedValue == NULL) {
    assert(aSignature != NULL);
    assert(value != NULL);

    AttributeParser parser = aSignature->GetParser();
    assert(parser != NULL);

    StringTokenizer st(*value);
    parsedValue = parser(st);
  }

  return parsedValue;
}

bool
MathMLAttribute::Equal(const MathMLAttribute& attribute) const
{
  if (IsA() != attribute.IsA()) return false;

  const String* aValue = value;
  const String* bValue = attribute.GetValue();

  if (aValue == NULL && bValue != NULL) return false;
  if (aValue != NULL && bValue == NULL) return false;
  if (aValue != NULL && bValue != NULL) return aValue->Equal(*bValue);

  return true;
}
