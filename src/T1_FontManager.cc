// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#ifdef HAVE_LIBT1

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "t1lib.h"

#include "T1_Font.hh"
#include "Globals.hh"
#include "T1_FontManager.hh"

bool T1_FontManager::firstTime = true;

T1_FontManager::T1_FontManager()
{
  if (firstTime) {
    void* res = T1_InitLib(LOGFILE | IGNORE_FONTDATABASE);
    if (res == NULL) {
      Globals::logger(LOG_ERROR, "could not initialize T1 library (aborted)");
      exit(-1);
    }
    assert(res != NULL);
    firstTime = false;
  }
}

T1_FontManager::~T1_FontManager()
{
  int res = T1_CloseLib();
  if (res != 0)
    Globals::logger(LOG_WARNING, "T1 lib could not uninitialize itself properly, please consult the log file");

  firstTime = true;
}

bool
T1_FontManager::IsAvailable(const FontAttributes& fa,
			    const ExtraFontAttributes* efa) const
{
  if (efa == NULL) return false;

  const std::string type = efa->GetProperty("type");
  if (type != "type1" && type != "ps") return false;

  const std::string fileName = efa->GetProperty("ps-file");
  if (fileName == "") return false;

  return true;
}

int
T1_FontManager::SearchT1FontId(const char* fileName) const
{
  int n = T1_Get_no_fonts();
  int i;
  for (i = 0; i < n && strcmp(fileName, T1_GetFontFileName(i)); i++) ;

  if (i == n) {
    Globals::logger(LOG_INFO, "adding font file `%s' to the font database", fileName);
    //i = T1_AddFont(const_cast<char*>(fileName));
    // this causes a memory leak, but T1 is really buggy
    i = T1_AddFont(strdup(fileName));
    if (i < 0) {
      Globals::logger(LOG_WARNING, "could not load Type1 font file `%s'", fileName);
      return -1;
    }
    Globals::logger(LOG_INFO, "loading font ID: %d", i);
    T1_LoadFont(i);
  } else {
    Globals::logger(LOG_INFO, "font file `%s' already loaded in the database", fileName);
  }

  return i;
}

int
T1_FontManager::SearchNativeFontAux(const FontAttributes& fa,
				    const ExtraFontAttributes* efa,
				    float& size) const
{
  assert(efa != NULL);

  const std::string type = efa->GetProperty("type");
  if (type == "") {
    Globals::logger(LOG_ERROR, "could not determine font type (check the font configuration file)");
    return -1;
  }

  if (type != "type1" && type != "ps") return -1;

  const std::string fileName = efa->GetProperty("ps-file");
  assert(fileName != "");

  int i = SearchT1FontId(fileName.c_str());
  
  size = 1.0;
  if (fa.HasSize())
    size = (fa.size.GetUnitId() == UNIT_PT) ? fa.size.GetValue() : sp2pt(fa.size.ToScaledPoints());

  return i;
}

const AFont*
T1_FontManager::SearchNativeFont(const FontAttributes& fa,
                                 const ExtraFontAttributes* efa) const
{
  float size;
  int i = SearchNativeFontAux(fa, efa, size);
  AFont* f = (i >= 0) ? new T1_Font(i, size) : NULL;
  return f;
}

#endif // HAVE_LIBT1
