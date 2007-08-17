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
					  UChar8 index, int size) const
{ return SVG_TFMGlyphArea::create(getFont(fontNameId, designSize, size), index); }

bool
SVG_TFMComputerModernShaper::getGlyphData(const AreaRef& area, SmartPtr<TFMFont>& font, UChar8& index) const
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
