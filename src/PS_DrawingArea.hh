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

#ifndef PS_DrawingArea_hh
#define PS_DrawingArea_hh

#ifdef HAVE_LIBT1

#include <stdio.h>

#include "AFont.hh"
#include "DrawingArea.hh"

inline float sp2ps(scaled s) { return sp2in(s) * 72; }

class PS_DrawingArea : public DrawingArea {
public:
  PS_DrawingArea(const GraphicsContextValues&, scaled, scaled, FILE* = NULL);
  virtual ~PS_DrawingArea();

  virtual const GraphicsContext* GetGC(const GraphicsContextValues&) const;

  virtual void DrawLine(const GraphicsContext*, scaled, scaled, scaled, scaled) const;
  virtual void DrawRectangle(const GraphicsContext*, scaled, scaled, scaled, scaled) const;
  virtual void DrawChar(const GraphicsContext*, const AFont*, scaled, scaled, char) const;
  virtual void DrawString(const GraphicsContext*, const AFont*, scaled, scaled, const char*, unsigned) const;
  virtual void Clear(const GraphicsContext*, scaled, scaled, scaled, scaled) const;
  virtual void Update(scaled, scaled, scaled, scaled) const;

  void DisableColors();

  void DumpHeader(const char*, const char*, const Rectangle&) const;
  void DumpGrid(void) const;
  void DumpPreamble(void) const;
  void DumpEpilogue(void) const;

  FILE* GetOutputFile(void) const { return output; }
  void SetOutputFile(FILE*);

protected:
  void DrawCharAux(unsigned char) const;
  void SetGraphicsContext(const GraphicsContext*) const;
  void SetFont(const AFont*) const;

  mutable const class GraphicsContext* lastGC;
  mutable const class T1_Font* lastFont;

  FILE* output;

  bool colors;
};

#define TO_PS_DRAWINGAREA(area) (dynamic_cast<PS_DrawingArea*>(area))

#endif // HAVE_LIBT1

#endif // PS_DrawingArea
