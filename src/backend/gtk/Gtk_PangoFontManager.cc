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

#include <cassert>

#include <gdk/gdkx.h>

#include "Gtk_PangoFontManager.hh"

Gtk_PangoFontManager::Gtk_PangoFontManager()
{ }

Gtk_PangoFontManager::~Gtk_PangoFontManager()
{ }

String
Gtk_PangoFontManager::XLFD::toString() const
{
  static char buffer[128];
  sprintf(buffer, "-%s-%s-%s-%s-*--*-%d-75-75-*-*-%s",
	  vendor.c_str(), family.c_str(), weight.c_str(), slant.c_str(),
	  size, charset.c_str());
  return buffer;
}

PangoFontDescription*
Gtk_PangoFontManager::PangoFD::toPangoFontDescription() const
{
  PangoFontDescription* desc = pango_font_description_new();
  pango_font_description_set_family(desc, family.c_str());
  pango_font_description_set_style(desc, style);
  pango_font_description_set_weight(desc, weight);
  pango_font_description_set_size(desc, size);
  return desc;
}

PangoFont*
Gtk_PangoFontManager::getPangoFont(const XLFD& fd, PangoXSubfont& subfont) const
{
  const String xlfd = fd.toString();
  PangoFontCache::iterator p = fontCache.find(xlfd);
  if (p != fontCache.end())
    {
      subfont = p->second.subfont;
      return p->second.font;
    }
  else
    {
      PangoFont* font = createPangoFont(fd, xlfd, subfont);
      fontCache[xlfd] = CachedPangoFontData(font, subfont);
      return font;
    }
}

PangoFont*
Gtk_PangoFontManager::getPangoFont(const PangoFD& fd, PangoXSubfont& subfont) const
{
  PangoFontDescription* desc = fd.toPangoFontDescription();
  assert(desc);

  gchar* s_desc = pango_font_description_to_string(desc);
  assert(s_desc);

  String sDesc = s_desc;
  g_free(s_desc);

  PangoFontCache::iterator p = fontCache.find(sDesc);
  if (p != fontCache.end())
    {
      subfont = p->second.subfont;
      return p->second.font;
    }
  else
    {
      PangoFont* font = createPangoFont(desc, subfont);
      fontCache[sDesc] = CachedPangoFontData(font, subfont);
      return font;
    }
}

PangoFont*
Gtk_PangoFontManager::createPangoFont(PangoFontDescription* desc, PangoXSubfont& subfont) const
{
  assert(desc);
  Display* display = gdk_x11_get_default_xdisplay();

  PangoFont* font = pango_font_map_load_font(pango_x_font_map_for_display(display),
                                             pango_x_get_context(display),
                                             desc);

  char* charset = "adobe-fontspecific";
  const gboolean res = pango_x_find_first_subfont(font, &charset, 1, &subfont);
  assert(res);

  assert(font);
  return font;
}

PangoFont*
Gtk_PangoFontManager::createPangoFont(const XLFD& fd, const String& xlfd, PangoXSubfont& subfont) const
{
  // Note that we use the default values for the display
  // that is the value that was specified to the
  // X server on the command line. This will work on most cases
  PangoFont* font = pango_x_load_font(gdk_x11_get_default_xdisplay(), xlfd.c_str());
  assert(font);

  const char* charset = fd.charset.c_str();
  const gboolean res = pango_x_find_first_subfont(font, const_cast<char**>(&charset), 1, &subfont);
  assert(res);

  return font;
}

