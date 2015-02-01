// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __RenderingContext_hh__
#define __RenderingContext_hh__

#include "RGBColor.hh"

class RenderingContext
{
protected:
  enum ColorIndex { FOREGROUND_INDEX, BACKGROUND_INDEX, MAX_INDEX };

public:
  enum ColorStyle { NORMAL_STYLE, SELECTED_STYLE, MAX_STYLE };

  RenderingContext(void) : style(NORMAL_STYLE) { }
  virtual ~RenderingContext() { }

  void setForegroundColor(const RGBColor& c) { data[getStyle()].setColor(FOREGROUND_INDEX, c); }
  void setBackgroundColor(const RGBColor& c) { data[getStyle()].setColor(BACKGROUND_INDEX, c); }

  RGBColor getForegroundColor(void) const { return data[getStyle()].getColor(FOREGROUND_INDEX); }
  RGBColor getBackgroundColor(void) const { return data[getStyle()].getColor(BACKGROUND_INDEX); }

  void setStyle(ColorStyle s) { style = s; }
  ColorStyle getStyle(void) const { return style; }

  virtual void fill(const scaled&, const scaled&, const BoundingBox&) const = 0;

private:
  struct ContextData
  {
    RGBColor color[MAX_INDEX];

    void setColor(ColorIndex index, const RGBColor& c)
    { color[index] = c; }

    RGBColor getColor(ColorIndex index) const
    { return color[index]; }
  };

  ColorStyle style;
  ContextData data[MAX_STYLE];
};

#endif // __RenderingContext_hh__
