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

#include "BoundingBox.hh"
#include "MathMLSpaceNode.hh"

MathMLSpaceNode::MathMLSpaceNode(int s)
{
  SetSpacing(s);
}

MathMLSpaceNode::~MathMLSpaceNode()
{
}

void
MathMLSpaceNode::Setup(class RenderingEnvironment&)
{
  // if the box is set to be null then a possible spacing is not added
  // or, better, it is added, but the box remains null
  box.Set(0, 0, 0);
}

void
MathMLSpaceNode::DoLayout(const FormattingContext&)
{
}

void
MathMLSpaceNode::Render(const DrawingArea&)
{
}

bool
MathMLSpaceNode::IsSpace() const
{
  return true;
}

unsigned
MathMLSpaceNode::GetLogicalContentLength() const
{
  return 0;
}

String*
MathMLSpaceNode::GetRawContent() const
{
  return NULL;
}
