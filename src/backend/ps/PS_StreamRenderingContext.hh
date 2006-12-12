// Copyright (C) 2000-2006, Luca Padovani <padovani@sti.uniurb.it>.
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
// <padovani@sti.uniurb.it>


#ifndef __PS_StreamRenderingContext_hh__
#define __PS_StreamRenderingContext_hh__

#include <iostream>
#include <sstream>

#include "PS_RenderingContext.hh"
#include "String.hh"
#include "FontDataBase.hh"

class GMV_BackEnd_EXPORT PS_StreamRenderingContext : public PS_RenderingContext
{
public:
  PS_StreamRenderingContext(const SmartPtr<class AbstractLogger>&, std::ostream&);
  virtual ~PS_StreamRenderingContext();
  
  virtual void documentStart(const scaled& x, const scaled& y,
			     const BoundingBox& bbox, const char* name);
  virtual void documentEnd(void);

protected:
  virtual String getId(const SmartPtr<class Element>&) const;
  virtual void setGraphicsContext(const RGBColor& strokeColor, const scaled& strokeWidth);
  virtual void drawChar(unsigned char ch);
  virtual void text(const scaled&, const scaled&, const String&, const scaled&,
 		    const RGBColor&, const RGBColor&, const scaled&, const String&);
  virtual void rect(const scaled&, const scaled&, const scaled&, const scaled&,
		    const RGBColor& fillColor, const RGBColor& strokeColor,
		    const scaled& strokeWidth);
  virtual void line(const scaled&, const scaled&, const scaled&, const scaled&,
		    const RGBColor&, const scaled&);
private:
  std::ostream& output;
  std::ostringstream header;
  std::ostringstream body;
  FontDataBase fontDb;
};

#endif // __PS_StreamRenderingContext_hh__
