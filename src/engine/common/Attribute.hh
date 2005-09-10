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
