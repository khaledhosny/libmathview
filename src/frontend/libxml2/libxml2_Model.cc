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

#include <config.h>

#include "libxml2_Model.hh"

xmlNode*
libxml2_Model::parseXML(const String& path, bool)
{
  return reinterpret_cast<xmlNode*>(xmlParseFile(path.c_str()));
}

String
libxml2_Model::getAttribute(const Element& el, const String& name)
{
  xmlChar* res = xmlGetProp((xmlNode*) el, toModelString(name));
  String _res(fromModelString(res));
  xmlFree(res);
  return _res;
}

String
libxml2_Model::getNodeName(const Node& n)
{
  return fromModelString(n->name);
}

String
libxml2_Model::getNodeValue(const Node& n)
{
  return fromModelString(n->content);
}

String
libxml2_Model::getElementValue(const Element& el)
{
  xmlChar* res = xmlNodeGetContent((xmlNode*) el);
  String _res(fromModelString(res));
  xmlFree(res);
  return _res;
}

String
libxml2_Model::getNamespaceURI(const Node& n)
{
  if (n->ns)
    return fromModelString(n->ns->href);
  else
    return String();
}

bool
libxml2_Model::hasAttribute(const Element& el, const String& name)
{
  return xmlHasProp((xmlNode*) el, toModelString(name));
}
