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
View::getElementArea(const SmartPtr<Element>& elem) const
{ return elem ? elem->getNamespaceContext()->format(elem) : 0; }

SmartPtr<Element>
View::getRootElement() const
{
  Clock perf;
	
  const SmartPtr<Element> oldRootElement = rootElement;
  bool rootDirty = rootElement && (rootElement->dirtyStructure() || rootElement->dirtyAttribute() || rootElement->dirtyAttributeP());

  perf.Start();
  rootElement = builder->getRootElement();
  perf.Stop();

  if (rootDirty || rootElement != oldRootElement)
    Globals::logger(LOG_INFO, "build time: %dms", perf());
  
  return rootElement;
}

void
View::resetRootElement()
{
  rootElement = 0;
}

AreaRef
View::getRootArea() const
{ return getElementArea(getRootElement()); }

#include "scaledAux.hh"

bool
View::getElementAt(const scaled& x, const scaled& y, SmartPtr<Element>& elem) const
{
  if (AreaRef rootArea = getRootArea())
    {
      BoundingBox box = rootArea->box();
      AreaId id(rootArea);
      if (rootArea->searchByCoords(id, x + x0, y + box.height + y0))
	for (AreaId::AreaVector::const_reverse_iterator p = id.getAreas().rbegin(); p != id.getAreas().rend(); p++)
	  if (SmartPtr<const WrapperArea> area = smart_cast<const WrapperArea>(*p))
	    if ((elem = area->getElement()))
	      return true;
    }
  
  return false;
}

bool
View::getElementExtents(const SmartPtr<Element>& elem, scaled& x, scaled& y, BoundingBox& box) const
{
  assert(elem);
  if (AreaRef rootArea = getRootArea())
    if (AreaRef elemArea = elem->getArea())
      {
	AreaId elemId(rootArea);
	if (rootArea->searchByArea(elemId, elemArea))
	  {
	    elemId.getOrigin(x, y);
	    box = elemArea->box();
	    return true;
	  }
      }
							    
  return false;
}

bool
View::getElementLength(const SmartPtr<Element>& elem, int& length) const
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

bool
View::getCharAt(const scaled& x, const scaled& y, SmartPtr<Element>& elem, int& index) const
{
  if (getElementAt(x, y, elem))
    {
      scaled ex;
      scaled ey;
      BoundingBox ebox;
      if (getElementExtents(elem, ex, ey, ebox))
	if (AreaRef elemArea = elem->getArea())
	  if (elemArea->indexOfPosition(x - ex, y - ey, index))
	    return true;
    }
  
  return false;
}

bool
View::getCharExtents(const SmartPtr<Element>& elem, int index, scaled& x, scaled& y, BoundingBox& box) const
{
  assert(elem);
  if (getRootArea()) // just to be sure the tree has been formatted
    if (AreaRef elemArea = elem->getArea())
      if (elemArea->positionOfIndex(index, x, y, box))
	return true;
							    
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
View::render(RenderingContext& ctxt) const
{
  if (AreaRef rootArea = getRootArea())
    {
      Clock perf;
      perf.Start();
      BoundingBox box = rootArea->box();
      rootArea->render(ctxt, -x0, -box.height - y0);
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

