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
#include "Globals.hh"
#include "Gtk_FontManager.hh"

Gtk_FontManager::Gtk_FontManager()
{
}

Gtk_FontManager::~Gtk_FontManager()
{
}

bool
Gtk_FontManager::IsAvailable(const FontAttributes& fa,
			     const ExtraFontAttributes* efa) const
{
  if (efa != NULL) {
    const std::string type = efa->GetProperty("type");
    if (type != "x") return false;
  }

  return true;
}

const AFont*
Gtk_FontManager::SearchNativeFont(const FontAttributes& fa,
				  const ExtraFontAttributes* efa) const
{
  std::string foundry;
  std::string family;
  std::string weight;
  std::string slant;
  std::string width;
  std::string style;
  std::string pixels;
  std::string points;
  std::string hres;
  std::string vres;
  std::string spacing;
  std::string avgwidth;
  std::string registry;
  std::string encoding;

  if (efa != NULL) {
    const std::string type = efa->GetProperty("type");
    assert(type == "x");

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

  if (foundry == "") foundry = "*";
  if (family == "") family = fa.HasFamily() ? fa.family : "*";

  if (weight == "")
    weight = fa.HasWeight() ? ((fa.weight == FONT_WEIGHT_NORMAL) ? "medium" : "bold") : "*";

  if (slant == "")
    slant = fa.HasStyle() ? ((fa.style == FONT_STYLE_NORMAL) ? "r" : "i") : "*";

  if (width == "") width = "*";
  if (style == "") style = "*";
  if (pixels == "") pixels = "*";

  if (points == "") {
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

  if (hres == "") hres = "*";
  if (vres == "") vres = "*";
  if (spacing == "") spacing = "*";
  if (avgwidth == "") avgwidth = "*";
  if (registry == "") registry = "*";
  if (encoding == "") encoding = "*";

  std::string fontName = "-" + foundry + "-" + family + "-" + weight + "-" + slant +
    "-" + width + "-" + style + "-" + pixels + "-" + points + "-" + hres + "-" + vres +
    "-" + spacing + "-" + avgwidth + "-" + registry + "-" + encoding;

  GdkFont* font = gdk_font_load(fontName.c_str());
  Globals::logger(LOG_DEBUG, "loading font: %s --> %p", fontName.c_str(), font);

  if (font == NULL)
    Globals::logger(LOG_WARNING, "unable to find X font `%s'", fontName.c_str());

  AFont* f = (font != NULL) ? new Gtk_Font(font) : NULL;

  return f;
}

