// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <map>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "PS_Backend.hh"
#include "PS_AreaFactory.hh"
#include "PS_MathGraphicDevice.hh"
#if GMV_ENABLE_BOXML
#include "PS_BoxGraphicDevice.hh"
#else
#include "BoxGraphicDevice.hh"
#endif // GMV_ENABLE_BOXML
#include "SpaceShaper.hh"
#include "NullShaper.hh"
#ifdef GMV_ENABLE_TFM
#include "TFMManager.hh"
#include "TFMFontManager.hh"
#include "PS_TFMComputerModernMathGraphicDevice.hh"
#include "PS_TFMComputerModernShaper.hh"
#endif // GMV_ENABLE_TFM
#include "ShaperManager.hh"

PS_Backend::PS_Backend(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : Backend(l, conf)
{
  SmartPtr<PS_AreaFactory> factory = PS_AreaFactory::create();

#if GMV_ENABLE_TFM
  SmartPtr<TFMComputerModernShaper> cmShaper;
  // the fact that the Type1 and TT versions of the computer modern
  // shapers for the PS backend share the same font manager is just a
  // twisted coincidence. Beware
  SmartPtr<TFMManager> tfm = TFMManager::create();
  SmartPtr<TFMFontManager> fm = TFMFontManager::create(tfm);
#endif // GMV_ENABLE_TFM

  std::multimap<int, SmartPtr<Shaper> > shaperSet;
  if (conf->getBool(l, "ps-backend/null-shaper/enabled", false))
    shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "ps-backend/null-shaper/priority", 0),
						      NullShaper::create(l)));

  if (conf->getBool(l, "ps-backend/space-shaper/enabled", false))
    shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "ps-backend/space-shaper/priority", 0),
						      SpaceShaper::create()));

#if GMV_ENABLE_TFM
  if (conf->getBool(l, "ps-backend/type1-computer-modern-shaper/enabled", false))
    {
      cmShaper = PS_TFMComputerModernShaper::create(l, conf);
      cmShaper->setFontManager(fm);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "ps-backend/type1-computer-modern-shaper/priority", 0),
							cmShaper));
    }
/*
  if (conf->getBool(l, "ps-backend/ttf-computer-modern-shaper/enabled", false))
    {
      cmShaper = PS_TTF_TFMComputerModernShaper::create(l, conf);
      cmShaper->setFontManager(fm);
      shaperSet.insert(std::pair<int,SmartPtr<Shaper> >(conf->getInt(l, "ps-backend/ttf-computer-modern-shaper/priority", 0),
							cmShaper));
    }
*/
#endif // GMV_ENABLE_TFM

#if GMV_ENABLE_TFM
  SmartPtr<MathGraphicDevice> mgd;
  if (cmShaper)
    {
      SmartPtr<TFMComputerModernMathGraphicDevice> tfmMGD = PS_TFMComputerModernMathGraphicDevice::create(l, conf);
      tfmMGD->setFamily(cmShaper->getFamily());
      tfmMGD->setTFMManager(tfm);
      mgd = tfmMGD;
    }
  else
    mgd = PS_MathGraphicDevice::create(l, conf);
#else
  SmartPtr<MathGraphicDevice> mgd = PS_MathGraphicDevice::create(l, conf);
#endif // GMV_ENABLE_TFM
  mgd->setFactory(factory);
  setMathGraphicDevice(mgd);
#if GMV_ENABLE_BOXML
  SmartPtr<BoxGraphicDevice> bgd = PS_BoxGraphicDevice::create(l, conf);
  bgd->setFactory(factory);
  setBoxGraphicDevice(bgd);
#endif // GMV_ENABLE_BOXML

  for (std::multimap<int, SmartPtr<Shaper> >::const_iterator p = shaperSet.begin();
       p != shaperSet.end();
       p++)
    getShaperManager()->registerShaper(p->second);
}

PS_Backend::~PS_Backend()
{ }

SmartPtr<PS_Backend>
PS_Backend::create(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
{ return new PS_Backend(l, conf); }
