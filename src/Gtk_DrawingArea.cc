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
#include <assert.h>
#include <stddef.h>
#include <gtk/gtk.h>

#include "Gtk_Font.hh"
#include "UnitValue.hh"
#include "Gtk_DrawingArea.hh"
#include "Gtk_GraphicsContext.hh"

Gtk_DrawingArea::Gtk_DrawingArea(const GraphicsContextValues& v, scaled xm, scaled ym, GtkWidget* widget,
				 RGBValue f, RGBValue b) :
  DrawingArea(v, xm, ym, f, b)
{
  assert(widget != NULL);
  gtk_widget = widget;

  width = px2sp(gtk_widget->allocation.width);
  height = px2sp(gtk_widget->allocation.height);

  x0 = y0 = 0;

  gdk_pixmap = NULL;
}

Gtk_DrawingArea::~Gtk_DrawingArea()
{
}

const GraphicsContext*
Gtk_DrawingArea::GetGC(const GraphicsContextValues& values) const
{
  assert(gtk_widget != NULL);

  GdkVisual* gdk_visual = gtk_widget_get_visual(gtk_widget);
  GdkColormap* gdk_colormap = gtk_widget_get_colormap(gtk_widget);

  assert(gdk_visual != NULL);
  assert(gdk_colormap != NULL);

  GdkGCValues gdk_values;
  unsigned gdk_mask = 0;

  gdk_values.foreground.red = GETRED(values.foreground) * 256;
  gdk_values.foreground.green = GETGREEN(values.foreground) * 256;
  gdk_values.foreground.blue = GETBLUE(values.foreground) * 256;    
  gdk_colormap_alloc_color(gdk_colormap, &gdk_values.foreground, FALSE, TRUE);
  gdk_mask |= GDK_GC_FOREGROUND;

  gdk_values.background.red = GETRED(values.background) * 256;
  gdk_values.background.green = GETGREEN(values.background) * 256;
  gdk_values.background.blue = GETBLUE(values.background) * 256;
  gdk_colormap_alloc_color(gdk_colormap, &gdk_values.background, FALSE, TRUE);
  gdk_mask |= GDK_GC_BACKGROUND;

  gdk_values.line_width = sp2ipx(values.lineWidth);
  gdk_mask |= GDK_GC_LINE_WIDTH;

  gdk_values.line_style = (values.lineStyle == LINE_STYLE_SOLID) ? GDK_LINE_SOLID : GDK_LINE_ON_OFF_DASH;
  gdk_mask |= GDK_GC_LINE_STYLE;

  GdkGC* gdk_gc = gtk_gc_get(gdk_visual->depth, gdk_colormap, &gdk_values, (GdkGCValuesMask) gdk_mask);
  assert(gdk_gc != NULL);

  return new Gtk_GraphicsContext(values, gdk_gc);
}

void
Gtk_DrawingArea::Realize()
{
  // actually, nothing to do
}

void
Gtk_DrawingArea::SetPixmap(GdkPixmap* pixmap)
{
  gdk_pixmap = pixmap;
}

void
Gtk_DrawingArea::SetTop(scaled x, scaled y)
{
  x0 = x;
  y0 = y;
}

void
Gtk_DrawingArea::SetTopX(scaled x)
{
  x0 = x;
}

void
Gtk_DrawingArea::SetTopY(scaled y)
{
  y0 = y;
}

void
Gtk_DrawingArea::DrawLine(const GraphicsContext* gc,
			  scaled x1, scaled y1,
			  scaled x2, scaled y2) const
{
  const Gtk_GraphicsContext* gtk_gc = TO_GTK_GRAPHICS_CONTEXT(gc);
  assert(gtk_gc != NULL);
  gdk_draw_line(gdk_pixmap,
		gtk_gc->GetNativeGraphicsContext(),
		sp2ipx(x1 - x0), sp2ipx(y1 - y0), sp2ipx(x2 - x0), sp2ipx(y2 - y0));
  MoveTo(x2, y2);
}

