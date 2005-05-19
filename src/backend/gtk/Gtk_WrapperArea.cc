// (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include "Gtk_WrapperArea.hh"
#include "Gtk_RenderingContext.hh"
#include "SearchingContext.hh"
#include "Element.hh"

Gtk_WrapperArea::Gtk_WrapperArea(const AreaRef& area, const BoundingBox& b, const SmartPtr<Element>& el)
  : WrapperArea(area, b, el), selected(0)
{ }

SmartPtr<Gtk_WrapperArea>
Gtk_WrapperArea::create(const AreaRef& area, const BoundingBox& b, const SmartPtr<class Element>& el)
{ return new Gtk_WrapperArea(area, b, el); }

AreaRef
Gtk_WrapperArea::clone(const AreaRef& area) const
{ return create(area, box(), getElement()); }

void
Gtk_WrapperArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  Gtk_RenderingContext& context = dynamic_cast<Gtk_RenderingContext&>(c);

  Gtk_RenderingContext::ColorStyle old_style = context.getStyle();

  switch (old_style)
    {
    case Gtk_RenderingContext::NORMAL_STYLE:
      if (selected == 1) context.setStyle(Gtk_RenderingContext::SELECTED_STYLE);
      break;
    case Gtk_RenderingContext::SELECTED_STYLE:
      if (selected == -1) context.setStyle(Gtk_RenderingContext::NORMAL_STYLE);
      break;
    default:
      break;
    }

  if (old_style != context.getStyle())
    {
      RGBColor old_foregroundColor;
      RGBColor backgroundColor;
      context.getForegroundColor(old_foregroundColor);
      context.getBackgroundColor(backgroundColor);
      context.setForegroundColor(backgroundColor);

      context.fill(x, y, box());

      context.setForegroundColor(old_foregroundColor);
    }

  getChild()->render(context, x, y);

  context.setStyle(old_style);
}
