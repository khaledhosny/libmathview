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
#include <map>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "SVG_AreaFactory.hh"
#include "SVG_MathGraphicDevice.hh"
#include "SpaceShaper.hh"
#include "NullShaper.hh"
#ifdef ENABLE_TFM
#include "TFMManager.hh"
#include "TFMFontManager.hh"
#include "SVG_TFMComputerModernShaper.hh"
#include "SVG_TTF_TFMComputerModernShaper.hh"
#endif // ENABLE_TFM

SVG_MathGraphicDevice::SVG_MathGraphicDevice(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : MathGraphicDevice(l)
{
  setFactory(SVG_AreaFactory::create());

#if ENABLE_TFM
  // the fact that the Type1 and TT versions of the computer modern
  // shapers for the SVG backend share the same font manager is just a
  // twisted coincidence. Beware
  SmartPtr<TFMManager> tfm = TFMManager::create();
  SmartPtr<TFMFontManager> fm = TFMFontManager::create(tfm);
#endif // ENABLE_TFM

  std::multimap<int, SmartPtr<Shaper> > shaperSet;
  if (conf->getBool(l, "svg-backend/null-shaper/enabled", true))
    shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "svg-backend/null-shaper/priority", 0),
						      NullShaper::create()));

  if (conf->getBool(l, "svg-backend/space-shaper/enabled", false))
    shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "svg-backend/space-shaper/priority", 0),
						      SpaceShaper::create()));

#if ENABLE_TFM
  if (conf->getBool(l, "svg-backend/type1-computer-modern-shaper/enabled", false))
    {
      SmartPtr<SVG_TFMComputerModernShaper> cmShaper = SVG_TFMComputerModernShaper::create();
      cmShaper->setFontManager(fm);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "svg-backend/type1-computer-modern-shaper/priority", 0),
							cmShaper));
    }

  if (conf->getBool(l, "svg-backend/ttf-computer-modern-shaper/enabled", false))
    {
      SmartPtr<SVG_TTF_TFMComputerModernShaper> cmShaper = SVG_TTF_TFMComputerModernShaper::create();
      cmShaper->setFontManager(fm);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "svg-backend/ttf-computer-modern-shaper/priority", 0),
							cmShaper));
    }
#endif // ENABLE_TFM

  for (std::multimap<int, SmartPtr<Shaper> >::const_iterator p = shaperSet.begin();
       p != shaperSet.end();
       p++)
    getShaperManager()->registerShaper(p->second);
}

SVG_MathGraphicDevice::~SVG_MathGraphicDevice()
{ }

SmartPtr<SVG_MathGraphicDevice>
SVG_MathGraphicDevice::create(const SmartPtr<AbstractLogger>& logger,
			      const SmartPtr<Configuration>& conf)
{ return new SVG_MathGraphicDevice(logger, conf); }
