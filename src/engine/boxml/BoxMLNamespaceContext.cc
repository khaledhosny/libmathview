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
#include "BoxMLNamespaceContext.hh"
#include "BoxGraphicDevice.hh"
#include "BoxMLElementFactory.hh"
#include "Linker.hh"

BoxMLNamespaceContext::BoxMLNamespaceContext(const SmartPtr<View>& v,
					     const SmartPtr<Linker>& l,
					     const SmartPtr<BoxMLElementFactory>& f,
					     const SmartPtr<BoxGraphicDevice>& d)
  : NamespaceContext(BOXML_NS_URI, v, l), factory(f), device(d)
{ }

BoxMLNamespaceContext::~BoxMLNamespaceContext()
{ }

SmartPtr<Element>
BoxMLNamespaceContext::construct(const DOM::Element& el) const
{
  assert(el);
  if (SmartPtr<BoxMLElement> elem = getLinker()->get<BoxMLElement>(el, getFactory()))
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
BoxMLNamespaceContext::format(const SmartPtr<Element>& el) const
{
  SmartPtr<BoxMLElement> elem = smart_cast<BoxMLElement>(el);
  assert(elem);
  if (elem->dirtyLayout())
    {
      BoxFormattingContext ctxt(device);
      scaled l = device->evaluate(ctxt, Length(defaultFontSize, Length::PT_UNIT), scaled::zero());
      //ctxt.setSize(device->evaluate(ctxt, Length(28, Length::PT_UNIT), scaled::zero()));
      ctxt.setSize(l);
      ctxt.setActualSize(ctxt.getSize());
      Clock perf;
      perf.Start();
      elem->format(ctxt);
      perf.Stop();
      Globals::logger(LOG_INFO, "formatting time: %dms", perf());
    }
  return elem->getArea();
}

SmartPtr<BoxMLElementFactory>
BoxMLNamespaceContext::getFactory() const
{
  return factory;
}
