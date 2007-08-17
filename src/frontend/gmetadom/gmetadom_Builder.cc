// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include "gmetadom_Builder.hh"
#include "gmetadom_Model.hh"
#include "TemplateBuilder.hh"
#include "TemplateRefinementContext.hh"

gmetadom_Builder::~gmetadom_Builder()
{
  setRootModelElement(DOM::Element());
}

typedef TemplateBuilder<gmetadom_Model,gmetadom_Builder,TemplateRefinementContext<gmetadom_Model> > BUILDER;

SmartPtr<gmetadom_Builder>
gmetadom_Builder::create()
{ return BUILDER::create(); }

SmartPtr<Element>
gmetadom_Builder::findSelfOrAncestorElement(const DOM::Element& el) const
{
  for (DOM::Element p = el; p; p = p.get_parentNode())
    if (SmartPtr<Element> elem = linker.assoc(p))
      return elem;
  return 0;
}

DOM::Element
gmetadom_Builder::findSelfOrAncestorModelElement(const SmartPtr<Element>& elem) const
{
  for (SmartPtr<Element> p(elem); p; p = p->getParent())
    if (DOM::Element el = linker.assoc(p))
      return el;
  return DOM::Element();
}

bool
gmetadom_Builder::notifyStructureChanged(const DOM::Element& target)
{
  if (SmartPtr<Element> elem = findSelfOrAncestorElement(target))
    {
      elem->setDirtyStructure();
      elem->setDirtyAttributeD();
      return true;
    }
  else
    return false;
}

bool
gmetadom_Builder::notifyAttributeChanged(const DOM::Element& target, const DOM::GdomeString&)
{
  if (SmartPtr<Element> elem = findSelfOrAncestorElement(target))
    {
      elem->setDirtyAttribute();
      return true;
    }
  else
    return false;
}

void
gmetadom_Builder::DOMSubtreeModifiedListener::handleEvent(const DOM::Event& ev)
{
  DOM::MutationEvent me(ev);
  assert(me);
  //std::cerr << "gmetadom_Builder::DOMSubtreeModifiedListener::handleEvent" << std::endl;
  builder->notifyStructureChanged(DOM::Node(me.get_target()));
}

void
gmetadom_Builder::DOMAttrModifiedListener::handleEvent(const DOM::Event& ev)
{
  DOM::MutationEvent me(ev);
  assert(me);
  //std::cerr << "gmetadom_Builder::DOMAttrModifiedListener::handleEvent " << DOM::Node(me.get_target()).get_nodeName() << std::endl;
  builder->notifyAttributeChanged(DOM::Node(me.get_target()), me.get_newValue());
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

void
gmetadom_Builder::linkerAdd(const DOM::Element& el, Element* elem) const
{ linker.add(el, elem); }

void
gmetadom_Builder::linkerRemove(Element* elem) const
{ linker.remove(elem); }
