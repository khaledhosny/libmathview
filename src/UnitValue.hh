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

#ifndef UnitValue_hh
#define UnitValue_hh

#include <config.h>

#include "scaled.hh"
#include "keyword.hh"

struct UnitValue {
  float  value;
  UnitId unitId;

  void   Null(void);
  void   Reset(void);
  void   Set(float, UnitId);
  void   SetValue(float);
  float  GetValue(void) const { return value; }
  UnitId GetUnitId(void) const { return unitId; }
  bool   IsNull(void) const { return unitId == UNIT_NOTVALID; }
  bool   IsPercentage(void) const { return unitId == UNIT_PERCENTAGE; }
  scaled ToScaledPoints(void) const;
  scaled ToScaledPoints(float, float) const;
};

#endif
