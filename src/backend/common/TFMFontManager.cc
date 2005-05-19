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

#include "TFMManager.hh"
#include "TFMFont.hh"
#include "TFMFontManager.hh"

TFMFontManager::TFMFontManager(const SmartPtr<TFMManager>& tm)
  : tfmManager(tm)
{ }

TFMFontManager::~TFMFontManager()
{ }

SmartPtr<TFMFontManager>
TFMFontManager::create(const SmartPtr<TFMManager>& tm)
{ return new TFMFontManager(tm); }

SmartPtr<TFMFont>
TFMFontManager::createFont(const SmartPtr<TFM>& tfm, const scaled& size) const
{ return TFMFont::create(tfm, size); }

SmartPtr<TFMFont>
TFMFontManager::getFont(const SmartPtr<TFM>& tfm, const scaled& size) const
{
  const CachedFontKey key(tfm->getName(), size);
  FontCache::iterator p = fontCache.find(key);
  if (p != fontCache.end())
    return p->second;
  else if (const SmartPtr<TFMFont> font = createFont(tfm, size))
    {
      fontCache[key] = font;
      return font;
    }
  else
    return 0;
}

SmartPtr<TFMFont>
TFMFontManager::getFont(const String& name, const scaled& size) const
{ return getFont(tfmManager->getTFM(name), size); }
