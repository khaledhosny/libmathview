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

#ifndef __PS_AreaFactory_hh__
#define __PS_AreaFactory_hh__

#include "AreaFactory.hh"
#include "PS_ColorArea.hh"
#include "PS_BackgroundArea.hh"
#include "PS_InkArea.hh"


class PS_AreaFactory : public AreaFactory
{
protected: 
  PS_AreaFactory(void) { }
  virtual ~PS_AreaFactory() { }

public:
  static SmartPtr<PS_AreaFactory> create(void)
  { return new PS_AreaFactory(); }

  virtual SmartPtr<ColorArea> color(const AreaRef& area, const RGBColor& color) const
  { return PS_ColorArea::create(area, color); }
  virtual SmartPtr<InkArea> ink(const AreaRef& area) const
  { return PS_InkArea::create(area); }
  virtual AreaRef background(const AreaRef& area, const RGBColor& color) const
  { return PS_BackgroundArea::create(area, color); }
};

#endif // __PS_AreaFactory_hh__
