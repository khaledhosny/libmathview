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

#include <config.h>

#include <cassert>

#include "gmetadom.hh"
#include "Linker.hh"
#include "Element.hh"
#include "ElementFactory.hh"

Linker::Linker()
{ }

Linker::~Linker()
{ }

SmartPtr<Element>
Linker::get(const DOM::Element& elem) const
{
  assert(elem);
  DOMNodeMap::const_iterator p = nodeMap.find(elem);
  if (p != nodeMap.end())
    return static_cast<Element*>((*p).second);
  else
    return 0;
}

SmartPtr<Element>
Linker::get(const DOM::Element& elem, const SmartPtr<ElementFactory>& factory)
{
  assert(elem);

  if (SmartPtr<Element> res = get(elem))
    return res;
  else if (factory)
    {
      SmartPtr<Element> res = factory->createElement(DOMX::nodeLocalName(elem));
      assert(res);
      res->setLinker(this);
      res->setDOMElement(elem);
      add(elem, res);
      return res;
    }
  else
    return 0;
}

#if 0
SmartPtr<MathMLElement>
Linker::findFormattingNode(const DOM::Node& node) const
{
  for (DOM::Node p = node; p; p = p.get_parentNode())
    if (SmartPtr<MathMLElement> fNode = getFormattingNode(p))
      return fNode;
  
  return 0;
}
#endif

void
Linker::add(const DOM::Element& elem, const SmartPtr<Element>& fElem)
{
  assert(elem);
  nodeMap[elem] = static_cast<Element*>(fElem);
}

bool
Linker::remove(const DOM::Element& elem)
{
  assert(elem);
  DOMNodeMap::iterator p = nodeMap.find(elem);
  if (p != nodeMap.end())
    {
      nodeMap.erase(p);
      return true;
    }
  else
    return false;
}
