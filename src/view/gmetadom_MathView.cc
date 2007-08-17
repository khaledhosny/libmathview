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
#include "gmetadom_Model.hh"
#include "gmetadom_Builder.hh"
#include "gmetadom_Setup.hh"
// WARNING: the following #includes MUST be the last one (???)
#include "gmetadom_MathView.hh"
#include "Element.hh"

gmetadom_MathView::gmetadom_MathView(const SmartPtr<AbstractLogger>& logger)
  : View(logger)
{
  setBuilder(gmetadom_Builder::create());
}

gmetadom_MathView::~gmetadom_MathView()
{
  unload();
}

SmartPtr<gmetadom_MathView>
gmetadom_MathView::create(const SmartPtr<AbstractLogger>& logger)
{ return new gmetadom_MathView(logger); }

void
gmetadom_MathView::unload()
{
  resetRootElement();
  currentDoc = DOM::Document();
  if (SmartPtr<gmetadom_Builder> builder = smart_cast<gmetadom_Builder>(getBuilder()))
    builder->setRootModelElement(DOM::Element());
}

bool
gmetadom_MathView::loadURI(const String& name)
{
  if (DOM::Document doc = gmetadom_Model::document(*getLogger(), name, true))
    if (loadDocument(doc))
      return true;

  unload();
  return false;
}

bool
gmetadom_MathView::loadBuffer(const String& buffer)
{
  if (DOM::Document doc = gmetadom_Model::documentFromBuffer(*getLogger(), buffer, true))
    if (loadDocument(doc))
      return true;

  unload();
  return false;
}

bool
gmetadom_MathView::loadDocument(const DOM::Document& doc)
{
  assert(doc);

  if (DOM::Element root = doc.get_documentElement())
    if (loadRootElement(root))
      {
	currentDoc = doc;
	return true;
      }

  unload();
  return false;
}

bool
gmetadom_MathView::loadRootElement(const DOM::Element& elem)
{
  assert(elem);

  if (SmartPtr<gmetadom_Builder> builder = smart_cast<gmetadom_Builder>(getBuilder()))
    {
      resetRootElement();
      builder->setRootModelElement(elem);
      return true;
    }

  unload();
  return false;
}

SmartPtr<Element>
gmetadom_MathView::elementOfModelElement(const DOM::Element& el) const
{
  if (SmartPtr<gmetadom_Builder> builder = smart_cast<gmetadom_Builder>(getBuilder()))
    return builder->findElement(el);
  else
    return 0;  
}

DOM::Element
gmetadom_MathView::modelElementOfElementAux(const SmartPtr<Element>& elem) const
{
  if (SmartPtr<gmetadom_Builder> builder = smart_cast<gmetadom_Builder>(getBuilder()))
    return builder->findSelfOrAncestorModelElement(elem);
  else
    return DOM::Element();
}

bool
gmetadom_MathView::notifyStructureChanged(const DOM::Element& el) const
{
  if (SmartPtr<gmetadom_Builder> builder = smart_cast<gmetadom_Builder>(getBuilder()))
    return builder->notifyStructureChanged(el);
  else
    return false;
}

bool
gmetadom_MathView::notifyAttributeChanged(const DOM::Element& el, const DOM::GdomeString& name) const
{
  if (SmartPtr<gmetadom_Builder> builder = smart_cast<gmetadom_Builder>(getBuilder()))
    return builder->notifyAttributeChanged(el, name);
  else
    return false;
}

SmartPtr<Element>
gmetadom_MathView::elementOfModelElement(GdomeElement* el) const
{ return elementOfModelElement(GdomeSmartDOM::Element(el)); }

bool
gmetadom_MathView::loadConfiguration(const SmartPtr<AbstractLogger>& logger,
				     const SmartPtr<Configuration>& configuration, const String& path)
{ return gmetadom_Setup::loadConfiguration(*logger, *configuration, path); }

bool
gmetadom_MathView::loadOperatorDictionary(const SmartPtr<AbstractLogger>& logger,
					  const SmartPtr<MathMLOperatorDictionary>& dictionary, const String& path)
{ return gmetadom_Setup::loadOperatorDictionary(*logger, *dictionary, path); }

