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

#ifndef __AreaFactory_hh__
#define __AreaFactory_hh__

#include "BoxArea.hh"
#include "ColorArea.hh"
#include "HideArea.hh"
#include "HorizontalArrayArea.hh"
#include "HorizontalFillerArea.hh"
#include "HorizontalSpaceArea.hh"
#include "IdArea.hh"
#include "IgnoreArea.hh"
#include "InkArea.hh"
#include "Object.hh"
#include "OverlapArrayArea.hh"
#include "ShiftArea.hh"
#include "VerticalArrayArea.hh"
#include "VerticalFillerArea.hh"
#include "VerticalSpaceArea.hh"

class AreaFactory : public Object
{
protected:
  AreaFactory(void) { }
  virtual ~AreaFactory() { }

public:
  static SmartPtr<AreaFactory> create(void)
  { return new AreaFactory(); }

  virtual SmartPtr<BoxArea> box(const AreaRef& area, const BoundingBox& box) const
  { return BoxArea::create(area, box); }
  virtual SmartPtr<ColorArea> color(const AreaRef& area, const RGBColor& color) const
  { return ColorArea::create(area, color); }
  virtual SmartPtr<HideArea> hide(const AreaRef& area) const
  { return HideArea::create(area); }
  virtual SmartPtr<HorizontalArrayArea> horizontalArray(const std::vector<AreaRef>& content) const
  { return HorizontalArrayArea::create(content); }
  virtual SmartPtr<VerticalArrayArea> verticalArray(const std::vector<AreaRef>& content, unsigned ref) const
  { return VerticalArrayArea::create(content, ref); }
  virtual SmartPtr<OverlapArrayArea> overlapArray(const std::vector<AreaRef>& content) const
  { return OverlapArrayArea::create(content); }
  virtual SmartPtr<IdArea> id(const AreaRef& area) const
  { return IdArea::create(area); }
  virtual SmartPtr<IgnoreArea> ignore(const AreaRef& area) const
  { return IgnoreArea::create(area); }
  virtual SmartPtr<InkArea> ink(const AreaRef& area) const
  { return InkArea::create(area); }
  virtual SmartPtr<HorizontalSpaceArea> horizontalSpace(const scaled& w) const
  { return HorizontalSpaceArea::create(w); }
  virtual SmartPtr<VerticalSpaceArea> verticalSpace(const scaled& h, const scaled& d) const
  { return VerticalSpaceArea::create(h, d); }
  virtual SmartPtr<HorizontalFillerArea> horizontalFiller(void) const
  { return HorizontalFillerArea::create(); }
  virtual SmartPtr<VerticalFillerArea> verticalFiller(void) const
  { return VerticalFillerArea::create(); }
  virtual SmartPtr<ShiftArea> shift(const AreaRef& area, const scaled& s) const
  { return ShiftArea::create(area, s); }

  // macro areas

  virtual AreaRef background(const AreaRef& area, const RGBColor& color) const;
  virtual AreaRef horizontalLine(const scaled& thickness, const RGBColor& color) const;
  virtual AreaRef verticalLine(const scaled& thickness, const RGBColor& color) const;
  virtual AreaRef center(const AreaRef& area) const;
  virtual AreaRef left(const AreaRef& area) const;
  virtual AreaRef right(const AreaRef& area) const;
  virtual AreaRef middle(const AreaRef& area) const;
  virtual AreaRef top(const AreaRef& area) const;
  virtual AreaRef bottom(const AreaRef& area) const;
  virtual AreaRef freeze(const AreaRef& area) const;
};

#endif // __AreaFactory_hh__
