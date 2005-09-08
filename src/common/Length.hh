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

#ifndef __Length_hh__
#define __Length_hh__

#include "gmv_defines.h"

struct GMV_MathView_EXPORT Length
{
  enum Unit {
    UNDEFINED_UNIT,
    PURE_UNIT,
    INFINITY_UNIT,
    LT_UNIT,
    EM_UNIT,
    EX_UNIT,
    PX_UNIT,
    IN_UNIT,
    CM_UNIT,
    MM_UNIT,
    PT_UNIT,
    PC_UNIT,
    PERCENTAGE_UNIT,
    NEGATIVE_VERYVERYTHICK_SPACE,
    NEGATIVE_VERYTHICK_SPACE,
    NEGATIVE_THICK_SPACE,
    NEGATIVE_MEDIUM_SPACE,
    NEGATIVE_THIN_SPACE,
    NEGATIVE_VERYTHIN_SPACE,
    NEGATIVE_VERYVERYTHIN_SPACE,
    ZERO_SPACE,
    VERYVERYTHIN_SPACE,
    VERYTHIN_SPACE,
    THIN_SPACE,
    MEDIUM_SPACE,
    THICK_SPACE,
    VERYTHICK_SPACE,
    VERYVERYTHICK_SPACE,

    LAST_UNIT
  };
  
  Length(float v = 0.0f, Unit u = UNDEFINED_UNIT) : value(v), type(u) { }

  void set(float v = 0.0f, Unit u = UNDEFINED_UNIT) { value = v; type = u; }
  void unset(void) { type = UNDEFINED_UNIT; value = 0.0f; }
  bool defined(void) const { return type != UNDEFINED_UNIT; }
  bool absolute(void) const { return type >= IN_UNIT && type <= PC_UNIT; }
  bool isSpace(void) const { return type >= NEGATIVE_VERYVERYTHIN_SPACE && type <= VERYVERYTHICK_SPACE; }

  float value;
  Unit  type;
};

#endif // __Length_hh__
