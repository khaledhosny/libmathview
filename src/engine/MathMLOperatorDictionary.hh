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

#ifndef __MathMLOperatorDictionary_hh__
#define __MathMLOperatorDictionary_hh__

#include "AttributeSignature.hh"
#include "SmartPtr.hh"
#include "String.hh"
#include "StringHash.hh"
#include <unordered_map>
#include "Object.hh"

class MathMLOperatorDictionary : public Object
{
protected:
  MathMLOperatorDictionary(void);
  virtual ~MathMLOperatorDictionary();

public:
  static SmartPtr<MathMLOperatorDictionary> create(void) { return new MathMLOperatorDictionary; }

  void search(const String&,
	      SmartPtr<class AttributeSet>&,
	      SmartPtr<class AttributeSet>&,
	      SmartPtr<class AttributeSet>&) const;

private:
  void getAttribute(const char*,
                    const AttributeSignature&,
                    const SmartPtr<AttributeSet>&);

  struct FormDefaults
  {
    SmartPtr<class AttributeSet> prefix;
    SmartPtr<class AttributeSet> infix;
    SmartPtr<class AttributeSet> postfix;
  };

  typedef std::unordered_map<String,FormDefaults,StringHash,StringEq> Dictionary;
  Dictionary items;
};

#endif // __MathMLOperatorDictionary_hh__

