// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <sstream>

#include "SVG_StreamRenderingContext.hh"

#include "TFM.hh"
#include "TFM_T1Font.hh"
#include "scaledAux.hh"
#include "BoundingBoxAux.hh"
#include "RGBColorAux.hh"

SVG_StreamRenderingContext::SVG_StreamRenderingContext(const SmartPtr<AbstractLogger>& logger,
						       std::ostream& os)
  : SVG_RenderingContext(logger), output(os)
{ }

SVG_StreamRenderingContext::~SVG_StreamRenderingContext()
{ }

String
SVG_StreamRenderingContext::scaledToSVG(const scaled& s)
{
  std::ostringstream buffer;
  buffer << s.toFloat() << "pt";
  return buffer.str();
}

String
SVG_StreamRenderingContext::RGBColorToSVG(const RGBColor& c)
{
  assert(!c.transparent);
  return toString(c);
}

void
SVG_StreamRenderingContext::prologue(const scaled& width, const scaled& height) const
{
  output << "<?xml version=\"1.0\"?>" << std::endl;
  output << "<svg"
	 << " version=\"1\""
	 << " width=\"" << scaledToSVG(width) << "\""
	 << " height=\"" << scaledToSVG(height) << "\""
	 << " xmlns=\"http://www.w3.org/2000/svg\""
	 << ">" << std::endl;
}

void
SVG_StreamRenderingContext::epilogue() const
{
  output << "</svg>" << std::endl;
}

void
SVG_StreamRenderingContext::fill(const scaled& x, const scaled& y, const BoundingBox& box) const
{
  output << "<rect"
	 << " x=\"" << scaledToSVG(x) << "\""
	 << " y=\"" << scaledToSVG(y + box.height) << "\""
	 << " width=\"" << scaledToSVG(box.horizontalExtent()) << "\""
	 << " height=\"" << scaledToSVG(box.verticalExtent()) << "\""
	 << " fill=\"" << RGBColorToSVG(getForegroundColor()) << "\""
	 << " stroke-width=\"0\""
	 << "/>" << std::endl;
}

void
SVG_StreamRenderingContext::draw(const scaled& x, const scaled& y, const SmartPtr<TFM_T1Font>& font, Char8 index) const
{
  SmartPtr<TFM> tfm = font->getTFM();
  assert(tfm);
  const int mappedIndex = (index < 32) ? 256 + index : index;
  
  output << "<text"
	 << " x=\"" << scaledToSVG(x) << "\""
	 << " y=\"" << scaledToSVG(y) << "\""
	 << " font-family=\"" << tfm->getFamily() << tfm->getDesignSize().toInt() << "\""
	 << " font-size=\"" << scaledToSVG(font->getSize()) << "\""
	 << ">";
  output << "&#" << mappedIndex << ";";
  output << "</text>" << std::endl;
}

