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

#include <config.h>
#include <assert.h>

#if defined(HAVE_GMETADOM)

#include "gmetadom.hh"

namespace GdomeSmartDOM {

bool
nodeIsBlank(const Node& node)
{
  assert(node);

  switch (node.get_nodeType()) {
  case Node::COMMENT_NODE:
    return true;

  case Node::TEXT_NODE:
    {
      GdomeString content = node.get_nodeValue();
      return content.empty();
    }

  default:
    return false;
  }
}

GdomeString
elementValue(const Element& elem)
{
  GdomeString res = "";

  for (Node p = elem.get_firstChild(); p; p = p.get_nextSibling()) {
    switch (p.get_nodeType()) {
    case Node::CDATA_SECTION_NODE:
    case Node::TEXT_NODE:
      res = res + p.get_nodeValue();
      break;
    default:
      break;
    }
  }

  return res;
}

GdomeString
nodeLocalName(const Node& node)
{
  assert(node);
  if (!node.get_namespaceURI().null()) return node.get_localName();
  else return node.get_nodeName();
}

}

#endif // HAVE_GMETADOM
