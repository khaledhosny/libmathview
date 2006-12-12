// Copyright (C) 2000-2006, Luca Padovani <padovani@sti.uniurb.it>.
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
// <padovani@sti.uniurb.it>

#include <config.h>

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "PS_AreaFactory.hh"
#include "PS_TFMComputerModernShaper.hh"
#include "PS_TFMGlyphArea.hh"
#include "TFMFont.hh"

PS_TFMComputerModernShaper::PS_TFMComputerModernShaper(const SmartPtr<AbstractLogger>& l,
						       const SmartPtr<Configuration>& conf)
  : TFMComputerModernShaper(l, conf)
{ }

PS_TFMComputerModernShaper::~PS_TFMComputerModernShaper()
{ }

SmartPtr<PS_TFMComputerModernShaper>
PS_TFMComputerModernShaper::create(const SmartPtr<AbstractLogger>& l,
				   const SmartPtr<Configuration>& conf)
{ return new PS_TFMComputerModernShaper(l, conf); }

AreaRef
PS_TFMComputerModernShaper::getGlyphArea(ComputerModernFamily::FontNameId fontNameId,
					 ComputerModernFamily::FontSizeId designSize,
					 UChar8 index, int size) const
{ return PS_TFMGlyphArea::create(getFont(fontNameId, designSize, size), index); }

bool
PS_TFMComputerModernShaper::getGlyphData(const AreaRef& area, SmartPtr<TFMFont>& font, UChar8& index) const
{
  if (SmartPtr<const PS_TFMGlyphArea> glyphArea = smart_cast<const PS_TFMGlyphArea>(area))
    {
      font = glyphArea->getFont();
      index = glyphArea->getIndex();
      return true;
    }
  else
    return false;
}
