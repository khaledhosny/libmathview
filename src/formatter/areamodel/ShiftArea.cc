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

#include <config.h>

#include "scaledAux.hh"
#include "ShiftArea.hh"

void
ShiftArea::render(class RenderingContext& context, const scaled& x, const scaled& y) const
{
  getChild()->render(context, x, y + shift);
}

bool
ShiftArea::find(class SearchingContext& context, const scaled& x, const scaled& y) const
{
  return getChild()->find(context, x, y + shift);
}

BoundingBox
ShiftArea::box() const
{
  BoundingBox childBox = getChild()->box();
  return BoundingBox(childBox.width, childBox.height + shift, childBox.depth - shift);  
}

DOM::Element
ShiftArea::dump(const DOM::Document& doc) const
{
  DOM::Element el = doc.createElementNS(STD_AREAMODEL_NAMESPACE_URI, "a:shift");
  el.setAttribute("by", toString(shift));
  return dumpAux(doc, el);
}
