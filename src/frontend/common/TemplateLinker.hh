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

#ifndef __TemplateLinker_hh__
#define __TemplateLinker_hh__

#include <unordered_map>

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

  void
  update(const ELEMENT& el, class Element* elem)
  {
    assert(el);
    assert(elem);
    class Element*& oldElem = forwardMap[el];
    if (oldElem != elem)
      {
	oldElem = elem;
	backwardMap[elem] = el;
      }
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

  typedef std::unordered_map<ELEMENT, class Element*, typename Model::Hash> ForwardMap;
  typedef std::unordered_map<class Element*, ELEMENT, Element_hash> BackwardMap;
  ForwardMap forwardMap;
  BackwardMap backwardMap;
};

#endif // __TemplateLinker_hh__
