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

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "Gtk_AreaFactory.hh"
#include "Gtk_MathGraphicDevice.hh"
#include "Gtk_XftFontManager.hh"
#include "Gtk_DefaultPangoShaper.hh"
#include "Gtk_PangoShaper.hh"
#include "Gtk_AdobeShaper.hh"
#if HAVE_LIBT1
#include "t1lib_T1FontManager.hh"
#if HAVE_TFM
#include "t1lib_TFM_T1FontManager.hh"
#endif // HAVE_TFM
#include "Gtk_T1ComputerModernShaper.hh"
#endif // HAVE_LIBT1
#include "SpaceShaper.hh"
#include "MathMLElement.hh"
#include "FormattingContext.hh"

Gtk_MathGraphicDevice::Gtk_MathGraphicDevice(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : MathGraphicDevice(l), gtk_factory(Gtk_AreaFactory::create())
{
  setFactory(gtk_factory);

  GObjectPtr<PangoContext> context = gdk_pango_context_get();
  SmartPtr<Gtk_XftFontManager> xftFontManager = Gtk_XftFontManager::create();
#if HAVE_LIBT1
  SmartPtr<t1lib_T1FontManager> t1FontManager;
#endif // HAVE_LIBT1

  for (std::vector<Configuration::ConfiguredShaper>::const_iterator p = conf->getShapers().begin();
       p != conf->getShapers().end();
       p++)
    {
      const String name = p->getName();
      if (name == "pango-default")
	{
	  getLogger()->out(LOG_INFO, "Activating default Pango shaper");
	  SmartPtr<Gtk_DefaultPangoShaper> defaultPangoShaper = Gtk_DefaultPangoShaper::create();
	  defaultPangoShaper->setPangoContext(context);
	  getShaperManager()->registerShaper(defaultPangoShaper);
	}
      else if (name == "space")
	{
	  getLogger()->out(LOG_INFO, "Activating Space shaper");
	  getShaperManager()->registerShaper(SpaceShaper::create());
	}
      else if (name == "pango")
	{
	  getLogger()->out(LOG_INFO, "Activating Pango shaper");
	  SmartPtr<Gtk_PangoShaper> pangoShaper = Gtk_PangoShaper::create();
	  pangoShaper->setPangoContext(context);
	  getShaperManager()->registerShaper(pangoShaper);
	}
      else if (name == "adobe")
	{
	  getLogger()->out(LOG_INFO, "Activating Adobe shaper");	
	  SmartPtr<Gtk_AdobeShaper> adobeShaper = Gtk_AdobeShaper::create();
	  adobeShaper->setFontManager(xftFontManager);
	  //getShaperManager()->registerShaper(adobeShaper);
	}
      else if (name == "computer-modern")
	{
	  getLogger()->out(LOG_INFO, "Activating ComputerModern shaper (TFM %s)", conf->getUseTFM() ? "enabled" : "disabled");
#if HAVE_LIBT1
	  if (!t1FontManager)
	    {
#if HAVE_TFM
	      if (conf->getUseTFM())
		t1FontManager = t1lib_TFM_T1FontManager::create(TFMManager::create());
	      else
		t1FontManager = t1lib_T1FontManager::create();
#else
	      t1FontManager = t1lib_T1FontManager::create();
#endif // HAVE_TFM
	    }
	  SmartPtr<Gtk_T1ComputerModernShaper> cmShaper = Gtk_T1ComputerModernShaper::create();
	  cmShaper->setFontManager(t1FontManager);
	  getShaperManager()->registerShaper(cmShaper);
#else
	  getLogger()->out(LOG_WARNING, "t1lib support has not been compiled in, ");
#endif // HAVE_LIBT1
	}
      else
	getLogger()->out(LOG_WARNING, "unknown shaper `%s' (ignored)", name.c_str());
    }
}

Gtk_MathGraphicDevice::~Gtk_MathGraphicDevice()
{ }

SmartPtr<Gtk_MathGraphicDevice>
Gtk_MathGraphicDevice::create(const SmartPtr<AbstractLogger>& logger,
			      const SmartPtr<Configuration>& conf)
{ 
  return new Gtk_MathGraphicDevice(logger, conf);
}

AreaRef
Gtk_MathGraphicDevice::wrapper(const FormattingContext& context,
			       const AreaRef& base) const
{
  return gtk_factory->wrapper(base, base->box(), context.getMathMLElement());
}
