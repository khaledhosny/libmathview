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

#include "gmetadomIterators.hh"

namespace GdomeSmartDOMExt
{

  DOM::Node
  ChildNodesIterator::node()
  {
    if (DOM::Node res = p)
      {
	p = p.get_nextSibling();
	return res;
      }
    else
      return DOM::Node();
  }

  ChildElementsIterator::ChildElementsIterator(const DOM::Node& root,
					       const DOM::GdomeString& ns0,
					       const DOM::GdomeString& tag0)
    : p(root.get_firstChild()), ns(ns0), tag(tag0)
  {
    next();
  }

  void
  ChildElementsIterator::next()
  {
    while (p && p.get_nodeType() != DOM::Node::ELEMENT_NODE &&
	   (ns == "*" || ns != p.get_namespaceURI()) &&
	   (tag == "*" || tag != p.get_localName()))
      p = p.get_nextSibling();
  }

  DOM::Element
  ChildElementsIterator::element()
  {
    if (DOM::Element res = p)
      {
	p = p.get_nextSibling();
	next();
	return res;
      }
    else
      return DOM::Element();
  }

}
