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
