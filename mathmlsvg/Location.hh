// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __Location_hh__
#define __Location_hh__

#include "String.hh"
#include "BoundingBox.hh"
#include "scaled.hh"
#include "Object.hh"
#include "SmartPtr.hh"

class Location : public Object
{
protected:
  Location(const String&, const scaled&, const scaled&, const BoundingBox&);
  ~Location();

public:
  static SmartPtr<Location> create(const String&, const scaled&, const scaled&, const BoundingBox&);

  String getId(void) const { return id; }
  scaled getX(void) const { return x; }
  scaled getY(void) const { return y; }
  BoundingBox getBoundingBox(void) const { return box; }

private:
  String id;
  scaled x;
  scaled y;
  BoundingBox box;
};

#endif // __Location_hh__
