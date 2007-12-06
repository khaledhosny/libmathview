// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
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

#ifndef __libxmlXmlReader_hh__
#define __libxmlXmlReader_hh__

#include <libxml/xmlreader.h>

#include "Object.hh"
#include "SmartPtr.hh"
#include "String.hh"

class libxmlXmlReader : public Object
{
protected:
  libxmlXmlReader(xmlTextReaderPtr r);
  virtual ~libxmlXmlReader();

public:
  enum {
    TEXT_NODE = XML_TEXT_NODE,
    ELEMENT_NODE = XML_ELEMENT_NODE
  };

  static SmartPtr<libxmlXmlReader> create(xmlTextReaderPtr r)
  { return new libxmlXmlReader(r); }

  static SmartPtr<libxmlXmlReader> create(const String& path, bool subst = false)
  { 
    if (xmlTextReaderPtr reader = xmlNewTextReaderFilename(path.c_str()))
      {
	int err;
	do err = xmlTextReaderRead(reader); while (err == 1 && xmlTextReaderNodeType(reader) != ELEMENT_NODE);
	return (err == 1) ? new libxmlXmlReader(reader) : 0;
      }
    else
      return 0;
  }

  bool more(void) const;

  int getNodeType(void) const;
  String getNodeName(void) const;
  String getNodeValue(void) const;
  String getNodeNamespaceURI(void) const;
  size_t getNodeId(void) const { return 0; }

  int getAttributeCount(void) const;
  void getAttribute(int, String&, String&, String&) const;
  String getAttribute(const String&) const;
  bool hasAttribute(const String&) const;

  void reset(void);
  void moveToFirstChild(void);
  void moveToNextSibling(void);
  void moveToParentNode(void);

protected:
  bool valid(void) const { return fresh && error == 1; }
  static String fromReaderString(const xmlChar* str) { return reinterpret_cast<const String::value_type*>(str); }
  static const xmlChar* toReaderString(const String& str) { return BAD_CAST(str.c_str()); }

private:
  xmlTextReaderPtr reader;
  bool fresh;
  int error;
  int depth;
};

#endif // __libxmlXmlReader_hh__
