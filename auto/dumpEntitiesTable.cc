// Copyright (C) 2000-2001, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include "config.h"
#include <stdlib.h> // for exit(...)
#include <stdio.h>

#if defined(HAVE_MINIDOM)

#include <minidom.h>

bool
dump(const char* fileName)
{
  mDOMDocRef doc = mdom_load(fileName, FALSE, NULL);
  if (doc == NULL) return false;
  
  mDOMNodeRef root = mdom_doc_get_root_node(doc);
  if (root == NULL) {
    mdom_unload(doc);
    return false;
  }

  mDOMNodeRef p = mdom_node_get_first_child(root);
  while (p != NULL) {
    if (!mdom_node_is_blank(p)) {
      mDOMStringRef name = mdom_node_get_attribute(p, DOM_CONST_STRING("name"));
      mDOMStringRef value = mdom_node_get_attribute(p, DOM_CONST_STRING("value"));

      if (name != NULL && value != NULL) {
	printf("{ \"%s\", \"", C_CONST_STRING(name));
	
	// WARNING: the line below is libxml dependent!!!
	for (xmlChar* s = value; s != NULL && *s != '\0'; s++) {
	  printf("\\x%02x", *s);
	}
	
	printf("\\x00\", 0, 0 },\n");
      }

      mdom_string_free(name);
      mdom_string_free(value);
    }

    p = mdom_node_get_next_sibling(p);
  }

  mdom_unload(doc);

  return true;
}

#elif defined(HAVE_GMETADOM)

#include "gmetadom.hh"

bool
dump(const char* fileName)
{
  DOM::DOMImplementation di;

  try {
    DOM::Document doc = di.createDocumentFromURI(fileName, 0);
    if (!doc) return false;
  
    DOM::Element root = doc.get_documentElement();
    if (!root) return false;

    for (DOM::Node p = root.get_firstChild(); p; p = p.get_nextSibling()) {
      if (p.get_nodeType() == DOM::Node::ELEMENT_NODE) {
	DOM::Element elem(p);

	std::string name = elem.getAttribute("name");
	DOM::GdomeString value = elem.getAttribute("value");

	if (!name.empty() && !value.empty()) {
	  DOM::UTF8String s_value = value;

	  printf("{ \"%s\", \"", name.c_str());
	  for (unsigned i = 0; i < s_value.length(); i++) printf("\\x%02x", s_value[i] & 0xff);
	  printf("\\x00\", 0, 0 },\n");
	}
      }
    }

    return true;
  } catch (DOM::DOMException) {
    return false;
  }
}

#endif

int
main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: dumpEntitiesTable <file-name>\n");
    exit(1);
  }

  dump(argv[1]);

  return 0;
}
