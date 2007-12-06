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

#ifndef __Gtk_XftGlyphArea_hh__
#define __Gtk_XftGlyphArea_hh__

#include <ft2build.h>
#include <X11/Xft/Xft.h>

#include "GlyphArea.hh"

class Gtk_XftGlyphArea : public GlyphArea
{
protected:
  Gtk_XftGlyphArea(XftFont*, FcChar8);
  virtual ~Gtk_XftGlyphArea();

public:
  static SmartPtr<Gtk_XftGlyphArea> create(XftFont* font, FcChar8 glyph)
  { return new Gtk_XftGlyphArea(font, glyph); }

  virtual BoundingBox box(void) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;

  XftFont* getFont(void) const { return font; }
  FcChar8 getGlyph(void) const { return glyph; }

private:
  void getXftExtents(XGlyphInfo&) const;

  XftFont* font;
  FcChar8 glyph;
};

#endif // __Gtk_XftGlyphArea_hh__
