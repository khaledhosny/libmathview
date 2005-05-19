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
#include <map>

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

  std::multimap<int, SmartPtr<Shaper> > shaperSet;
  if (conf->getBool(l, "gtk-backend/pango-default-shaper/enabled", true))
    {
      SmartPtr<Gtk_DefaultPangoShaper> defaultPangoShaper = Gtk_DefaultPangoShaper::create();
      defaultPangoShaper->setPangoContext(context);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/pango-default-shaper/priority", 0), defaultPangoShaper));
    }

  if (conf->getBool(l, "gtk-backend/space-shaper/enabled", false))
    {
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/space-shaper/priority", 0), SpaceShaper::create()));
    }

  if (conf->getBool(l, "gtk-backend/pango-shaper/enabled", false))
    {
      SmartPtr<Gtk_PangoShaper> pangoShaper = Gtk_PangoShaper::create();
      pangoShaper->setPangoContext(context);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/pango-shaper/priority", 0), pangoShaper));
    }

  if (conf->getBool(l, "gtk-backend/adobe-shaper/enabled", false))
    {
      // FIXME: the XFT font manager works really bad
      SmartPtr<Gtk_XftFontManager> xftFontManager = Gtk_XftFontManager::create();
      SmartPtr<Gtk_AdobeShaper> adobeShaper = Gtk_AdobeShaper::create();
      adobeShaper->setFontManager(xftFontManager);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/adobe-shaper/priority", 0), adobeShaper));
    }

  if (conf->getBool(l, "gtk-backend/type1-computer-modern-shaper/enabled", false))
    {
      const bool useTFM = conf->getBool(l, "gtk-backend/type1-computer-modern-shaper/use-tfm", false);
#if HAVE_LIBT1
      SmartPtr<t1lib_T1FontManager> t1FontManager;
#if HAVE_TFM
      if (useTFM)
	t1FontManager = t1lib_TFM_T1FontManager::create(TFMManager::create());
      else
	t1FontManager = t1lib_T1FontManager::create();
#else
      t1FontManager = t1lib_T1FontManager::create();
#endif // HAVE_TFM
      SmartPtr<Gtk_T1ComputerModernShaper> cmShaper = Gtk_T1ComputerModernShaper::create();
      cmShaper->setFontManager(t1FontManager);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "gtk-backend/type1-computer-modern-shaper/priority", 0), cmShaper));
#else
      getLogger()->out(LOG_WARNING, "t1lib support has not been compiled in, ");
#endif // HAVE_LIBT1
    }

  for (std::multimap<int, SmartPtr<Shaper> >::const_iterator p = shaperSet.begin();
       p != shaperSet.end();
       p++)
    getShaperManager()->registerShaper(p->second);
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
