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

#include <cassert>

#include "libxml2_Builder.hh"
#include "libxml2_Model.hh"
#include "TemplateBuilder.hh"
#include "TemplateRefinementContext.hh"

libxml2_Builder::~libxml2_Builder()
{
  setRootModelElement(0);
}

SmartPtr<libxml2_Builder>
libxml2_Builder::create()
{ return TemplateBuilder<libxml2_Model,libxml2_Builder,TemplateRefinementContext<libxml2_Model> >::create(); }

SmartPtr<Element>
libxml2_Builder::findSelfOrAncestorElement(xmlElement* el) const
{
  for (xmlNode* p = libxml2_Model::asNode(el); p; p = p->parent)
    if (SmartPtr<Element> elem = linker.assoc(libxml2_Model::asElement(p)))
      return elem;
  return 0;
}

xmlElement*
libxml2_Builder::findSelfOrAncestorModelElement(const SmartPtr<Element>& elem) const
{
  for (SmartPtr<Element> p(elem); p; p = p->getParent())
    if (xmlElement* el = linker.assoc(p))
      return el;
  return 0;
}

void
libxml2_Builder::setRootModelElement(xmlElement* el)
{
  root = el;
}

bool
libxml2_Builder::notifyStructureChanged(xmlElement* target)
{
  if (SmartPtr<Element> elem = findSelfOrAncestorElement(target))
    {
      elem->setDirtyStructure();
      elem->setDirtyAttributeD();
      return true;
    }
  else
    return false;
}

bool
libxml2_Builder::notifyAttributeChanged(xmlElement* target, const xmlChar*)
{
  if (SmartPtr<Element> elem = findSelfOrAncestorElement(target))
    {
      elem->setDirtyAttribute();
      return true;
    }
  else
    return false;
}
