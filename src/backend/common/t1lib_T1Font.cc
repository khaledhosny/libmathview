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

String
t1lib_T1Font::getFontFileName(void) const
{
  assert(fontId >= 0 && fontId < T1_GetNoFonts());
  return T1_GetFontFileName(fontId);
}

