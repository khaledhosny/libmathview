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

#ifndef __Qt_AreaFactory_hh__
#define __Qt_AreaFactory_hh__

#include "AreaFactory.hh"
#include "Qt_ColorArea.hh"
#include "Qt_BackgroundArea.hh"
#include "Qt_InkArea.hh"
#include "Qt_GlyphArea.hh"
#include "Qt_WrapperArea.hh"
#include <QtGlobal>

class Qt_AreaFactory : public AreaFactory
{
protected:
    Qt_AreaFactory(void) { }
    virtual ~Qt_AreaFactory() { }

public:
    static SmartPtr<Qt_AreaFactory> create(void)
    {
        return new Qt_AreaFactory();
    }

    // redefined methods

    virtual SmartPtr<ColorArea> color(const AreaRef& area, const RGBColor& color) const
    {
        return Qt_ColorArea::create(area, color);
    }
    virtual SmartPtr<InkArea> ink(const AreaRef& area) const
    {
        return Qt_InkArea::create(area);
    }
    virtual AreaRef background(const AreaRef& area, const RGBColor& color) const
    {
        return Qt_BackgroundArea::create(area, color);
    }

    // new methods

    virtual SmartPtr<Qt_GlyphArea> qtGlyph(unsigned glyph, QRawFont& rawFont, qreal size) const
    {
        return Qt_GlyphArea::create(glyph, rawFont, size);
    }
    virtual SmartPtr<Qt_WrapperArea> wrapper(const AreaRef& area, const BoundingBox& box, const SmartPtr<class Element>& el) const
    {
        return Qt_WrapperArea::create(area, box, el);
    }
};

#endif // __Qt_AreaFactory_hh__
