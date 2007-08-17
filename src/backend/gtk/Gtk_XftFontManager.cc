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

#include "Gtk_XftFontManager.hh"

Gtk_XftFontManager::Gtk_XftFontManager()
{ }

Gtk_XftFontManager::~Gtk_XftFontManager()
{
  // FIXME: free the fonts?
}

String
Gtk_XftFontManager::XLFD::toString() const
{
  static char buffer[128];
  sprintf(buffer, "-%s-%s-%s-%s-*-*-*-%d-100-100-*-*-%s",
	  vendor.c_str(), family.c_str(), weight.c_str(), slant.c_str(),
	  static_cast<int>(size), charset.c_str());
  return buffer;
}

XftFont*
Gtk_XftFontManager::getXftFont(const XLFD& fd) const
{
  const String xlfd = fd.toString();
  XftFontCache::iterator p = fontCache.find(xlfd);
  if (p != fontCache.end())
    return p->second;
  else
    {
      XftFont* font = createXftFont(fd);
      fontCache[xlfd] = font;
      return font;
    }
}

#include <iostream>

XftFont*
Gtk_XftFontManager::createXftFont(const XLFD& fd) const
{
  // Note that we use the default values for the display
  // that is the value that was specified to the
  // X server on the command line. This will work on most cases
#if 0
  XftFont* font = XftFontOpenXlfd(GDK_DISPLAY(),
				  gdk_x11_get_default_screen(),
				  fd.toString().c_str());
#if 0
  std::cerr << "creating xft font " << fd.toString() << " " << font << std::endl;
  FcPatternPrint(font->pattern);
#endif
#else
  XftFont* font = XftFontOpen(GDK_DISPLAY(),
			      gdk_x11_get_default_screen(),
			      XFT_FAMILY, XftTypeString, fd.family.c_str(),
#if 0
			      XFT_SLANT, XftTypeInteger, XFT_SLANT_ROMAN,
			      XFT_WEIGHT, XftTypeInteger, XFT_WEIGHT_MEDIUM,
#endif
			      XFT_SIZE, XftTypeDouble, fd.size,
			      NULL);
  FcPatternPrint(font->pattern);
#endif			      

  assert(font);
  return font;
}

