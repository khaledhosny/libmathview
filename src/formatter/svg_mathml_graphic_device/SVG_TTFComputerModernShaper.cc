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

#include "SVG_T1Font.hh"
#include "SVG_TTFComputerModernShaper.hh"
#include "SVG_TTF_T1GlyphArea.hh"

SVG_TTFComputerModernShaper::SVG_TTFComputerModernShaper()
{ }

SVG_TTFComputerModernShaper::~SVG_TTFComputerModernShaper()
{ }

AreaRef
SVG_TTFComputerModernShaper::getGlyphArea(const SmartPtr<AreaFactory>& factory,
					  ComputerModernShaper::FontNameId fontNameId, 
					  Char8 index, const scaled& size) const
{

  const SmartPtr<SVG_T1Font> font = getT1Font(fontNameId, size);
  assert(font);
  return SVG_TTF_T1GlyphArea::create(font, index, ComputerModernShaper::toTTFGlyphIndex(fontNameId, index));
}
