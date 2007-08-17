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

#include "Clock.hh"
#include "AbstractLogger.hh"
#include "gmetadom_Model.hh"
#include "MathMLEntitiesTable.hh"

DOM::Document
gmetadom_Model::document(const AbstractLogger& logger, const String& path, bool subst)
{
  DOM::Document res;

  Clock perf;
  perf.Start();
  if (!subst)
    res = DOM::DOMImplementation().createDocumentFromURI(path.c_str());
  else
    {
      GdomeDOMImplementation* di = gdome_di_mkref();
      assert(di != NULL);
      GdomeException exc = 0;
      GdomeDocument* doc = gdome_di_createDocFromURIWithEntitiesTable(di,
								      path.c_str(),
								      getMathMLEntities(),
								      GDOME_LOAD_PARSING | GDOME_LOAD_SUBSTITUTE_ENTITIES,
								      &exc);
      if (exc != 0)
	{
	  gdome_di_unref(di, &exc);
	  gdome_doc_unref(doc, &exc);
	  return DOM::Document(0);
	}

      if (doc == 0)
	{
	  // FIXME: this should be signalled as an exception, I think
	  gdome_di_unref(di, &exc);
	  return DOM::Document(0);
	}

      res = DOM::Document(doc);
      gdome_di_unref(di, &exc);
      assert(exc == 0);
      gdome_doc_unref(doc, &exc);
      assert(exc == 0);
    }
  perf.Stop();
  logger.out(LOG_INFO, "parsing time: %dms", perf());

  return res;
}

DOM::Document
gmetadom_Model::documentFromBuffer(const AbstractLogger& logger, const String& buffer, bool subst)
{
  DOM::Document res;

  Clock perf;
  perf.Start();
  if (!subst)
    res = DOM::DOMImplementation().createDocumentFromMemory(buffer.c_str(), buffer.length());
  else
    {
      GdomeDOMImplementation* di = gdome_di_mkref();
      assert(di != NULL);
      GdomeException exc = 0;
      // FIXME: the cast should not be necessary if the gdome API were OK
      GdomeDocument* doc = gdome_di_createDocFromMemoryWithEntitiesTable(di,
									 const_cast<String::value_type*>(buffer.c_str()),
									 getMathMLEntities(),
									 GDOME_LOAD_PARSING | GDOME_LOAD_SUBSTITUTE_ENTITIES,
									 &exc);
      if (exc != 0)
	{
	  gdome_di_unref(di, &exc);
	  gdome_doc_unref(doc, &exc);
	  return DOM::Document(0);
	}

      if (doc == 0)
	{
	  // FIXME: this should be signalled as an exception, I think
	  gdome_di_unref(di, &exc);
	  return DOM::Document(0);
	}

      res = DOM::Document(doc);
      gdome_di_unref(di, &exc);
      assert(exc == 0);
      gdome_doc_unref(doc, &exc);
      assert(exc == 0);
    }
  perf.Stop();
  logger.out(LOG_INFO, "parsing time: %dms", perf());

  return res;
}

String
gmetadom_Model::getElementValue(const DOM::Element& elem)
{
  DOM::GdomeString res = "";
  
  for (DOM::Node p = elem.get_firstChild(); p; p = p.get_nextSibling())
    {
      switch (p.get_nodeType()) {
      case DOM::Node::CDATA_SECTION_NODE:
      case DOM::Node::TEXT_NODE:
	res = res + p.get_nodeValue();
	break;
      default:
	break;
      }
    }
  
  return res;
}

String
gmetadom_Model::getNodeName(const DOM::Node& node)
{
  assert(node);
  if (!node.get_namespaceURI().null()) return node.get_localName();
  else return node.get_nodeName();
}

