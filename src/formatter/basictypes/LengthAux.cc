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

#include <config.h>

#include <string>
#include <sstream>

#include <cassert>

#include "LengthAux.hh"

static const char* unitName[] = {
  "??",
  "",
  "infinity",
  "em",
  "ex",
  "px",
  "in",
  "cm",
  "mm",
  "pt",
  "pc",
  "%",
  "negativeveryverythickspace",
  "negativeverythickspace",
  "negativethickspace",
  "negativemediumspace",
  "negativethinspace",
  "negativeverythinspace",
  "negativeveryverythinspace",
  "zerospace",
  "veryverythinspace",
  "verythinspace",
  "thinspace",
  "mediumspace",
  "thickspace",
  "verythickspace",
  "veryverythickspace",
  0
};

std::string
toString(Length::Unit unit)
{
  assert(unit >= Length::UNDEFINED_UNIT && unit < Length::LAST_UNIT);
  return unitName[unit];
}

bool
fromString(const std::string& s, Length::Unit& unit)
{
  for (unsigned i = 0; unitName[i]; i++)
    if (s == unitName[i])
      {
	unit = Length::Unit(i);
	return true;
      }
  return false;
}

std::ostream&
operator<<(std::ostream& os, const Length::Unit& unit)
{ return os << toString(unit); }

std::string
toString(const Length& length)
{
  if (!length.defined())
    return "[undefined length]";
  if (length.isSpace())
    return unitName[length.type];
  else if (length.type == Length::INFINITY_UNIT)
    return (length.value < 0.0) ? "-infinity" : "infinity";
  else
    {
      std::ostringstream os;
      os << length.value << length.type;
      return os.str();
    }
}

bool
fromString(const std::string& s, Length& res)
{
  std::istringstream is(s);
  float f;
  is >> f;

  assert(false);
}

std::ostream&
operator<<(std::ostream& os, const Length& length)
{ return os << toString(length); }


