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
#include "t1lib_TFM_T1Font.hh"
#include "t1lib_TFM_T1FontManager.hh"

t1lib_TFM_T1FontManager::t1lib_TFM_T1FontManager(const SmartPtr<TFMManager>& tm)
  : tfmManager(tm)
{ }

t1lib_TFM_T1FontManager::~t1lib_TFM_T1FontManager()
{ }

SmartPtr<t1lib_TFM_T1FontManager>
t1lib_TFM_T1FontManager::create(const SmartPtr<TFMManager>& tm)
{ return new t1lib_TFM_T1FontManager(tm); }

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
