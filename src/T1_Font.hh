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

#ifndef T1_Font_hh
#define T1_Font_hh

#ifdef HAVE_LIBT1

#include "AFont.hh"
#include "String.hh"
#include "BoundingBox.hh"

class T1_Font : public AFont {
public:
  T1_Font(unsigned, float);
  virtual ~T1_Font();

  virtual scaled CharWidth(char) const;
  virtual scaled CharHeight(char) const;
  virtual scaled GetAscent(void) const;
  virtual scaled GetDescent(void) const;
  virtual float  GetItalicAngle(void) const;
  virtual scaled GetEm(void) const;
  virtual scaled GetEx(void) const;
  virtual scaled GetAxis(void) const;
  virtual scaled GetKerning(char, char) const;
  virtual scaled GetLineThickness(void) const;
  virtual void   GetBoundingBox(BoundingBox&) const;
  virtual void   CharBox(char, BoundingBox&) const;
  virtual void   StringBox(const char*, unsigned, BoundingBox&) const;

  unsigned GetNativeFontId(void) const { return nativeFontId; }
  unsigned GetFontId(void) const { return fontId; }
  double   GetScale(void) const { return scale; }

  friend class T1_Gtk_DrawingArea;

private:
  unsigned nativeFontId;
  unsigned fontId;
  float    scale;

  static unsigned nextId;
};

#define TO_T1_FONT(font) (dynamic_cast<const T1_Font*>(font))

#endif // HAVE_LIBT1

#endif // T1_Font_hh
