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

#ifndef Gtk_Font_hh
#define Gtk_Font_hh

#include <gdk/gdk.h>

#include "AFont.hh"
#include "String.hh"
#include "BoundingBox.hh"

class Gtk_Font : public AFont {
public:
  Gtk_Font(GdkFont*);
  Gtk_Font(const Gtk_Font&);
  virtual ~Gtk_Font();

  virtual scaled CharWidth(char) const;
  virtual scaled CharHeight(char) const;
  virtual scaled GetAscent(void) const;
  virtual scaled GetDescent(void) const;
  virtual float  GetItalicAngle(void) const;
  virtual scaled GetSize(void) const { return GetAscent() + GetDescent(); }
  virtual scaled GetEm(void) const;
  virtual scaled GetEx(void) const;
  virtual scaled GetAxis(void) const;
  virtual scaled GetKerning(char, char) const;
  virtual scaled GetLineThickness(void) const;
  virtual void   GetBoundingBox(BoundingBox&) const;
  virtual void   CharBox(char, BoundingBox&) const;
  virtual void   StringBox(const char*, unsigned, BoundingBox&) const;

  GdkFont* GetNativeFont(void) const { return font; }

private:
  GdkFont* font;
};

#define TO_GTK_FONT(font) (dynamic_cast<const Gtk_Font*>(font))

#endif // Gtk_Font_hh
