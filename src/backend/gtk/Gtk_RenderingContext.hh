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

#ifndef __Gtk_RenderingContext_hh__
#define __Gtk_RenderingContext_hh__

#include <gtk/gtk.h>
#include <gdk/gdk.h>

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
  void draw(const scaled&, const scaled&, const SmartPtr<class t1lib_T1Font>&, Char8) const;

  void setT1OpaqueMode(bool b) { t1_opaque_mode = b; }
  bool getT1OpaqueMode(void) const { return t1_opaque_mode; }
  void setT1AntiAliasedMode(bool b) { t1_aa_mode = b; }
  bool getT1AntiAliasedMode(void) const { return t1_aa_mode; }

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
  { data[getStyle()].setColor<index,set>(c, gdk_colormap); }

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
      color[index] = c;

      GdkColor gdk_c;
      gdk_c.red = c.red * (65535/255);
      gdk_c.green = c.green * (65535/255);
      gdk_c.blue = c.blue * (65535/255);
      gdk_c.pixel = c.red * 65536 + c.green * 256 + c.blue;
      
      setColor<index,set>(gdk_c, gdk_colormap);
    }

    template <ColorIndex index>
    void getColor(RGBColor& c) const
    { c = color[index]; }

    template <ColorIndex index, void (*set)(GdkGC*, const GdkColor*)>
    void setColor(const GdkColor& c, const GObjectPtr<GdkColormap>& gdk_colormap)
    { 
      gdk_color[index] = c;

      assert(gdk_colormap);
      const gboolean ret = gdk_colormap_alloc_color(gdk_colormap, &gdk_color[index], FALSE, TRUE);
      assert(ret == TRUE);
      
      set(gdk_gc, &gdk_color[index]);
    }

    template <ColorIndex index>
    void getColor(GdkColor& c) const
    { c = gdk_color[index]; }
  };

  void releaseResources(void);

  SmartPtr<class AbstractLogger> logger;

  ColorStyle style;
  ContextData data[MAX_STYLE];

  // GDK-specific fields
  GObjectPtr<GdkDrawable> gdk_drawable;
  GObjectPtr<GdkColormap> gdk_colormap;

  // t1lib-specific fields
  bool t1_opaque_mode;
  bool t1_aa_mode;
};

#endif // __Gtk_RenderingContext_hh__
