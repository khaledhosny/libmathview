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

#include <stdio.h>

#include "MathMLObject.hh"

unsigned MathMLObject::indentLevel = 0;
#ifdef DEBUG
int MathMLObject::counter = 0;
#endif

MathMLObject::MathMLObject()
{
#ifdef DEBUG
  counter++;
#endif
}

MathMLObject::~MathMLObject()
{
#ifdef DEBUG
  counter--;
#endif
}

void
MathMLObject::PrintIndentation() const
{
  for (unsigned i = 0; i < indentLevel; i++) putchar(' ');
}

bool
MathMLObject::IsNode() const
{
  return false;
}

bool
MathMLObject::IsText() const
{
  return false;
}

bool
MathMLObject::IsString() const
{
  return false;
}

bool
MathMLObject::IsChar() const
{
  return false;
}

bool
MathMLObject::IsStretchyChar() const
{
  return false;
}

bool
MathMLObject::IsCombinedChar() const
{
  return false;
}

bool
MathMLObject::IsSpace() const
{
  return false;
}

bool
MathMLObject::IsElement() const
{
  return false;
}

bool
MathMLObject::IsToken() const
{
  return false;
}

bool
MathMLObject::IsContainer() const
{
  return false;
}

bool
MathMLObject::IsDocument() const
{
  return false;
}

bool
MathMLObject::IsOperator() const
{
  return false;
}

bool
MathMLObject::IsEmbellishedOperator() const
{
  return false;
}

bool
MathMLObject::IsBreakable() const
{
  return false;
}

bool
MathMLObject::IsMark() const
{
  return false;
}
