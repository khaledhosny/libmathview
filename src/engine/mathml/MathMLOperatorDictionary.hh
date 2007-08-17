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

#ifndef __MathMLOperatorDictionary_hh__
#define __MathMLOperatorDictionary_hh__

#include "SmartPtr.hh"
#include "String.hh"
#include "StringHash.hh"
#include "HashMap.hh"
#include "Object.hh"

class GMV_MathView_EXPORT MathMLOperatorDictionary : public Object
{
protected:
  MathMLOperatorDictionary(void);
  virtual ~MathMLOperatorDictionary();

public:
  static SmartPtr<MathMLOperatorDictionary> create(void) { return new MathMLOperatorDictionary; }

  void add(const class AbstractLogger&,
	   const String&, const String&, const SmartPtr<class AttributeSet>&);
  void search(const String&,
	      SmartPtr<class AttributeSet>&,
	      SmartPtr<class AttributeSet>&,
	      SmartPtr<class AttributeSet>&) const;

private:
  void unload(void);

  struct FormDefaults
  {
    SmartPtr<class AttributeSet> prefix;
    SmartPtr<class AttributeSet> infix;
    SmartPtr<class AttributeSet> postfix;
  };

  typedef HASH_MAP_NS::hash_map<String,FormDefaults,StringHash,StringEq> Dictionary;
  Dictionary items;
};

#endif // __MathMLOperatorDictionary_hh__

