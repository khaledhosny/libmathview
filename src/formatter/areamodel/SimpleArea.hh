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

#ifndef __SimpleArea_hh__
#define __SimpleArea_hh__

#include "Area.hh"

class SimpleArea : public Area
{
protected:
  SimpleArea(void) { }
  virtual ~SimpleArea() { }

public:
  virtual SmartPtr<Area> clone(void) const;
  virtual AreaRef fit(const scaled&, const scaled&, const scaled&) const;
  virtual AreaRef replace(const ReplacementContext&) const;

  virtual bool      idOf(const AreaRef& area, AreaIdFactory& factory) const;
  virtual AreaRef   node(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual void      render(class RenderingContext&, const scaled&, const scaled&) const;
  virtual bool      find(class SearchingContext&, const scaled&, const scaled&) const;
  virtual scaled    origin(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual scaled    leftSide(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual scaled    rightSide(AreaId::const_iterator, AreaId::const_iterator) const;
  virtual void      strength(int&, int&, int&) const;
};

#endif // __SimpleArea_hh__
