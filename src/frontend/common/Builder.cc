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

#include "defs.h"
#include "Builder.hh"
#include "Attribute.hh"
#include "Element.hh"

Builder::Builder()
{
  ns_map[MATHML_NS_URI] = MATHML_NS_ID;
  ns_map[BOXML_NS_URI] = BOXML_NS_ID;
}

Builder::~Builder()
{
}

void
Builder::registerElementBuilder(const String& ns, const String& tag, const SmartPtr<ElementBuilder>& b)
{
  NamespaceMap::const_iterator p = ns_map.find(ns);
  if (p != ns_map.end()) builder[p->second][tag] = b;
}

void
Builder::registerDummyElementBuilder(const String& ns, const SmartPtr<ElementBuilder>& builder)
{
  NamespaceMap::const_iterator p = ns_map.find(ns);
  if (p != ns_map.end()) dummyBuilder[p->second] = builder;
}

void
Builder::refineAttribute(const SmartPtr<Element>& el, const AttributeSignature& signature) const
{
  SmartPtr<Attribute> attr;
  
  if (signature.fromElement)
    attr = getAttribute(signature);

  if (!attr && signature.fromContext)
    attr = getAttributeFromContext(signature);

  if (attr) el->setAttribute(attr);
  else el->removeAttribute(signature);
}

SmartPtr<Element>
Builder::getElement()
{
  if (SmartPtr<Element> elem = getElementNoCreate())
    return elem;
  else
    return getDummyElement();
}

SmartPtr<Element>
Builder::getElementNoCreate()
{
  if (more())
    {
      const SmartPtr<ElementBuilder> builder = getElementBuilder();

      SmartPtr<Element> elem = getCachedElement();
      if (!elem) elem = builder->create();

      if (elem->dirtyAttribute() || elem->dirtyAttributeP())
	{
	  builder->refine(*this, elem);
	  elem->resetDirtyAttribute(); // should not reset dirtyAttributeP
	}

      beginContent();
      if (elem->dirtyStructure() || elem->dirtyAttributeP())
	{
	  builder->update(*this, elem);
	  elem->resetDirtyStructure();
	  elem->resetDirtyAttribute(); // should be resetDirtyAttributeP
	}
      endContent();

      return elem;
    }
  else
    return 0;
}

SmartPtr<Builder::ElementBuilder>
Builder::getDummyElementBuilder(const String& ns) const
{
  NamespaceMap::const_iterator p = ns_map.find(ns);
  return (p != ns_map.end()) ? getDummyElementBuilder(p->second) : 0;
}

SmartPtr<Builder::ElementBuilder>
Builder::getDummyElementBuilder(NamespaceID nsID) const
{
  assert(nsID >= 0 && nsID < LAST_NS_ID);
  return dummyBuilder[nsID];
}

