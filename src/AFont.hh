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

#ifndef AFont_hh
#define AFont_hh

#include "String.hh"
#include "BoundingBox.hh"

#define NULLCHAR ((char) -1)

class AFont {
public:
  virtual ~AFont() = 0;

  virtual scaled CharWidth(char) const = 0;
  virtual scaled CharHeight(char) const = 0;
  virtual scaled GetAscent(void) const = 0;
  virtual scaled GetDescent(void) const = 0;
  virtual float  GetItalicAngle(void) const = 0;
  virtual scaled GetEm(void) const = 0;
  virtual scaled GetEx(void) const = 0;
  virtual scaled GetAxis(void) const = 0;
  virtual scaled GetKerning(char, char) const = 0;
  virtual scaled GetLineThickness(void) const = 0;
  virtual void   GetBoundingBox(BoundingBox&) const = 0;
  virtual scaled StringWidth(const char*, unsigned) const;
  virtual void   CharBox(char, BoundingBox&) const = 0;
  virtual void   StringBox(const char*, unsigned, BoundingBox&) const = 0;

  scaled GetSize(void) const { return GetAscent() + GetDescent(); }
};

#endif // AFont_hh
