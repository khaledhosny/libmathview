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

#include <config.h>

#include <cassert>

#include <gdk/gdkx.h>

#include "Gtk_RenderingContext.hh"

Gtk_RenderingContext::Gtk_RenderingContext()
{
  xft_draw = 0;
}

Gtk_RenderingContext::~Gtk_RenderingContext()
{
  releaseResources();
}

void
Gtk_RenderingContext::releaseResources()
{
  if (xft_draw)
    {
      // It seems that by using XftDrawDestroy the drawable will be destroyed
      // as well, so we only free the structure hoping that nothing bad
      // will happen. This was taken from gr_UnixGraphics.cpp in AbiWord
      free(xft_draw);
      xft_draw = 0;
    }
}

void
Gtk_RenderingContext::setDrawable(const GObjectPtr<GdkDrawable>& drawable)
{
  releaseResources();

  gdk_drawable = drawable;
  gdk_gc = gdk_gc_new(gdk_drawable);
  gdk_colormap = gdk_rgb_get_colormap();

  xft_draw = XftDrawCreate(GDK_DISPLAY(),
			   gdk_x11_drawable_get_xid(drawable),
			   GDK_VISUAL_XVISUAL(gdk_drawable_get_visual(drawable)),
			   GDK_COLORMAP_XCOLORMAP(gdk_colormap));
  assert(xft_draw);
}

void
Gtk_RenderingContext::setForegroundColor(const RGBColor& color)
{
  if (color.red != foregroundColor.red ||
      color.green != foregroundColor.green ||
      color.blue != foregroundColor.blue)
    {
      foregroundColor = color;

      gdk_foregroundColor.red = color.red << 8;
      gdk_foregroundColor.green = color.green << 8;
      gdk_foregroundColor.blue = color.blue << 8;
      gdk_foregroundColor.pixel = 0;

      gboolean ret = gdk_colormap_alloc_color(gdk_colormap, &gdk_foregroundColor, FALSE, TRUE);
      assert(ret == TRUE);

      setForegroundColor(gdk_foregroundColor);
    }
}

// The only reason why the argument is not const is that
// the GDK function does not respect const-ness :-(
void
Gtk_RenderingContext::setForegroundColor(/* const */ GdkColor& color)
{
  // Set the color in the GDK GC
  gdk_gc_set_foreground(gdk_gc, &color);

  // Set the color for Xft, note that we reuse the same pixel field
  // so that we don't need to reallocate the color
  xft_foregroundColor.color.red = color.red;
  xft_foregroundColor.color.green = color.green;
  xft_foregroundColor.color.blue = color.blue;
  xft_foregroundColor.color.alpha = 0xffff;
  xft_foregroundColor.pixel = color.pixel;
}

scaled
Gtk_RenderingContext::fromGtkPixels(int s)
{ return scaled(s / 72.0); }

scaled
Gtk_RenderingContext::fromPangoPixels(int s)
{
  assert(scaled::getPrecision() >= PANGO_SCALE);
  return scaled(s << (scaled::getPrecision() - PANGO_SCALE), true);
}
