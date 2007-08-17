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

#ifndef __ColorArea_hh__
#define __ColorArea_hh__

#include "RGBColor.hh"
#include "BinContainerArea.hh"

class GMV_MathView_EXPORT ColorArea : public BinContainerArea
{
protected:
  ColorArea(const AreaRef& area, const RGBColor& c) : BinContainerArea(area), color(c) { }
  virtual ~ColorArea() { }

public:
  static SmartPtr<ColorArea> create(const AreaRef& area, const RGBColor& c) { return new ColorArea(area, c); }
  virtual AreaRef clone(const AreaRef&) const;

  RGBColor getColor(void) const { return color; }

private:
  RGBColor color;
};

#endif // __ColorArea_hh__
