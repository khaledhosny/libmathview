// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <assert.h>
#include <stddef.h>

#include "Shape.hh"
#include "Iterator.hh"
#include "ShapeFactory.hh"
#include "MathMLContainerElement.hh"

ShapeFactory::ShapeFactory(Rectangle* rect)
{
  newRow = true;
  done   = false;
  shape  = new Shape;

  if (rect != NULL) Add(rect);
}

ShapeFactory::~ShapeFactory()
{
  assert(done);
}

void
ShapeFactory::Add(Rectangle* rect)
{
  assert(rect != NULL);

  if (newRow) {
    Rectangle* r = new Rectangle(*rect);
    shape->content.Append(r);
    newRow = false;
  } else {
    assert(shape->content.GetSize() > 0);
    Rectangle* r = shape->content.GetLast();
    assert(r != NULL);
    r->Union(*rect);
  }
}

void
ShapeFactory::Add(const Shape& shape)
{
  if (shape.content.GetSize() == 0) return;

  for (Iterator<Rectangle*> rect(shape.content); rect.More(); rect.Next()) {
    assert(rect() != NULL);
    Add(rect());
    newRow = true;
  }
  newRow = false;
}

void
ShapeFactory::SetNewRow()
{
  newRow = true;
}

const Shape*
ShapeFactory::GetShape()
{
  assert(!done);
  done = true;
  return shape;
}

