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

#include <functional>
#include <algorithm>

#include <cassert>

#include "AttributeSet.hh"
#include "AttributeSignature.hh"

struct IsPredicate
  : public std::binary_function<SmartPtr<Attribute>,AttributeId,bool>
{
  bool operator()(const SmartPtr<Attribute>& attr, const AttributeId& id) const
  {
    assert(attr);
    return ATTRIBUTE_ID_OF_SIGNATURE(attr->getSignature()) == id;
  }
};

AttributeSet::AttributeSet()
{ }

AttributeSet::~AttributeSet()
{ }

bool
AttributeSet::set(const SmartPtr<Attribute>& attr)
{
  assert(attr);
#if 1
  // this version should be slightly more efficient
  // see Scott Meyers' book on effective STL
  auto lb = content.lower_bound(ATTRIBUTE_ID_OF_SIGNATURE(attr->getSignature()));
  if (lb != content.end() && !(content.key_comp()(ATTRIBUTE_ID_OF_SIGNATURE(attr->getSignature()), lb->first)))
    {
      if (!attr->equal(lb->second))
	{
	  lb->second = attr;
	  return true;
	}
      else
	return false;
    }
  else
    {
      content.insert(lb, Map::value_type(ATTRIBUTE_ID_OF_SIGNATURE(attr->getSignature()), attr));
      return true;
    }
#else
  Map::iterator p = content.find(ATTRIBUTE_ID_OF_SIGNATURE(attr->getSignature()));
  if (p != content.end())
    {
      bool eq = attr->equal(p->second);
      p->second = attr;
      return !eq;
    }
  else
    {
      content[ATTRIBUTE_ID_OF_SIGNATURE(attr->getSignature())] = attr;
      return true;
    }
#endif
}

SmartPtr<Attribute>
AttributeSet::get(const AttributeId& id) const
{
  auto p = content.find(id);
  return (p != content.end()) ? p->second : nullptr;
}

bool
AttributeSet::remove(const AttributeId& id)
{
  auto p = content.find(id);
  if (p != content.end())
    {
      content.erase(p);
      return true;
    }
  else
    return false;
}
