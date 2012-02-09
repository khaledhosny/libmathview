// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include <gtk/gtk.h>

#include <map>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "Gtk_Backend.hh"
#include "Gtk_AreaFactory.hh"
#include "Gtk_MathGraphicDevice.hh"
#if GMV_ENABLE_BOXML
#include "Gtk_BoxGraphicDevice.hh"
#else
#include "BoxGraphicDevice.hh"
#endif // GMV_ENABLE_BOXML
#include "Gtk_PangoFontManager.hh"
#include "Gtk_DefaultPangoShaper.hh"
#include "Gtk_PangoShaper.hh"
#include "Gtk_PangoComputerModernShaper.hh"
#include "Gtk_AdobeShaper.hh"
#include "NullShaper.hh"
#include "SpaceShaper.hh"
#include "ShaperManager.hh"

Gtk_Backend::Gtk_Backend(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : Backend(l, conf)
{
  SmartPtr<Gtk_AreaFactory> factory = Gtk_AreaFactory::create();
  SmartPtr<Gtk_MathGraphicDevice> mgd = Gtk_MathGraphicDevice::create(l, conf);
  mgd->setFactory(factory);
  setMathGraphicDevice(mgd);
#if GMV_ENABLE_BOXML
  SmartPtr<Gtk_BoxGraphicDevice> bgd = Gtk_BoxGraphicDevice::create(l, conf);
  bgd->setFactory(factory);
  setBoxGraphicDevice(bgd);
#endif // GMV_ENABLE_BOXML

  SmartPtr<Gtk_DefaultPangoShaper> defaultPangoShaper;
  GObjectPtr<PangoContext> context = gdk_pango_context_get();
  std::multimap<int, SmartPtr<Shaper> > shaperSet;
  if (conf->getBool(l, "gtk-backend/null-shaper/enabled", false))
    {
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/null-shaper/priority", 0), NullShaper::create(l)));
    }

  if (conf->getBool(l, "gtk-backend/pango-default-shaper/enabled", false))
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
