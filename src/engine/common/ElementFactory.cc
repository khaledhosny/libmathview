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

#include "Element.hh"
#include "ElementFactory.hh"
#include "gmetadom.hh"
#include "Linker.hh"

SmartPtr<Element>
ElementFactory::getElement(const DOM::Element& elem) const
{
  assert(elem);

  if (SmartPtr<Element> el = linker->get(elem))
    return el;

  SmartPtr<Element> res = createElement(nodeLocalName(elem));
  assert(res);

  res->setDOMElement(elem); // formatting node -> DOM node
  res->link(linker, elem); // DOM node -> formatting node

  return res;
}

SmartPtr<Linker>
ElementFactory::getLinker() const
{
  return linker;
}
