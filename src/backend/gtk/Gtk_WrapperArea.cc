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
      GdkColor old_foregroundColor;
      GdkColor backgroundColor;
      context.getForegroundColor(old_foregroundColor);
      context.getBackgroundColor(backgroundColor);
      context.setForegroundColor(backgroundColor);

      BoundingBox areaBox = box();
      const scaled margin = Gtk_RenderingContext::fromGtkPixels(1);
      areaBox.width += margin * 2;
      areaBox.height += margin;
      areaBox.depth += margin;
      context.fill(x - margin, y, areaBox);

      context.setForegroundColor(old_foregroundColor);
    }

  getChild()->render(context, x, y);

  context.setStyle(old_style);
}
