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

#include "NamespaceRegistry.hh"
#include "namespaceContext.hh"

NamespaceRegistry::NamespaceRegistry()
{ }

NamespaceRegistry::~NamespaceRegistry()
{ }

bool
NamespaceRegistry::add(const SmartPtr<NamespaceContext>& context)
{
  if (map.find(context->getURI()) != map.end())
    return false;

  map[context->getURI()] = context;
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
