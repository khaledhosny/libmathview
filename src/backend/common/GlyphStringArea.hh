// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __GlyphStringArea_hh__
#define __GlyphStringArea_hh__

#include "String.hh"
#include "HorizontalArrayArea.hh"

class GMV_MathView_EXPORT GlyphStringArea : public HorizontalArrayArea
{
protected:
  GlyphStringArea(const std::vector<AreaRef>& children, const std::vector<CharIndex>& c, const UCS4String& s)
    : HorizontalArrayArea(children), counters(c), source(s)
  { assert(children.size() == counters.size()); }
  virtual ~GlyphStringArea() { }

public:
  static SmartPtr<GlyphStringArea> create(const std::vector<AreaRef>& children, const std::vector<CharIndex>& c, const UCS4String& s)
  { return new GlyphStringArea(children, c, s); }
  virtual AreaRef clone(const std::vector<AreaRef>& c) const { return create(c, counters, source); }

  virtual CharIndex length(void) const;
  virtual CharIndex lengthTo(AreaIndex) const;
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point*, BoundingBox*) const;
  const UCS4String& getSource() const { return source; }
  
  virtual SmartPtr<const class GlyphStringArea> getGlyphStringArea(void) const;  
  
private:
  std::vector<CharIndex> counters;
  UCS4String source;
};

#endif // __GlyphStringArea_hh__
