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

#include <cassert>

#include "AbstractLogger.hh"
#include "Gtk_AreaFactory.hh"
#include "Gtk_BoxGraphicDevice.hh"
#include "Gtk_RenderingContext.hh"
#include "FormattingContext.hh"
#include "BoxMLElement.hh"
#include "Configuration.hh"

Gtk_BoxGraphicDevice::Gtk_BoxGraphicDevice(const SmartPtr<AbstractLogger>& logger,
					   const SmartPtr<Configuration>& conf)
  : BoxGraphicDevice(logger)
{
  pango_context = gdk_pango_context_get();
}

Gtk_BoxGraphicDevice::~Gtk_BoxGraphicDevice()
{ }

SmartPtr<Gtk_BoxGraphicDevice>
Gtk_BoxGraphicDevice::create(const SmartPtr<AbstractLogger>& logger,
			     const SmartPtr<Configuration>& conf)
{ return new Gtk_BoxGraphicDevice(logger, conf); }

void
Gtk_BoxGraphicDevice::setFactory(const SmartPtr<Gtk_AreaFactory>& f)
{
  BoxGraphicDevice::setFactory(f);
  gtk_factory = f;
}

AreaRef
Gtk_BoxGraphicDevice::dummy(const FormattingContext& context) const
{
  return getFactory()->color(string(context, StringOfUCS4String(UCS4String(1, 0xfffd)), 0), RGBColor::RED());
}

AreaRef
Gtk_BoxGraphicDevice::string(const FormattingContext& context,
			     const String& str, const scaled& width) const
{
#if 0
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
      return gtk_factory->pangoLayout(layout);
    }
#else
  return BoxGraphicDevice::string(context, str, width);
#endif
}

#if 0
#include "BoundingBoxAux.hh"
#include "scaledAux.hh"

AreaRef
Gtk_BoxGraphicDevice::paragraph(const FormattingContext& context,
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

//   std::cerr << "the final box will be " << finalBox << " the first line ascent being " << first_line_ascent << std::endl;

  std::vector<BoxedLayoutArea::XYArea> c;
  c.push_back(BoxedLayoutArea::XYArea(scaled::zero(), scaled::zero(), getFactory()->pangoLayout(layout)));
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

// 	std::cerr << "found object baseline " << baseline << std::endl;

	c.push_back(BoxedLayoutArea::XYArea(Gtk_RenderingContext::fromPangoPixels(pos.x),
					    finalBox.height - Gtk_RenderingContext::fromPangoPixels(baseline),
					    (*obj).area));

      }
  
  return getFactory()->boxedLayout(finalBox, c);
}
#endif

AreaRef
Gtk_BoxGraphicDevice::wrapper(const FormattingContext& context,
			      const AreaRef& base) const
{
  return gtk_factory->wrapper(base, base->box(), context.getBoxMLElement());
}

