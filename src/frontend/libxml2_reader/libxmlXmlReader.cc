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

#include <cassert>

#include "libxmlXmlReader.hh"

libxmlXmlReader::libxmlXmlReader(xmlTextReaderPtr r)
  : reader(r), fresh(true), error(1), depth(xmlTextReaderDepth(r))
{
  assert(reader);
}

libxmlXmlReader::~libxmlXmlReader()
{
  xmlFreeTextReader(reader);
}

bool
libxmlXmlReader::more() const
{
  return valid() && depth == xmlTextReaderDepth(reader);
}

int
libxmlXmlReader::getNodeType() const
{
  assert(valid());
  return xmlTextReaderNodeType(reader);
}

String
libxmlXmlReader::getNodeName() const
{
  assert(valid());
  if (const xmlChar* localName = xmlTextReaderConstLocalName(reader))
    return fromReaderString(localName);
  else
    return fromReaderString(xmlTextReaderConstName(reader));
}

String
libxmlXmlReader::getNodeValue() const
{
  assert(valid());
  return fromReaderString(xmlTextReaderConstValue(reader));
}

String
libxmlXmlReader::getNodeNamespaceURI() const
{
  assert(valid());
  return fromReaderString(xmlTextReaderConstNamespaceUri(reader));
}

int
libxmlXmlReader::getAttributeCount() const
{
  assert(valid());
  return xmlTextReaderAttributeCount(reader);
}

void
libxmlXmlReader::getAttribute(int index, String& namespaceURI, String& name, String& value) const
{
  assert(valid());
  int res = xmlTextReaderMoveToAttributeNo(reader, index);
  assert(res == 1);
  if (const xmlChar* ns = xmlTextReaderConstNamespaceUri(reader))
    namespaceURI = fromReaderString(ns);
  else
    namespaceURI.clear();
  name = fromReaderString(xmlTextReaderConstName(reader));
  value = fromReaderString(xmlTextReaderConstValue(reader));
  xmlTextReaderMoveToElement(reader);
}

String
libxmlXmlReader::getAttribute(const String& name) const
{
  assert(valid());
  if (xmlTextReaderMoveToAttribute(reader, toReaderString(name.c_str())))
    {
      String res = fromReaderString(xmlTextReaderConstValue(reader));
      xmlTextReaderMoveToElement(reader);
      return res;
    }
  else
    return String();
}

bool
libxmlXmlReader::hasAttribute(const String& name) const
{
  assert(valid());
  if (xmlTextReaderMoveToAttribute(reader, toReaderString(name.c_str())))
    {
      xmlTextReaderMoveToElement(reader);
      return true;
    }
  else
    return false;
}

void
libxmlXmlReader::reset()
{
  // currently there is no way to reset a libxml text reader
  // so this method just seeks the element node (assuming
  // that it's called only once)
  while (valid() && xmlTextReaderNodeType(reader) != XML_ELEMENT_NODE)
    error = xmlTextReaderRead(reader);
}

void
libxmlXmlReader::moveToFirstChild()
{
  assert(valid());
  assert(xmlTextReaderNodeType(reader) == XML_ELEMENT_NODE);
  if (!xmlTextReaderIsEmptyElement(reader))
    error = xmlTextReaderRead(reader);
  depth++;
}

void
libxmlXmlReader::moveToNextSibling()
{
  assert(depth == xmlTextReaderDepth(reader));
  do
    error = xmlTextReaderRead(reader);
  while (error == 1 && xmlTextReaderDepth(reader) > depth);
  fresh = true;
}

void
libxmlXmlReader::moveToParentNode()
{
  depth--;
  while (xmlTextReaderDepth(reader) > depth && error == 1)
    xmlTextReaderRead(reader);
  fresh = false;
}

