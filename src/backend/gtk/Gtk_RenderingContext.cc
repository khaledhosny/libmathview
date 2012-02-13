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

#include <config.h>

#include <cassert>

#include "AbstractLogger.hh"
#include "Gtk_RenderingContext.hh"

Gtk_RenderingContext::Gtk_RenderingContext(const SmartPtr<AbstractLogger>& l)
  : logger(l), style(NORMAL_STYLE)
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
