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

#ifndef MathMLFrame_hh
#define MathMLFrame_hh

#include "Coords.hh"
#include "BoundingBox.hh"
#include "MathMLNode.hh"
#include "DrawingArea.hh"

class MathMLFrame : public MathMLNode
{
protected:
  MathMLFrame(void);
  virtual ~MathMLFrame();

public:
  virtual void   SetPosition(scaled, scaled);
  virtual void   Render(const DrawingArea&) = 0;

  scaled         GetX(void) const { return position.x; }
  scaled         GetY(void) const { return position.y; }
  const BoundingBox& GetBoundingBox(void) const { return box; }
  Rectangle      GetRectangle(void) const;
  virtual scaled GetLeftEdge(void) const = 0;
  virtual scaled GetRightEdge(void) const = 0;
  virtual bool 	 IsInside(scaled, scaled) const = 0;

protected:
  Coords      position;
  BoundingBox box;
};

#endif // MathMLFrame_hh
