// Copyright (C) 2000-2006, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <iostream>

#include "Model.hh"

String
Model::getAttributeNS(const Element& el, const String& name, const String& nsUri)
{
  assert(el);
  if (xmlChar* res = xmlGetNsProp(Model::asNode(el), toModelString(name), toModelString(nsUri)))
    {
      String _res(fromModelString(res));
      xmlFree(res);
      return _res;
    }
  else
    return String();
}

String
Model::getAttributeNoNS(const Element& el, const String& name)
{
  assert(el);
  if (xmlChar* res = xmlGetNoNsProp(Model::asNode(el), toModelString(name)))
    {
      String _res(fromModelString(res));
      xmlFree(res);
      return _res;
    }
  else
    return String();
}

void
Model::setAttribute(const Element& el, const String& name, const String& value)
{
  assert(el);
  xmlSetProp((xmlNode*) el, toModelString(name), toModelString(value));
}

bool
Model::setAttributeNS(const Element& el, const String& name, const String& nsUri, const String& value)
{
  assert(el);

  Model::Node n = Model::asNode(el);
  if (xmlNs* ns = xmlSearchNs(n->doc, n, toModelString(nsUri)))
    {
      xmlSetNsProp(n, ns, toModelString(name), toModelString(value));
      return true;
    }
  else
    return false;
}

