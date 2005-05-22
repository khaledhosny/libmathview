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

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "SVG_TTF_TFMComputerModernShaper.hh"
#include "SVG_TTF_TFMGlyphArea.hh"
#include "TFMFont.hh"

SVG_TTF_TFMComputerModernShaper::SVG_TTF_TFMComputerModernShaper(const SmartPtr<AbstractLogger>& l,
								 const SmartPtr<Configuration>& conf)
  : SVG_TFMComputerModernShaper(l, conf)
{
  setPostShapingMode(conf->getString(l, "svg-backend/ttf-computer-modern-shaper/post-shaping", "never"));
}

SVG_TTF_TFMComputerModernShaper::~SVG_TTF_TFMComputerModernShaper()
{ }

SmartPtr<SVG_TTF_TFMComputerModernShaper>
SVG_TTF_TFMComputerModernShaper::create(const SmartPtr<AbstractLogger>& l,
					const SmartPtr<Configuration>& conf)
{ return new SVG_TTF_TFMComputerModernShaper(l, conf); }

AreaRef
SVG_TTF_TFMComputerModernShaper::getGlyphArea(FontNameId fontNameId, FontSizeId designSize, 
					      Char8 index, int size) const
{ return SVG_TTF_TFMGlyphArea::create(getFont(fontNameId, designSize, size), index,
				      toTTFGlyphIndex(encIdOfFontNameId(fontNameId), index)); }
