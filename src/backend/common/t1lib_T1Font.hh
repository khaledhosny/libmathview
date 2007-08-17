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

#ifndef __t1lib_T1Font_hh__
#define __t1lib_T1Font_hh__

#include "String.hh"
#include "T1Font.hh"
#include "SmartPtr.hh"
#include "BoundingBox.hh"

class GMV_MathView_EXPORT t1lib_T1Font : public T1Font
{
protected:
  t1lib_T1Font(int fid, const scaled& s) : T1Font(s), fontId(fid) { }
  virtual ~t1lib_T1Font();

public:
  static SmartPtr<t1lib_T1Font> create(int fid, const scaled& s)
  { return new t1lib_T1Font(fid, s); }

  int getFontId(void) const { return fontId; }
  String getFontFileName(void) const;

  virtual scaled getGlyphLeftEdge(Char8) const;
  virtual scaled getGlyphRightEdge(Char8) const;
  virtual BoundingBox getGlyphBoundingBox(Char8) const;

private:
  int fontId;
};

#endif // __t1lib_T1Font_hh__
