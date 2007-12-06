// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include "Gtk_AreaFactory.hh"
#include "Gtk_XftFontManager.hh"
#include "Gtk_ComputerModernShaper.hh"

Gtk_ComputerModernShaper::Gtk_ComputerModernShaper()
{ }

Gtk_ComputerModernShaper::~Gtk_ComputerModernShaper()
{ }

void
Gtk_ComputerModernShaper::setFontManager(const SmartPtr<Gtk_XftFontManager>& fm)
{
  xftFontManager = fm;
}

#include <iostream>


AreaRef
Gtk_ComputerModernShaper::getGlyphArea(const SmartPtr<AreaFactory>& f,
				       ComputerModernShaper::FontMapId map, UChar8 index,
				       const scaled& size) const
{
  std::cerr << "requesting glyph " << (int) map << " " << (int) index << " " << index << std::endl;
  SmartPtr<Gtk_AreaFactory> factory = smart_cast<Gtk_AreaFactory>(f);
  assert(factory);

  const char* mapName = "cmr10";
  switch (map) {
  case CMR: mapName = "cmr10"; break;
  case CMM: mapName = "cmm10"; break;
  case CMS: mapName = "cms10"; break;
  }

  assert(xftFontManager);
  Gtk_XftFontManager::XLFD fd("*", mapName, "*", "*", round(size * 10).toInt(), "*");
  XftFont* font = xftFontManager->getXftFont(Gtk_XftFontManager::XLFD(fd));
  assert(font);

  std::vector<AreaRef> c;
  c.reserve(256);
  for (int i = 0; i < 256; i++)
    c.push_back(factory->xftGlyph(font, i));

  return factory->horizontalArray(c);
}
				       
