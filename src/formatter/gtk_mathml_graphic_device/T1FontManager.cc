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

#include <t1lib.h>

#include "T1FontManager.hh"

bool T1FontManager::firstTime = true;

#include <iostream>

T1FontManager::T1FontManager()
{
  if (firstTime)
    {
      const void* res = T1_InitLib(LOGFILE | IGNORE_FONTDATABASE);
      assert(res != 0);
      firstTime = false;
      T1_SetLogLevel(T1LOG_DEBUG);

      assert(T1_QueryX11Support());

      int i = T1_AddFont("cmr10.pfb");
      assert(i >= 0);
      T1_LoadFont(i);

      const int n = T1_GetNoFonts();
      for (int i = 0; i < n; i++)
	fontCache[String(T1_GetFontFileName(i))] = i;

      std::cerr << "initialized " << n << " T1 fonts" << std::endl;
    }
}

T1FontManager::~T1FontManager()
{
  const int res = T1_CloseLib();
  assert(res == 0);
  firstTime = true;
}

T1FontID
T1FontManager::getT1Font(const String& name) const
{
  T1FontCache::iterator p = fontCache.find(name);
  return (p != fontCache.end()) ? p->second : -1;
}
