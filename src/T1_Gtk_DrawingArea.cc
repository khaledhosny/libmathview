// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#ifdef HAVE_LIBT1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <t1lib.h>
#include <t1libx.h>

#include <gdk/gdkx.h>

#include "T1_Font.hh"
#include "Globals.hh"
#include "T1_Gtk_DrawingArea.hh"
#include "Gtk_GraphicsContext.hh"
#include "scaledConv.hh"

bool T1_Gtk_DrawingArea::firstTime = true;

T1_Gtk_DrawingArea::T1_Gtk_DrawingArea(const GraphicsContextValues& values, const scaled& xm, const scaled& ym,
				       GtkWidget* widget, RGBValue f, RGBValue b) :
  Gtk_DrawingArea(values, xm, ym, widget, f, b)
{
  antiAliasing = false;
  mode = T1_OPAQUE;
}

T1_Gtk_DrawingArea::~T1_Gtk_DrawingArea()
{
}

void
T1_Gtk_DrawingArea::Realize()
{
  assert(gtk_widget != NULL);

  if (firstTime) {
    if (T1_QueryX11Support() == 0) {
      Globals::logger(LOG_ERROR, "T1 library has no X11 support (aborted)");
      exit(-1);
    }

    Display* xdisplay = GDK_DRAWABLE_XDISPLAY(gtk_widget->window);
    assert(xdisplay != NULL);
    GdkColormap* colormap = gtk_widget_get_colormap(gtk_widget);
    assert(colormap != NULL);
    Colormap xcolormap = GDK_COLORMAP_XCOLORMAP(colormap);
    GdkVisual* visual = gdk_colormap_get_visual(colormap);
    assert(visual != NULL);
    Visual* xvisual = GDK_VISUAL_XVISUAL(visual);
    assert(xvisual != NULL);

    T1_AASetBitsPerPixel(visual->depth);
    Globals::logger(LOG_DEBUG, "X11 depth: %d", visual->depth);
    Globals::logger(LOG_DEBUG, "X11 AAGetLevel() --> %d", T1_AAGetLevel());
    Globals::logger(LOG_DEBUG, "X11 AAGetBitsPerPixel() --> %d", T1_AAGetBitsPerPixel());
    T1_SetX11Params(xdisplay, xvisual, visual->depth, xcolormap);

    firstTime = false;
  }
}

void
T1_Gtk_DrawingArea::DrawChar(const GraphicsContext* gc, const AFont* font,
			     const scaled& x, const scaled& y, char ch) const
{
  assert(gc != NULL);
  assert(font != NULL);
  const Gtk_GraphicsContext* gtk_gc = TO_GTK_GRAPHICS_CONTEXT(gc);
  assert(gtk_gc != NULL);
  const T1_Font* t1_font = TO_T1_FONT(font);

  if (t1_font == NULL) {
    // maybe it's a Gtk font
    Gtk_DrawingArea::DrawChar(gc, font, x, y, ch);
    return;
  }

  GC xgc = GDK_GC_XGC(gtk_gc->GetNativeGraphicsContext());
  XID xdrawable = GDK_DRAWABLE_XID(gdk_pixmap);

  if (antiAliasing)
    T1_AASetCharX(xdrawable, xgc, mode, sp2ipx(x - x0), sp2ipx(y - y0),
		  t1_font->GetNativeFontId(), ch, t1_font->GetScale(),
		  NULL);
  else
    T1_SetCharX(xdrawable, xgc, mode, sp2ipx(x - x0), sp2ipx(y - y0),
		t1_font->GetNativeFontId(), ch, t1_font->GetScale(),
		NULL);
}

void
T1_Gtk_DrawingArea::DrawString(const GraphicsContext* gc, const AFont* font,
			       const scaled& x, const scaled& y,
			       const char* text,
			       unsigned len) const
{
  assert(gc != NULL);
  assert(font != NULL);

  const Gtk_GraphicsContext* gtk_gc = TO_GTK_GRAPHICS_CONTEXT(gc);
  assert(gtk_gc != NULL);
  const T1_Font* t1_font = TO_T1_FONT(font);

  if (t1_font == NULL) {
    // maybe it's a Gtk font
    Gtk_DrawingArea::DrawString(gc, font, x, y, text, len);
    return;
  }

  GC xgc = GDK_GC_XGC(gtk_gc->GetNativeGraphicsContext());
  XID xdrawable = GDK_DRAWABLE_XID(gdk_pixmap);

  if (antiAliasing)
    T1_AASetStringX(xdrawable, xgc, mode, sp2ipx(x - x0), sp2ipx(y - y0),
		    t1_font->GetNativeFontId(), const_cast<char*>(text), len, 0,
		    0, t1_font->GetScale(), NULL);
  else
    T1_SetStringX(xdrawable, xgc, mode, sp2ipx(x - x0), sp2ipx(y - y0),
		  t1_font->GetNativeFontId(), const_cast<char*>(text), len, 0,
		  0, t1_font->GetScale(), NULL);
}

void
T1_Gtk_DrawingArea::SetAntiAliasing(bool aa)
{
  antiAliasing = aa;
}

bool
T1_Gtk_DrawingArea::GetTransparency() const
{
  return mode == T1_TRANSPARENT;
}

void
T1_Gtk_DrawingArea::SetTransparency(bool b)
{
  if (b) mode = T1_TRANSPARENT;
  else mode = T1_OPAQUE;
}

#endif // HAVE_LIBT1
