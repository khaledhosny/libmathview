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
#include "AttributeSignature.hh"
#include "AttributeList.hh"
#include "Linker.hh"

Element::Element()
{
  setDirtyStructure();
  setDirtyAttribute();
  setDirtyLayout();
}

Element::~Element()
{
  this->unlink();
}

void
Element::setParent(const SmartPtr<Element>& p)
{
  parent = static_cast<Element*>(p);
  if (p)
    {
      if (dirtyStructure()) setFlagUp(FDirtyStructure);
      if (dirtyAttribute()) setFlagUp(FDirtyAttributeP);
      if (p->dirtyAttributeD()) setFlagDown(FDirtyAttributeD);
      if (dirtyLayout()) setFlagUp(FDirtyLayout);
      if (p->dirtyLayout()) setFlagDown(FDirtyLayout);
    }
}

unsigned
Element::getDepth() const
{
  unsigned depth = 0;

  for (SmartPtr<const Element> p = this; p; p = p->getParent())
    depth++;

  return depth;
}

SmartPtr<Attribute>
Element::getAttribute(const AttributeSignature& signature) const
{
  return attributes ? attributes->get(ATTRIBUTE_ID_OF_SIGNATURE(signature)) : 0;
}

void
Element::setAttribute(const SmartPtr<Attribute>& attr)
{
  assert(attr);
  if (!attributes) attributes = AttributeList::create();
  if (attributes->set(attr)) setDirtyLayout();
}

void
Element::removeAttribute(const AttributeSignature& signature)
{
  if (attributes && attributes->remove(ATTRIBUTE_ID_OF_SIGNATURE(signature)))
    setDirtyLayout();
}

SmartPtr<Value>
Element::getAttributeValueNoDefault(const AttributeSignature& signature) const
{
  if (SmartPtr<Attribute> attr = getAttribute(signature))
    return attr->getValue();
  else
    return 0;
}

SmartPtr<Value>
Element::getAttributeValue(const AttributeSignature& signature) const
{
  if (SmartPtr<Value> value = getAttributeValueNoDefault(signature))
    return value;
  else
    return signature.getDefaultValue();
}

void
Element::setDirtyStructure()
{
  if (!dirtyStructure())
    {
      setFlag(FDirtyStructure);
      setFlagUp(FDirtyStructure);
    }
}

void
Element::setDirtyAttribute()
{
  if (!dirtyAttribute())
    {
      setFlag(FDirtyAttribute);
      setFlagUp(FDirtyAttributeP);
    }
}

void
Element::setDirtyAttributeD()
{
  if (!dirtyAttributeD())
    {
      setFlagDown(FDirtyAttributeD);
      setFlagUp(FDirtyAttributeP);
    }
}

void
Element::setDirtyLayout()
{
  if (!dirtyLayout())
    {
      setFlagDown(FDirtyLayout);
      setFlagUp(FDirtyLayout);
    }
}

void
Element::setFlag(Flags f)
{
  flags.set(f);
}

void
Element::setFlagUp(Flags f)
{
  for (SmartPtr<Element> p = getParent(); p && !p->getFlag(f); p = p->getParent())
    p->setFlag(f);
}

void
Element::resetFlagUp(Flags f)
{
  for (SmartPtr<Element> p = getParent(); p && p->getFlag(f); p = p->getParent())
    p->resetFlag(f);
}

void
Element::setFlagDown(Flags f)
{
  setFlag(f);
}

void
Element::resetFlagDown(Flags f)
{
  resetFlag(f);
}

void
Element::setDOMElement(const DOM::Element& el)
{
  assert(!elem);
  elem = el;
}

void
Element::link(const SmartPtr<Linker>& l, const DOM::Element& el)
{
  assert(l);
  assert(el);
  assert(!linker);
  linker = l;
  linker->add(el, this);
  setDOMElement(el);
}

void
Element::unlink()
{
  if (elem)
    {
      assert(linker);
      linker->remove(elem);
      elem = DOM::Element(0);
    }
}
