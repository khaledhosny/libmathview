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

#include "gmetadom_Reader.hh"
#include "gmetadom_Builder.hh"
#include "Linker.hh"
#include "Element.hh"

bool
gmetadom_Builder::more() const
{
  return reader->more();
}

void
gmetadom_Builder::beginContent()
{
  reader->firstChild();
}

void
gmetadom_Builder::endContent()
{
  reader->parentNode();
}

void
gmetadom_Builder::pushRefinementContext()
{
  refinementContext.push(reader->getNode());
}

void
gmetadom_Builder::dropRefinementContext()
{
  refinementContext.pop();
}

SmartPtr<Attribute>
gmetadom_Builder::getAttributeFromContext(const AttributeSignature& signature) const
{
  return refinementContext.get(signature);
}

String
gmetadom_Builder::getElementNamespace() const
{
  return reader->namespaceURI();
}

String
gmetadom_Builder::getElementTag() const
{
  return reader->name();
}

SmartPtr<Element>
gmetadom_Builder::getCachedElement() const
{
  return linker->get(reader->getNode());
}

SmartPtr<Element>
gmetadom_Builder::getDummyElement() const
{
  return getDummyElementBuilder(getElementNamespaceID())->create();
}

SmartPtr<Attribute>
gmetadom_Builder::getAttribute(const AttributeSignature& signature) const
{
  return (reader->hasAttribute(signature.name)) ?
    Attribute::create(signature, reader->getAttribute(signature.name)) : 0;
}
