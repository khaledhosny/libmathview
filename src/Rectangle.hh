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

#ifndef Rectangle_hh
#define Rectangle_hh

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "scaled.hh"

struct Rectangle {
  scaled x;
  scaled y;
  scaled width;
  scaled height;

  void Zero(void) { x = y = width = height = 0; }

  bool Overlaps(const Rectangle& rect) const {
    return Overlaps(rect.x, rect.y, rect.width, rect.height);
  }

  bool Overlaps(scaled x0, scaled y0, scaled w0, scaled h0) const {
    return
      scaledLeq(x, x0 + w0) && scaledGeq(x + width, x0) &&
      scaledLeq(y, y0 + h0) && scaledGeq(y + height, y0);
  }

  bool IsNull(void) const { return (width < EPSILON) || (height < EPSILON); }

  bool IsInside(scaled x0, scaled y0) const {
    return
      scaledIn(x0, x, x + width) &&
      scaledIn(y0, y, y + height);
  }

  void Union(const Rectangle&);

  void Dump(void) const;
};

#endif
