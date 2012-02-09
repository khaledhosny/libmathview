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

#ifndef __Gtk_AreaFactory_hh__
#define __Gtk_AreaFactory_hh__

#include <pango/pango.h>

#include "AreaFactory.hh"
#include "Gtk_ColorArea.hh"
#include "Gtk_BackgroundArea.hh"
#include "Gtk_InkArea.hh"
#include "Gtk_PangoGlyphArea.hh"
#include "Gtk_PangoLayoutArea.hh"
#include "Gtk_PangoLayoutLineArea.hh"
#include "Gtk_WrapperArea.hh"

class Gtk_AreaFactory : public AreaFactory
{
protected:
  Gtk_AreaFactory(void) { }
  virtual ~Gtk_AreaFactory() { }

public:
  static SmartPtr<Gtk_AreaFactory> create(void)
  { return new Gtk_AreaFactory(); }

  // redefined methods

  virtual SmartPtr<ColorArea> color(const AreaRef& area, const RGBColor& color) const
  { return Gtk_ColorArea::create(area, color); }
  virtual SmartPtr<InkArea> ink(const AreaRef& area) const
  { return Gtk_InkArea::create(area); }
  virtual AreaRef background(const AreaRef& area, const RGBColor& color) const
  { return Gtk_BackgroundArea::create(area, color); }

  // new methods

  virtual SmartPtr<Gtk_PangoGlyphArea> pangoGlyph(PangoFont* f, PangoGlyphString* gs) const
  { return Gtk_PangoGlyphArea::create(f, gs); }
  virtual SmartPtr<Gtk_PangoLayoutArea> pangoLayout(PangoLayout* layout) const
  { return Gtk_PangoLayoutArea::create(layout); }
  virtual SmartPtr<Gtk_PangoLayoutArea> pangoLayoutLine(PangoLayout* layout) const
  { return Gtk_PangoLayoutLineArea::create(layout); }
  virtual SmartPtr<Gtk_WrapperArea> wrapper(const AreaRef& area, const BoundingBox& box, const SmartPtr<class Element>& el) const
  { return Gtk_WrapperArea::create(area, box, el); }
};

#endif // __Gtk_AreaFactory_hh__
