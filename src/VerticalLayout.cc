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

#include "VerticalLayout.hh"
#include "MathMLSpaceElement.hh"

void
VerticalLayout::SetSpacing(scaled rs, scaled rms)
{
  rowSpacing = rs;
  rowMinSpacing = rms;
}

void
VerticalLayout::Add(const Ptr<MathMLElement>& el)
{
  if (content.empty())
    content.push_back(HorizontalLayout::create());

  Ptr<HorizontalLayout> row = content.back();
  row->Add(el);

  if (Ptr<MathMLSpaceElement> space = smart_cast<MathMLSpaceElement>(el))
    {
      if (space->GetBreakability() == BREAK_YES ||
	  space->GetBreakability() == BREAK_INDENT)
	content.push_back(HorizontalLayout::create());
    }
}

void
VerticalLayout::RemoveAll()
{
  content.clear();
}

scaled
VerticalLayout::GetRowSpacing(const std::vector< Ptr<HorizontalLayout> >::const_iterator& p0,
			      const std::vector< Ptr<HorizontalLayout> >::const_iterator& p1) const
{
  if (p0 + 1 == content.begin() || p1 == content.end())
    return 0;
  else
    {
      scaled s = rowSpacing;
      if (s - ((*p0)->GetBoundingBox().descent + (*p1)->GetBoundingBox().ascent) < rowMinSpacing)
	s = rowMinSpacing + ((*p0)->GetBoundingBox().descent + (*p1)->GetBoundingBox().ascent);
      return s;
    }
}

void
VerticalLayout::SetPosition(scaled x, scaled y)
{
  for (std::vector< Ptr<HorizontalLayout> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      (*p)->SetPosition(x, y);
      y += GetRowSpacing(p, p + 1);
    }
}

BoundingBox
VerticalLayout::GetBoundingBox() const
{
  BoundingBox box;
  for (std::vector< Ptr<HorizontalLayout> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    {
      BoundingBox pBox = (*p)->GetBoundingBox();
      if (p == content.begin()) box.Set(0, pBox.ascent, 0, pBox.lBearing, pBox.rBearing);
      if (p + 1 == content.end()) box.descent += pBox.descent;
      if (p != content.begin()) box.descent += GetRowSpacing(p - 1, p);
      box.width = scaledMax(box.width, pBox.width);
      box.lBearing = scaledMin(box.lBearing, pBox.lBearing);
      box.rBearing = scaledMax(box.rBearing, pBox.rBearing);
    }
  return box;
}

BoundingBox
VerticalLayout::GetLinearBoundingBox() const
{
  BoundingBox box;
  for (std::vector< Ptr<HorizontalLayout> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    box.Append((*p)->GetLinearBoundingBox());
  return box;
}

scaled
VerticalLayout::GetExitBaseline() const
{
  scaled bl = 0;
  for (std::vector< Ptr<HorizontalLayout> >::const_iterator p = content.begin();
       p != content.end();
       p++)
    bl += GetRowSpacing(p, p + 1);
  return bl;
}
