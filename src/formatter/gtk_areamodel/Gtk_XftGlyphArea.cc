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

#include <gdk/gdkx.h>

#include "GObjectPtr.hh"
#include "Gtk_XftGlyphArea.hh"
#include "Gtk_RenderingContext.hh"

Gtk_XftGlyphArea::Gtk_XftGlyphArea(XftFont* f, FT_UInt g)
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
  GObjectPtr<GdkDisplay> gdk_display = GDK_DISPLAY();
  Display* display = gdk_x11_display_get_xdisplay(gdk_display);

  XftGlyphExtents(display, font, &glyph, 1, &gInfo);
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
  XftDrawGlyphs(context.getXftDraw(),
		context.getXftColor(),
		font,
		Gtk_RenderingContext::toXftPixels(x - context.getXOrigin()),
		Gtk_RenderingContext::toXftPixels(y - context.getYOrigin()),
		&glyph, 1);
}

DOM::Element
Gtk_XftGlyphArea::dump(const DOM::Document& doc) const
{
  return DOM::Element(0);
}
