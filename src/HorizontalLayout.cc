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

#include "HorizontalLayout.hh"
#include "MathMLBreakableRowElement.hh"

void
HorizontalLayout::Add(const Ptr<MathMLElement>& el)
{
  content.push_back(el);
}

void
HorizontalLayout::RemoveAll()
{
  content.clear();
}

void
HorizontalLayout::SetPosition(scaled x, scaled y)
{
  for (std::vector< Ptr<MathMLElement> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      (*p)->SetPosition(x, y);
      x += (*p)->GetBoundingBox().width;
      if (Ptr<MathMLBreakableRowElement> brow = smart_cast<MathMLBreakableRowElement>(*p))
	y += brow->GetExitBaseline();
    }
}

scaled
HorizontalLayout::GetExitBaseline() const
{
  scaled bl = 0;
  for (std::vector< Ptr<MathMLElement> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    if (Ptr<MathMLBreakableRowElement> brow = smart_cast<MathMLBreakableRowElement>(*p))
      bl += brow->GetExitBaseline();
  return bl;
}

BoundingBox
HorizontalLayout::GetBoundingBox() const
{
  return GetLinearBoundingBox();
}

BoundingBox
HorizontalLayout::GetLinearBoundingBox() const
{
  scaled bl = 0;
  BoundingBox box;
  box.Null();
  for (std::vector< Ptr<MathMLElement> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      BoundingBox pBox = (*p)->GetBoundingBox();
      pBox.ascent -= bl;
      pBox.descent += bl;
      box.Append(pBox);
      if (Ptr<MathMLBreakableRowElement> brow = smart_cast<MathMLBreakableRowElement>(*p))
	bl += brow->GetExitBaseline();
    }
  return box;
}
