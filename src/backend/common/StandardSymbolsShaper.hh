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

#ifndef __StandardSymbolsShaper_hh__
#define __StandardSymbolsShaper_hh__

#include "HashMap.hh"
#include "Shaper.hh"

class GMV_MathView_EXPORT StandardSymbolsShaper : public Shaper
{
public:
  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void shape(class ShapingContext&) const;

  struct HStretchyChar
  {
    Char16 ch;
    Char8 normal;
    Char8 left;
    Char8 glue;
    Char8 right;
  };
  
  struct VStretchyChar
  {
    Char16 ch;
    Char8 normal;
    Char8 top;
    Char8 glue;
    Char8 middle;
    Char8 bottom;
  };

protected:
  virtual void registerChar(const SmartPtr<class ShaperManager>&, unsigned, Char16, Char8);
  virtual void registerStretchyCharH(const SmartPtr<class ShaperManager>&, unsigned, const HStretchyChar&, Char8);
  virtual void registerStretchyCharV(const SmartPtr<class ShaperManager>&, unsigned, const VStretchyChar&, Char8);
  virtual AreaRef getGlyphArea(const SmartPtr<class AreaFactory>&, Char8, const scaled&) const = 0;

  AreaRef shapeChar(const class ShapingContext&) const;
  AreaRef shapeStretchyCharV(const class ShapingContext&) const;
  AreaRef shapeStretchyCharH(const class ShapingContext&) const;
};

#endif // __StandardSymbolsShaper_hh__
