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

#include "Gtk_ColorArea.hh"
#include "Gtk_RenderingContext.hh"

void
Gtk_ColorArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);

  if (context.getStyle() == Gtk_RenderingContext::NORMAL_STYLE)
    {
      RGBColor oldColor; // dunno why asking for the GdkColor, which would save a color allocation, doesn't work
      context.getForegroundColor(oldColor);
      context.setForegroundColor(getColor());
      getChild()->render(context, x, y);
      context.setForegroundColor(oldColor);
    }
  else
    getChild()->render(context, x, y);    
}
