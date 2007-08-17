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

#include "Attribute.hh"
#include "TemplateBuilder.hh"
#include "custom_reader_Model.hh"
#include "custom_reader_Builder.hh"
#include "TemplateReaderRefinementContext.hh"

typedef TemplateBuilder<custom_reader_Model,
			custom_reader_Builder,
			TemplateReaderRefinementContext<customXmlReader> > BUILDER;

SmartPtr<custom_reader_Builder>
custom_reader_Builder::create()
{ return BUILDER::create(); }

void*
custom_reader_Builder::findSelfOrAncestorModelElement(const SmartPtr<Element>& elem) const
{
  for (SmartPtr<Element> p(elem); p; p = p->getParent())
    if (void* id = linker.assoc(p))
      return id;
  return 0;
}

bool
custom_reader_Builder::notifyStructureChanged(void* id)
{
  if (SmartPtr<Element> elem = linker.assoc(id))
    {
      elem->setDirtyStructure();
      elem->setDirtyAttributeD();
      return true;
    }
  else
    return false;
}

bool
custom_reader_Builder::notifyAttributeChanged(void* id, const char*)
{
  if (SmartPtr<Element> elem = linker.assoc(id))
    {
      elem->setDirtyAttribute();
      return true;
    }
  else
    return false;
}

