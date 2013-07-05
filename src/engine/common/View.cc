// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include "defs.h"
#include "config.dirs"

#include "Clock.hh"
#include "View.hh"
#include "Element.hh"
#include "Builder.hh"
#include "MathMLNamespaceContext.hh"
#include "MathMLOperatorDictionary.hh"
#include "AreaId.hh"
#include "AbstractLogger.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

View::View(const SmartPtr<AbstractLogger>& l)
  : logger(l), defaultFontSize(DEFAULT_FONT_SIZE), freezeCounter(0)
{ }

View::~View()
{
  // When the view is destroyed the formatting tree must have
  // been destroyed already, because elements need a healthy
  // view for de-registering themselves from the linker
  assert(!rootElement);
}

bool
View::freeze()
{
  return freezeCounter++ == 0;
}

bool
View::thaw()
{
  assert(freezeCounter > 0);
  return --freezeCounter == 0;
}

String
View::getDefaultConfigurationPath()
{
return PKGSYSCONFDIR"/gtkmathview.conf.xml"; 
}

String
View::getDefaultOperatorDictionaryPath()
{ 
return PKGDATADIR"/dictionary.xml"; 
}

SmartPtr<AbstractLogger>
View::getLogger() const
{ return logger; }

void
View::setOperatorDictionary(const SmartPtr<MathMLOperatorDictionary>& d)
{ dictionary = d; }

SmartPtr<MathMLOperatorDictionary>
View::getOperatorDictionary() const
{ return dictionary; }

void
View::setBuilder(const SmartPtr<Builder>& b)
{
  resetRootElement();
  builder = b;
  if (builder)
    {
      builder->setMathMLNamespaceContext(mathmlContext);
      builder->setLogger(logger);
    }
}

SmartPtr<Builder>
View::getBuilder() const
{ return builder; }

AreaRef
View::formatElement(const SmartPtr<Element>& elem) const
{
  if (!elem) return 0;

  if (elem->dirtyLayout())
    {
      const SmartPtr<MathGraphicDevice> mgd = mathmlContext ? mathmlContext->getGraphicDevice() : 0;
      assert(mgd != 0);
      FormattingContext ctxt(mgd);
      Length defaultSize(getDefaultFontSize(), Length::PT_UNIT);
      scaled l = mgd->evaluate(ctxt, defaultSize, scaled::zero());
      ctxt.setSize(l);
      ctxt.setActualSize(ctxt.getSize());
      ctxt.setAvailableWidth(getAvailableWidth());
      Clock perf;
      perf.Start();
      elem->format(ctxt);
      perf.Stop();
      getLogger()->out(LOG_INFO, "formatting time: %dms", perf());
    }

  return elem->getArea();
}

SmartPtr<Element>
View::getRootElement() const
{
  bool rootDirty = !rootElement ||
    rootElement->dirtyStructure() || rootElement->dirtyAttribute() ||
    rootElement->dirtyAttributeP();

  if (rootDirty)
    {
      Clock perf;
	
      perf.Start();
      rootElement = builder->getRootElement();
      perf.Stop();

      getLogger()->out(LOG_INFO, "build time: %dms", perf());
    }
  
  return rootElement;
}

void
View::resetRootElement()
{
  rootElement = 0;
}

AreaRef
View::getRootArea() const
{ return formatElement(getRootElement()); }

BoundingBox
View::getBoundingBox() const
{
  if (AreaRef rootArea = getRootArea())
    return rootArea->box();
  else
    return BoundingBox();
}

SmartPtr<Element>
View::getElementAt(const scaled& x, const scaled& y, Point* elemOrigin, BoundingBox* elemBox) const
{
  if (AreaRef rootArea = getRootArea())
    {
      AreaId deepId(rootArea);
      if (rootArea->searchByCoords(deepId, x, y))
	for (int i = deepId.size(); i >= 0; i--)
	  {
	    AreaRef area = deepId.getArea(i);
	    if (SmartPtr<Element> elem = area->getElement())
	      {
		if (elemOrigin) deepId.getOrigin(*elemOrigin, 0, i);
		if (elemBox) *elemBox = area->box();
		return elem;
	      }
	  }
    }

  return 0;
}

bool
View::getElementExtents(const SmartPtr<Element>& refElem, const SmartPtr<Element>& elem,
			Point* elemOrigin, BoundingBox* elemBox) const
{
  assert(refElem);
  assert(elem);
  if (getRootArea())
    if (AreaRef elemArea = elem->getArea())
      {
	if (elemOrigin)
	  {
	    if (AreaRef refArea = refElem->getArea())
	      {
		AreaId elemId(refArea);
		if (refArea->searchByArea(elemId, elemArea))
		  elemId.getOrigin(*elemOrigin);
		else
		  return false;
	      }
	    else
	      return false;
	  }
	
	if (elemBox) *elemBox = elemArea->box();

	return true;
      }

  return false;
}

