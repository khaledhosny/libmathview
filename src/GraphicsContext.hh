// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef GraphicsContext_hh
#define GraphicsContext_hh

#include "RGBValue.hh"
#include "Rectangle.hh"
#include "FontAttributes.hh"

enum LineStyle {
  LINE_STYLE_NOTVALID = -1,
  LINE_STYLE_SOLID,
  LINE_STYLE_DASHED
};

enum GraphicsContextValuesMask {
  GC_MASK_FOREGROUND = 1 << 0,
  GC_MASK_BACKGROUND = 1 << 1,
  GC_MASK_LINE_WIDTH = 1 << 2,
  GC_MASK_LINE_STYLE = 1 << 3
};

struct GraphicsContextValues {
  RGBValue  foreground;
  RGBValue  background;
  LineStyle lineStyle;
  scaled    lineWidth;

  bool Equals(const GraphicsContextValues& v) const {
    return
      foreground == v.foreground &&
      background == v.background &&
      lineStyle == v.lineStyle &&
      scaledEq(lineWidth, v.lineWidth);
  }
};

class GraphicsContext {
public:
  GraphicsContext(const GraphicsContextValues&);
  virtual ~GraphicsContext();

  RGBValue  GetForeground(void) const { return values.foreground; }
  RGBValue  GetBackground(void) const { return values.background; }
  LineStyle GetLineStyle(void) const { return values.lineStyle; }
  scaled    GetLineWidth(void) const { return values.lineWidth; }

  const GraphicsContextValues& GetValues(void) const { return values; }

private:
  GraphicsContextValues values;
};

#endif // GraphicsContext_hh

