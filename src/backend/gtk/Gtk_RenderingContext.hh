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

#include <gtk/gtk.h>

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

  void setForegroundColor(const RGBColor& c) { setColor<FOREGROUND_INDEX>(c); }
  void setBackgroundColor(const RGBColor& c) { setColor<BACKGROUND_INDEX>(c); }
  void setForegroundColor(GdkColor& c) { setColor<FOREGROUND_INDEX>(c); }
  void setBackgroundColor(GdkColor& c) { setColor<BACKGROUND_INDEX>(c); }

  // the return value is passed as an argument so that
  // we can use overloading
  void getForegroundColor(RGBColor& c) const { getColor<RGBColor, FOREGROUND_INDEX>(c); }
  void getBackgroundColor(RGBColor& c) const { getColor<RGBColor, BACKGROUND_INDEX>(c); }
  void getForegroundColor(GdkColor& c) const { getColor<GdkColor, FOREGROUND_INDEX>(c); }
  void getBackgroundColor(GdkColor& c) const { getColor<GdkColor, BACKGROUND_INDEX>(c); }

  void setDrawable(const GObjectPtr<GdkDrawable>&);

  void setStyle(ColorStyle s) { style = s; }
  ColorStyle getStyle(void) const { return style; }

  void fill(const scaled&, const scaled&, const BoundingBox&) const;
  void draw(const scaled&, const scaled&, PangoLayout*) const;
  void draw(const scaled&, const scaled&, PangoLayoutLine*) const;
  void draw(const scaled&, const scaled&, PangoFont*, PangoGlyphString*) const;

  static int toGtkPixels(const scaled& s)
  { return round(s * (72.27 / 72.0)).toInt(); }
  static int toPangoPixels(const scaled& s)
  { return round(s * PANGO_SCALE * (72.27 / 72.0)).toInt(); }
  static int toPangoPoints(const scaled& s)
  { return round(s * PANGO_SCALE).toInt(); }
  static scaled fromGtkPixels(int s)
  { return scaled(s * (72.0 / 72.27)); }
  static scaled fromPangoPixels(int s)
  { return scaled((s * (72.0 / 72.27)) / PANGO_SCALE); }

  static int toGtkX(const scaled& x)
  { return toGtkPixels(x); }
  static int toGtkY(const scaled& y)
  { return toGtkPixels(-y); }

  static scaled fromGtkX(int x)
  { return fromGtkPixels(x); }
  static scaled fromGtkY(int y)
  { return fromGtkPixels(-y); }

protected:
  void prepare(const scaled& x, const scaled& y) const;

  template <ColorIndex index>
  void setColor(const RGBColor& c)
  { data[getStyle()].setColor<index>(c); }

  template <ColorIndex index>
  void setColor(const GdkColor& c)
  { data[getStyle()].setColor<index>(c); }

  template <typename C, ColorIndex index>
  void getColor(C& c) const
  { data[getStyle()].getColor<index>(c); }
  
  struct ContextData
  {
    RGBColor color[MAX_INDEX];
    GdkColor gdk_color[MAX_INDEX];
    
    template <ColorIndex index>
    void setColor(const RGBColor& c)
    {
      color[index] = c;

      GdkColor gdk_c;
      gdk_c.red = c.red * (65535/255);
      gdk_c.green = c.green * (65535/255);
      gdk_c.blue = c.blue * (65535/255);
      gdk_c.pixel = c.red * 65536 + c.green * 256 + c.blue;
      
      gdk_color[index] = gdk_c;
    }

    template <ColorIndex index>
    void getColor(RGBColor& c) const
    { c = color[index]; }

    template <ColorIndex index>
    void setColor(const GdkColor& c)
    { 
      gdk_color[index] = c;

      RGBColor rgb_c;
      rgb_c.red = c.red/65535. * 255;
      rgb_c.green = c.green/65535. * 255;
      rgb_c.blue = c.blue/65535. * 255;
      rgb_c.alpha = 255;

      color[index] = rgb_c;
    }

    template <ColorIndex index>
    void getColor(GdkColor& c) const
    { c = gdk_color[index]; }
  };

  void releaseResources(void);

  SmartPtr<class AbstractLogger> logger;

  ColorStyle style;
  ContextData data[MAX_STYLE];

  // Cairo
  cairo_t * cairo_context;
};

#endif // __Gtk_RenderingContext_hh__
