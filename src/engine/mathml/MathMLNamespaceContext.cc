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

#include "View.hh"
#include "Globals.hh"
#include "Clock.hh"
#include "RefinementContext.hh"
#include "MathMLNamespaceContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLElementFactory.hh"
#include "Linker.hh"

MathMLNamespaceContext::MathMLNamespaceContext(const SmartPtr<View>& v,
					       const SmartPtr<Linker>& l,
					       const SmartPtr<MathMLElementFactory>& f,
					       const SmartPtr<MathGraphicDevice>& d)
  : NamespaceContext(MATHML_NS_URI, v, l), factory(f), device(d), context(d)
{
  defaultFontSize = Globals::configuration.GetFontSize();
}

MathMLNamespaceContext::~MathMLNamespaceContext()
{ }

void
MathMLNamespaceContext::setDefaultFontSize(unsigned size)
{
  assert(size > 0);
  if (defaultFontSize != size)
    {
      defaultFontSize = size;
      if (SmartPtr<Element> elem = getView()->getRootElement())
	{
	  elem->setDirtyAttributeD();
	  elem->setDirtyLayout();	  
	}
    }
}

SmartPtr<Element>
MathMLNamespaceContext::construct(const DOM::Element& el) const
{
  assert(el);
  if (SmartPtr<MathMLElement> elem = smart_cast<MathMLElement>(getLinker()->get(el, getFactory())))
    {
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

      return elem;
    }

  return 0;
}

AreaRef
MathMLNamespaceContext::format(const SmartPtr<Element>& el) const
{
  SmartPtr<MathMLElement> elem = smart_cast<MathMLElement>(el);
  assert(elem);
  if (elem->dirtyLayout())
    {
      Clock perf;
      perf.Start();
      elem->format(context);
      perf.Stop();
      Globals::logger(LOG_INFO, "formatting time: %dms", perf());
    }
  return elem->getArea();
}

SmartPtr<MathMLElementFactory>
MathMLNamespaceContext::getFactory() const
{
  return factory;
}
