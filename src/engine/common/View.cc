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

#include "Globals.hh"
#include "Clock.hh"
#include "View.hh"
#include "Element.hh"
#include "Builder.hh"
#include "MathMLNamespaceContext.hh"
#if ENABLE_BOXML
#include "BoxMLNamespaceContext.hh"
#endif // ENABLE_BOXML
#include "AreaId.hh"
#include "scaled.hh"
#include "scaledConv.hh"
#include "WrapperArea.hh"

View::View(const SmartPtr<Builder>& b)

  : builder(b), defaultFontSize(Globals::configuration.getFontSize()), freezeCounter(0)
{ }

View::~View()
{
  // When the view is destroyed the formatting tree must have
  // been destroyed already, because elements need a healthy
  // view for de-registering themselves from the linker
  assert(!rootElement);
}

SmartPtr<View>
View::create(const SmartPtr<Builder>& builder)
{
  SmartPtr<View> view = new View(builder);
  return view;
}

#if ENABLE_BOXML
void
View::initialize(const SmartPtr<MathGraphicDevice>& mgd,
		 const SmartPtr<BoxGraphicDevice>& bgd)
{
  // the following fields cannot be initialized within the constructor
  // because there the reference counter is still 0, so it is
  // harmful to pass `this' as pointer
  mathmlContext = MathMLNamespaceContext::create(this, mgd);
  boxmlContext = BoxMLNamespaceContext::create(this, bgd);
  getBuilder()->setNamespaceContexts(mathmlContext, boxmlContext);
}
#else
void
View::initialize(const SmartPtr<MathGraphicDevice>& mgd)
{
  // the following fields cannot be initialized within the constructor
  // because there the reference counter is still 0, so it is
  // harmful to pass `this' as pointer
  mathmlContext = MathMLNamespaceContext::create(this, mgd);
  getBuilder()->setNamespaceContext(mathmlContext);
}
#endif // ENABLE_BOXML

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

SmartPtr<Builder>
View::getBuilder() const
{ return builder; }

AreaRef
View::formatElement(const SmartPtr<Element>& elem) const
{ return elem ? elem->getNamespaceContext()->format(elem) : 0; }

SmartPtr<Element>
View::getRootElement() const
{
  const SmartPtr<Element> oldRootElement = rootElement;
  bool rootDirty = !rootElement ||
    rootElement->dirtyStructure() || rootElement->dirtyAttribute() ||
    rootElement->dirtyAttributeP();

  if (rootDirty)
    {
      Clock perf;
	
      perf.Start();
      rootElement = builder->getRootElement();
      perf.Stop();

      Globals::logger(LOG_INFO, "build time: %dms", perf());
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
	  if (SmartPtr<const WrapperArea> area = smart_cast<const WrapperArea>(deepId.getArea(i)))
	    {
	      SmartPtr<Element> elem = area->getElement();
	      assert(elem);
	      if (elemOrigin) deepId.getOrigin(*elemOrigin, 0, i);
	      if (elemBox) *elemBox = area->box();
	      return elem;
	    }
    }

  return 0;
}

bool
View::getElementExtents(const SmartPtr<Element>& elem, Point* elemOrigin, BoundingBox* elemBox) const
{
  assert(elem);
  if (AreaRef rootArea = getRootArea())
    if (AreaRef elemArea = elem->getArea())
      {
	if (elemOrigin)
	  {
	    AreaId elemId(rootArea);
	    if (rootArea->searchByArea(elemId, elemArea))
	      elemId.getOrigin(*elemOrigin);
	    else
	      return false;
	  }
	
	if (elemBox) *elemBox = elemArea->box();

	return true;
      }

  return false;
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
	  if (SmartPtr<const WrapperArea> elemArea = smart_cast<const WrapperArea>(deepId.getArea(i)))
	    {
	      SmartPtr<Element> elem = elemArea->getElement();
	      assert(elem);
	      
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

  return false;
}

bool
View::getCharExtents(const SmartPtr<Element>& elem, CharIndex index, Point* charOrig, BoundingBox* charBox) const
{
  assert(elem);

  Point elemOrig;
  if (getElementOrigin(elem, elemOrig))
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

SmartPtr<MathMLNamespaceContext>
View::getMathMLNamespaceContext(void) const
{ return mathmlContext; }

#if ENABLE_BOXML
SmartPtr<BoxMLNamespaceContext>
View::getBoxMLNamespaceContext(void) const
{ return boxmlContext; }
#endif // ENABLE_BOXML

void
View::render(RenderingContext& ctxt, const scaled& x, const scaled& y) const
{
  if (AreaRef rootArea = getRootArea())
    {
      Clock perf;
      perf.Start();
      // FIXME: think of the rational of why the coordinates must be
      // inverted.
      rootArea->render(ctxt, -x, -y);
      perf.Stop();
      //Globals::logger(LOG_INFO, "rendering time: %dms", perf());
    }
}

void
View::setDefaultFontSize(unsigned size)
{
  assert(size > 0);
  if (defaultFontSize != size)
    {
      defaultFontSize = size;
      if (SmartPtr<Element> elem = getRootElement())
	{
	  //elem->setDirtyAttributeD();
	  elem->setDirtyLayout();	  
	}
    }
}

