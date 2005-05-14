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
#if HAVE_LIBT1
#include <t1lib.h>
#include <t1libx.h>
#include "t1lib_T1Font.hh"
#endif // HAVE_LIBT1

#include "AbstractLogger.hh"
#include "Gtk_RenderingContext.hh"

Gtk_RenderingContext::Gtk_RenderingContext(const SmartPtr<AbstractLogger>& l)
  : logger(l), style(NORMAL_STYLE), xft_draw(0), t1_opaque_mode(false), t1_aa_mode(false)
{
  assert(logger);
}

Gtk_RenderingContext::~Gtk_RenderingContext()
{
  releaseResources();
}

void
Gtk_RenderingContext::releaseResources()
{
  // should free the gc's? 
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
  if (gdk_drawable)
    {
      for (unsigned i = 0; i < MAX_STYLE; i++)
	data[i].gdk_gc = gdk_gc_new(gdk_drawable);

      xft_draw = XftDrawCreate(GDK_DISPLAY(),
			       gdk_x11_drawable_get_xid(drawable),
			       GDK_VISUAL_XVISUAL(gdk_drawable_get_visual(drawable)),
			       GDK_COLORMAP_XCOLORMAP(gdk_colormap));
      assert(xft_draw);

#if HAVE_LIBT1
      Display* xdisplay = GDK_DRAWABLE_XDISPLAY(drawable);
      assert(xdisplay != NULL);
      Colormap xcolormap = GDK_COLORMAP_XCOLORMAP(gdk_colormap);
      GdkVisual* visual = gdk_colormap_get_visual(gdk_colormap);
      assert(visual != NULL);
      Visual* xvisual = GDK_VISUAL_XVISUAL(visual);
      assert(xvisual != NULL);

      T1_AASetBitsPerPixel(visual->depth);
      logger->out(LOG_DEBUG, "t1lib: X11 depth = %d AAGetLevel = %d AAGetBitsPerPixel = %d", 
		  visual->depth, T1_AAGetLevel(), T1_AAGetBitsPerPixel());
      T1_SetX11Params(xdisplay, xvisual, visual->depth, xcolormap);
#endif // HAVE_LIBT1
    }
  else
    {
      for (unsigned i = 0; i < MAX_STYLE; i++)
	data[i].gdk_gc = 0;
    }
}

void
Gtk_RenderingContext::fill(const scaled& x, const scaled& y, const BoundingBox& box) const
{
  gdk_draw_rectangle(getDrawable(),
		     getGC(),
		     TRUE,
		     Gtk_RenderingContext::toGtkX(x),
		     Gtk_RenderingContext::toGtkY(y + box.height),
		     Gtk_RenderingContext::toGtkPixels(box.width),
		     Gtk_RenderingContext::toGtkPixels(box.height + box.depth));
}

void
Gtk_RenderingContext::draw(const scaled& x, const scaled& y, PangoLayout* layout) const
{
  gdk_draw_layout(getDrawable(),
		  getGC(),
		  Gtk_RenderingContext::toGtkX(x),
		  Gtk_RenderingContext::toGtkY(y),
		  layout);
}

void
Gtk_RenderingContext::draw(const scaled& x, const scaled& y, PangoLayoutLine* line) const
{
  gdk_draw_layout_line(getDrawable(),
		       getGC(),
		       Gtk_RenderingContext::toGtkX(x),
		       Gtk_RenderingContext::toGtkY(y),
		       line);
}

void
Gtk_RenderingContext::draw(const scaled& x, const scaled& y, PangoFont* font,
			   PangoGlyphString* glyphs) const
{
  gdk_draw_glyphs(getDrawable(),
		  getGC(),
		  font,
		  Gtk_RenderingContext::toGtkX(x),
		  Gtk_RenderingContext::toGtkY(y),
		  glyphs);
}

void
Gtk_RenderingContext::draw(const scaled& x, const scaled& y, XftFont* font, FcChar8 glyph) const
{
  XftDrawString8(xft_draw,
		 getXftForegroundColor(),
		 font,
		 Gtk_RenderingContext::toXftX(x),
		 Gtk_RenderingContext::toXftY(y),
		 &glyph, 1);
}

#if HAVE_LIBT1
void
Gtk_RenderingContext::draw(const scaled& x, const scaled& y, const SmartPtr<t1lib_T1Font>& font, Char8 glyph) const
{
  if (t1_aa_mode)
    T1_AASetCharX(GDK_DRAWABLE_XID(getDrawable()),
		  GDK_GC_XGC(getGC()), t1_opaque_mode ? T1_OPAQUE : T1_TRANSPARENT,
		  Gtk_RenderingContext::toGtkX(x), Gtk_RenderingContext::toGtkY(y),
		  font->getFontId(), glyph, font->getScale(), NULL);
  else
    T1_SetCharX(GDK_DRAWABLE_XID(getDrawable()),
		GDK_GC_XGC(getGC()), t1_opaque_mode ? T1_OPAQUE : T1_TRANSPARENT,
		Gtk_RenderingContext::toGtkX(x), Gtk_RenderingContext::toGtkY(y),
		font->getFontId(), glyph, font->getScale(), NULL);
}
#endif // HAVE_LIBT1
