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
#include <stdio.h>

#include <gdk/gdk.h>

#include "Gtk_Font.hh"
#include "Iterator.hh"
#include "MathEngine.hh"
#include "Gtk_FontManager.hh"

Gtk_FontManager::Gtk_FontManager()
{
}

Gtk_FontManager::~Gtk_FontManager()
{
}

const AFont*
Gtk_FontManager::SearchNativeFont(const FontAttributes& fa,
				  const ExtraFontAttributes* efa) const
{
  const char* foundry = NULL;
  const char* family = NULL;
  const char* weight = NULL;
  const char* slant = NULL;
  const char* width = NULL;
  const char* style = NULL;
  const char* pixels = NULL;
  const char* points = NULL;
  const char* hres = NULL;
  const char* vres = NULL;
  const char* spacing = NULL;
  const char* avgwidth = NULL;
  const char* registry = NULL;
  const char* encoding = NULL;

  if (efa != NULL) {
    const char* type = efa->GetProperty("type");
    if (type != NULL && strcmp(type, "x")) return NULL;

    foundry = efa->GetProperty("x-foundry");
    family = efa->GetProperty("x-family");
    weight = efa->GetProperty("x-weight");
    slant = efa->GetProperty("x-slant");
    width = efa->GetProperty("x-width");
    style = efa->GetProperty("x-style");
    pixels = efa->GetProperty("x-pixels");
    points = efa->GetProperty("x-points");
    hres = efa->GetProperty("x-hres");
    vres = efa->GetProperty("x-vres");
    spacing = efa->GetProperty("x-spacing");
    avgwidth = efa->GetProperty("x-avgwidth");
    registry = efa->GetProperty("x-registry");
    encoding = efa->GetProperty("x-encoding");
  }

  if (foundry == NULL) foundry = "*";
  if (family == NULL) family = fa.HasFamily() ? fa.family : "*";

  if (weight == NULL)
    weight = fa.HasWeight() ? ((fa.weight == FONT_WEIGHT_NORMAL) ? "medium" : "bold") : "*";

  if (slant == NULL)
    slant = fa.HasStyle() ? ((fa.style == FONT_STYLE_NORMAL) ? "r" : "i") : "*";

  if (width == NULL) width = "*";
  if (style == NULL) style = "*";
  if (pixels == NULL) pixels = "*";

  if (points == NULL) {
    if (fa.HasSize()) {
      int ptSize = (fa.size.GetUnitId() == UNIT_PT) ?
	roundFloat(fa.size.GetValue() * 10) :
	roundFloat(sp2pt(fa.size.ToScaledPoints()) * 10);

      // we round the size (in points) to be a multiple of five
      ptSize = (ptSize + 4) / 5;
      ptSize *= 5;

      static char size[32];
      
      g_snprintf(size, 31, "%d", ptSize);
      points = size;
      pixels = "*";
    } else
      points = "*";
  }

  if (hres == NULL) hres = "*";
  if (vres == NULL) vres = "*";
  if (spacing == NULL) spacing = "*";
  if (avgwidth == NULL) avgwidth = "*";
  if (registry == NULL) registry = "*";
  if (encoding == NULL) encoding = "*";

  static char fontName[128];

  g_snprintf(fontName, 127, "-%s-%s-%s-%s-%s-%s-%s-%s-%s-%s-%s-%s-%s-%s",
	     foundry, family, weight, slant, width, style, pixels, points,
	     hres, vres, spacing, avgwidth, registry, encoding);

  GdkFont* font = gdk_font_load(fontName);
  MathEngine::logger(LOG_DEBUG, "loading font: %s --> %p", fontName, font);

  if (font == NULL)
    MathEngine::logger(LOG_WARNING, "unable to find X font `%s'", fontName);

  AFont* f = (font != NULL) ? new Gtk_Font(font) : NULL;

  return f;
}

