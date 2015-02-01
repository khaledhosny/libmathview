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

#ifndef __BackgroundArea_hh__
#define __BackgroundArea_hh__

#include "ColorArea.hh"

class BackgroundArea : public ColorArea
{
protected:
  BackgroundArea(const AreaRef& area, const RGBColor& c) : ColorArea(area, c) { }
  virtual ~BackgroundArea() { }

public:
  static SmartPtr<BackgroundArea> create(const AreaRef& area, const RGBColor& c)
  { return new BackgroundArea(area, c); }
  AreaRef clone(const AreaRef& area) const { return create(area, getColor()); }

  void render(RenderingContext&, const scaled&, const scaled&) const;
};

#endif // __BackgroundArea_hh__
