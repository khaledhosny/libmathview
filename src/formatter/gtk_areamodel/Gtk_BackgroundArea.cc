// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include "Gtk_BackgroundArea.hh"
#include "Gtk_RenderingContext.hh"

void
Gtk_BackgroundArea::render(RenderingContext& context, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& c = dynamic_cast<Gtk_RenderingContext&>(context);

  if (c.getStyle() == Gtk_RenderingContext::NORMAL_STYLE)
    {
      RGBColor old_foregroundColor;
      RGBColor old_backgroundColor;
      c.getForegroundColor(old_foregroundColor);
      c.getBackgroundColor(old_backgroundColor);

      c.setForegroundColor(getColor());
      c.setBackgroundColor(getColor());
      BoundingBox bbox = box();
      gdk_draw_rectangle(c.getDrawable(),
			 c.getGC(),
			 TRUE,
			 Gtk_RenderingContext::toGtkX(x),
			 Gtk_RenderingContext::toGtkY(y + bbox.height),
			 Gtk_RenderingContext::toGtkPixels(bbox.width) + 1,
			 Gtk_RenderingContext::toGtkPixels(bbox.height + bbox.depth) + 1);

      c.setForegroundColor(old_foregroundColor);
      getChild()->render(context, x, y);

      c.setBackgroundColor(old_backgroundColor);
    }
  else
    getChild()->render(context, x, y);    
}
