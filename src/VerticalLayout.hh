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

#ifndef __VerticalLayout_hh__
#define __VerticalLayout_hh__

#include "MathMLElement.hh"
#include "HorizontalLayout.hh"

class VerticalLayout : public MathMLObject
{
protected:
  VerticalLayout(void) : rowSpacing(0), rowMinSpacing(0) { }
  virtual ~VerticalLayout() { }

public:
  static Ptr<VerticalLayout> create(void)
  { return new VerticalLayout(); }

  void SetSpacing(scaled, scaled);
  void Add(const Ptr<MathMLElement>&);
  void RemoveAll(void);
  void SetPosition(scaled, scaled);

  BoundingBox GetBoundingBox(void) const;
  BoundingBox GetLinearBoundingBox(void) const;
  scaled GetExitBaseline(void) const;

protected:
  scaled
  VerticalLayout::GetRowSpacing(const std::vector< Ptr<HorizontalLayout> >::const_iterator&,
				const std::vector< Ptr<HorizontalLayout> >::const_iterator&) const;

  std::vector< Ptr<HorizontalLayout> > content;
  scaled rowSpacing;
  scaled rowMinSpacing;
};

#endif // __VerticalLayout_hh__
