// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include <cassert>

#include "Gtk_AreaFactory.hh"
#include "Gtk_MathGraphicDevice.hh"
#include "Gtk_XftFontManager.hh"
#include "Gtk_DefaultPangoShaper.hh"
#include "Gtk_PangoShaper.hh"
#include "Gtk_AdobeShaper.hh"
#include "SpaceShaper.hh"
#include "MathMLElement.hh"

Gtk_MathGraphicDevice::Gtk_MathGraphicDevice()
  : gtk_factory(Gtk_AreaFactory::create())
{
  setFactory(gtk_factory);

  //GObjectPtr<PangoContext> context = gtk_widget_create_pango_context(widget);
  GObjectPtr<PangoContext> context = gdk_pango_context_get();
  SmartPtr<Gtk_DefaultPangoShaper> defaultPangoShaper = Gtk_DefaultPangoShaper::create();
  defaultPangoShaper->setPangoContext(context);
  getShaperManager()->registerShaper(defaultPangoShaper);

  getShaperManager()->registerShaper(SpaceShaper::create());

#if 1
  SmartPtr<Gtk_PangoShaper> pangoShaper = Gtk_PangoShaper::create();
  pangoShaper->setPangoContext(context);
  getShaperManager()->registerShaper(pangoShaper);
#else
  SmartPtr<Gtk_XftFontManager> xftFontManager = Gtk_XftFontManager::create();
  SmartPtr<Gtk_AdobeShaper> adobeShaper = Gtk_AdobeShaper::create();
  adobeShaper->setFontManager(xftFontManager);
  getShaperManager()->registerShaper(adobeShaper);
#endif
}

Gtk_MathGraphicDevice::~Gtk_MathGraphicDevice()
{ }

AreaRef
Gtk_MathGraphicDevice::wrapper(const MathFormattingContext& context,
			       const AreaRef& base) const
{
  BoundingBox box = base->box();
  return gtk_factory->wrapper(base->fit(box.width, box.height, box.depth), box, context.getElement());
}
