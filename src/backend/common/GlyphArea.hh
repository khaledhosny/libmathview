// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __GlyphArea_hh__
#define __GlyphArea_hh__

#include "SimpleArea.hh"
#include "BoundingBox.hh"

class GMV_MathView_EXPORT GlyphArea : public SimpleArea
{
protected:
  GlyphArea(void) { }
  virtual ~GlyphArea();

public:
  virtual bool indexOfPosition(const scaled&, const scaled&, CharIndex&) const;
  virtual bool positionOfIndex(CharIndex, class Point*, BoundingBox*) const;
  virtual bool searchByIndex(AreaId&, CharIndex) const;

  virtual SmartPtr<const class GlyphArea> getGlyphArea(void) const;
};

#endif // __GlyphArea_hh__
