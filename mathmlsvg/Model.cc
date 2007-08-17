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

