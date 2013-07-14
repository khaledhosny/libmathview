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
{ }

void
Gtk_RenderingContext::fill(const scaled& x, const scaled& y, const BoundingBox& box) const
{
  cairo_save(getContext());

  RGBColor fg = getForegroundColor();
  cairo_set_source_rgba(getContext(), fg.red / 255., fg.green / 255., fg.blue / 255., fg.alpha / 255.);
  cairo_rectangle(getContext(),
                  toGtkX(x),
                  toGtkY(y + box.height),
                  toGtkPixels(box.width),
                  toGtkPixels(box.height + box.depth));
  cairo_fill(getContext());

  cairo_restore(getContext());
}

void
Gtk_RenderingContext::draw(const scaled& x, const scaled& y, PangoLayout* layout) const
{
  cairo_save(getContext());

  RGBColor fg = getForegroundColor();
  cairo_set_source_rgba(getContext(), fg.red / 255., fg.green / 255., fg.blue / 255., fg.alpha / 255.);
  cairo_move_to(getContext(), toGtkX(x), toGtkY(y));
  pango_cairo_show_layout(getContext(), layout);

  cairo_restore(getContext());
}

void
Gtk_RenderingContext::draw(const scaled& x, const scaled& y, PangoLayoutLine* line) const
{
  cairo_save(getContext());

  RGBColor fg = getForegroundColor();
  cairo_set_source_rgba(getContext(), fg.red / 255., fg.green / 255., fg.blue / 255., fg.alpha / 255.);
  cairo_move_to(getContext(), toGtkX(x), toGtkY(y));
  pango_cairo_show_layout_line(getContext(), line);

  cairo_restore(getContext());
}

void
Gtk_RenderingContext::draw(const scaled& x, const scaled& y, PangoFont* font,
                           PangoGlyphString* glyphs) const
{
  cairo_save(getContext());

  RGBColor fg = getForegroundColor();
  cairo_set_source_rgba(getContext(), fg.red / 255., fg.green / 255., fg.blue / 255., fg.alpha / 255.);
  cairo_move_to(getContext(), toGtkX(x), toGtkY(y));
  pango_cairo_show_glyph_string(getContext(), font, glyphs);

  cairo_restore(getContext());
}
