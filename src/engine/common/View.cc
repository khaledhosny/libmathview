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
#include "NamespaceRegistry.hh"
#include "NamespaceContext.hh"
//#include "SearchingContext.hh"
#include "AreaId.hh"
#include "scaled.hh"
#include "scaledConv.hh"
// FIXME:
#include "Gtk_WrapperArea.hh"

View::View()
{
  registry = NamespaceRegistry::create();
  freezeCounter = 0;
}

View::~View()
{ }

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

#if 0
void
View::setRootElement(const SmartPtr<Element>& root)
{
  rootElement = root;
}

SmartPtr<Element>
View::getRootElement() const
{
  return rootElement;
}
#endif

AreaRef
View::getElementArea(const SmartPtr<Element>& elem) const
{ return elem ? elem->getNamespaceContext()->format(elem) : 0; }

AreaRef
View::getRootArea() const
{ return getElementArea(getRootElement()); }

#include "scaledAux.hh"

SmartPtr<Element>
View::getElementAt(const scaled& x, const scaled& y) const
{
  if (AreaRef rootArea = getRootArea())
    {
      BoundingBox box = rootArea->box();
#if 0
      SearchingContext context(x, y);
      
      if (rootArea->find(context, -x0, -box.height - y0))
	{
	  SearchingContext::Result result = context.getResult();
	  if (SmartPtr<const Gtk_WrapperArea> area = smart_cast<const Gtk_WrapperArea>(result.area))
	    if (SmartPtr<Element> elem = smart_cast<Element>(area->getElement()))
	      return elem;
	}
#endif
      AreaId id(rootArea);
      std::cerr << "searching at " << (x) << "," << (y + box.height) << std::endl;
      if (rootArea->searchByCoords(id, x, y + box.height))
	{
	  std::cerr << "FOUND!" << std::endl;
	  for (AreaId::AreaVector::const_reverse_iterator p = id.getAreas().rbegin(); p != id.getAreas().rend(); p++)
	    if (SmartPtr<const Gtk_WrapperArea> area = smart_cast<const Gtk_WrapperArea>(*p))
	      {
		std::cerr << "FOUND WRAPPER, with element? " << (area->getElement() != 0) << std::endl;
		if (SmartPtr<Element> elem = smart_cast<Element>(area->getElement()))
		  return elem;
	      }
	}
    }
  
  return 0;
}

bool
View::getElementExtents(const SmartPtr<Element>& elem, scaled& x, scaled& y, BoundingBox& box) const
{
  assert(elem);
  if (AreaRef rootArea = getRootArea())
    if (AreaRef elemArea = elem->getArea())
      {
#if 0
	AreaId elemId = rootArea->idOf(elemArea);

	for (AreaId id = elemId; !id.empty(); id = id.tail())
	  std::cout << id.head() << "/";
	std::cout << std::endl;

	std::pair<scaled,scaled> orig = rootArea->origin(elemId);
	x = orig.first;
	y = orig.second;
	box = elemArea->box();
	return true;
#endif
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

SmartPtr<NamespaceRegistry>
View::getRegistry(void) const
{
  return registry;
}

#if 0
AreaRef
View::getRootArea() const
{
  if (root && !frozen())
    {
      SmartPtr<MathMLElement> elem = getRootElement();
      assert(elem);

      if (elem->dirtyStructure())
	{
	  Clock perf;
	  perf.Start();
	  elem->construct();
	  perf.Stop();
	  Globals::logger(LOG_INFO, "construction time: %dms", perf());
	}

      if (elem->dirtyAttribute() || elem->dirtyAttributeP())
	{
	  RefinementContext rc;
	  Clock perf;
	  perf.Start();
	  elem->refine(rc);
	  perf.Stop();
	  Globals::logger(LOG_INFO, "refinement time: %dms", perf());
	}

      if (elem->dirtyLayout())
	{
	  MathFormattingContext ctxt(device);
	  scaled l = device->evaluate(ctxt, Length(defaultFontSize, Length::PT_UNIT), scaled::zero());
	  //ctxt.setSize(device->evaluate(ctxt, Length(28, Length::PT_UNIT), scaled::zero()));
	  ctxt.setSize(l);
	  ctxt.setActualSize(ctxt.getSize());
	  Clock perf;
	  perf.Start();
	  elem->format(ctxt);
	  perf.Stop();
	  Globals::logger(LOG_INFO, "format time: %dms", perf());
	}

      return elem->getArea();
    }

  return 0;
}

BoundingBox
View::getBoundingBox() const
{
  if (AreaRef rootArea = getRootArea())
    return rootArea->box();
  else
    return BoundingBox();
}

Rectangle
View::getRectangle() const
{
  if (AreaRef rootArea = getRootArea())
    return Rectangle(scaled::zero(), scaled::zero(), rootArea->box());
  else
    return Rectangle();
}

#endif

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
      std::cerr << "rendering time: " << perf() << "ms" << std::endl;
    }
}
