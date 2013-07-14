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

#ifndef __Gtk_RenderingContext_hh__
#define __Gtk_RenderingContext_hh__

#include <cairo/cairo.h>
#include <pango/pangocairo.h>

#include "Char.hh"
#include "SmartPtr.hh"
#include "GObjectPtr.hh"
#include "RGBColor.hh"
#include "Rectangle.hh"
#include "RenderingContext.hh"

class Gtk_RenderingContext : public RenderingContext
{
protected:
  enum ColorIndex { FOREGROUND_INDEX, BACKGROUND_INDEX, MAX_INDEX };

public:
  enum ColorStyle { NORMAL_STYLE, SELECTED_STYLE, MAX_STYLE };

  Gtk_RenderingContext(const SmartPtr<class AbstractLogger>&);
  virtual ~Gtk_RenderingContext();

  void setForegroundColor(const RGBColor& c) { data[getStyle()].setColor(FOREGROUND_INDEX, c); }
  void setBackgroundColor(const RGBColor& c) { data[getStyle()].setColor(BACKGROUND_INDEX, c); }

  RGBColor getForegroundColor(void) const { return data[getStyle()].getColor(FOREGROUND_INDEX); }
  RGBColor getBackgroundColor(void) const { return data[getStyle()].getColor(BACKGROUND_INDEX); }

  void setContext(cairo_t* cr) { cairo_context = cr; }
  cairo_t* getContext(void) const { return cairo_context; }

  void setStyle(ColorStyle s) { style = s; }
  ColorStyle getStyle(void) const { return style; }

  void fill(const scaled&, const scaled&, const BoundingBox&) const;
  void draw(const scaled&, const scaled&, PangoLayout*) const;
  void draw(const scaled&, const scaled&, PangoLayoutLine*) const;
  void draw(const scaled&, const scaled&, PangoFont*, PangoGlyphString*) const;

  static double toGtkPixels(const scaled& s)
  { return (s * (72.27 / 72.0)).toDouble(); }
  static double toPangoPixels(const scaled& s)
  { return (s * PANGO_SCALE * (72.27 / 72.0)).toDouble(); }
  static double toPangoPoints(const scaled& s)
  { return (s * PANGO_SCALE).toDouble(); }
  static scaled fromGtkPixels(double s)
  { return scaled(s * (72.0 / 72.27)); }
  static scaled fromPangoPixels(int s)
  { return scaled((s * (72.0 / 72.27)) / PANGO_SCALE); }

  static double toGtkX(const scaled& x)
  { return toGtkPixels(x); }
  static double toGtkY(const scaled& y)
  { return toGtkPixels(-y); }

  static scaled fromGtkX(double x)
  { return fromGtkPixels(x); }
  static scaled fromGtkY(double y)
  { return fromGtkPixels(-y); }

protected:
  struct ContextData
  {
    RGBColor color[MAX_INDEX];
    
    void setColor(ColorIndex index, const RGBColor& c)
    { color[index] = c; }

    RGBColor getColor(ColorIndex index) const
    { return color[index]; }
  };

  SmartPtr<class AbstractLogger> logger;

  ColorStyle style;
  ContextData data[MAX_STYLE];

  cairo_t* cairo_context;
};

#endif // __Gtk_RenderingContext_hh__