void
Gtk_DrawingArea::DrawRectangle(const GraphicsContext* gc,
			       scaled x, scaled y, scaled width, scaled height) const
{
  const Gtk_GraphicsContext* gtk_gc = TO_GTK_GRAPHICS_CONTEXT(gc);
  assert(gtk_gc != NULL);
  gdk_draw_rectangle(gdk_pixmap,
		     gtk_gc->GetNativeGraphicsContext(),
		     FALSE,
		     sp2ipx(x - x0), sp2ipx(y - y0), sp2ipx(width), sp2ipx(height));
}

void
Gtk_DrawingArea::DrawChar(const GraphicsContext* gc, const AFont* font,
			  scaled x, scaled y, char ch) const
{
  const Gtk_GraphicsContext* gtk_gc = TO_GTK_GRAPHICS_CONTEXT(gc);
  const Gtk_Font* gtk_font = TO_GTK_FONT(font);
  assert(gtk_gc != NULL);
  assert(gtk_font != NULL);

  GdkWChar wc = ch;
  gdk_draw_text_wc(gdk_pixmap,
		   gtk_font->GetNativeFont(),
		   gtk_gc->GetNativeGraphicsContext(),
		   sp2ipx(x - x0), sp2ipx(y - y0),
		   &wc, 1);  
}

void
Gtk_DrawingArea::DrawString(const GraphicsContext* gc, const AFont* font,
			    scaled x, scaled y,
			    const char* text,
			    unsigned len) const
{
  const Gtk_GraphicsContext* gtk_gc = TO_GTK_GRAPHICS_CONTEXT(gc);
  const Gtk_Font* gtk_font = TO_GTK_FONT(font);
  assert(gtk_gc != NULL);
  assert(gtk_font != NULL);
  gdk_draw_text(gdk_pixmap,
		gtk_font->GetNativeFont(),
		gtk_gc->GetNativeGraphicsContext(),
		sp2ipx(x - x0), sp2ipx(y - y0),
		text, len);
}

void
Gtk_DrawingArea::Clear(const GraphicsContext* gc,
		       scaled x, scaled y, scaled width, scaled height) const
{
  assert(gc != NULL);
  const Gtk_GraphicsContext* gtk_gc = TO_GTK_GRAPHICS_CONTEXT(gc);
  assert(gtk_gc != NULL);
  gdk_draw_rectangle(gdk_pixmap,
		     gtk_gc->GetNativeGraphicsContext(),
		     TRUE,
		     // width and height must be incremented, see GDK for explanation
		     sp2ipx(x - x0), sp2ipx(y - y0), sp2ipx(width) + 1, sp2ipx(height) + 1);
}

#include <stdio.h>

void
Gtk_DrawingArea::Update(scaled x, scaled y, scaled width, scaled height) const
{
  assert(gtk_widget != NULL);

  GdkRectangle updateRect;
  updateRect.x = sp2ipx(x - x0);
  updateRect.y = sp2ipx(y - y0);
  updateRect.width = sp2ipx(width);
  updateRect.height = sp2ipx(height);

  //printf("about to update rect %f %f %f %f\n", (double) updateRect.x, (double) updateRect.y, (double) updateRect.width, (double) updateRect.height);

  gtk_widget_draw(gtk_widget, &updateRect);
}

void
Gtk_DrawingArea::Update() const
{
#if 0
  //  Update(x0, y0, width, height);
  assert(gtk_widget != NULL);

  GdkRectangle updateRect;
  updateRect.x = 0;
  updateRect.y = 0;
  updateRect.width = gtk_widget->allocation.width;
  updateRect.height = gtk_widget->allocation.height;

  gtk_widget_draw(gtk_widget, &updateRect);
#endif
  gtk_widget_draw(gtk_widget, NULL);
}
