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

#include "Clock.hh"
#include "AbstractLogger.hh"
#include "libxml2_Model.hh"

#include <iostream>

xmlDoc*
libxml2_Model::document(const AbstractLogger& logger, const String& path, bool)
{
  Clock perf;
  perf.Start();
  xmlDoc* doc = xmlParseFile(path.c_str());
  perf.Stop();
  logger.out(LOG_INFO, "parsing time: %dms", perf());

  return doc;
}

xmlDoc*
libxml2_Model::documentFromBuffer(const AbstractLogger& logger, const String& buffer, bool)
{
  Clock perf;
  perf.Start();
  xmlDoc* doc = xmlReadDoc(toModelString(buffer.c_str()), NULL, NULL, 0);
  perf.Stop();
  logger.out(LOG_INFO, "parsing time: %dms", perf());

  return doc;  
}

xmlElement*
libxml2_Model::getDocumentElement(const Document& doc)
{
  assert(doc);
  return reinterpret_cast<xmlElement*>(xmlDocGetRootElement(doc));
}

String
libxml2_Model::getAttribute(const Element& el, const String& name)
{
  assert(el);
  if (xmlChar* res = xmlGetProp((xmlNode*) el, toModelString(name)))
    {
      String _res(fromModelString(res));
      xmlFree(res);
      return _res;
    }
  else
    return String();
}

String
libxml2_Model::getNodeName(const Node& n)
{
  assert(n);
  assert(n->name);
  return fromModelString(n->name);
}

String
libxml2_Model::getNodeValue(const Node& n)
{
  assert(n);
  if (xmlChar* res = xmlNodeGetContent(n))
    {
      String _res(fromModelString(res));
      xmlFree(res);
      return _res;
    }
  else
    return String();
}

String
libxml2_Model::getElementValue(const Element& el)
{ return getNodeValue(asNode(el)); }

String
libxml2_Model::getNodeNamespaceURI(const Node& n)
{
  assert(n);
  if (n->ns)
    {
      assert(n->ns->href);
      return fromModelString(n->ns->href);
    }
  else
    return String();
}

bool
libxml2_Model::hasAttribute(const Element& el, const String& name)
{
  assert(el);
  return xmlHasProp((xmlNode*) el, toModelString(name));
}
