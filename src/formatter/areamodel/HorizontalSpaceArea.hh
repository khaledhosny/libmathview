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

#ifndef __HorizontalSpaceArea_hh__
#define __HorizontalSpaceArea_hh__

#include "SimpleArea.hh"
#include "BoundingBox.hh"

class HorizontalSpaceArea : public SimpleArea
{
protected:
  HorizontalSpaceArea(const scaled& w) : width(w) { }
  virtual ~HorizontalSpaceArea();

public:
  // TODO: caching of areas: it is likely that there will be a large
  // number of horizontal space areas all with the same width
  static  SmartPtr<HorizontalSpaceArea> create(const scaled& w) { return new HorizontalSpaceArea(w); }

  virtual BoundingBox box(void) const;
  virtual DOM::Element dump(const DOM::Document&) const;
  virtual scaled      leftEdge(void) const;
  virtual scaled      rightEdge(void) const;

  scaled getWidth(void) const { return width; }

private:
  scaled width;
};

#endif // __HorizontalSpaceArea_hh__
