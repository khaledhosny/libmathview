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

#ifndef __Gtk_BackgroundArea_hh__
#define __Gtk_BackgroundArea_hh__

#include "ColorArea.hh"

class Gtk_BackgroundArea : public ColorArea
{
protected:
  Gtk_BackgroundArea(const AreaRef& area, const RGBColor& c) : ColorArea(area, c) { }
  virtual ~Gtk_BackgroundArea() { }

public:
  static SmartPtr<Gtk_BackgroundArea> create(const AreaRef& area, const RGBColor& c)
  { return new Gtk_BackgroundArea(area, c); }
  virtual AreaRef clone(const AreaRef& area) const { return create(area, getColor()); }

  virtual void render(RenderingContext&, const scaled&, const scaled&) const;
};

#endif // __Gtk_BackgroundArea_hh__
