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

#ifndef __Gtk_AreaFactory_hh__
#define __Gtk_AreaFactory_hh__

#include <pango/pango.h>

#include "DOM.hh"
#include "AreaFactory.hh"
#include "Gtk_ColorArea.hh"
#include "Gtk_InkArea.hh"
#include "Gtk_PangoGlyphArea.hh"
#include "Gtk_PangoLayoutArea.hh"
#include "Gtk_XftGlyphArea.hh"

class Gtk_AreaFactory : public AreaFactory
{
protected:
  Gtk_AreaFactory(void) { }
  virtual ~Gtk_AreaFactory() { }

public:
  static SmartPtr<Gtk_AreaFactory> create(void)
  { return new Gtk_AreaFactory(); }

  // redefined methods

  virtual SmartPtr<ColorArea> createColorArea(const AreaRef& area, const RGBColor& color) const
  { return Gtk_ColorArea::create(area, color); }
  virtual SmartPtr<InkArea> createInkArea(const AreaRef& area) const
  { return Gtk_InkArea::create(area); }

  // new methods

  virtual SmartPtr<Gtk_PangoGlyphArea> createPangoGlyphArea(PangoFont* f, PangoGlyphString* gs)
  { return Gtk_PangoGlyphArea::create(f, gs); }
  virtual SmartPtr<Gtk_PangoLayoutArea> createPangoLayoutArea(PangoLayout* layout)
  { return Gtk_PangoLayoutArea::create(layout); }
  virtual SmartPtr<Gtk_XftGlyphArea> createXftGlyphArea(XftFont* font, FT_UInt glyph)
  { return Gtk_XftGlyphArea::create(font, glyph); }
};

#endif // __Gtk_AreaFactory_hh__
