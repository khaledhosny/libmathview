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

Gtk_BoxGraphicDevice::Gtk_BoxGraphicDevice()
{
  factory = Gtk_AreaFactory::create();
  pango_context = gdk_pango_context_get();
}

Gtk_BoxGraphicDevice::~Gtk_BoxGraphicDevice()
{ }

SmartPtr<AreaFactory>
Gtk_BoxGraphicDevice::getFactory() const
{
  return factory;
}

AreaRef
Gtk_BoxGraphicDevice::dummy(const BoxFormattingContext& context) const
{
  return getFactory()->color(string(context, StringOfUCS4String(UCS4String(1, 0xfffd)), 0), RGBColor::RED());
}

AreaRef
Gtk_BoxGraphicDevice::string(const BoxFormattingContext& context,
			     const String& str, const scaled& width) const
{
  if (str.length() == 0)
    return dummy(context);
  else
    {
      const UTF8String utf8_string = str;

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
}

#if 0
#include "BoundingBoxAux.hh"
#include "scaledAux.hh"

AreaRef
Gtk_BoxGraphicDevice::paragraph(const BoxFormattingContext& context,
				const BoxedParagraph& p,
				const scaled& width) const
{
  const UTF8String utf8_string = p.getContent();

  PangoLayout* layout = pango_layout_new(pango_context);
  pango_layout_set_text(layout, utf8_string.data(), utf8_string.length());
  pango_layout_set_width(layout, Gtk_RenderingContext::toPangoPixels(width));
  pango_layout_set_single_paragraph_mode(layout, TRUE);
  pango_layout_set_wrap(layout, PANGO_WRAP_WORD);
  //pango_layout_set_justify(layout, TRUE); // doesn't work????

  PangoAttrList* attrList = pango_attr_list_new();

  for (std::vector<BoxedParagraph::Object>::const_iterator obj = p.begin(); obj != p.end(); obj++)
    if ((*obj).area)
      {
	const BoundingBox box = (*obj).area->box();
	PangoRectangle rect;
	rect.x = 0;
	rect.y = -Gtk_RenderingContext::toPangoPixels(box.height);
	rect.width = Gtk_RenderingContext::toPangoPixels(box.width);
	rect.height = Gtk_RenderingContext::toPangoPixels(box.verticalExtent());
	PangoAttribute* aShape = pango_attr_shape_new(&rect, &rect);
	aShape->start_index = (*obj).start_index;
	aShape->end_index = (*obj).end_index;
	pango_attr_list_insert(attrList, aShape);
      }
    else
      {
#if 0
	const RGBColor fColor = (*obj).foreground;
	PangoAttribute* fAttr = pango_attr_foreground_new(fColor.red << 8, fColor.green << 8, fColor.blue << 8);
	fAttr->start_index = (*obj).start_index;
	fAttr->end_index = (*obj).end_index;
	pango_attr_list_insert(attrList, fAttr);

	if (!(*obj).background.transparent)
	  {
	    const RGBColor bColor = (*obj).background;
	    PangoAttribute* bAttr = pango_attr_background_new(bColor.red << 8, bColor.green << 8, bColor.blue << 8);
	    bAttr->start_index = (*obj).start_index;
	    bAttr->end_index = (*obj).end_index;
	    pango_attr_list_insert(attrList, bAttr);
	  }
#endif

	PangoAttribute* aSize = pango_attr_size_new(Gtk_RenderingContext::toPangoPixels((*obj).size));
	aSize->start_index = (*obj).start_index;
	aSize->end_index = (*obj).end_index;
	pango_attr_list_insert(attrList, aSize);
      }

  pango_layout_set_attributes(layout, attrList);

  PangoRectangle rect;
  pango_layout_get_extents(layout, 0, &rect);

#if 0
  PangoLayoutLine* first_line = pango_layout_get_line(layout, 0);
  PangoRectangle first_line_rect;
  pango_layout_line_get_extents(first_line, 0, &first_line_rect);
  const scaled first_line_ascent = Gtk_RenderingContext::fromPangoPixels(PANGO_ASCENT(first_line_rect));
#else
  PangoLayoutIter* iter = pango_layout_get_iter(layout);
  const scaled first_line_ascent = Gtk_RenderingContext::fromPangoPixels(pango_layout_iter_get_baseline(iter));
  pango_layout_iter_free(iter);
#endif

  BoundingBox finalBox(Gtk_RenderingContext::fromPangoPixels(rect.width),
		       first_line_ascent,
		       Gtk_RenderingContext::fromPangoPixels(rect.height) - first_line_ascent);

  std::cerr << "the final box will be " << finalBox << " the first line ascent being " << first_line_ascent << std::endl;

  std::vector<BoxedLayoutArea::XYArea> c;
  c.push_back(BoxedLayoutArea::XYArea(scaled::zero(), scaled::zero(), factory->pangoLayout(layout)));
  for (std::vector<BoxedParagraph::Object>::const_iterator obj = p.begin(); obj != p.end(); obj++)
    if ((*obj).area)
      {
	PangoRectangle pos;
	pango_layout_index_to_pos(layout, (*obj).start_index, &pos);
#if 0
	std::cerr << "Positioning object with box " << (*obj).area->box() << " at "
		  << Gtk_RenderingContext::fromPangoPixels(pos.x) << ","
		  << Gtk_RenderingContext::fromPangoPixels(pos.y) << ","
		  << Gtk_RenderingContext::fromPangoPixels(pos.width) << ","
		  << Gtk_RenderingContext::fromPangoPixels(pos.height) << std::endl;
	std::cerr << "The computation yields " << finalBox.height - Gtk_RenderingContext::fromPangoPixels(pos.y) - (*obj).area->box().height << std::endl;
#endif

	PangoLayoutIter* iter = pango_layout_get_iter(layout);
	int baseline = pango_layout_iter_get_baseline(iter);
	while (baseline < pos.y)
	  {
	    pango_layout_iter_next_line(iter);
	    baseline = pango_layout_iter_get_baseline(iter);
	  }
	pango_layout_iter_free(iter);

	std::cerr << "found object baseline " << baseline << std::endl;

	c.push_back(BoxedLayoutArea::XYArea(Gtk_RenderingContext::fromPangoPixels(pos.x),
					    finalBox.height - Gtk_RenderingContext::fromPangoPixels(baseline),
					    (*obj).area));

      }
  
  return factory->boxedLayout(finalBox, c);
}
#endif

AreaRef
Gtk_BoxGraphicDevice::wrapper(const BoxFormattingContext& context,
			      const AreaRef& base) const
{
  BoundingBox box = base->box();
  return factory->wrapper(base->fit(box.width, box.height, box.depth), box, context.getElement());
}

