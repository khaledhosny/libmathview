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

#ifndef __customXmlReader_hh__
#define __customXmlReader_hh__

#include "c_customXmlReader.h"

#include "Object.hh"
#include "SmartPtr.hh"
#include "String.hh"

class customXmlReader : public Object
{
protected:
  customXmlReader(const c_customXmlReader*, c_customModelUserData);
  virtual ~customXmlReader();

public:
  enum {
    TEXT_NODE = C_CUSTOM_TEXT_NODE,
    ELEMENT_NODE = C_CUSTOM_ELEMENT_NODE
  };

  static SmartPtr<customXmlReader> create(const c_customXmlReader* r, c_customModelUserData data)
  { return new customXmlReader(r, data); }

  bool more(void) const { return (*reader->more)(user_data); }

  int getNodeType(void) const { return (*reader->get_node_type)(user_data); }
  String getNodeName(void) const { return fromReaderString((*reader->get_node_name)(user_data)); }
  String getNodeValue(void) const { return fromReaderString((*reader->get_node_value)(user_data)); }
  String getNodeNamespaceURI(void) const { return fromReaderString((*reader->get_node_namespace_uri)(user_data)); }
  void* getNodeId(void) const { return (*reader->get_node_id)(user_data); }

  int getAttributeCount(void) const { return (*reader->get_attribute_count)(user_data); }
  void getAttribute(int index, String&, String&, String&) const;
  String getAttribute(const String& name) const { return fromReaderString((*reader->get_attribute)(user_data, name.c_str())); }
  bool hasAttribute(const String& name) const { return (*reader->has_attribute)(user_data, name.c_str()); }

  void reset(void) { (*reader->reset)(user_data); }
  void moveToFirstChild(void) { (*reader->move_to_first_child)(user_data); }
  void moveToNextSibling(void) { (*reader->move_to_next_sibling)(user_data); }
  void moveToParentNode(void) { (*reader->move_to_parent_node)(user_data); }

protected:
  String fromReaderString(char*) const;

private:
  const c_customXmlReader* reader;
  c_customModelUserData user_data;
};

#endif // __customXmlReader_hh__
