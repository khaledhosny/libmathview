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

#include "defs.h"
#include "View.hh"
#include "Clock.hh"
#include "AbstractLogger.hh"
#include "MathMLElement.hh"
#include "MathMLNamespaceContext.hh"
#include "MathGraphicDevice.hh"

MathMLNamespaceContext::MathMLNamespaceContext(const SmartPtr<View>& v,
					       const SmartPtr<MathGraphicDevice>& d)
  : NamespaceContext(MATHML_NS_URI, v), device(d)
{ }

MathMLNamespaceContext::~MathMLNamespaceContext()
{ }

#include <iostream>

AreaRef
MathMLNamespaceContext::format(const SmartPtr<Element>& el) const
{
  SmartPtr<MathMLElement> elem = smart_cast<MathMLElement>(el);
  assert(elem);
#if 0
  std::cerr << "element to be formatted? " << static_cast<MathMLElement*>(elem)
	    << " element has dirty flag? " << elem->dirtyLayout() << std::endl;
#endif
  if (elem->dirtyLayout())
    {
      MathFormattingContext ctxt(device);
      scaled l = device->evaluate(ctxt, Length(getView()->getDefaultFontSize(), Length::PT_UNIT), scaled::zero());
      //ctxt.setSize(device->evaluate(ctxt, Length(28, Length::PT_UNIT), scaled::zero()));
      ctxt.setSize(l);
      ctxt.setActualSize(ctxt.getSize());
      Clock perf;
      perf.Start();
      elem->format(ctxt);
      perf.Stop();
      getView()->getLogger()->out(LOG_INFO, "formatting time: %dms", perf());
    }
#if 0
  std::cerr << "element  formatted to area? " << static_cast<const Area*>(elem->getArea())
	    << " element has dirty flag? " << elem->dirtyLayout() << std::endl;
#endif
  return elem->getArea();
}

SmartPtr<MathGraphicDevice>
MathMLNamespaceContext::getGraphicDevice() const
{ return device; }
