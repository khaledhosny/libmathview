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
