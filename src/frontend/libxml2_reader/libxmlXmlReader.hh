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
