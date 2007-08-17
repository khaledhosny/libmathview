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

#ifndef __gmetadom_Model_hh__
#define __gmetadom_Model_hh__

#include <cassert>

#include <GdomeSmartDOM.hh>

#include "String.hh"
#include "TemplateNodeIterator.hh"
#include "TemplateElementIterator.hh"

namespace DOM = GdomeSmartDOM;

struct GMV_FrontEnd_EXPORT gmetadom_Model
{
  enum {
    TEXT_NODE = DOM::Node::TEXT_NODE,
    ELEMENT_NODE = DOM::Node::ELEMENT_NODE
  };

  typedef DOM::Node Node;
  typedef DOM::Element Element;
  typedef DOM::Document Document;
  typedef TemplateNodeIterator<gmetadom_Model> NodeIterator;
  typedef TemplateElementIterator<gmetadom_Model> ElementIterator;

  // method for parsing a model
  // MUST be available
  static DOM::Document document(const class AbstractLogger&, const String&, bool = false);
  static DOM::Document documentFromBuffer(const class AbstractLogger&, const String&, bool = false);

  // conversion methods
  static DOM::Element asNode(const DOM::Element& el) { return el; }
  static DOM::Node asElement(const DOM::Node& n) { return n; }

  // method for retrieving the root element of a document
  static DOM::Element getDocumentElement(const DOM::Document& doc) { return doc.get_documentElement(); }

  // methods for querying nodes and elements
  // MUST be available for TemplateBuilder and TemplateSetup to work
  static String getAttribute(const DOM::Element& el, const String& name) { return el.getAttribute(name); }
  static unsigned getNodeType(const DOM::Node& n) { return n.get_nodeType(); }
  static String getNodeName(const DOM::Node&);
  static String getNodeValue(const DOM::Node& n) { return n.get_nodeValue(); }
  static String getElementValue(const DOM::Element&);
  static String getNodeNamespaceURI(const DOM::Node& n)
  { if (DOM::GdomeString ns = n.get_namespaceURI()) return ns; else return String(); }
  // MUST be implemented if the default RefinementContext is used
  static bool hasAttribute(const DOM::Element& el, const String& name) { return el.hasAttribute(name); }

  // methods for navigating the model
  // must be available if the default iterators are used
  static DOM::Node getNextSibling(const DOM::Node& n) { return n.get_nextSibling(); }
  static DOM::Node getFirstChild(const DOM::Node& n) { return n.get_firstChild(); }

  // MUST be available if the default linker is used
  struct Hash
  {
    size_t operator()(const DOM::Element& el) const
    {
      assert(el);
      return reinterpret_cast<size_t>(el.id());
    }
  };

};

#endif // __gmetadom_Model_hh__
