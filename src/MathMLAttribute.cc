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

MathMLAttribute::MathMLAttribute(AttributeId i, const String& v)
  : id(i), value(v)
{
}

MathMLAttribute::~MathMLAttribute()
{
}

SmartPtr<Value>
MathMLAttribute::getParsedValue(const AttributeSignature* aSignature) const
{
  if (!parsedValue)
    {
      assert(aSignature);

      AttributeParser parser = aSignature->GetParser();
      assert(parser);

      StringTokenizer st(value);
      parsedValue = parser(st);
    }

  return parsedValue;
}

bool
MathMLAttribute::equal(const SmartPtr<MathMLAttribute>& attribute) const
{
  return attribute->id == id && attribute->value == value;
}
