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

#include "gmetadom_Builder.hh"
#include "gmetadom_Model.hh"
#include "TemplateBuilder.hh"

gmetadom_Builder::~gmetadom_Builder()
{
  setRootModelElement(DOM::Element());
}

typedef TemplateBuilder<gmetadom_Model> BUILDER;

SmartPtr<gmetadom_Builder>
gmetadom_Builder::create()
{ return BUILDER::create(); }

SmartPtr<Element>
gmetadom_Builder::findSelfOrAncestorElement(const DOM::Node& el) const
{
  for (DOM::Node p = el; p; p = p.get_parentNode())
    if (SmartPtr<Element> elem = linker.assoc(p))
      return elem;
  return 0;
}

DOM::Node
gmetadom_Builder::findSelfOrAncestorModelNode(const SmartPtr<Element>& elem) const
{
  for (SmartPtr<Element> p(elem); p; p = p->getParent())
    if (DOM::Node el = linker.assoc(p))
      return el;
  return DOM::Node(0);
}

void
gmetadom_Builder::DOMSubtreeModifiedListener::handleEvent(const DOM::Event& ev)
{
  DOM::MutationEvent me(ev);
  assert(me);
  //std::cerr << "RECEIVING SUBTREE MODIFIED" << std::endl;
  if (SmartPtr<Element> elem = builder->findSelfOrAncestorElement(me.get_target()))
    {
      //std::cerr << "FOUND LINKED ELEMENT" << std::endl;
      elem->setDirtyStructure();
      elem->setDirtyAttributeD();
    }
}

void
gmetadom_Builder::DOMAttrModifiedListener::handleEvent(const DOM::Event& ev)
{
  DOM::MutationEvent me(ev);
  assert(me);

  if (SmartPtr<Element> elem = builder->findSelfOrAncestorElement(me.get_target()))
    elem->setDirtyAttribute();
}

void
gmetadom_Builder::setRootModelElement(const DOM::Element& el)
{
  if (root == el) return;

  if (root)
    {
      DOM::EventTarget et(root);
      assert(et);

      et.removeEventListener("DOMNodeInserted", *subtreeModifiedListener, false);
      et.removeEventListener("DOMNodeRemoved", *subtreeModifiedListener, false);
      et.removeEventListener("DOMCharacterDataModified", *subtreeModifiedListener, false);
      et.removeEventListener("DOMAttrModified", *attrModifiedListener, false);

      delete subtreeModifiedListener;
      delete attrModifiedListener;
      subtreeModifiedListener = 0;
      attrModifiedListener = 0;
    }

  root = el;

  if (root)
    {
      DOM::EventTarget et(root);
      assert(et);

      subtreeModifiedListener = new DOMSubtreeModifiedListener(this);
      attrModifiedListener = new DOMAttrModifiedListener(this);

      et.addEventListener("DOMNodeInserted", *subtreeModifiedListener, false);
      et.addEventListener("DOMNodeRemoved", *subtreeModifiedListener, false);
      et.addEventListener("DOMCharacterDataModified", *subtreeModifiedListener, false);
      et.addEventListener("DOMAttrModified", *attrModifiedListener, false);
    }
}
