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

#ifndef __SVG_TTF_T1GlyphArea_hh__
#define __SVG_TTF_T1GlyphArea_hh__

#include "SVG_TFMGlyphArea.hh"

class SVG_TTF_TFMGlyphArea : public SVG_TFMGlyphArea
{
protected:
  SVG_TTF_TFMGlyphArea(const SmartPtr<class TFMFont>&, Char8, Char8);
  virtual ~SVG_TTF_TFMGlyphArea();

public:
  static SmartPtr<SVG_TTF_TFMGlyphArea> create(const SmartPtr<class TFMFont>&, Char8, Char8);

  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;

  Char8 getTTFIndex(void) const { return ttf_index; }

private:
  Char8 ttf_index;
};

#endif // __SVG_TTF_TFMGlyphArea_hh__
