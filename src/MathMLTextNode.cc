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

#include <stdio.h>

#include "MathMLTextNode.hh"
#include "MathMLElement.hh"

// A MathMLTextNode is used to represent an UNBREAKABLE chunk of
// text and/or space. This means that TextNode is an Atom.
// The breakability of the chunk is somehow determined by the
// node content

MathMLTextNode::MathMLTextNode()
{
  SetSpacing(0);
  SetBreakability(BREAK_AUTO);
}

MathMLTextNode::~MathMLTextNode()
{
}

bool
MathMLTextNode::IsText() const
{
  return true;
}

void
MathMLTextNode::SetPosition(scaled x, scaled y)
{
  //printf("set position x %d y %d\n", sp2ipx(x), sp2ipx(y));
  position.x = x;
  position.y = y;
}

void
MathMLTextNode::SetSpacing(int s)
{
  spacing = s;
}

void
MathMLTextNode::AddSpacing(int ds)
{
  spacing += ds;
}

void
MathMLTextNode::SetBreakability(BreakId bid)
{
  breakability = bid;
}

void
MathMLTextNode::AddBreakability(BreakId bid)
{
  breakability = (bid > breakability) ? bid : breakability;
}

BreakId
MathMLTextNode::GetBreakability() const
{
  return static_cast<BreakId>(breakability);
}

void
MathMLTextNode::SetDirty(const Rectangle* rect)
{
  if (IsDirty()) return;

  if (rect != NULL && !rect->Overlaps(GetX(), GetY() - box.ascent, box.width, box.GetHeight()))
    return;

  MathMLFrame::SetDirty(rect);
}

bool
MathMLTextNode::IsInside(scaled x, scaled y) const
{
  return
    scaledGeq(x, position.x) &&
    scaledGeq(y, position.y - box.ascent) &&
    scaledLeq(x, position.x + box.width) &&
    scaledLeq(y, position.y + box.descent);
}

scaled
MathMLTextNode::GetLeftEdge() const
{
  return GetX() + box.lBearing;
}

scaled
MathMLTextNode::GetRightEdge() const
{
  return GetX() + box.rBearing;
}

bool
MathMLTextNode::HasDecimalPoint() const
{
  return false;
}

scaled
MathMLTextNode::GetDecimalPointEdge() const
{
  return 0;
}

