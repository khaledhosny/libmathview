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

#include "config.h"
#include <stdlib.h> // for exit(...)
#include <stdio.h>

#if defined(HAVE_GMETADOM)

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
