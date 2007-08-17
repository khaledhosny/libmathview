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

#ifndef __SVG_WrapperArea_hh__
#define __SVG_WrapperArea_hh__

#include "WrapperArea.hh"

class SVG_WrapperArea : public WrapperArea
{
protected:
  SVG_WrapperArea(const AreaRef&, const BoundingBox&, const SmartPtr<class Element>&);
  virtual ~SVG_WrapperArea();

public:
  static SmartPtr<SVG_WrapperArea> create(const AreaRef&, const BoundingBox&, const SmartPtr<class Element>&);
  virtual AreaRef clone(const AreaRef&) const;

  virtual void render(RenderingContext&, const scaled&, const scaled&) const;
};

#endif // __SVG_WrapperArea_hh__
