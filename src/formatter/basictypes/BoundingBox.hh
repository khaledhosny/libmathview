// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef __BoundingBox_hh__
#define __BoundingBox_hh__

#include "scaled.hh"

struct BoundingBox
{
  BoundingBox(void) : width(scaled::zero()), height(scaled::min()), depth(scaled::min()) { }
  BoundingBox(const scaled& w, const scaled& h, const scaled& d) : width(w), height(h), depth(d) { }

  bool defined(void) const { return height != scaled::min() || depth != scaled::min(); }

  void append(const BoundingBox&);
  void overlap(const BoundingBox&);
  void under(const BoundingBox&);
  void over(const BoundingBox&);

  scaled width;
  scaled height;
  scaled depth;
};

#endif // __BoundingBox_hh__
