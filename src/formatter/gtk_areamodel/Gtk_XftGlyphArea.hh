// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef __Gtk_XftGlyphArea_hh__
#define __Gtk_XftGlyphArea_hh__

#include <X11/Xft/Xft.h>

#include "SimpleArea.hh"

class Gtk_XftGlyphArea : public SimpleArea
{
protected:
  Gtk_XftGlyphArea(XftFont* font, FT_UInt glyph);
  virtual ~Gtk_XftGlyphArea();

public:
  static SmartPtr<Gtk_XftGlyphArea> create(XftFont* font, FT_UInt glyph)
  { return new Gtk_XftGlyphArea(font, glyph); }

  virtual BoundingBox box(void) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual DOM::Element dump(const DOM::Document&) const;

  XftFont* getFont(void) const { return font; }
  FT_UInt getGlyph(void) const { return glyph; }

private:
  void getXftExtents(XGlyphInfo&) const;

  XftFont* font;
  FT_UInt glyph;
};

#endif // __Gtk_XftGlyphArea_hh__
