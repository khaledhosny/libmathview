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
  : logger(l), style(NORMAL_STYLE), t1_opaque_mode(false), t1_aa_mode(false)
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
