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

#ifndef __Attribute_hh__
#define __Attribute_hh__

#include "SmartPtr.hh"
#include "Object.hh"
#include "Value.hh"
#include "String.hh"
#include "AttributeSignature.hh"

class GMV_MathView_EXPORT Attribute : public Object
{
protected:
  Attribute(const class AttributeSignature&, const String&);
  virtual ~Attribute();

public:
  static SmartPtr<Attribute> create(const class AttributeSignature& sig, const String& value)
  { return new Attribute(sig, value); }

  const class AttributeSignature& getSignature(void) const { return signature; }
  String getUnparsedValue(void) const { return unparsedValue; }
  SmartPtr<Value> getValue(void) const;
  bool equal(const SmartPtr<Attribute>&) const;

private:
  const class AttributeSignature& signature;
  String unparsedValue;
  mutable SmartPtr<Value> value;
};

#endif // __Attribute_hh__
