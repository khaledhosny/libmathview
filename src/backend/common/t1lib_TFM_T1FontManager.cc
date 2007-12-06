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
