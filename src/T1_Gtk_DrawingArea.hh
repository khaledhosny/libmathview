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

#ifndef T1_Gtk_DrawingArea_hh
#define T1_Gtk_DrawingArea_hh

#include "Gtk_DrawingArea.hh"

class T1_Gtk_DrawingArea : public Gtk_DrawingArea {
public:
  T1_Gtk_DrawingArea(const GraphicsContextValues&, scaled, scaled, GtkWidget*, RGBValue, RGBValue);
  virtual ~T1_Gtk_DrawingArea();

  virtual void DrawChar(const GraphicsContext*, const AFont*, scaled, scaled, char) const;
  virtual void DrawString(const GraphicsContext*, const AFont*, scaled, scaled, const char*, unsigned) const;

  virtual void Realize(void);

  bool GetAntiAliasing(void) const { return antiAliasing; }
  void SetAntiAliasing(bool);
  bool GetTransparency(void) const;
  void SetTransparency(bool);

private:
  static bool firstTime;

  bool antiAliasing;
  int  mode;
};

#define TO_T1_GTK_DRAWING_AREA(area) (dynamic_cast<T1_Gtk_DrawingArea*>(area))

#endif // T1_Gtk_DrawingArea_hh
