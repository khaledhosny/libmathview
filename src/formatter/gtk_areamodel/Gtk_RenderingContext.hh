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

#include <ft2build.h>
#include <X11/Xft/Xft.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "GObjectPtr.hh"
#include "RGBColor.hh"
#include "Rectangle.hh"
#include "RenderingContext.hh"

class Gtk_RenderingContext : public RenderingContext
{
protected:
  enum ColorIndex { FOREGROUND, BACKGROUND };

public:
  Gtk_RenderingContext(void);
  virtual ~Gtk_RenderingContext();

  void setForegroundColor(const RGBColor& c, int i = 0) { setColor<FOREGROUND, gdk_gc_set_foreground>(c, i); }
  void setBackgroundColor(const RGBColor& c, int i = 0) { setColor<BACKGROUND, gdk_gc_set_background>(c, i); }
  void setForegroundColor(GdkColor& c, int i = 0) { setColor<FOREGROUND, gdk_gc_set_foreground>(c, i); }
  void setBackgroundColor(GdkColor& c, int i = 0) { setColor<BACKGROUND, gdk_gc_set_background>(c, i); }

  // the return value is passed as an argument so that
  // we can use overloading
  void getForegroundColor(RGBColor& c, int i = 0) { getColor<RGBColor, FOREGROUND>(c, i); }
  void getBackgroundColor(RGBColor& c, int i = 0) { getColor<RGBColor, BACKGROUND>(c, i); }
  void getForegroundColor(GdkColor& c, int i = 0) { getColor<GdkColor, FOREGROUND>(c, i); }
  void getBackgroundColor(GdkColor& c, int i = 0) { getColor<GdkColor, BACKGROUND>(c, i); }

  void setWidget(const GObjectPtr<GtkWidget>&);
  GObjectPtr<GtkWidget> getWidget(void) const { return gtk_widget; }
  void setDrawable(const GObjectPtr<GdkDrawable>&);
  GObjectPtr<GdkDrawable> getDrawable(void) const { return gdk_drawable; }
  GObjectPtr<GdkGC> getGC(void) const
  {
    assert(selection >= 0 && selection <= MAX_SELECTION_LEVEL);
    return data[selection].gdk_gc;
  }

  XftDraw* getXftDraw(void) const { return xft_draw; }

  const XftColor* getXftForegroundColor(int i = 0) const { return getXftColor<FOREGROUND>(i); }
  const XftColor* getXftBackgroundColor(int i = 0) const { return getXftColor<BACKGROUND>(i); }

  void update(void) const;
  void update(const Rectangle&) const;

  int getSelection(void) const { return selection; }
  int setSelection(int s)
  {
    int oldSelection = selection;
    selection = s;
    return oldSelection;
  }
  int addSelection(int ds) { return setSelection(selection + ds); }
  int select(void) { return selection++; }
  int unselect(void) { return selection--; }

  static int toGtkPixels(const scaled& s)
  { return static_cast<int>(s.toFloat() * (72.27 / 72.0)); }
  static int toXftPixels(const scaled& s)
  { return toGtkPixels(s); }
  static int toPangoPixels(const scaled& s)
  { return static_cast<int>(s.toFloat() * PANGO_SCALE * (72.27 / 72.0)); }
  static scaled fromGtkPixels(int s)
  { return scaled(s * (72.0 / 72.27)); }
  static scaled fromXftPixels(int s)
  { return fromGtkPixels(s); }
  static scaled fromPangoPixels(int s)
  { return scaled((s * (72.0 / 72.27)) / PANGO_SCALE); }

  static int toGtkX(const scaled& x)
  { return toGtkPixels(x); }
  static int toGtkY(const scaled& y)
  { return -toGtkPixels(y); }
  static int toXftX(const scaled& x)
  { return toGtkX(x); }
  static int toXftY(const scaled& y)
  { return toGtkY(y); }

  static scaled fromGtkX(int x)
  { return fromGtkPixels(x); }
  static scaled fromGtkY(int y)
  { return fromGtkPixels(-y); }
  static scaled fromXftX(int x)
  { return fromGtkX(x); }
  static scaled fromXftY(int y)
  { return fromGtkY(y); }

protected:
  enum { MAX_SELECTION_LEVEL = 1 };

  template <ColorIndex index, void (*set)(GdkGC*, GdkColor*)>
  void setColor(const RGBColor& c, int i)
  {
    assert(i >= 0 && i <= MAX_SELECTION_LEVEL);
    data[i].setColor<index,set>(c, gdk_colormap);
  }

  template <ColorIndex index, void (*set)(GdkGC*, GdkColor*)>
  void setColor(GdkColor& c, int i)
  {
    assert(i >= 0 && i <= MAX_SELECTION_LEVEL);
    data[i].setColor<index,set>(c);
  }

  template <typename C, ColorIndex index>
  void getColor(C& c, int i) const
  {
    assert(i >= 0 && i <= MAX_SELECTION_LEVEL);
    data[i].getColor<index>(c);
  }

  template <ColorIndex index>
  const XftColor* getXftColor(int i) const
  {
    assert(i >= 0 && i <= MAX_SELECTION_LEVEL);
    return data[i].getXftColor<index>();
  }

  struct ContextData
  {
    GObjectPtr<GdkGC> gdk_gc;
    RGBColor color[2];
    GdkColor gdk_color[2];
    XftColor xft_color[2];
    
    template <ColorIndex index, void (*set)(GdkGC*, GdkColor*)>
    void setColor(const RGBColor& c, const GObjectPtr<GdkColormap>& gdk_colormap)
    {
      if (c.red != color[index].red ||
	  c.green != color[index].green ||
	  c.blue != color[index].blue)
	{
	  color[index] = c;

	  gdk_color[index].red = c.red << 8;
	  gdk_color[index].green = c.green << 8;
	  gdk_color[index].blue = c.blue << 8;
	  gdk_color[index].pixel = 0;

	  assert(gdk_colormap);
	  gboolean ret = gdk_colormap_alloc_color(gdk_colormap, &gdk_color[index], FALSE, TRUE);
	  assert(ret == TRUE);

	  setColor<index,set>(gdk_color[index]);
	}
    }

    template <ColorIndex index>
    void getColor(RGBColor& c) const
    { c = color[index]; }

    // The only reason why the argument is not const is that
    // the GDK function does not respect const-ness :-(
    template <ColorIndex index, void (*set)(GdkGC*, GdkColor*)>
    void setColor(/* const */ GdkColor& c)
    {
      // Set the color in the GDK GC
      set(gdk_gc, &c);

      // Set the color for Xft, note that we reuse the same pixel field
      // so that we don't need to reallocate the color
      xft_color[index].color.red = c.red;
      xft_color[index].color.green = c.green;
      xft_color[index].color.blue = c.blue;
      xft_color[index].color.alpha = 0xffff;
      xft_color[index].pixel = c.pixel;
    }

    template <ColorIndex index>
    void getColor(GdkColor& c) const
    { c = gdk_color[index]; }

    template <ColorIndex index>
    const XftColor* getXftColor(void) const
    { return &xft_color[index]; }
  };

  void releaseResources(void);

  int selection;
  ContextData data[1 + MAX_SELECTION_LEVEL];

  // GTK-specific fields
  GObjectPtr<GtkWidget> gtk_widget;

  // GDK-specific fields
  GObjectPtr<GdkDrawable> gdk_drawable;
  GObjectPtr<GdkColormap> gdk_colormap;

  // Xft-specific fields
  XftDraw* xft_draw;
};

#endif // __Gtk_RenderingContext_hh__
