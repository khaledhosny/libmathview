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

#include "DOMXmlReader.hh"

DOMXmlReader::DOMXmlReader(DOM::Node& n) : node(n), fresh(true)
{
}

DOMXmlReader::~DOMXmlReader()
{
}

void
DOMXmlReader::setNode(const DOM::Node& p, const DOM::Node& n, bool f)
{
  prev = p;
  node = n;
  fresh = f;
  aMap = 0;
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
  return getAttributes().getNamedItem(name);
}

String
DOMXmlReader::getAttributeNS(const String& ns, const String name) const
{
  assert(valid());
  return getAttributes().getNamedItemNS(name);
}

String
DOMXmlReader::getAttributeNo(unsigned index) const
{
  assert(valid());
  return getAttributes().item(index);
}

unsigned
DOMXmlReader::nodeType() const
{
  assert(valid());
  return node.get_nodeType();
}

void
DOMXmlReader::firstChild()
{
  assert(node);
  setNode(node, node.get_firstChild(), true);
}

void
DOMXmlReader::parentNode()
{
  assert(prev);
  setNode(prev.get_parentNode(), prev, false);
}

void
DOMXmlReader::nextSibling()
{
  assert(node);
  setNode(prev, node.get_nextSibling(), true);
}
