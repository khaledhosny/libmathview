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

#ifndef __libxml2_Model_hh__
#define __libxml2_Model_hh__

#include "libxml/tree.h"
#include "String.hh"

#include "TemplateLinker.hh"
#include "TemplateRefinementContext.hh"
#include "TemplateNodeIterator.hh"
#include "TemplateElementIterator.hh"

struct libxml2_Model
{
  enum {
    TEXT_NODE = XML_TEXT_NODE,
    ELEMENT_NODE = XML_ELEMENT_NODE
  };

  typedef xmlNode* Node;
  typedef xmlElement* Element;
  typedef TemplateLinker<libxml2_Model> Linker;
  typedef TemplateNodeIterator<libxml2_Model> NodeIterator;
  typedef TemplateElementIterator<libxml2_Model> ElementIterator;
  typedef TemplateRefinementContext<libxml2_Model> RefinementContext;
  typedef class libxml2_Builder Builder;

  // method for parsing a model
  // MUST be available
  static xmlNode* parseXML(const String&, bool = false);

  // methods for casting an element to a node and viceversa
  static Node asNode(const Element& el) { return (Node) el; }
  static Element asElement(const Node& n) { return (n && n->type == XML_ELEMENT_NODE) ? (xmlElement*) n : 0; }

  // methods for querying nodes
  // MUST be available for TemplateBuilder and TemplateSetup to work
  static unsigned getNodeType(const Node& n) { return n->type; }
  static String getNodeName(const Node&);
  static String getNodeValue(const Node&);
  static String getNamespaceURI(const Node&);

  // methods for querying elements
  // MUST be available for TemplateBuilder and TemplateSetup to work
  static String getElementValue(const Element&);
  static String getAttribute(const Element&, const String&);
  // MUST be implemented if the default RefinementContext is used
  static bool hasAttribute(const Element&, const String&);

  // methods for navigating the model
  // must be available if the default iterators are used
  static Node getNextSibling(const Node& n)
  { return n->next; }
  static Node getFirstChild(const Node& n)
  { return n->children; }

  // auxiliary conversion functions from/to libxml2 strings
  static String fromModelString(const xmlChar* str) { return reinterpret_cast<const String::value_type*>(str); }
  static const xmlChar* toModelString(const String& str) { return BAD_CAST(str.c_str()); }

  // MUST be available if the default linker is used
  struct Hash
  {
    size_t operator()(xmlElement* el) const
    {
      assert(el);
      return reinterpret_cast<size_t>(el);
    }
  };


};

#endif // __libxml2_Model_hh__
