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

#ifndef __HorizontalLayout_hh__
#define __HorizontalLayout_hh__

#include "MathMLElement.hh"

class HorizontalLayout : public Object
{
protected:
  HorizontalLayout(void) { }
  virtual ~HorizontalLayout() { }

public:
  static SmartPtr<HorizontalLayout> create()
  { return new HorizontalLayout(); }

  void Add(const SmartPtr<MathMLElement>&);
  void RemoveAll(void);
  void SetPosition(const scaled&, const scaled&);

  BoundingBox GetBoundingBox(void) const;
  BoundingBox GetLinearBoundingBox(void) const;
  scaled GetExitBaseline(void) const;

protected:
  std::vector< SmartPtr<MathMLElement> > content;
};

#endif // __HorizontalLayout_hh__
