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

#ifndef __Gtk_RenderingContext_hh__
#define __Gtk_RenderingContext_hh__

#include <gtk/gtk.h>
#include <gdk/gdk.h>

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

  Gtk_RenderingContext(void);
  virtual ~Gtk_RenderingContext();

  void setForegroundColor(const RGBColor& c) { setColor<FOREGROUND_INDEX, gdk_gc_set_foreground>(c); }
  void setBackgroundColor(const RGBColor& c) { setColor<BACKGROUND_INDEX, gdk_gc_set_background>(c); }
  void setForegroundColor(GdkColor& c) { setColor<FOREGROUND_INDEX, gdk_gc_set_foreground>(c); }
  void setBackgroundColor(GdkColor& c) { setColor<BACKGROUND_INDEX, gdk_gc_set_background>(c); }

  // the return value is passed as an argument so that
  // we can use overloading
  void getForegroundColor(RGBColor& c) { getColor<RGBColor, FOREGROUND_INDEX>(c); }
  void getBackgroundColor(RGBColor& c) { getColor<RGBColor, BACKGROUND_INDEX>(c); }
  void getForegroundColor(GdkColor& c) { getColor<GdkColor, FOREGROUND_INDEX>(c); }
  void getBackgroundColor(GdkColor& c) { getColor<GdkColor, BACKGROUND_INDEX>(c); }

  void setColorMap(const GObjectPtr<GdkColormap>& cm) { gdk_colormap = cm; }
  GObjectPtr<GdkColormap> getColorMap(void) const { return gdk_colormap; }
  void setDrawable(const GObjectPtr<GdkDrawable>&);
  GObjectPtr<GdkDrawable> getDrawable(void) const { return gdk_drawable; }
  GObjectPtr<GdkGC> getGC(void) const { return data[getStyle()].gdk_gc; }

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
  template <ColorIndex index, void (*set)(GdkGC*, const GdkColor*)>
  void setColor(const RGBColor& c)
  { data[getStyle()].setColor<index,set>(c, gdk_colormap); }

  template <ColorIndex index, void (*set)(GdkGC*, const GdkColor*)>
  void setColor(const GdkColor& c)
  { data[getStyle()].setColor<index,set>(c); }

  template <typename C, ColorIndex index>
  void getColor(C& c) const
  { data[getStyle()].getColor<index>(c); }

  struct ContextData
  {
    GObjectPtr<GdkGC> gdk_gc;
    RGBColor color[MAX_INDEX];
    GdkColor gdk_color[MAX_INDEX];
    
    template <ColorIndex index, void (*set)(GdkGC*, const GdkColor*)>
    void setColor(const RGBColor& c, const GObjectPtr<GdkColormap>& gdk_colormap)
    {
      if (c != color[index])
	{
	  color[index] = c;

	  gdk_color[index].red = c.red * (65535/255);
	  gdk_color[index].green = c.green * (65535/255);
	  gdk_color[index].blue = c.blue * (65535/255);
	  gdk_color[index].pixel = c.red * 65536 + c.green * 256 + c.blue;

	  assert(gdk_colormap);
	  gboolean ret = gdk_colormap_alloc_color(gdk_colormap, &gdk_color[index], FALSE, TRUE);
	  assert(ret == TRUE);

	  setColor<index,set>(gdk_color[index]);
	}
    }

    template <ColorIndex index>
    void getColor(RGBColor& c) const
    { c = color[index]; }

    template <ColorIndex index, void (*set)(GdkGC*, const GdkColor*)>
    void setColor(const GdkColor& c)
    { set(gdk_gc, &c); }

    template <ColorIndex index>
    void getColor(GdkColor& c) const
    { c = gdk_color[index]; }
  };

  void releaseResources(void);

  ColorStyle style;
  ContextData data[MAX_STYLE];

  // GDK-specific fields
  GObjectPtr<GdkDrawable> gdk_drawable;
  GObjectPtr<GdkColormap> gdk_colormap;
};

#endif // __Gtk_RenderingContext_hh__
