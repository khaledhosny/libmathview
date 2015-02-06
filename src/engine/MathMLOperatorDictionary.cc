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

#include "StringAux.hh"
#include "Attribute.hh"
#include "AbstractLogger.hh"
#include "MathMLAttributeSignatures.hh"
#include "MathMLOperatorDictionary.hh"
#include "AttributeSet.hh"

struct OperatorDictionaryEntry {
  const char* name;
  const char* accent;
  const char* fence;
  const char* form;
  const char* largeop;
  const char* linebreakstyle;
  const char* lspace;
  const char* movablelimits;
  const char* priority;
  const char* rspace;
  const char* separator;
  const char* stretchy;
  const char* symmetric;
} dictionary[] = {
#include "operatorDictionary.cc"
};

MathMLOperatorDictionary::MathMLOperatorDictionary()
{
  for (auto & entry : dictionary)
    {
      String form(entry.form);
      String opName(entry.name);
      FormDefaults& formDefaults = items[opName];
      SmartPtr<AttributeSet> defaults = AttributeSet::create();

      getAttribute(entry.accent, ATTRIBUTE_SIGNATURE(MathML, Operator, accent), defaults);
      getAttribute(entry.fence, ATTRIBUTE_SIGNATURE(MathML, Operator, fence), defaults);
      getAttribute(entry.form, ATTRIBUTE_SIGNATURE(MathML, Operator, form), defaults);
      getAttribute(entry.largeop, ATTRIBUTE_SIGNATURE(MathML, Operator, largeop), defaults);
      getAttribute(entry.lspace, ATTRIBUTE_SIGNATURE(MathML, Operator, lspace), defaults);
    //getAttribute(entry.maxsize, ATTRIBUTE_SIGNATURE(MathML, Operator, maxsize), defaults);
    //getAttribute(entry.minsize, ATTRIBUTE_SIGNATURE(MathML, Operator, minsize), defaults);
      getAttribute(entry.movablelimits, ATTRIBUTE_SIGNATURE(MathML, Operator, movablelimits), defaults);
      getAttribute(entry.rspace, ATTRIBUTE_SIGNATURE(MathML, Operator, rspace), defaults);
      getAttribute(entry.separator, ATTRIBUTE_SIGNATURE(MathML, Operator, separator), defaults);
      getAttribute(entry.stretchy, ATTRIBUTE_SIGNATURE(MathML, Operator, stretchy), defaults);
      getAttribute(entry.symmetric, ATTRIBUTE_SIGNATURE(MathML, Operator, symmetric), defaults);

      if (form == "prefix")
        formDefaults.prefix = defaults;
      else if (form == "infix")
        formDefaults.infix = defaults;
      else if (form == "postfix")
        formDefaults.postfix = defaults;
      //else
      //  logger.out(LOG_WARNING,
      //           "invalid `form' attribute for entry `%s' in operator dictionary (ignored)",
      //           escape(UCS4StringOfString(opName)).c_str());
    }
}

MathMLOperatorDictionary::~MathMLOperatorDictionary()
{ }

void
MathMLOperatorDictionary::getAttribute(const char* value,
                                       const AttributeSignature& signature,
                                       const SmartPtr<AttributeSet>& aList)
{
  if (value)
    {
      SmartPtr<Attribute> attribute = Attribute::create(signature, value);
      aList->set(attribute);
    }
}

void
MathMLOperatorDictionary::search(const String& opName,
				 SmartPtr<AttributeSet>& prefix,
				 SmartPtr<AttributeSet>& infix,
				 SmartPtr<AttributeSet>& postfix) const
{
  prefix = infix = postfix = nullptr;

  auto p = items.find(opName);
  if (p != items.end())
    {
      prefix = (*p).second.prefix;
      infix = (*p).second.infix;
      postfix = (*p).second.postfix;
    }
}

