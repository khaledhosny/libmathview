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

#include <functional>
#include <algorithm>

#include <assert.h>
#include <stddef.h>

#include "MathMLAttributeList.hh"

struct IsPredicate
  : public std::binary_function<SmartPtr<MathMLAttribute>,AttributeId,bool>
{
  bool operator()(const SmartPtr<MathMLAttribute>& attr, AttributeId id) const
  {
    assert(attr);
    return attr->isA() == id;
  }
};

MathMLAttributeList::MathMLAttributeList()
{
}

MathMLAttributeList::~MathMLAttributeList()
{
}

bool
MathMLAttributeList::set(const SmartPtr<MathMLAttribute>& attr)
{
  assert(attr);
  std::vector< SmartPtr<MathMLAttribute> >::iterator p =
    std::find_if(content.begin(), content.end(), std::bind2nd(IsPredicate(), attr->isA()));
  if (p != content.end())
    {
      bool res = attr->equal(*p);
      *p = attr;
      return res;
    }
  else
    {
      content.push_back(attr);
      return true;
    }
}

SmartPtr<MathMLAttribute>
MathMLAttributeList::get(AttributeId id) const
{
  std::vector< SmartPtr<MathMLAttribute> >::const_iterator p =
    std::find_if(content.begin(), content.end(), std::bind2nd(IsPredicate(), id));
  return (p != content.end()) ? *p : 0;
}

bool
MathMLAttributeList::remove(AttributeId id)
{
  std::vector< SmartPtr<MathMLAttribute> >::iterator p =
    std::remove_if(content.begin(), content.end(), std::bind2nd(IsPredicate(), id));
  if (p != content.end())
    {
      content.erase(p, content.end());
      return true;
    }
  else
    return false;
}