bool
View::getElementExtents(const SmartPtr<Element>& elem, Point* elemOrigin, BoundingBox* elemBox) const
{
  return getElementExtents(getRootElement(), elem, elemOrigin, elemBox);
}

bool
View::getElementLength(const SmartPtr<Element>& elem, CharIndex& length) const
{
  assert(elem);
  if (getRootArea())
    if (AreaRef elemArea = elem->getArea())
      {
	length = elemArea->length();
	return true;
      }

  return false;
}

SmartPtr<Element>
View::getCharAt(const scaled& x, const scaled& y, CharIndex& index, Point* charOrig, BoundingBox* charBox) const
{
  if (AreaRef rootArea = getRootArea())
    {
      AreaId deepId(rootArea);
      if (rootArea->searchByCoords(deepId, x, y))
	for (int i = deepId.size(); i >= 0; i--)
	  {
	    AreaRef area = deepId.getArea(i);
	    if (SmartPtr<Element> elem = area->getElement())
	      {
		Point deepOrigin;
		deepId.accumulateOrigin(deepOrigin);
		
		AreaRef deepArea = deepId.getArea();
		CharIndex deepIndex;
		if (!deepArea->indexOfPosition(x - deepOrigin.x, y - deepOrigin.y, deepIndex))
		  deepIndex = 0;
		
		index = deepId.getLength(i, -1) + deepIndex;
		
		if (charOrig || charBox)
		  {
		    if (!deepArea->positionOfIndex(deepIndex, charOrig, charBox))
		      return 0;
		  }
		
		return elem;
	      }
	  }
    }

  return 0;
}

bool
View::getCharExtents(const SmartPtr<Element>& refElem, const SmartPtr<Element>& elem, CharIndex index,
		     Point* charOrig, BoundingBox* charBox) const
{
  assert(refElem);
  assert(elem);

  Point elemOrig;
  if (getElementOrigin(refElem, elem, elemOrig))
    if (AreaRef elemArea = elem->getArea())
      {
	AreaId deepId(elemArea);
	if (elemArea->searchByIndex(deepId, index))
	  {
	    AreaRef deepArea = deepId.getArea();
	    Point deepOrig;
	    deepId.getOrigin(deepOrig);

	    if (deepArea->positionOfIndex(index - deepId.getLength(), charOrig, charBox))
	      {
		if (charOrig)
		  {
		    charOrig->x += elemOrig.x + deepOrig.x;
		    charOrig->y += elemOrig.y + deepOrig.y;
		  }

		return true;
	      }
	  }
      }
  
  return false;
}

bool
View::getCharExtents(const SmartPtr<Element>& elem, CharIndex index,
		     Point* charOrig, BoundingBox* charBox) const
{
  return getCharExtents(getRootElement(), elem, index, charOrig, charBox);
}

void
View::setMathMLNamespaceContext(const SmartPtr<MathMLNamespaceContext>& ctxt)
{
  mathmlContext = ctxt;
  if (builder) builder->setMathMLNamespaceContext(mathmlContext);
}

SmartPtr<MathMLNamespaceContext>
View::getMathMLNamespaceContext(void) const
{ return mathmlContext; }

void
View::render(RenderingContext& ctxt, const scaled& x, const scaled& y) const
{
  //std::cerr << "View::render " << &ctxt << std::endl;
  if (AreaRef rootArea = getRootArea())
    {
      Clock perf;
      perf.Start();

      // Basically (x, y) are the coordinates of the origin
      rootArea->render(ctxt, x, y);

      perf.Stop();
      getLogger()->out(LOG_INFO, "rendering time: %dms", perf());
    }
}

void
View::setDirtyLayout() const
{
  if (SmartPtr<Element> elem = getRootElement())
    {
      //elem->setDirtyAttributeD();
      elem->setDirtyLayoutD();	  
    }
}

void
View::setDefaultFontSize(unsigned size)
{
  assert(size > 0);
  if (defaultFontSize != size)
    {
      defaultFontSize = size;
      setDirtyLayout();
    }
}

void
View::setAvailableWidth(const scaled& width)
{
  if (width != availableWidth)
    {
      availableWidth = width;
      setDirtyLayout();
    }
}
