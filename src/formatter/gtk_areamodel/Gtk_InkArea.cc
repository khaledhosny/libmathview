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

#include "Gtk_InkArea.hh"
#include "Gtk_RenderingContext.hh"

SmartPtr<Area>
Gtk_InkArea::clone() const
{
  return new Gtk_InkArea(getChild());
}

void
Gtk_InkArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);

  BoundingBox box = getChild()->box();
  gdk_draw_rectangle(context.getDrawable(),
		     context.getGC(),
		     TRUE,
		     Gtk_RenderingContext::toGtkX(x),
		     Gtk_RenderingContext::toGtkY(y + box.height),
		     Gtk_RenderingContext::toGtkPixels(box.width),
		     Gtk_RenderingContext::toGtkPixels(box.height + box.depth));
}
