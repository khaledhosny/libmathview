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

#ifndef __Gtk_WrapperArea_hh__
#define __Gtk_WrapperArea_hh__

#include "WrapperArea.hh"

class Gtk_WrapperArea : public WrapperArea
{
protected:
  Gtk_WrapperArea(const AreaRef&, const BoundingBox&, const SmartPtr<class Element>&);
  virtual ~Gtk_WrapperArea() { }

public:
  static SmartPtr<Gtk_WrapperArea> create(const AreaRef&, const BoundingBox&, const SmartPtr<class Element>&);
  virtual AreaRef clone(const AreaRef&) const;

  virtual void render(RenderingContext&, const scaled&, const scaled&) const;

  int getSelected(void) const { return selected; }
  void setSelected(int n) const { selected = n; }

private:
  mutable int selected;
};

#endif // __Gtk_WrapperArea_hh__
