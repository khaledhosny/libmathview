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

#include "gmetadom_Reader.hh"

gmetadom_Reader::gmetadom_Reader() : fresh(true)
{ }

gmetadom_Reader::~gmetadom_Reader()
{ }

void
gmetadom_Reader::setStatus(const DOM::Node& p, const DOM::Node& n, bool f)
{
  parent = p;
  node = n;
  fresh = f;
  aMap = DOM::NamedNodeMap();
}

DOM::NamedNodeMap
gmetadom_Reader::getAttributes() const
{
  if (!aMap) aMap = node.get_attributes();
  return aMap;
}

unsigned
gmetadom_Reader::nodeType() const
{
  assert(valid());
  return node.get_nodeType();
}

String
gmetadom_Reader::name() const
{
  assert(valid());
  String res = node.get_localName();
  return res.empty() ? String(node.get_nodeName()) : res;
}

String
gmetadom_Reader::namespaceURI() const
{
  assert(valid());
  return node.get_namespaceURI();
}

String
gmetadom_Reader::prefix() const
{
  assert(valid());
  return node.get_prefix();
}

String
gmetadom_Reader::value() const
{
  assert(valid());
  return node.get_nodeValue();
}

bool
gmetadom_Reader::hasAttribute(const String& name) const
{
  assert(valid());
  return getAttributes().getNamedItem(name) != 0;
}

bool
gmetadom_Reader::hasAttributeNS(const String& nsURI, const String& name) const
{
  assert(valid());
  return getAttributes().getNamedItemNS(nsURI, name) != 0;
}

unsigned
gmetadom_Reader::attributeCount() const
{
  assert(valid());
  return getAttributes().get_length();
}

String
gmetadom_Reader::getAttribute(const String& name) const
{
  assert(valid());
  return getAttributes().getNamedItem(name).get_nodeValue();
}

String
gmetadom_Reader::getAttributeNS(const String& ns, const String& name) const
{
  assert(valid());
  return getAttributes().getNamedItemNS(ns, name).get_nodeValue();
}

String
gmetadom_Reader::getAttributeNo(unsigned index) const
{
  assert(valid());
  return getAttributes().item(index).get_nodeValue();
}

void
gmetadom_Reader::firstChild()
{
  assert(node);
  assert(fresh);
  setStatus(node, node.get_firstChild(), true);
}

void
gmetadom_Reader::parentNode()
{
  assert(parent);
  setStatus(parent.get_parentNode(), parent, false);
}

void
gmetadom_Reader::nextSibling()
{
  assert(node);
  setStatus(parent, node.get_nextSibling(), true);
}
