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
