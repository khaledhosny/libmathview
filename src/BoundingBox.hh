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

#ifndef BoundingBox_hh
#define BoundingBox_hh

#include "scaled.hh"

struct BoundingBox {
  bool   null;
  scaled width;
  scaled ascent;
  scaled descent;
  scaled lBearing;
  scaled rBearing;

  BoundingBox(void) { Null(); }

  void Null(void);
  bool IsNull(void) const { return null; }

  scaled GetWidth(void) const { return width; }
  scaled GetHeight(void) const { return ascent + descent; }

  void Set(scaled w, scaled a, scaled d) { Set(w, a, d, 0, w); }
  void Set(scaled, scaled, scaled, scaled, scaled);
  void Append(const BoundingBox&);
  void Append(scaled);
  void Max(const BoundingBox&);

  struct Rectangle GetRectangle(scaled, scaled) const;

  void Dump(void) const;
};

#endif // BoundingBox_hh
