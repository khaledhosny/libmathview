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

#ifndef __GlyphSpec_hh__
#define __GlyphSpec_hh__

class GlyphSpec
{
public:
  GlyphSpec(void) : shaperId(0), fontId(0), glyphId(0) { }
  GlyphSpec(unsigned si, unsigned fi, unsigned gi) : shaperId(si), fontId(fi), glyphId(gi) { }

  unsigned getGlyphId(void) const { return glyphId; }
  unsigned getShaperId(void) const { return shaperId; }
  unsigned getFontId(void) const { return fontId; }

private:
  unsigned shaperId : 8; // this can be lowered to match ShaperManager::MAX_SHAPERS
  unsigned fontId : 8;
  unsigned glyphId : 16;
};

#endif // __GlyphSpec_hh__
