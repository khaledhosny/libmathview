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

#include "T1Font.hh"
#include "Gtk_AreaFactory.hh"
#include "T1FontManager.hh"
#include "Gtk_T1ComputerModernShaper.hh"
#include "Gtk_T1GlyphArea.hh"

static const char* fontFile[] = {
  NULL, "cmr10.pfb", "cmb10.pfb", "cmbxti10.pfb", "cmti10.pfb", "cmss10.pfb",
  "cmssi10.pfb", "cmssbx10.pfb", "cmtt10.pfb", "cmsy10.pfb", "cmbsy10.pfb",
  "cmmi10.pfb", "cmmib10.pfb", "cmex10.pfb"
};

Gtk_T1ComputerModernShaper::Gtk_T1ComputerModernShaper()
{ }

Gtk_T1ComputerModernShaper::~Gtk_T1ComputerModernShaper()
{ }

void
Gtk_T1ComputerModernShaper::setFontManager(const SmartPtr<T1FontManager>& fm)
{
  assert(fm);
  t1FontManager = fm;
}

SmartPtr<T1Font>
Gtk_T1ComputerModernShaper::getT1Font(ComputerModernShaper::FontNameId fontNameId,
				      const scaled& size) const
{
  assert(fontNameId >= 0 && fontNameId < sizeof(fontFile) / sizeof(const char*));
  assert(fontFile[fontNameId] != NULL);
  return t1FontManager->getT1Font(fontFile[fontNameId], size);
}

AreaRef
Gtk_T1ComputerModernShaper::getGlyphArea(const SmartPtr<AreaFactory>& factory,
					 ComputerModernShaper::FontNameId fontNameId, 
					 Char8 index, const scaled& size) const
{

  assert(t1FontManager);
  const SmartPtr<T1Font> font = getT1Font(fontNameId, size);
  assert(font);

#if 0
  std::vector<AreaRef> c;
  c.reserve(256);
  for (int i = 0; i < 256; i++)
    c.push_back(Gtk_T1GlyphArea::create(font, i));

  return factory->horizontalArray(c);
#endif
  AreaRef res = Gtk_T1GlyphArea::create(font, index);
  return res;
}
