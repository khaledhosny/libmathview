// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "DOMXmlReader.hh"

DOMXmlReader::DOMXmlReader() : fresh(true)
{ }

DOMXmlReader::~DOMXmlReader()
{ }

void
DOMXmlReader::setStatus(const DOM::Node& p, const DOM::Node& n, bool f)
{
  parent = p;
  node = n;
  fresh = f;
  aMap = DOM::NamedNodeMap();
}

DOM::NamedNodeMap
DOMXmlReader::getAttributes() const
{
  if (!aMap) aMap = node.get_attributes();
  return aMap;
}

bool
DOMXmlReader::valid() const
{
  return node && fresh;
}

String
DOMXmlReader::name() const
{
  assert(valid());
  return node.get_nodeName();
}

String
DOMXmlReader::localName() const
{
  assert(valid());
  return node.get_localName();
}

String
DOMXmlReader::namespaceURI() const
{
  assert(valid());
  return node.get_namespaceURI();
}

String
DOMXmlReader::prefix() const
{
  assert(valid());
  return node.get_prefix();
}

String
DOMXmlReader::value() const
{
  assert(valid());
  return node.get_nodeValue();
}

unsigned
DOMXmlReader::attributeCount() const
{
  assert(valid());
  return getAttributes().get_length();
}

String
DOMXmlReader::getAttribute(const String& name) const
{
  assert(valid());
  return getAttributes().getNamedItem(name).get_nodeValue();
}

String
DOMXmlReader::getAttributeNS(const String& ns, const String& name) const
{
  assert(valid());
  return getAttributes().getNamedItemNS(ns, name).get_nodeValue();
}

String
DOMXmlReader::getAttributeNo(unsigned index) const
{
  assert(valid());
  return getAttributes().item(index).get_nodeValue();
}

unsigned
DOMXmlReader::nodeType() const
{
  assert(valid());
  return node.get_nodeType();
}

bool
DOMXmlReader::more() const
{
  return valid();
}

void
DOMXmlReader::skip()
{
  assert(node);
  assert(fresh);
  setStatus(parent, node, false);
}

void
DOMXmlReader::down()
{
  assert(node);
  assert(fresh);
  setStatus(node, node.get_firstChild(), true);
}

void
DOMXmlReader::up()
{
  assert(parent);
  assert(!node);
  setStatus(parent.get_parentNode(), parent, false);
}

void
DOMXmlReader::next()
{
  assert(node);
  assert(!fresh);
  setStatus(parent, node.get_nextSibling(), true);
}
