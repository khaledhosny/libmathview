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

#ifndef DrawingArea_hh
#define DrawingArea_hh

#include <vector>

#include "AFont.hh"
#include "String.hh"
#include "BoundingBox.hh"
#include "GraphicsContext.hh"

class DrawingArea {
public:
  DrawingArea(const GraphicsContextValues&, const scaled&, const scaled&,
	      RGBValue = WHITE_COLOR,
	      RGBValue = BLUE_COLOR);
  virtual ~DrawingArea();

  virtual const GraphicsContext* GetGC(const GraphicsContextValues&) const = 0;
  const GraphicsContext* GetGC(const GraphicsContextValues&, unsigned) const;
  void ReleaseGCs(void);

  virtual void DrawLine(const GraphicsContext*, const scaled&, const scaled&, const scaled&, const scaled&) const = 0;
  virtual void DrawRectangle(const GraphicsContext*, const scaled&, const scaled&, const scaled&, const scaled&) const = 0;
  virtual void DrawChar(const GraphicsContext*, const AFont*, const scaled&, const scaled&, char) const = 0;
  virtual void DrawString(const GraphicsContext*, const AFont*, const scaled&, const scaled&, const char*, unsigned) const = 0;
  virtual void Clear(const GraphicsContext*, const scaled&, const scaled&, const scaled&, const scaled&) const = 0;
  virtual void Update(const scaled&, const scaled&, const scaled&, const scaled&) const = 0;
  virtual void Update(void) const;

  void MoveTo(const scaled&, const scaled&) const;
  void DrawLineTo(const GraphicsContext*, const scaled&, const scaled&) const;
  void DrawLineToDelta(const GraphicsContext*, const scaled&, const scaled&) const;
  void DrawRectangle(const GraphicsContext*, const scaled&, const scaled&, const BoundingBox&) const;
  void DrawRectangle(const GraphicsContext*, const Rectangle&) const;
  void DrawBoundingBox(const GraphicsContext*, const scaled&, const scaled&, const BoundingBox&, bool = true) const;
  void DrawString(const GraphicsContext*, const AFont*, const scaled&, const scaled&, const String&) const;
  void Clear(const GraphicsContext*, const scaled&, const scaled&, const BoundingBox&) const;
  void Clear(const GraphicsContext*, const Rectangle&) const;

  void Update(const Rectangle&) const;

  scaled GetX(void) const { return x; }
  scaled GetY(void) const { return y; }

  scaled GetXMargin(void) const { return mx; }
  scaled GetYMargin(void) const { return my; }

  void   SetTop(const scaled& x, const scaled& y) { x0 = x; y0 = y; }
  void   SetTopX(const scaled& x) { x0 = x; }
  void   SetTopY(const scaled& y) { y0 = y; }
  void   SetSize(const scaled&, const scaled&);
  scaled GetTopX(void) const { return x0; }
  scaled GetTopY(void) const { return y0; }
  scaled GetWidth(void) const { return width; }
  scaled GetHeight(void) const { return height; }
  bool   Exposed(const scaled&, const scaled&, const scaled&, const scaled&) const;
  //bool   Exposed(const scaled& x, const scaled& y, const scaled& w, const scaled& h) const
  //{ return (x >= x0 || x + w <= x0 + width) && (y >= y0 || y + h <= y0 + height); }

  const GraphicsContextValues& GetDefaultGraphicsContextValues(void) const { return defaultValues; }
  RGBValue GetSelectionForeground(void) const { return selectionForeground; }
  RGBValue GetSelectionBackground(void) const { return selectionBackground; }

protected:
  mutable scaled x; // current x point for drawing lines
  mutable scaled y; // current y point for drawing lines

  scaled x0; // x-coordinate of the top-left corner
  scaled y0; // y-coordinate of the top-left corner
  scaled width;  // drawing area total width
  scaled height; // drawing area total height

  scaled mx; // left margin
  scaled my; // bottom margin

  GraphicsContextValues defaultValues;
  RGBValue selectionForeground;
  RGBValue selectionBackground;

  mutable std::vector<const GraphicsContext*> poolGC;
};

#endif // DrawingArea_hh
