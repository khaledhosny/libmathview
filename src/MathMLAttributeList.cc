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
    return attr->IsA() == id;
  }
};

MathMLAttributeList::MathMLAttributeList()
{
}

MathMLAttributeList::~MathMLAttributeList()
{
}

void
MathMLAttributeList::Append(const SmartPtr<MathMLAttribute>& attr)
{
  assert(attr);
  content.push_back(attr);
}

SmartPtr<MathMLAttribute>
MathMLAttributeList::GetAttribute(AttributeId id) const
{
  std::vector< SmartPtr<MathMLAttribute> >::const_iterator p =
    std::find_if(content.begin(), content.end(), std::bind2nd(IsPredicate(), id));
  return (p != content.end()) ? *p : 0;
}

bool
MathMLAttributeList::Equal(const MathMLAttributeList& aList) const
{
  if (content.size() != aList.content.size()) return false;

  for (std::vector< SmartPtr<MathMLAttribute> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      assert(*p);

      if (SmartPtr<MathMLAttribute> attribute = aList.GetAttribute((*p)->IsA()))
	{
	  if (!(*p)->Equal(attribute)) return false;
	}
      else
	return false;
    }

  return true;
}
