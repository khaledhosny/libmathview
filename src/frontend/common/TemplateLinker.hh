// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __TemplateLinker_hh__
#define __TemplateLinker_hh__

#include "HashMap.hh"

template <class Model, typename ELEMENT = typename Model::Element>
class TemplateLinker
{
public:
  TemplateLinker(void) { }
  ~TemplateLinker() { }

  void
  add(const ELEMENT& el, class Element* elem)
  {
    assert(el);
    assert(elem);
    forwardMap[el] = elem;
    backwardMap[elem] = el;
  }

  bool
  remove(const ELEMENT& el)
  {
    assert(el);
    typename ForwardMap::iterator p = forwardMap.find(el);
    if (p != forwardMap.end())
      {
	backwardMap.erase((*p).second);
	forwardMap.erase(p);
	return true;
      }
    else
      return false;
  }

  bool
  remove(class Element* elem)
  {
    assert(elem);
    typename BackwardMap::iterator p = backwardMap.find(elem);
    if (p != backwardMap.end())
      {
	forwardMap.erase((*p).second);
	backwardMap.erase(p);
	return true;
      }
    else
      return false;
  }

  class Element*
  assoc(const ELEMENT& el) const
  {
    assert(el);
    typename ForwardMap::const_iterator p = forwardMap.find(el);
    if (p != forwardMap.end())
      return (*p).second;
    else
      return 0;
  }

  ELEMENT
  assoc(class Element* elem) const
  {
    assert(elem);
    typename BackwardMap::const_iterator p = backwardMap.find(elem);
    if (p != backwardMap.end())
      return (*p).second;
    else
      return ELEMENT();
  }

protected:
  struct Element_hash
  {
    size_t operator()(class Element* elem) const
    {
      assert(elem);
      return reinterpret_cast<size_t>(elem);
    }
  };

  typedef HASH_MAP_NS::hash_map<ELEMENT, class Element*, typename Model::Hash> ForwardMap;
  typedef HASH_MAP_NS::hash_map<class Element*, ELEMENT, Element_hash> BackwardMap;
  ForwardMap forwardMap;
  BackwardMap backwardMap;
};

#endif // __TemplateLinker_hh__
