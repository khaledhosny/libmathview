// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2014, Khaled Hosny <khaledhosny@eglug.org>.
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

#include "Clock.hh"
#include "AbstractLogger.hh"
#include "libxml2_EntitiesTable.hh"
#include "libxml2_Model.hh"

#include <iostream>
#include <libxml/parserInternals.h>
#include <string.h>

static xmlEntity*
entity_resolver(void* ctxt, const xmlChar *name) {
  xmlEntity *entity = NULL;

  const EntitiesTableEntry* entities = getMathMLEntities();

  for (size_t i = 0; entities[i].name != NULL; i++) {
    if (strcmp((const char*) name, entities[i].name) == 0) {
      entity = xmlNewEntity(NULL, name, XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, libxml2_Model::toModelString(entities[i].value));
      break;
    }
  }

  if (entity == NULL)
    entity = xmlGetPredefinedEntity(name);

  return entity;
}

xmlDoc*
libxml2_Model::document(const AbstractLogger& logger, const String& path, bool subst)
{
  Clock perf;
  perf.Start();

  xmlDoc* doc = NULL;

  if (!subst) {
    doc = xmlParseFile(path.c_str());
  } else {
    xmlSubstituteEntitiesDefault(1);
    xmlParserCtxt *ctxt = xmlCreateFileParserCtxt(path.c_str());
    if (ctxt != NULL) {
      ctxt->sax->getEntity = entity_resolver;
      xmlParseDocument(ctxt);
      doc = ctxt->myDoc;
      xmlFreeParserCtxt(ctxt);
    }
  }

  perf.Stop();
  logger.out(LOG_INFO, "parsing time: %dms", perf());

  return doc;
}

xmlDoc*
libxml2_Model::documentFromBuffer(const AbstractLogger& logger, const String& buffer, bool subst)
{
  Clock perf;
  perf.Start();

  xmlDoc* doc = NULL;

  if (!subst) {
    doc = xmlReadDoc(toModelString(buffer.c_str()), NULL, NULL, 0);
  } else {
    xmlSubstituteEntitiesDefault(1);
    xmlParserCtxt *ctxt = xmlCreateMemoryParserCtxt(buffer.c_str(), strlen(buffer.c_str()));
    if (ctxt != NULL) {
      ctxt->sax->getEntity = entity_resolver;
      xmlParseDocument(ctxt);
      doc = ctxt->myDoc;
      xmlFreeParserCtxt(ctxt);
    }
  }

  perf.Stop();
  logger.out(LOG_INFO, "parsing time: %dms", perf());

  return doc;  
}

void
libxml2_Model::freeDocument(xmlDoc* doc)
{
  xmlFreeDoc(doc);
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
