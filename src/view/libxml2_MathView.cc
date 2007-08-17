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

#include "AbstractLogger.hh"
#include "libxml2_MathView.hh"
#include "libxml2_Model.hh"
#include "libxml2_Setup.hh"
#include "libxml2_Builder.hh"

libxml2_MathView::libxml2_MathView(const SmartPtr<AbstractLogger>& logger)
  : View(logger), currentDoc(0), docOwner(false)
{
  setBuilder(libxml2_Builder::create());
}

libxml2_MathView::~libxml2_MathView()
{
  if (docOwner && currentDoc) xmlFreeDoc(currentDoc);
  currentDoc = 0;
  docOwner = false;
}

SmartPtr<libxml2_MathView>
libxml2_MathView::create(const SmartPtr<AbstractLogger>& logger)
{ return new libxml2_MathView(logger); }

void
libxml2_MathView::unload()
{
  resetRootElement();
  if (docOwner && currentDoc) xmlFreeDoc(currentDoc);
  currentDoc = 0;
  docOwner = false;
  if (SmartPtr<libxml2_Builder> builder = smart_cast<libxml2_Builder>(getBuilder()))
    builder->setRootModelElement(0);
}

bool
libxml2_MathView::loadURI(const char* name)
{
  assert(name);
  if (xmlDoc* doc = libxml2_Model::document(*getLogger(), name, true))
    {
      if (loadDocument(doc))
	{
	  docOwner = true;
	  return true;
	}
      else
	{
	  resetRootElement();
	  xmlFreeDoc(doc);
	  return false;
	}
    }

  unload();
  return false;
}

bool
libxml2_MathView::loadBuffer(const char* buffer)
{
  assert(buffer);
  if (xmlDoc* doc = libxml2_Model::documentFromBuffer(*getLogger(), buffer, true))
    {
      if (loadDocument(doc))
	{
	  docOwner = true;
	  return true;
	}
      else
	{
	  resetRootElement();
	  xmlFreeDoc(doc);
	  return false;
	}
    }
  
  unload();
  return false;
}

bool
libxml2_MathView::loadDocument(xmlDoc* doc)
{
  assert(doc);

  if (xmlNode* root = xmlDocGetRootElement(doc))
    {
      const bool res = loadRootElement((xmlElement*) root);
      if (res) currentDoc = doc;
      return res;
    }

  unload();
  return false;
}

bool
libxml2_MathView::loadRootElement(xmlElement* elem)
{
  assert(elem);

  if (SmartPtr<libxml2_Builder> builder = smart_cast<libxml2_Builder>(getBuilder()))
    {
      resetRootElement();
      builder->setRootModelElement(elem);
      return true;
    }

  unload();
  return false;
}

SmartPtr<Element>
libxml2_MathView::elementOfModelElement(xmlElement* el) const
{
  if (SmartPtr<libxml2_Builder> builder = smart_cast<libxml2_Builder>(getBuilder()))
    return builder->findElement(el);
  else
    return 0;  
}

xmlElement*
libxml2_MathView::modelElementOfElement(const SmartPtr<Element>& elem) const
{
  if (SmartPtr<libxml2_Builder> builder = smart_cast<libxml2_Builder>(getBuilder()))
    return (xmlElement*) builder->findSelfOrAncestorModelElement(elem);
  else
    return 0;
}

bool
libxml2_MathView::notifyStructureChanged(xmlElement* el) const
{
  if (SmartPtr<libxml2_Builder> builder = smart_cast<libxml2_Builder>(getBuilder()))
    return builder->notifyStructureChanged(el);
  else
    return false;
}

bool
libxml2_MathView::notifyAttributeChanged(xmlElement* el, const xmlChar* name) const
{
  if (SmartPtr<libxml2_Builder> builder = smart_cast<libxml2_Builder>(getBuilder()))
    return builder->notifyAttributeChanged(el, name);
  else
    return false;
}

bool
libxml2_MathView::loadConfiguration(const SmartPtr<AbstractLogger>& logger, 
				    const SmartPtr<Configuration>& configuration, const String& path)
{ return libxml2_Setup::loadConfiguration(*logger, *configuration, path); }

bool
libxml2_MathView::loadOperatorDictionary(const SmartPtr<AbstractLogger>& logger,
					 const SmartPtr<MathMLOperatorDictionary>& dictionary, const String& path)
{ return libxml2_Setup::loadOperatorDictionary(*logger, *dictionary, path); }

