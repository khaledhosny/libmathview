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

#ifndef Gtk_DrawingArea_hh
#define Gtk_DrawingArea_hh

#include <gtk/gtk.h>

#include "AFont.hh"
#include "DrawingArea.hh"
#include "GraphicsContext.hh"

class Gtk_DrawingArea : public DrawingArea {
public:
  Gtk_DrawingArea(const GraphicsContextValues&, scaled, scaled, GtkWidget*, RGBValue, RGBValue);
  virtual ~Gtk_DrawingArea();

  virtual const GraphicsContext* GetGC(const GraphicsContextValues&) const;

  virtual void DrawLine(const GraphicsContext*, scaled, scaled, scaled, scaled) const;
  virtual void DrawRectangle(const GraphicsContext*, scaled, scaled, scaled, scaled) const;
  virtual void DrawChar(const GraphicsContext*, const AFont*, scaled, scaled, char) const;
  virtual void DrawString(const GraphicsContext*, const AFont*, scaled, scaled, const char*, unsigned) const;
  virtual void Clear(const GraphicsContext*, scaled, scaled, scaled, scaled) const;
  virtual void Update(scaled, scaled, scaled, scaled) const;
  virtual void Update(void) const;

  virtual void Realize(void);

  scaled GetTopX(void) const { return x0; }
  scaled GetTopY(void) const { return y0; }

  void SetTop(scaled, scaled);
  void SetTopX(scaled);
  void SetTopY(scaled);
  void SetPixmap(GdkPixmap*);
  
protected:
  scaled x0; // x-coordinate of the top-left corner
  scaled y0; // y-coordinate of the top-left corner

  GtkWidget* gtk_widget;
  GdkPixmap* gdk_pixmap;
};

#define TO_GTK_DRAWINGAREA(area) (dynamic_cast<Gtk_DrawingArea*>(area))

#endif
