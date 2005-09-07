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

#ifndef __gmetadom_Model_hh__
#define __gmetadom_Model_hh__

#include <cassert>

#include <GdomeSmartDOM.hh>

#include "String.hh"
#include "TemplateNodeIterator.hh"
#include "TemplateElementIterator.hh"

namespace DOM = GdomeSmartDOM;

struct GMV_EXPORT gmetadom_Model
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
