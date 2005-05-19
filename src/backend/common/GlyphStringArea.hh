// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __GlyphStringArea_hh__
#define __GlyphStringArea_hh__

#include "HorizontalArrayArea.hh"

class GlyphStringArea : public HorizontalArrayArea
{
protected:
  GlyphStringArea(const std::vector<AreaRef>& children, const std::vector<CharIndex>& c)
    : HorizontalArrayArea(children), counters(c)
  { assert(children.size() == counters.size()); }
  virtual ~GlyphStringArea() { }

public:
  static SmartPtr<GlyphStringArea> create(const std::vector<AreaRef>& children, const std::vector<CharIndex>& c)
  { return new GlyphStringArea(children, c); }
  virtual AreaRef clone(const std::vector<AreaRef>& c) const { return create(c, counters); }

  virtual CharIndex length(void) const;
  virtual CharIndex lengthTo(AreaIndex) const;
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point*, BoundingBox*) const;

private:
  std::vector<CharIndex> counters;
};

#endif // __GlyphStringArea_hh__
