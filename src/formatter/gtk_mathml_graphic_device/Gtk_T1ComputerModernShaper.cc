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
#include "T1FontManager.hh"
#include "Gtk_T1ComputerModernShaper.hh"
#include "Gtk_T1GlyphArea.hh"

Gtk_T1ComputerModernShaper::Gtk_T1ComputerModernShaper()
{ }

Gtk_T1ComputerModernShaper::~Gtk_T1ComputerModernShaper()
{ }

void
Gtk_T1ComputerModernShaper::setFontManager(const SmartPtr<T1FontManager>& fm)
{
  t1FontManager = fm;
}

AreaRef
Gtk_T1ComputerModernShaper::getGlyphArea(const SmartPtr<AreaFactory>& factory,
					 Char8 map, Char8 index,
					 const scaled& size) const
{

  assert(t1FontManager);
  T1FontID fontId = t1FontManager->getT1Font("cmr10.pfb");
  assert(fontId != -1);

  std::vector<AreaRef> c;
  c.reserve(256);
  for (int i = 0; i < 256; i++)
    c.push_back(Gtk_T1GlyphArea::create(fontId, size.toFloat(), i));

  return factory->horizontalArray(c);
}
