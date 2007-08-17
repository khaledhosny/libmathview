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

#ifndef __Gtk_PangoLayoutArea_hh__
#define __Gtk_PangoLayoutArea_hh__

#include <pango/pango.h>

#include "GObjectPtr.hh"
#include "GlyphArea.hh"

class Gtk_PangoLayoutArea : public GlyphArea 
{
protected:
  Gtk_PangoLayoutArea(PangoLayout*);
  Gtk_PangoLayoutArea(PangoLayout*, bool);
  virtual ~Gtk_PangoLayoutArea();

public:
  static SmartPtr<Gtk_PangoLayoutArea> create(PangoLayout* layout)
  { return new Gtk_PangoLayoutArea(layout); }
  static SmartPtr<Gtk_PangoLayoutArea> create(PangoLayout* layout, bool)
  { return new Gtk_PangoLayoutArea(layout, true); }

  virtual BoundingBox box(void) const;
  virtual scaled leftEdge(void) const;
  virtual scaled rightEdge(void) const;
  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point*, BoundingBox*) const;
  virtual CharIndex length(void) const;

protected:
  GObjectPtr<PangoLayout> layout;
  BoundingBox bbox;
};

#endif // __Gtk_PangoLayoutArea_hh__
