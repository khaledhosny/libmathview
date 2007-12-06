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

#include <config.h>

#include "SVG_WrapperArea.hh"
#include "SVG_RenderingContext.hh"
#include "Element.hh"

SVG_WrapperArea::SVG_WrapperArea(const AreaRef& area, const BoundingBox& b, const SmartPtr<Element>& el)
  : WrapperArea(area, b, el)
{ }

SVG_WrapperArea::~SVG_WrapperArea()
{ }

SmartPtr<SVG_WrapperArea>
SVG_WrapperArea::create(const AreaRef& area, const BoundingBox& b, const SmartPtr<Element>& el)
{ return new SVG_WrapperArea(area, b, el); }

AreaRef
SVG_WrapperArea::clone(const AreaRef& area) const
{ return create(area, box(), getElement()); }

void
SVG_WrapperArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
  SVG_RenderingContext& context = dynamic_cast<SVG_RenderingContext&>(c);
  context.wrapperStart(x, y, box(), getElement());
  WrapperArea::render(context, x, y);
  context.wrapperEnd();
}
