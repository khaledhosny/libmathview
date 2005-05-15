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

#include <cassert>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "SVG_AreaFactory.hh"
#include "SVG_MathGraphicDevice.hh"
#include "SVG_T1ComputerModernShaper.hh"
#include "SVG_TTFComputerModernShaper.hh"
#include "SpaceShaper.hh"
#include "NullShaper.hh"
#include "TFMManager.hh"
#include "TFM_T1FontManager.hh"

SVG_MathGraphicDevice::SVG_MathGraphicDevice(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : MathGraphicDevice(l)
{
  setFactory(SVG_AreaFactory::create());

  // the fact that the Type1 and TT versions of the computer modern
  // shapers for the SVG backend share the same font manager is just a
  // twisted coincidence. Beware
  SmartPtr<TFMManager> tfm = TFMManager::create();
  SmartPtr<TFM_T1FontManager> fm = TFM_T1FontManager::create(tfm);

  for (std::vector<Configuration::ConfiguredShaper>::const_iterator p = conf->getShapers().begin();
       p != conf->getShapers().end();
       p++)
    if (p->getBackend() == "" || p->getBackend() == "svg")
      {
	const String name = p->getName();
	if (name == "null")
	  {
	    getLogger()->out(LOG_INFO, "Activating Null shaper");
	    getShaperManager()->registerShaper(NullShaper::create());
	  }
	else if (name == "computer-modern-type1")
	  {
	    getLogger()->out(LOG_INFO, "Activating ComputerModern/Type1 shaper (TFM %s)", conf->getUseTFM() ? "enabled" : "disabled");
	    SmartPtr<SVG_T1ComputerModernShaper> cmShaper = SVG_T1ComputerModernShaper::create();
	    cmShaper->setFontManager(fm);
	    getShaperManager()->registerShaper(cmShaper);
	  }
	else if (name == "computer-modern-ttf")
	  {
	    getLogger()->out(LOG_INFO, "Activating ComputerModern/TT shaper (TFM %s)", conf->getUseTFM() ? "enabled" : "disabled");
	    SmartPtr<SVG_TTFComputerModernShaper> cmShaper = SVG_TTFComputerModernShaper::create();
	    cmShaper->setFontManager(fm);
	    getShaperManager()->registerShaper(cmShaper);
	  }
	else if (name == "space")
	  {
	    getLogger()->out(LOG_INFO, "Activating Space shaper");
	    getShaperManager()->registerShaper(SpaceShaper::create());
	  }
	else
	  getLogger()->out(LOG_WARNING, "shaper `%s' unsupported by the SVG graphic device (ignored)", name.c_str());
      }
}

SVG_MathGraphicDevice::~SVG_MathGraphicDevice()
{ }

SmartPtr<SVG_MathGraphicDevice>
SVG_MathGraphicDevice::create(const SmartPtr<AbstractLogger>& logger,
			      const SmartPtr<Configuration>& conf)
{ return new SVG_MathGraphicDevice(logger, conf); }

