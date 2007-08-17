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

#include <sstream>

#include "SVG_StreamRenderingContext.hh"

SVG_StreamRenderingContext::SVG_StreamRenderingContext(const SmartPtr<AbstractLogger>& logger,
						       std::ostream& os)
  : SVG_RenderingContext(logger), output(os)
{ }

SVG_StreamRenderingContext::~SVG_StreamRenderingContext()
{ }

void
SVG_StreamRenderingContext::wrapperStart(const scaled& x, const scaled& y, const BoundingBox& box,
					 const SmartPtr<Element>& elem)
{
  const String id = getId(elem);
  output << "<g";
  if (!id.empty()) output << " id=\"" << id << "\"";
  output << " gmv:x=\"" << toSVGLength(SVG_RenderingContext::toSVGX(x)) << "\""
	 << " gmv:y=\"" << toSVGLength(SVG_RenderingContext::toSVGY(y)) << "\""
	 << " gmv:width=\"" << toSVGLength(box.horizontalExtent()) << "\""
	 << " gmv:height=\"" << toSVGLength(box.height) << "\""
	 << " gmv:depth=\"" << toSVGLength(box.depth) << "\""
	 << ">" << std::endl;
}

String
SVG_StreamRenderingContext::getId(const SmartPtr<Element>& elem) const
{ return ""; }

void
SVG_StreamRenderingContext::wrapperEnd()
{
  output << "</g>" << std::endl;
}

void
SVG_StreamRenderingContext::beginDocument(const BoundingBox& bbox)
{
  output << "<?xml version=\"1.0\"?>" << std::endl;
  output << "<svg"
	 << " version=\"1\""
	 << " width=\"" << toSVGLength(bbox.horizontalExtent()) << "\""
	 << " height=\"" << toSVGLength(bbox.verticalExtent()) << "\""
	 << " xmlns=\"http://www.w3.org/2000/svg\""
	 << " xmlns:gmv=\"http://helm.cs.unibo.it/2005/GtkMathView\""
	 << ">" << std::endl;
}

void
SVG_StreamRenderingContext::endDocument()
{
  output << "</svg>" << std::endl;
}

void
SVG_StreamRenderingContext::metadata(const String& content)
{
  output << "<metadata>" << content << "</metadata>" << std::endl;
}

void
SVG_StreamRenderingContext::rect(const scaled& x, const scaled& y, const scaled& width, const scaled& height,
				 const RGBColor& fillColor, const RGBColor& strokeColor, const scaled& strokeWidth)
{
  output << "<rect"
	 << " x=\"" << toSVGLength(x) << "\""
	 << " y=\"" << toSVGLength(y) << "\""
	 << " width=\"" << toSVGLength(width) << "\""
	 << " height=\"" << toSVGLength(height) << "\""
	 << " fill=\"" << toSVGColor(fillColor) << "\""
	 << " fill-opacity=\"" << toSVGOpacity(fillColor) << "\""
	 << " stroke=\"" << toSVGColor(strokeColor) << "\""
	 << " stroke-opacity=\"" << toSVGOpacity(strokeColor) << "\""
	 << " stroke-width=\"" << toSVGLength(strokeWidth) << "\""
	 << "/>" << std::endl;
}

void
SVG_StreamRenderingContext::text(const scaled& x, const scaled& y, 
				 const String& family, const scaled& size,
				 const RGBColor& fillColor, const RGBColor& strokeColor,
				 const scaled& strokeWidth, const String& content)
{
#if 0
  output << "<rect"
	 << " x=\"" << toSVGLength(SVG_RenderingContext::toSVGX(x)) << "\""
	 << " y=\"" << toSVGLength(SVG_RenderingContext::toSVGY(y + box.height)) << "\""
	 << " width=\"" << toSVGLength(box.horizontalExtent()) << "\""
	 << " height=\"" << toSVGLength(box.verticalExtent()) << "\""
	 << " stroke=\"red\""
	 << " stroke-width=\"0.1\""
	 << " fill-opacity=\"0\""
	 << "/>" << std::endl;
  output << "<line"
	 << " x1=\"" << toSVGLength(SVG_RenderingContext::toSVGX(x)) << "\""
	 << " y1=\"" << toSVGLength(SVG_RenderingContext::toSVGY(y)) << "\""
	 << " x2=\"" << toSVGLength(SVG_RenderingContext::toSVGX(x + box.horizontalExtent())) << "\""
	 << " y2=\"" << toSVGLength(SVG_RenderingContext::toSVGY(y)) << "\""
	 << " stroke=\"red\""
	 << " stroke-width=\"0.1\""
	 << "/>" << std::endl;
#endif
  output << "<text"
	 << " x=\"" << toSVGLength(x) << "\""
	 << " y=\"" << toSVGLength(y) << "\""
	 << " font-family=\"" << family << "\""
	 << " font-size=\"" << toSVGLength(size) << "\""
	 << " fill=\"" << toSVGColor(fillColor) << "\""
	 << " fill-opacity=\"" << toSVGOpacity(fillColor) << "\""
	 << " stroke=\"" << toSVGColor(strokeColor) << "\""
	 << " stroke-opacity=\"" << toSVGOpacity(strokeColor) << "\""
	 << " stroke-width=\"" << toSVGLength(strokeWidth) << "\""
	 << ">"
	 << content
	 << "</text>" << std::endl;
}

void
SVG_StreamRenderingContext::line(const scaled&, const scaled&, const scaled&, const scaled&,
				 const RGBColor&, const scaled&)
{
  assert(false);
}
