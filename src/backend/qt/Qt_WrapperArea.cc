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

#include "Qt_WrapperArea.hh"
#include "Qt_RenderingContext.hh"
#include "SearchingContext.hh"
#include "Element.hh"

Qt_WrapperArea::Qt_WrapperArea(const AreaRef& area, const BoundingBox& b, const SmartPtr<Element>& el)
    : WrapperArea(area, b, el), selected(0)
{ }

SmartPtr<Qt_WrapperArea>
Qt_WrapperArea::create(const AreaRef& area, const BoundingBox& b, const SmartPtr<class Element>& el)
{
    return new Qt_WrapperArea(area, b, el);
}

AreaRef
Qt_WrapperArea::clone(const AreaRef& area) const
{
    return create(area, box(), getElement());
}

void
Qt_WrapperArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
    Qt_RenderingContext& context = dynamic_cast<Qt_RenderingContext&>(c);

    RenderingContext::ColorStyle old_style = context.getStyle();

    switch (old_style)
    {
    case RenderingContext::NORMAL_STYLE:
        if (selected == 1) context.setStyle(RenderingContext::SELECTED_STYLE);
        break;
    case RenderingContext::SELECTED_STYLE:
        if (selected == -1) context.setStyle(RenderingContext::NORMAL_STYLE);
        break;
    default:
        break;
    }

    if (old_style != context.getStyle())
    {
        RGBColor old_foregroundColor = context.getForegroundColor();
        RGBColor backgroundColor = context.getBackgroundColor();
        context.setForegroundColor(backgroundColor);

        BoundingBox areaBox = box();
        const scaled margin = Qt_RenderingContext::fromQtPixels(1);
        areaBox.width += margin * 2;
        areaBox.height += margin;
        areaBox.depth += margin;
        context.fill(x - margin, y, areaBox);

        context.setForegroundColor(old_foregroundColor);
    }

    getChild()->render(context, x, y);

    context.setStyle(old_style);
}
