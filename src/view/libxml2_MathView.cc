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

#include "AbstractLogger.hh"
#include "libxml2_MathView.hh"
#include "libxml2_Model.hh"
#include "libxml2_Builder.hh"

libxml2_MathView::libxml2_MathView()
  : currentDoc(0), docOwner(false)
{
  setBuilder(libxml2_Builder::create());
}

libxml2_MathView::~libxml2_MathView()
{
  if (docOwner && currentDoc) xmlFreeDoc(currentDoc);
  currentDoc = 0;
  docOwner = false;
}

void
libxml2_MathView::unload()
{
  resetRootElement();
  if (docOwner && currentDoc) xmlFreeDoc(currentDoc);
  currentDoc = 0;
  docOwner = false;
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
