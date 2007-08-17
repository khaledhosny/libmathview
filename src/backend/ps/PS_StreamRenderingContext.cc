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
#include <iostream>
#include <sstream>
#include <ctime>

#include "PS_StreamRenderingContext.hh"
#include "String.hh"
#include "FontDataBase.hh"
#include "T1_FontDataBase.hh"

PS_StreamRenderingContext::PS_StreamRenderingContext(const SmartPtr<AbstractLogger>& logger,
						     std::ostream& os,
						     SmartPtr<FontDataBase> fDb)
  : PS_RenderingContext(logger), output(os), fontDb(fDb)
{ }

PS_StreamRenderingContext::~PS_StreamRenderingContext()
{ }

/*
String
PS_StreamRenderingContext::getId(const SmartPtr<Element>& elem) const
{ return ""; }
*/

void
PS_StreamRenderingContext::documentStart(const scaled& x, const scaled& y,
  				         const BoundingBox& bbox, const char* name)
{
  time_t curTime = time(NULL);
  std::ostringstream appName; 
  appName << "MathML to PostScript - written by Luca Padovani & Nicola Rossi";
 
  header << "%!PS-Adobe-3.0 EPSF-3.0" << std::endl;
  header << "%%BoundingBox: " << PS_RenderingContext::toPS(x) << " " 
	 << PS_RenderingContext::toPS(y) << " " 
         << toPS(bbox.width) << " " 
         << toPS(bbox.verticalExtent()) << std::endl
	 << "%%Creator: " << appName.str() << std::endl
	 << "%%CreationDate: " << asctime(localtime(&curTime))
         << "%%EndComments" << std::endl 
	 << "%%Version: v" << VERSION << std::endl
 	 << "%%Pages: 1" << std::endl
         << "%%Title: " << "\"" << name << "\"" << std::endl << std::endl;
}

void
PS_StreamRenderingContext::documentEnd(void)
{
  output << header.str();
  fontDb->dumpFontTable(output);
  output << std::endl; 
  output << body.str();
  output << "showpage" << std::endl;
  output << "%%Trailer" << std::endl;
  output << "%%EOF" << std::endl;
}

void
PS_StreamRenderingContext::setGraphicsContext(const RGBColor& strokeColor,
					      const scaled& strokeWidth)
{
  body << strokeColor.red / 255.0 << " "
       << strokeColor.green / 255.0 << " "
       << strokeColor.blue / 255.0  
       << " setrgbcolor" << std::endl;
  body << PS_RenderingContext::toPS(strokeWidth) 
       << " setlinewidth" << std::endl;
}

void
PS_StreamRenderingContext::rect(const scaled& x, const scaled& y, 
				const scaled& width, const scaled& height,
				const RGBColor& fillColor, const RGBColor& strokeColor,
			        const scaled& strokeWidth)
{
  setGraphicsContext(strokeColor, strokeWidth);

  body << "newpath" << std::endl;
  body << PS_RenderingContext::toPS(x) << " "
       << PS_RenderingContext::toPS(y)
       << " moveto" << std::endl;	
  body << PS_RenderingContext::toPS(width) << " "
       << 0.0 
       << " rlineto" << std::endl;
  body << 0.0 << " "
       << -(PS_RenderingContext::toPS(height)) 
       << " rlineto" << std::endl;
  body << -(PS_RenderingContext::toPS(width)) << " "
       << 0.0 
       << " rlineto" << std::endl;
  body << "closepath" << std::endl;
  body << fillColor.red / 255.0 << " "
       << fillColor.green / 255.0 << " "
       << fillColor.blue / 255.0 << " "
       << "setrgbcolor" << " fill" << std::endl;
  body << "stroke" << std::endl;
}

void
PS_StreamRenderingContext::text(const scaled& x, const scaled& y, 
			        const String& family, const scaled& size,
				const RGBColor& fillColor, const RGBColor& strokeColor,
				const scaled& strokeWidth, const String& content)
{
  int familyId = fontDb->getFontId(family, toPS(size));
  fontDb->recallFont(familyId, body);
  setGraphicsContext(strokeColor, strokeWidth);
  
  body << "newpath" << std::endl;
  body << PS_RenderingContext::toPS(x) << " "
       << PS_RenderingContext::toPS(y) << " "
       << "moveto" << std::endl;
  
  body << "("; 
  for (String::const_iterator i = content.begin(); i != content.end(); i++)
    drawChar((unsigned char) (*i));
  body << ") show" << std::endl;
  
  fontDb->usedChar(content, family);

  body << fillColor.red / 255.0 << " "
       << fillColor.green / 255.0 << " "
       << fillColor.blue / 255.0 << " "
       << "setrgbcolor" << " fill" << std::endl;
}

void
PS_StreamRenderingContext::drawChar(unsigned char ch)
{
  switch(ch) {
  case '(': body << "\\("; break;
  case ')': body << "\\)"; break;
  case '\\': body << "\\\\"; break;
  default:
    if (isprint(ch) && !isspace(ch))
      body << ch;
    else
      body << "\\" << (ch / 64) % 8 << (ch / 8) % 8 << ch % 8;
    break; 
  }
}

void
PS_StreamRenderingContext::line(const scaled& x1, const scaled& y1, 
     				const scaled& x2, const scaled& y2,
				const RGBColor& strokeColor, const scaled& strokeWidth)
{
  setGraphicsContext(strokeColor, strokeWidth);

  body << "newpath" << std::endl;
  body << PS_RenderingContext::toPS(x1) << " "
       << PS_RenderingContext::toPS(y1) << " "
       << "moveto" << std::endl;
  body << PS_RenderingContext::toPS(x2) << " "
       << PS_RenderingContext::toPS(y2) << " "
       << "lineto" << std::endl;
}
