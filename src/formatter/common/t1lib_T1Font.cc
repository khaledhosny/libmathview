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

#include <t1lib.h>
//#include <t1libx.h>
//#include <gdk/gdkx.h>

#include "t1lib_T1Font.hh"

t1lib_T1Font::~t1lib_T1Font()
{ }

scaled
t1lib_T1Font::getGlyphLeftEdge(Char8 index) const
{ return getScale() * T1_GetCharBBox(getFontId(), index).llx / 1000.0f; }

scaled
t1lib_T1Font::getGlyphRightEdge(Char8 index) const
{ return getScale() * T1_GetCharBBox(getFontId(), index).urx / 1000.0f; }

BoundingBox
t1lib_T1Font::getGlyphBoundingBox(Char8 index) const
{
  const float scale = getScale();
  const BBox charBox = T1_GetCharBBox(getFontId(), index);
  return BoundingBox(scale * T1_GetCharWidth(getFontId(), index) / 1000.0f,
		     scale * charBox.ury / 1000.0f,
		     scale * (-charBox.lly) / 1000.0f);
}
