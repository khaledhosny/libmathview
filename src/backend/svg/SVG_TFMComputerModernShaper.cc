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
#include "SVG_AreaFactory.hh"
#include "SVG_TFMComputerModernShaper.hh"
#include "SVG_TFMGlyphArea.hh"
#include "TFMFont.hh"

SVG_TFMComputerModernShaper::SVG_TFMComputerModernShaper(const SmartPtr<AbstractLogger>& l,
							 const SmartPtr<Configuration>& conf)
  : TFMComputerModernShaper(l, conf)
{ }

SVG_TFMComputerModernShaper::~SVG_TFMComputerModernShaper()
{ }

SmartPtr<SVG_TFMComputerModernShaper>
SVG_TFMComputerModernShaper::create(const SmartPtr<AbstractLogger>& l,
				    const SmartPtr<Configuration>& conf)
{ return new SVG_TFMComputerModernShaper(l, conf); }

AreaRef
SVG_TFMComputerModernShaper::getGlyphArea(ComputerModernFamily::FontNameId fontNameId,
					  ComputerModernFamily::FontSizeId designSize,
					  Char8 index, int size) const
{ return SVG_TFMGlyphArea::create(getFont(fontNameId, designSize, size), index); }

bool
SVG_TFMComputerModernShaper::getGlyphData(const AreaRef& area, SmartPtr<TFMFont>& font, Char8& index) const
{
  if (SmartPtr<const SVG_TFMGlyphArea> glyphArea = smart_cast<const SVG_TFMGlyphArea>(area))
    {
      font = glyphArea->getFont();
      index = glyphArea->getIndex();
      return true;
    }
  else
    return false;
}
