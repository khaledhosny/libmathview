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

#ifndef __LinearContainerTemplate_hh__
#define __LinearContainerTemplate_hh__

#include <vector>

#include "for_each_if.h"
#include "Adapters.hh"

template <class E, class T, class TPtr = SmartPtr<T> >
class LinearContainerTemplate
{
public:
  LinearContainerTemplate(void) { }

  unsigned getSize(void) const { return content.size(); }

  void setSize(E* elem, unsigned size)
  {
    if (size != content.size())
      {
	for (unsigned i = size; i < content.size(); i++) setChild(elem, i, 0);
	content.resize(size);
	elem->setDirtyLayout();
      }
  }

  TPtr getChild(unsigned i) const
  {
    assert(i < getSize());
    return content[i];
  }

  void setChild(E* elem, unsigned i, const TPtr& child)
  {
    assert(i <= getSize());

    if (i == getSize()) appendChild(elem, child);
    else if (content[i] != child)
      {
	if (child) T::setParent(child, elem);
	content[i] = child;
	elem->setDirtyLayout();
      }
  }

  void appendChild(E* elem, const TPtr& child)
  {
    if (child) T::setParent(child, elem);
    content.push_back(child);
    elem->setDirtyLayout();
  }

  void swapContent(E* elem, std::vector<TPtr>& newContent)
  {
    if (newContent != content)
      {
	if (T::hasParentLink())
	  for (typename std::vector<TPtr>::const_iterator p = newContent.begin();
	       p != newContent.end();
	       p++)
	    if (*p) T::setParent(*p, elem);
	
	content.swap(newContent);
	elem->setDirtyLayout();
      }
  }

  const std::vector<TPtr>& getContent(void) const { return content; }

  template <typename UnaryFunction>
  UnaryFunction for_each(UnaryFunction f) const
  { return for_each_if(content.begin(), content.end(), NotNullPredicate<T,TPtr>(), f); }

  typename std::vector<TPtr>::const_iterator begin(void) const
  { return content.begin(); }

  typename std::vector<TPtr>::const_iterator end(void) const
  { return content.end(); }

  typename std::vector<TPtr>::const_reverse_iterator rbegin(void) const
  { return content.rbegin(); }

  typename std::vector<TPtr>::const_reverse_iterator rend(void) const
  { return content.rend(); }

  void setFlagDown(Element::Flags f)
  { for_each(std::bind2nd(SetFlagDownAdapter<T,TPtr>(), f)); }

  void resetFlagDown(Element::Flags f)
  { for_each(std::bind2nd(ResetFlagDownAdapter<T,TPtr>(), f)); }

private:
  std::vector<TPtr> content;
};

#endif // __LinearContainerTemplate_hh__
