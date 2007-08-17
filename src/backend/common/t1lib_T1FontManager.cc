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

#include <t1lib.h>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "t1lib_T1Font.hh"
#include "t1lib_T1FontManager.hh"

bool t1lib_T1FontManager::firstTime = true;

// #include <iostream>

t1lib_T1FontManager::t1lib_T1FontManager(const SmartPtr<AbstractLogger>& logger,
					 const SmartPtr<Configuration>& configuration)
{
  if (firstTime)
    {
      if (getenv("T1LIB_CONFIG") == NULL)
	{
	  logger->out(LOG_WARNING, "T1LIB_CONFIG environment variable not set");

	  String path = configuration->getString(logger, "default/t1lib/configuration", "");
	  setenv("T1LIB_CONFIG", path.c_str(), 1);
	}

      const void* res = T1_InitLib(LOGFILE | IGNORE_FONTDATABASE);
      assert(res != 0);
      firstTime = false;
      T1_SetLogLevel(T1LOG_DEBUG);
    }
}

t1lib_T1FontManager::~t1lib_T1FontManager()
{
  const int res = T1_CloseLib();
  assert(res == 0);
  firstTime = true;

  // should free the structures
}

SmartPtr<t1lib_T1FontManager>
t1lib_T1FontManager::create(const SmartPtr<AbstractLogger>& logger,
			    const SmartPtr<Configuration>& configuration)
{ return new t1lib_T1FontManager(logger, configuration); }

int
t1lib_T1FontManager::loadFont(const String& name) const
{
  const int n = T1_GetNoFonts();
  for (int i = 0; i < n; i++)
    if (name == T1_GetFontFileName(i))
      return true; // font already in the database
  const int fontId = T1_AddFont(const_cast<char*>(name.c_str())); // DANGER, const cast
  if (fontId >= 0) T1_LoadFont(fontId);
//   std::cerr << "loading font " << name << " => " << fontId << std::endl;
  return fontId;
}

int
t1lib_T1FontManager::getFontId(const String& name) const
{
  const int n = T1_GetNoFonts();
  for (int i = 0; i < n; i++)
    if (name == T1_GetFontFileName(i))
      return i;
  return loadFont(name);
}

SmartPtr<t1lib_T1Font>
t1lib_T1FontManager::createT1Font(const String& name, const scaled& size) const
{
  const int fontId = getFontId(name);
  if (fontId >= 0)
    return t1lib_T1Font::create(fontId, size);
  else
    return 0;
}

SmartPtr<t1lib_T1Font>
t1lib_T1FontManager::getT1Font(const String& name, const scaled& size) const
{
  const CachedT1FontKey key(name, size);
  T1FontCache::iterator p = fontCache.find(key);
  if (p != fontCache.end())
    return p->second;
  else if (const SmartPtr<t1lib_T1Font> font = createT1Font(name, size))
    {
      fontCache[key] = font;
      return font;
    }
  else
    return 0;
}
