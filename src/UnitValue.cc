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

#include <config.h>
#include <assert.h>

#include "defs.h"
#include "UnitValue.hh"
#include "scaledConv.hh"

void
UnitValue::Null()
{
  value = 0.0;
  unitId = UNIT_NOTVALID;
}

void
UnitValue::Reset()
{
  value = 0.0;
  unitId = UNIT_ZERO;
}

void
UnitValue::Set(float d, UnitId id)
{
  value = d;
  unitId = id;
}

void
UnitValue::SetValue(float d)
{
  value = d;
}

scaled
UnitValue::ToScaledPoints() const
{
  assert(unitId != UNIT_EM);
  assert(unitId != UNIT_EX);
  return ToScaledPoints(0, 0);
}

scaled
UnitValue::ToScaledPoints(const scaled& sppm, const scaled& sppx) const
{
  switch (unitId)
    {
    case UNIT_ZERO: return 0;
    case UNIT_EM: return sppm * value;
    case UNIT_EX: return sppx * value;
    case UNIT_PX: return px2sp(value);
    case UNIT_IN: return in2sp(value);
    case UNIT_CM: return cm2sp(value);
    case UNIT_MM: return mm2sp(value);
    case UNIT_PT: return pt2sp(value);
    case UNIT_PC: return pc2sp(value);
    default: assert(false);
    }
}
