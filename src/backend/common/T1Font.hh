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

#ifndef __T1Font_hh__
#define __T1Font_hh__

#include "Char.hh"
#include "Object.hh"
#include "SmartPtr.hh"
#include "BoundingBox.hh"

class GMV_MathView_EXPORT T1Font : public Object
{
protected:
  T1Font(const scaled& s) : size(s) { }
  virtual ~T1Font() { }

public:
  virtual scaled getGlyphLeftEdge(Char8) const = 0;
  virtual scaled getGlyphRightEdge(Char8) const = 0;
  virtual BoundingBox getGlyphBoundingBox(Char8) const = 0;

  scaled getSize(void) const { return size; }
  float getScale(void) const { return getSize().toFloat(); }

private:
  scaled size;
};

#endif // __T1Font_hh__
