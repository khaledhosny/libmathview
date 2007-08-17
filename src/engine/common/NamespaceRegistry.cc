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

#include "NamespaceRegistry.hh"
#include "NamespaceContext.hh"

NamespaceRegistry::NamespaceRegistry()
{ }

NamespaceRegistry::~NamespaceRegistry()
{ }

bool
NamespaceRegistry::add(const SmartPtr<NamespaceContext>& context)
{
  if (map.find(context->getNamespaceURI()) != map.end())
    return false;

  map[context->getNamespaceURI()] = context;
  return true;
}

bool
NamespaceRegistry::remove(const String& namespaceURI)
{
  if (map.find(namespaceURI) == map.end())
    return false;

  map.erase(namespaceURI);
  return true;
}

SmartPtr<NamespaceContext>
NamespaceRegistry::get(const String& namespaceURI) const
{
  ElementFactoryMap::const_iterator p = map.find(namespaceURI);
  return (p != map.end()) ? (*p).second : 0;
}
