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

#include "TFM_T1FontManager.hh"
#include "SVG_T1Font.hh"
#include "SVG_AreaFactory.hh"
#include "SVG_T1ComputerModernShaper.hh"
#include "SVG_T1GlyphArea.hh"

static const char* fontId[] = {
  NULL,
  "cmr10",
  "cmb10",
  "cmbxti10",
  "cmti10",
  "cmss10",
  "cmssi10",
  "cmssbx10",
  "cmtt10",
  "cmsy10",
  "cmbsy10",
  "cmmi10",
  "cmmib10",
  "cmex10"
};

SVG_T1ComputerModernShaper::SVG_T1ComputerModernShaper()
{ }

SVG_T1ComputerModernShaper::~SVG_T1ComputerModernShaper()
{ }

void
SVG_T1ComputerModernShaper::setFontManager(const SmartPtr<TFM_T1FontManager>& fm)
{
  assert(fm);
  t1FontManager = fm;
}

SmartPtr<SVG_T1Font>
SVG_T1ComputerModernShaper::getT1Font(ComputerModernShaper::FontNameId fontNameId,
				      const scaled& size) const
{
  assert(fontNameId >= 0 && fontNameId < sizeof(fontId) / sizeof(const char*));
  assert(fontId[fontNameId] != NULL);
  return t1FontManager->getT1Font(fontId[fontNameId], size);
}

AreaRef
SVG_T1ComputerModernShaper::getGlyphArea(const SmartPtr<AreaFactory>& factory,
					 ComputerModernShaper::FontNameId fontNameId, 
					 Char8 index, const scaled& size) const
{

  assert(t1FontManager);
  const SmartPtr<SVG_T1Font> font = getT1Font(fontNameId, size);
  assert(font);
  return SVG_T1GlyphArea::create(font, index);
}
