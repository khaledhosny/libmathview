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

#include "MathMLFrame.hh"
#include "MathMLElement.hh"

MathMLFrame::MathMLFrame()
{
  selected = last = 0;
  dirty = dirtyChildren = dirtyBackground = 1;
  dirtyLayout = 1;
}

MathMLFrame::~MathMLFrame()
{
}

bool
MathMLFrame::IsFrame() const
{
  return true;
}

bool
MathMLFrame::IsLast() const
{
  return last != 0;
}

void
MathMLFrame::SetDirty(const Rectangle* rect)
{
  dirtyBackground =
    (GetParent() != NULL && (GetParent()->IsSelected() != IsSelected())) ? 1 : 0;
  
  if (IsDirty()) return;
  dirty = 1;
  SetDirtyChildren();
}

void
MathMLFrame::SetDirtyChildren()
{
  if (HasDirtyChildren()) return;
  dirtyChildren = 1;
  for (MathMLElement* elem = GetParent(); elem != NULL && !elem->HasDirtyChildren(); elem = elem->GetParent())
    elem->dirtyChildren = 1;
}

void
MathMLFrame::SetDirtyLayout()
{
  if (HasDirtyLayout()) return;
  dirtyLayout = 1;
  for (MathMLElement* elem = GetParent(); elem != NULL && !elem->HasDirtyLayout(); elem = elem->GetParent())
    elem->dirtyLayout = 1;
}

void
MathMLFrame::SetSelected()
{
  if (IsSelected()) return;
  selected = 1;
  SetDirty();
}

void
MathMLFrame::ResetSelected()
{
  if (!IsSelected()) return;
  SetDirty();
  selected = 0;
}

void
MathMLFrame::ResetLast()
{
  last = 0;
}

BreakId
MathMLFrame::GetBreakability() const
{
  return BREAK_AUTO;
}
