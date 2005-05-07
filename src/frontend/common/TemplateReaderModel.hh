// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#ifndef __TemplateReaderModel_hh__
#define __TemplateReaderModel_hh__

#include "SmartPtr.hh"
#include "String.hh"

#include "TemplateReaderNodeIterator.hh"
#include "TemplateReaderElementIterator.hh"

template <class Reader>
struct TemplateReaderModel
{
  enum {
    TEXT_NODE = Reader::TEXT_NODE,
    ELEMENT_NODE = Reader::ELEMENT_NODE
  };

  typedef SmartPtr<Reader> Node;
  typedef SmartPtr<Reader> Element;
  typedef SmartPtr<Reader> Document;
  typedef TemplateReaderNodeIterator<Reader> NodeIterator;
  typedef TemplateReaderElementIterator<Reader> ElementIterator;

  static SmartPtr<Reader> document(const class AbstractLogger&, const String& path, bool subst = false)
  { return Reader::create(path, subst); }

  static SmartPtr<Reader> getDocumentElement(const SmartPtr<Reader>& reader) { return reader; }

  static void             reset(const SmartPtr<Reader>& reader) { reader->reset(); }
  static SmartPtr<Reader> asNode(const SmartPtr<Reader>& reader) { return reader; }
  static SmartPtr<Reader> asElement(const SmartPtr<Reader>& reader) { return reader; }

  static unsigned getNodeType(const SmartPtr<Reader>& reader) { return reader->getNodeType(); }
  static String getNodeName(const SmartPtr<Reader>& reader) { return reader->getNodeName(); }
  static String getNodeValue(const SmartPtr<Reader>& reader) { return reader->getNodeValue(); }
  static String getNodeNamespaceURI(const SmartPtr<Reader>& reader) { return reader->getNodeNamespaceURI(); }
  
  static String getElementValue(const SmartPtr<Reader>& reader)
  {
    String value;
    for (NodeIterator iter(reader); iter.more(); iter.next())
      if (reader->getNodeType() == TEXT_NODE) value += reader->getNodeValue();
    return value;
  }
  static String getAttribute(const SmartPtr<Reader>& reader, const String& name) { return reader->getAttribute(name); }
  static bool hasAttribute(const SmartPtr<Reader>& reader, const String& name) { return reader->hasAttribute(name); }

  struct Hash
  {
    size_t operator()(const SmartPtr<Reader>& reader) const
    { return reader->getNodeId(); }
  };
};

#endif // __TemplateReaderModel_hh__
