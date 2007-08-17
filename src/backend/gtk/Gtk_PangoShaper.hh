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

#ifndef __Gtk_PangoShaper_hh__
#define __Gtk_PangoShaper_hh__

#include <pango/pango.h>

#include "Gtk_DefaultPangoShaper.hh"

class Gtk_PangoShaper : public Gtk_DefaultPangoShaper
{
protected:
  Gtk_PangoShaper(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);
  virtual ~Gtk_PangoShaper();

public:
  static SmartPtr<Gtk_PangoShaper> create(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&);

  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void shape(class ShapingContext&) const;

protected:
  AreaRef shapeChar(const class ShapingContext&) const;
  AreaRef shapeChunk(const class ShapingContext&, unsigned) const;
  AreaRef shapeStretchyCharH(const class ShapingContext&) const;
  AreaRef shapeStretchyCharV(const class ShapingContext&) const;
};

#endif // __Gtk_PangoShaper_hh__
