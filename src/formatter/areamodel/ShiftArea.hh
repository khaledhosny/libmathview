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

#ifndef __ShiftArea_hh__
#define __ShiftArea_hh__

#include "BinContainerArea.hh"

class ShiftArea : public BinContainerArea
{
protected:
  ShiftArea(const AreaRef& area, const scaled& s) : BinContainerArea(area), shift(s) { }
  virtual ~ShiftArea() { }

public:
  static SmartPtr<ShiftArea> create(const AreaRef& area, const scaled& s) { return new ShiftArea(area, s); }
  virtual SmartPtr<Area> clone(void) const { return new ShiftArea(getChild(), shift); }

  virtual void render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual bool find(class SearchingContext&, const scaled&, const scaled&) const;
  virtual DOM::Element dump(const DOM::Document&) const;
  virtual BoundingBox box(void) const;
  virtual std::pair<scaled,scaled> origin(AreaId::const_iterator, AreaId::const_iterator, const scaled&, const scaled&) const;

  scaled getShift(void) const { return shift; }

private:
  scaled shift;
};

#endif // __ShiftArea_hh__
