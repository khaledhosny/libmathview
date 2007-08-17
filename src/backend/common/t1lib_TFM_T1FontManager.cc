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

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "TFMManager.hh"
#include "t1lib_TFM_T1Font.hh"
#include "t1lib_TFM_T1FontManager.hh"

t1lib_TFM_T1FontManager::t1lib_TFM_T1FontManager(const SmartPtr<AbstractLogger>& logger,
						 const SmartPtr<Configuration>& conf,
						 const SmartPtr<TFMManager>& tm)
  : t1lib_T1FontManager(logger, conf), tfmManager(tm)
{ }

t1lib_TFM_T1FontManager::~t1lib_TFM_T1FontManager()
{ }

SmartPtr<t1lib_TFM_T1FontManager>
t1lib_TFM_T1FontManager::create(const SmartPtr<AbstractLogger>& logger,
				const SmartPtr<Configuration>& conf,
				const SmartPtr<TFMManager>& tm)
{ return new t1lib_TFM_T1FontManager(logger, conf, tm); }

SmartPtr<t1lib_T1Font>
t1lib_TFM_T1FontManager::createT1Font(const String& name, const scaled& size) const
{
  if (SmartPtr<TFM> tfm = tfmManager->getTFM(name))
    {
      const int fontId = getFontId(name);
      if (fontId >= 0)
	return t1lib_TFM_T1Font::create(fontId, size, tfm);
      else
	return 0;
    }
  else
    return t1lib_T1FontManager::createT1Font(name, size);
}
