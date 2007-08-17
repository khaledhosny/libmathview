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

#include <gdk/gdkx.h>

#include "GObjectPtr.hh"
#include "Gtk_XftGlyphArea.hh"
#include "Gtk_RenderingContext.hh"

Gtk_XftGlyphArea::Gtk_XftGlyphArea(XftFont* f, FcChar8 g)
  : font(f), glyph(g)
{ }

Gtk_XftGlyphArea::~Gtk_XftGlyphArea()
{
  // the font is supposed to be freed by the font manager
}

void
Gtk_XftGlyphArea::getXftExtents(XGlyphInfo& gInfo) const
{
  // don't know why it is necessary to know the display
  // for calculating glyph extents
  // Another possibility would be to precompute the box once
  // and for all, or to pass it at instantiation time.
  // Or, the are could store the rendering context since
  // it may also be used for unit conversion
  GdkDisplay* gdk_display = gdk_display_get_default();
  Display* display = gdk_x11_display_get_xdisplay(gdk_display);

  XftTextExtents8(display, font, &glyph, 1, &gInfo);
}

BoundingBox
Gtk_XftGlyphArea::box() const
{
  XGlyphInfo gInfo;
  getXftExtents(gInfo);
  return BoundingBox(Gtk_RenderingContext::fromXftPixels(gInfo.xOff),
		     Gtk_RenderingContext::fromXftPixels(gInfo.y),
		     Gtk_RenderingContext::fromXftPixels(gInfo.height - gInfo.y));
}

scaled
Gtk_XftGlyphArea::leftEdge() const
{
  XGlyphInfo gInfo;
  getXftExtents(gInfo);
  return Gtk_RenderingContext::fromXftPixels(gInfo.x); // or -gInfo.x ????
}

scaled
Gtk_XftGlyphArea::rightEdge() const
{
  XGlyphInfo gInfo;
  getXftExtents(gInfo);
  return Gtk_RenderingContext::fromXftPixels(gInfo.width);
}

void
Gtk_XftGlyphArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);
  context.draw(x, y, font, glyph);
}
