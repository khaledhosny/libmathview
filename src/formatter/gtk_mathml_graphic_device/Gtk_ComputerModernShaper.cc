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
				       Char8 map, Char8 index,
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
				       
