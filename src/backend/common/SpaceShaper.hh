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

#ifndef __SpaceShaper_hh__
#define __SpaceShaper_hh__

#include "Shaper.hh"

class GMV_MathView_EXPORT SpaceShaper : public Shaper
{
protected:
  SpaceShaper(void) { }
  virtual ~SpaceShaper() { }

public:
  static SmartPtr<SpaceShaper> create(void)
  { return new SpaceShaper(); }

  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void shape(class ShapingContext&) const;

protected:
  static void shapeFixedSpace(class ShapingContext&, const class GlyphSpec&);
  static void pushSpace(class ShapingContext&, int, unsigned = 1);
};

#endif // __SpaceShaper_hh__
