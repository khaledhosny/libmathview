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

#include <cassert>

#include "Gtk_AreaFactory.hh"
#include "Gtk_BoxGraphicDevice.hh"
#include "Gtk_RenderingContext.hh"
#include "BoxFormattingContext.hh"
#include "BoxMLElement.hh"

Gtk_BoxGraphicDevice::Gtk_BoxGraphicDevice(GtkWidget* widget)
{
  factory = Gtk_AreaFactory::create();
  pango_context = gtk_widget_create_pango_context(widget);
}

Gtk_BoxGraphicDevice::~Gtk_BoxGraphicDevice()
{ }

SmartPtr<AreaFactory>
Gtk_BoxGraphicDevice::getFactory() const
{
  return factory;
}

AreaRef
Gtk_BoxGraphicDevice::string(const BoxFormattingContext& context,
			     const String& str, const scaled& width) const
{
  const DOM::UTF8String utf8_string = str;

  PangoLayout* layout = pango_layout_new(pango_context);
  pango_layout_set_text(layout, utf8_string.data(), utf8_string.length());
  if (width < scaled::zero())
    pango_layout_set_width(layout, -1);
  else
    pango_layout_set_width(layout, Gtk_RenderingContext::toPangoPixels(width));
  pango_layout_set_single_paragraph_mode(layout, TRUE);

  PangoAttrList* attrList = pango_attr_list_new();
  PangoAttribute* aSize = pango_attr_size_new(Gtk_RenderingContext::toPangoPixels(context.getSize()));
  aSize->start_index = 0;
  aSize->end_index = utf8_string.length();
  pango_attr_list_insert(attrList, aSize);

  pango_layout_set_attributes(layout, attrList);

  // set attributes...
  return factory->pangoLayout(layout);
}

#include "BoundingBoxAux.hh"

AreaRef
Gtk_BoxGraphicDevice::paragraph(const BoxFormattingContext& context,
				const String& str,
				const BoxLayout& boxes,
				const scaled& width) const
{
  const DOM::UTF8String utf8_string = str;

  PangoLayout* layout = pango_layout_new(pango_context);
  pango_layout_set_text(layout, utf8_string.data(), utf8_string.length());
  pango_layout_set_width(layout, Gtk_RenderingContext::toPangoPixels(width));
  pango_layout_set_single_paragraph_mode(layout, TRUE);
  pango_layout_set_wrap(layout, PANGO_WRAP_WORD);

  PangoAttrList* attrList = pango_attr_list_new();
  PangoAttribute* aSize = pango_attr_size_new(Gtk_RenderingContext::toPangoPixels(context.getSize()));
  aSize->start_index = 0;
  aSize->end_index = utf8_string.length();
  pango_attr_list_insert(attrList, aSize);

  std::cerr << "************ formatting a paragraph with " << boxes.size() << " foreign objects" << std::endl;

  for (BoxLayout::const_iterator p = boxes.begin(); p != boxes.end(); p++)
    {
      const BoundingBox box = (*p).second->box();
      PangoRectangle rect;
      rect.x = 0;
      rect.y = -Gtk_RenderingContext::toPangoPixels(box.height);
      rect.width = Gtk_RenderingContext::toPangoPixels(box.width);
      rect.height = Gtk_RenderingContext::toPangoPixels(box.verticalExtent());
      PangoAttribute* aShape = pango_attr_shape_new(&rect, &rect);
      aShape->start_index = (*p).first;
      aShape->end_index = (*p).first + 3;
      pango_attr_list_insert(attrList, aShape);
      std::cerr << "setting foreign object at " << aShape->start_index << std::endl;
    }

  pango_layout_set_attributes(layout, attrList);

  PangoRectangle rect;
  pango_layout_get_extents(layout, 0, &rect);

  PangoLayoutLine* first_line = pango_layout_get_line(layout, 0);
  PangoRectangle first_line_rect;
  pango_layout_line_get_extents(first_line, 0, &first_line_rect);

  const scaled first_line_ascent = Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(first_line_rect));

  BoundingBox finalBox(Gtk_RenderingContext::fromPangoPixels(rect.width),
		       first_line_ascent,
		       Gtk_RenderingContext::fromPangoPixels(rect.height) - first_line_ascent);

  std::cerr << "the final box will be " << finalBox << std::endl;

  std::vector<BoxedLayoutArea::XYArea> c;
  c.reserve(boxes.size() + 1);
  c.push_back(BoxedLayoutArea::XYArea(scaled::zero(), scaled::zero(), factory->pangoLayout(layout)));
  for (BoxLayout::const_iterator p = boxes.begin(); p != boxes.end(); p++)
    {
      PangoRectangle pos;
      pango_layout_index_to_pos(layout, (*p).first, &pos);
      c.push_back(BoxedLayoutArea::XYArea(Gtk_RenderingContext::fromPangoPixels(pos.x),
					  finalBox.height - Gtk_RenderingContext::fromPangoPixels(pos.y) - (*p).second->box().height,
					  (*p).second));

      std::cerr << "the foreign object is at " << pos.x << " " << pos.y << " " << pos.width << " " << pos.height << std::endl;
    }
  return factory->boxedLayout(finalBox, c);
}

AreaRef
Gtk_BoxGraphicDevice::wrapper(const BoxFormattingContext& context,
			      const AreaRef& base) const
{
  BoundingBox box = base->box();
  return factory->wrapper(base->fit(box.width, box.height, box.depth), box, context.getElement());
}
