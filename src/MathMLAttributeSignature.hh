// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef __MathMLAttributeSignature_hh__
#define __MathMLAttributeSignature_hh__

#include "String.hh"
#include "Value.hh"
#include "SmartPtr.hh"

typedef SmartPtr<Value> (*MathMLAttributeParser)(const UCS4String::const_iterator&,
						 const UCS4String::const_iterator&,
						 UCS4String::const_iterator&);

struct MathMLAttributeSignature
{
  String name;
  MathMLAttributeParser parser;
  bool fromElement;
  bool fromContext;
  bool deprecated;
  bool emptyMeaningful;
  const char* defaultUnparsedValue;
  mutable SmartPtr<Value> defaultValue;

  SmartPtr<Value> getDefaultValue(void) const;
  SmartPtr<Value> parseValue(const String&) const;
};

typedef const MathMLAttributeSignature* MathMLAttributeId;

#define ATTRIBUTE_SIGNATURE(ns,name) sig_##ns##_##name
#define ATTRIBUTE_ID_OF_SIGNATURE(sig) (&(sig))
#define ATTRIBUTE_ID(ns,name) (ATTRIBUTE_ID_OF_SIGNATURE(ATTRIBUTE_SIGNATURE(ns,name)))
#define ATTRIBUTE_PARSER(ns,name) &Parse_##ns##_##name::parse
#define ATTRIBUTE_NAME_OF_ID(id) ((id)->name)
#define REFINE_ATTRIBUTE(ctxt,ns,name) refineAttribute(ctxt, ATTRIBUTE_SIGNATURE(ns,name))
#define GET_ATTRIBUTE_VALUE(ns,name) getAttributeValue(ATTRIBUTE_SIGNATURE(ns,name))
#define GET_OPERATOR_ATTRIBUTE_VALUE(ns,name) getOperatorAttributeValue(ATTRIBUTE_SIGNATURE(ns,name))
#define DECLARE_ATTRIBUTE(ns,name) extern const MathMLAttributeSignature ATTRIBUTE_SIGNATURE(ns,name)
#define DEFINE_ATTRIBUTE(ns,s,name,fe,fc,de,em,df) \
  const MathMLAttributeSignature ATTRIBUTE_SIGNATURE(ns,name) = \
  { s, ATTRIBUTE_PARSER(ns,name), fe, fc, de, em, df, 0 }

#include "MathMLAttributeSignatures.ihh"

#endif // __MathMLAttributeSignature_hh__
