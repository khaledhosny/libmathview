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

#include "Clock.hh"
#include "View.hh"
#include "Element.hh"
#include "Builder.hh"
#include "MathMLNamespaceContext.hh"
#include "BoxMLNamespaceContext.hh"
#include "AreaId.hh"
#include "scaled.hh"
#include "scaledConv.hh"
// FIXME:
#include "Gtk_WrapperArea.hh"

View::View(const SmartPtr<Builder>& b)

  : builder(b), freezeCounter(0)
{ }

View::~View()
{ }

SmartPtr<View>
View::create(const SmartPtr<Builder>& builder)
{
  SmartPtr<View> view = new View(builder);
  return view;
}

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
{ return builder->getRootElement(); }

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
	  if (SmartPtr<const Gtk_WrapperArea> area = smart_cast<const Gtk_WrapperArea>(*p))
	    if ((elem = smart_cast<Element>(area->getElement())))
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
View::getCharAt(const scaled& x, const scaled& y, SmartPtr<Element>& elem, int& index) const
{
  if (AreaRef rootArea = getRootArea())
    {
      BoundingBox box = rootArea->box();
      AreaId id(rootArea);
      if (rootArea->searchByCoords(id, x + x0, y + box.height + y0))
	for (AreaId::AreaVector::const_reverse_iterator p = id.getAreas().rbegin(); p != id.getAreas().rend(); p++)
	  if (SmartPtr<const Gtk_WrapperArea> area = smart_cast<const Gtk_WrapperArea>(*p))
	    if ((elem = smart_cast<Element>(area->getElement())))
	      {
		index = 0; // FIXME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return true;
	      }
    }
  
  return false;
}

bool
View::getCharExtents(const SmartPtr<Element>& elem, int index, scaled& x, scaled& y, BoundingBox& box) const
{
  assert(elem);
  if (getRootArea()) // just to be sure the tree has been formatted
    if (AreaRef elemArea = elem->getArea())
      {
	AreaId elemId(elemArea);
	if (elemArea->searchByIndex(elemId, index))
	  {
	    elemId.getOrigin(x, y);
	    box = elemId.getArea()->box();
	    return true;
	  }
      }
							    
  return false;
}

SmartPtr<MathMLNamespaceContext>
View::getMathMLNamespaceContext(void) const
{ return mathmlContext; }

SmartPtr<BoxMLNamespaceContext>
View::getBoxMLNamespaceContext(void) const
{ return boxmlContext; }

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
