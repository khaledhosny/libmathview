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

#include <config.h>

#include <cassert>

#include "MathMLDOMLinker.hh"
#include "MathMLElement.hh"

MathMLDOMLinker::MathMLDOMLinker()
{
}

MathMLDOMLinker::~MathMLDOMLinker()
{
}

SmartPtr<MathMLElement>
MathMLDOMLinker::get(const DOM::Element& elem) const
{
  assert(elem);

  DOMNodeMap::iterator p = nodeMap.find(elem);
  if (p != nodeMap.end()) return static_cast<MathMLElement*>((*p).second);
  else return 0;
}

#if 0
SmartPtr<MathMLElement>
MathMLDOMLinker::findFormattingNode(const DOM::Node& node) const
{
  for (DOM::Node p = node; p; p = p.get_parentNode())
    if (SmartPtr<MathMLElement> fNode = getFormattingNode(p))
      return fNode;
  
  return 0;
}
#endif

void
MathMLDOMLinker::add(const DOM::Element& elem, const SmartPtr<MathMLElement>& fElem)
{
  assert(elem);
  nodeMap[elem] = static_cast<MathMLElement*>(fElem);
}

bool
MathMLDOMLinker::remove(const DOM::Element& elem)
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
