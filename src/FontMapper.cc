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

#include <assert.h>
#include <string.h>

#include <gdk/gdk.h>

#include "FontMapper.hh"
#include "defaultFontMapper.hh"

static const char* fontNameTemplate = "-*-%s-%s-%c-*-*-*-%d-*-*-*-*-%s";

FontMapper::FontMapper()
{
  first     = NULL;
  lastFound = NULL;

  InitDefaultFontMapper(this);
}

FontMapper::~FontMapper()
{
  Reset();
}

void FontMapper::Reset()
{
  while (first != NULL) {
    const FontMap* next = first->next;
    delete first;
    first = next;
  }

  lastFound = NULL;
}

void FontMapper::Configure(const char* name,
			   Char start, Char end,
			   const char* defaultFamily,
			   const char* charSet,
			   CharMapping map)
{
  assert(start <= end);
  assert(map != NULL);

  FontMap* fMap  = new FontMap;
  fMap->name     = name;
  fMap->start 	 = start;
  fMap->end   	 = end;
  fMap->fontFamily = defaultFamily;
  fMap->charSet  = charSet;
  fMap->map      = map;
    
  fMap->next = first;
  first = fMap;
}

const Font* FontMapper::Map(const FontAttributes& fa, Char ch, CharMapping* map) const
{
  const FontMap* p = first;

  while (p != NULL && (ch < p->start || ch > p->end)) p = p->next;

  if (p != NULL) {
    lastFound = p;
    if (map != NULL) *map = p->map;
    return SearchNearFont(fa, p->fontFamily, p->charSet);
  } else {
    lastFound = NULL;
    return NULL;
  }
}

bool FontMapper::SameFont(Char ch) const
{
  return lastFound != NULL && ch >= lastFound->start && ch <= lastFound->end;
}

const Font* FontMapper::SearchNearFont(const FontAttributes& fa,
				       const char* family,
				       const char* charSet) const
{
  UnitValue size = fa.size;
  int ptSize = (size.GetUnitId() == UNIT_PT) ?
    static_cast<int>(size.GetValue() * 10) :
    static_cast<int>(sp2pt(size.ToScaledPoints()) * 10);
  int loSize = ptSize - 1;
  int hiSize = ptSize;

  const char* weight = (fa.weight == FONT_WEIGHT_NORMAL) ? "medium" : "bold";
  const char  style  = (fa.style == FONT_STYLE_NORMAL) ? 'r' : 'i';

  GdkFont* font = NULL;
  while (font == NULL && loSize > -1000 && hiSize < 1000) {
    font = SearchGdkFont(family != NULL ? family : fa.family, weight, style, hiSize, charSet);

    if (font == NULL) {
      loSize--;
      font = SearchGdkFont(family != NULL ? family : fa.family, weight, style, loSize, charSet);
      if (font == NULL) hiSize++;
    }      
  }

  Font* f = font != NULL ? new Font(font) : NULL;

  return f;
}

GdkFont* FontMapper::SearchGdkFont(const char* family, const char* weight,
				   char style, int size, const char* charSet) const
{
  GdkFont* font = NULL;
  static char font_name[128];

  if (charSet == NULL) charSet = "*-*";

  g_snprintf(font_name, 128, fontNameTemplate, family, weight, style, size, charSet);
  //printf("search font %s\n", font_name);
  font = gdk_font_load(font_name);
  if (font != NULL) return font;

  g_snprintf(font_name, 128, fontNameTemplate, family, "*", style, size, charSet);
  font = gdk_font_load(font_name);
  if (font != NULL) return font;

  g_snprintf(font_name, 128, fontNameTemplate, family, weight, '*', size, charSet);
  font = gdk_font_load(font_name);
  if (font != NULL) return font;

  g_snprintf(font_name, 128, fontNameTemplate, family, "*", '*', size, charSet);
  font = gdk_font_load(font_name);
  if (font != NULL) return font;

  g_snprintf(font_name, 128, fontNameTemplate, "*", "*", '*', size, charSet);
  font = gdk_font_load(font_name);
  if (font != NULL) return font;

  return NULL;
}
