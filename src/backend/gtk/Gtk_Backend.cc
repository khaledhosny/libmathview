// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <gtk/gtk.h>

#include <map>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "Gtk_Backend.hh"
#include "Gtk_AreaFactory.hh"
#include "Gtk_MathGraphicDevice.hh"
#if ENABLE_BOXML
#include "Gtk_BoxGraphicDevice.hh"
#else
#include "BoxGraphicDevice.hh"
#endif // ENABLE_BOXML
#include "Gtk_PangoFontManager.hh"
#include "Gtk_DefaultPangoShaper.hh"
#include "Gtk_PangoShaper.hh"
#include "Gtk_PangoComputerModernShaper.hh"
#include "Gtk_AdobeShaper.hh"
#if HAVE_LIBT1
#include "t1lib_T1FontManager.hh"
#if HAVE_TFM
#include "t1lib_TFM_T1FontManager.hh"
#endif // HAVE_TFM
#include "Gtk_T1ComputerModernShaper.hh"
#endif // HAVE_LIBT1
#include "SpaceShaper.hh"
#include "ShaperManager.hh"

Gtk_Backend::Gtk_Backend(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : Backend(conf)
{
  SmartPtr<Gtk_AreaFactory> factory = Gtk_AreaFactory::create();
  SmartPtr<Gtk_MathGraphicDevice> mgd = Gtk_MathGraphicDevice::create(l, conf);
  mgd->setFactory(factory);
#if ENABLE_BOXML
  SmartPtr<Gtk_BoxGraphicDevice> bgd = Gtk_BoxGraphicDevice::create(l, conf);
  bgd->setFactory(factory);
  setDevices(mgd, bgd);
#else
  setDevices(mgd, 0);
#endif // ENABLE_BOXML

  SmartPtr<Gtk_DefaultPangoShaper> defaultPangoShaper;
  GObjectPtr<PangoContext> context = gdk_pango_context_get();
  std::multimap<int, SmartPtr<Shaper> > shaperSet;
  if (conf->getBool(l, "gtk-backend/pango-default-shaper/enabled", true))
    {
      defaultPangoShaper = Gtk_DefaultPangoShaper::create(l, conf);
      defaultPangoShaper->setPangoContext(context);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/pango-default-shaper/priority", 0), defaultPangoShaper));
    }

  if (conf->getBool(l, "gtk-backend/space-shaper/enabled", false))
    {
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/space-shaper/priority", 0), SpaceShaper::create()));
    }

  if (conf->getBool(l, "gtk-backend/pango-shaper/enabled", false))
    {
      SmartPtr<Gtk_PangoShaper> pangoShaper = Gtk_PangoShaper::create(l, conf);
      pangoShaper->setPangoContext(context);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/pango-shaper/priority", 0), pangoShaper));
    }

  if (conf->getBool(l, "gtk-backend/adobe-shaper/enabled", false))
    {
      SmartPtr<Gtk_PangoFontManager> pangoFontManager = Gtk_PangoFontManager::create();
      SmartPtr<Gtk_AdobeShaper> adobeShaper = Gtk_AdobeShaper::create();
      adobeShaper->setFontManager(pangoFontManager);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/adobe-shaper/priority", 0), adobeShaper));
    }

  if (conf->getBool(l, "gtk-backend/pango-computer-modern-shaper/enabled", false))
    {
      if (defaultPangoShaper)
	{
	  SmartPtr<Gtk_PangoComputerModernShaper> pangoCMShaper = Gtk_PangoComputerModernShaper::create(l, conf);
	  pangoCMShaper->setPangoShaper(defaultPangoShaper);
	  shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/pango-computer-modern-shaper/priority", 0),
							    pangoCMShaper));
	}
      else
	l->out(LOG_WARNING, "default Pango shaper must be enabled before Pango Computer Modern shaper");
    }

  if (conf->getBool(l, "gtk-backend/type1-computer-modern-shaper/enabled", false))
    {
      const bool useTFM = conf->getBool(l, "gtk-backend/type1-computer-modern-shaper/use-tfm", false);
#if HAVE_LIBT1
      SmartPtr<t1lib_T1FontManager> t1FontManager;
#if HAVE_TFM
      if (useTFM)
	t1FontManager = t1lib_TFM_T1FontManager::create(l, conf, TFMManager::create());
      else
	t1FontManager = t1lib_T1FontManager::create(l, conf);
#else
      t1FontManager = t1lib_T1FontManager::create(l, conf);
#endif // HAVE_TFM
      SmartPtr<Gtk_T1ComputerModernShaper> cmShaper = Gtk_T1ComputerModernShaper::create(l, conf);
      cmShaper->setFontManager(t1FontManager);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/type1-computer-modern-shaper/priority", 0), cmShaper));
#else
      l->out(LOG_WARNING, "t1lib support has not been compiled in, ");
#endif // HAVE_LIBT1
    }

  for (std::multimap<int, SmartPtr<Shaper> >::const_iterator p = shaperSet.begin();
       p != shaperSet.end();
       p++)
    getShaperManager()->registerShaper(p->second);
}

Gtk_Backend::~Gtk_Backend()
{ }

SmartPtr<Gtk_Backend>
Gtk_Backend::create(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
{ return new Gtk_Backend(l, conf); }
