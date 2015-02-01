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

#include <config.h>

#include "View.hh"
#include "AbstractLogger.hh"
#include "Builder.hh"
#include "Element.hh"
#include "ElementFactory.hh"
#include "AttributeSignature.hh"
#include "AttributeSet.hh"
#include "NamespaceContext.hh"

Element::Element(const SmartPtr<NamespaceContext>& c) : context(c)
{
  assert(context);
  setDirtyStructure();
  setDirtyAttribute();
  setDirtyLayout();
}

Element::~Element()
{
  context->getView()->getBuilder()->forgetElement(this);
}

void
Element::setParent(const SmartPtr<Element>& p)
{
  parent = static_cast<Element*>(p);
  if (p)
    {
      // WARNING: the setFlagUp is smart so it does not propagate the flag
      // if the flag is already set.
      // the setFlagDown however doesn't check for the status of the flag
      // so it is more delicate
      if (dirtyStructure()) setFlagUp(FDirtyStructure);
      if (dirtyAttribute()) setFlagUp(FDirtyAttributeP);
      // WARNING: since the builder creates the formatting tree in a top-down fashion
      // and it resets dirty flags bottom-up, having a setFlagDown in the setParent
      // would cause sometimes the creation of inconsistent formatting tree
      // having subtrees with the DirtyAttributeD flag set, but no DirtyAttributeP
      // set above the subtree.
      // not propagating the DirtyAttributeD flag however may result into
      // wrong formatting in some cases. It is not clear what should be done
      // to fix this.
      //if (p->dirtyAttributeD() && !dirtyAttributeD()) setFlagDown(FDirtyAttributeD);
      if (dirtyLayout()) setFlagUp(FDirtyLayout);
      // if (p->dirtyLayout()) setFlagDown(FDirtyLayout); // NOOOOOOOOOOOOOOOOOOOO
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
  return attributes ? attributes->get(ATTRIBUTE_ID_OF_SIGNATURE(signature)) : nullptr;
}

void
Element::setAttribute(const SmartPtr<Attribute>& attr)
{
  assert(attr);
  if (!attributes) attributes = AttributeSet::create();
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
    return nullptr;
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
  //std::cerr << "Element::setDirtyAttribute " << this << " " << dirtyAttribute() << std::endl;
  if (!dirtyAttribute())
    {
      setFlag(FDirtyAttribute);
      setFlagUp(FDirtyAttributeP);
    }
}

void
Element::setDirtyAttributeD()
{
  //std::cerr << "Element::setDirtyAttributeD " << this << " " << dirtyAttribute() << std::endl;
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
      setFlag(FDirtyLayout);
      setFlagUp(FDirtyLayout);
    }
}

void
Element::setDirtyLayoutD()
{
  if (!dirtyLayout())
    {
      setFlagDown(FDirtyLayout);
      setDirtyLayout();
    }
}

void
Element::setFlag(Flags f)
{
  //if (f == FDirtyAttributeP) std::cerr << "Element::setFlag (FDirtyAttributeP) " << this << std::endl;
  //if (f == FDirtyAttribute) std::cerr << "Element::setFlag (FDirtyAttribute) " << this << std::endl;
  //if (f == FDirtyAttributeD) std::cerr << "Element::setFlag (FDirtyAttributeD) " << this << std::endl;
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

SmartPtr<NamespaceContext>
Element::getNamespaceContext() const
{ return static_cast<NamespaceContext*>(context); }

SmartPtr<AbstractLogger>
Element::getLogger() const
{ return getNamespaceContext()->getView()->getLogger(); }

AreaRef
Element::format(class FormattingContext&)
{
  if (dirtyLayout())
    {
      setArea(nullptr);
      resetDirtyLayout();
    }

  return getArea();
}
