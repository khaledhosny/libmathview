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

struct DeleteAdaptor
  : public std::unary_function<MathMLAttribute*,void>
{
  void operator()(MathMLAttribute* attr) const
  { delete attr; }
};

struct IsPredicate
  : public std::binary_function<MathMLAttribute*,AttributeId,bool>
{
  bool operator()(MathMLAttribute* attr, AttributeId id) const
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
  std::for_each(content.begin(), content.end(), DeleteAdaptor());
}

void
MathMLAttributeList::Append(MathMLAttribute* attr)
{
  assert(attr != 0);
  content.push_back(attr);
}

MathMLAttribute*
MathMLAttributeList::GetAttribute(AttributeId id) const
{
  std::vector<MathMLAttribute*>::const_iterator p =
    std::find_if(content.begin(), content.end(),
		 std::bind2nd(IsPredicate(), id));
  if (p != content.end()) return *p;
  else return 0;
}

bool
MathMLAttributeList::Equal(const MathMLAttributeList& aList) const
{
  if (content.size() != aList.content.size()) return false;

  for (std::vector<MathMLAttribute*>::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      assert(*p);
      MathMLAttribute* attribute = aList.GetAttribute((*p)->IsA());
      if (attribute == 0) return false;
      if (!(*p)->Equal(*attribute)) return false;
    }

  return true;
}
