// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __Gtk_WrapperArea_hh__
#define __Gtk_WrapperArea_hh__

#include "BoxArea.hh"

class Gtk_WrapperArea : public BoxArea
{
protected:
  Gtk_WrapperArea(const AreaRef&, const BoundingBox&, const SmartPtr<Object>&);
  virtual ~Gtk_WrapperArea() { }

public:
  static SmartPtr<Gtk_WrapperArea> create(const AreaRef& area, const BoundingBox& b, const SmartPtr<Object>& el)
  { return new Gtk_WrapperArea(area, b, el); }
  virtual SmartPtr<Area> clone(void) const
  { return new Gtk_WrapperArea(getChild(), box(), getElement()); }

  virtual void render(RenderingContext&, const scaled&, const scaled&) const;
  virtual bool find(class SearchingContext&, const scaled&, const scaled&) const;

  SmartPtr<Object> getElement(void) const;

  int getSelected(void) const { return selected; }
  void setSelected(int n) const { selected = n; }

private:
  SmartPtr<Object> element; // element that has created this area
  mutable int selected;
};

#endif // __Gtk_WrapperArea_hh__
