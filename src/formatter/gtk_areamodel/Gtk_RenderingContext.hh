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
public:
  Gtk_RenderingContext(void);
  virtual ~Gtk_RenderingContext();

  void setForegroundColor(const RGBColor&);
  void setForegroundColor(GdkColor&);
  void getForegroundColor(RGBColor& c) { c = foregroundColor; }
  void getForegroundColor(GdkColor& c) { c = gdk_foregroundColor; }

  void setSelectionColors(const RGBColor&, const RGBColor&);

  void setWidget(const GObjectPtr<GtkWidget>&);
  GObjectPtr<GtkWidget> getWidget(void) const { return gtk_widget; }
  void setDrawable(const GObjectPtr<GdkDrawable>&);
  GObjectPtr<GdkDrawable> getDrawable(void) const { return gdk_drawable; }
  GObjectPtr<GdkGC> getGC(void) const { return selection ? gdk_selected_gc : gdk_gc; }
  XftDraw* getXftDraw(void) const { return xft_draw; }
  const XftColor* getXftColor(void) const { return &xft_foregroundColor; }

  void update(void) const;
  void update(const Rectangle&) const;

  bool getSelection(void) const { return selection; }
  bool setSelection(bool b)
  {
    bool oldSelection = selection;
    selection = b;
    return oldSelection;
  }

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
  void releaseResources(void);

  RGBColor foregroundColor;
  bool selection;

  // GTK-specific fields
  GObjectPtr<GtkWidget> gtk_widget;

  // GDK-specific fields
  GObjectPtr<GdkDrawable> gdk_drawable;
  GObjectPtr<GdkGC> gdk_gc;
  GObjectPtr<GdkGC> gdk_selected_gc;
  GObjectPtr<GdkColormap> gdk_colormap;
  GdkColor gdk_foregroundColor;

  // Xft-specific fields
  XftColor xft_foregroundColor;
  XftDraw* xft_draw;
};

#endif // __Gtk_RenderingContext_hh__
