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
#include "gmetadom_Linker.hh"
#include "Element.hh"
#include "ElementFactory.hh"

gmetadom_Linker::gmetadom_Linker()
{ }

gmetadom_Linker::~gmetadom_Linker()
{ }

void
gmetadom_Linker::add(const DOM::Node& node, Element* elem)
{
  assert(node);
  assert(elem);

  //elem->ref(); // this should not be necessary

  forwardMap[node] = elem;
  backwardMap[elem] = node;
}

bool
gmetadom_Linker::remove(const DOM::Node& node)
{
  assert(node);
  ForwardMap::iterator p = forwardMap.find(node);
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
gmetadom_Linker::remove(Element* elem)
{
  assert(elem);
  BackwardMap::iterator p = backwardMap.find(elem);
  if (p != backwardMap.end())
    {
      forwardMap.erase((*p).second);
      backwardMap.erase(p);
      return true;
    }
  else
    return false;
}

Element*
gmetadom_Linker::assoc(const DOM::Node& node) const
{
  assert(node);
  ForwardMap::const_iterator p = forwardMap.find(node);
  if (p != forwardMap.end())
    return (*p).second;
  else
    return 0;
}

DOM::Node
gmetadom_Linker::assoc(Element* elem) const
{
  assert(elem);
  BackwardMap::const_iterator p = backwardMap.find(elem);
  if (p != backwardMap.end())
    return (*p).second;
  else
    return DOM::Node();
}

#if 0
SmartPtr<MathMLElement>
gmetadom_Linker::findFormattingNode(const DOM::Node& node) const
{
  for (DOM::Node p = node; p; p = p.get_parentNode())
    if (SmartPtr<MathMLElement> fNode = getFormattingNode(p))
      return fNode;
  
  return 0;
}
#endif
