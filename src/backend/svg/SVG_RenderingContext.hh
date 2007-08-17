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

#ifndef __Gtk_RenderingContext_hh__
#define __Gtk_RenderingContext_hh__

#include "Char.hh"
#include "SmartPtr.hh"
#include "RGBColor.hh"
#include "BoundingBox.hh"
#include "RenderingContext.hh"
#include "String.hh"

class GMV_BackEnd_EXPORT SVG_RenderingContext : public RenderingContext
{
public:
  SVG_RenderingContext(const SmartPtr<class AbstractLogger>&);
  virtual ~SVG_RenderingContext();

  void setForegroundColor(const RGBColor& c) { fgColor = c; }
  void setBackgroundColor(const RGBColor& c) { bgColor = c; }

  RGBColor getForegroundColor(void) const { return fgColor; }
  RGBColor getBackgroundColor(void) const { return bgColor; }

  virtual void documentStart(const BoundingBox&);
  virtual void documentEnd(void);
  virtual void fill(const scaled&, const scaled&, const BoundingBox&);
  virtual void draw(const scaled&, const scaled&, const SmartPtr<class TFMFont>&, Char8);
  virtual void wrapperStart(const scaled&, const scaled&, const BoundingBox&, const SmartPtr<class Element>&);
  virtual void wrapperEnd(void);

  static scaled toSVGX(const scaled& x) { return x; }
  static scaled toSVGY(const scaled& y) { return -y; }

  virtual String toSVGLength(const scaled&) const;
  virtual String toSVGColor(const RGBColor&) const;
  virtual String toSVGOpacity(const RGBColor&) const;

protected:
  virtual void beginDocument(const BoundingBox&);
  virtual void endDocument(void);
  virtual void metadata(const String&);
  virtual void text(const scaled&, const scaled&, const String&, const scaled&,
		    const RGBColor&, const RGBColor&, const scaled&, const String&);
  virtual void rect(const scaled&, const scaled&, const scaled&, const scaled&,
		    const RGBColor&, const RGBColor&, const scaled&);
  virtual void line(const scaled&, const scaled&, const scaled&, const scaled&,
		    const RGBColor&, const scaled&);

protected:
  SmartPtr<class AbstractLogger> logger;

  RGBColor fgColor;
  RGBColor bgColor;
};

#endif // __SVG_RenderingContext_hh__
