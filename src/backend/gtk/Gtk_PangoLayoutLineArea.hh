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

#ifndef __Gtk_PangoLayoutLineArea_hh__
#define __Gtk_PangoLayoutLineArea_hh__

#include <pango/pango.h>

#include "Gtk_PangoLayoutArea.hh"

class Gtk_PangoLayoutLineArea : public Gtk_PangoLayoutArea 
{
protected:
  Gtk_PangoLayoutLineArea(PangoLayout*);
  virtual ~Gtk_PangoLayoutLineArea();

public:
  static SmartPtr<Gtk_PangoLayoutLineArea> create(PangoLayout* layout)
  { return new Gtk_PangoLayoutLineArea(layout); }

  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
#if 1
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point*, BoundingBox*) const;
#endif
};

#endif // __Gtk_PangoLayoutLineArea_hh__
