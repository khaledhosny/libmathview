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

#ifndef __Qt_RenderingContext_hh__
#define __Qt_RenderingContext_hh__

#include "SmartPtr.hh"
#include "Rectangle.hh"
#include "RenderingContext.hh"

#include <QtGlobal>

class QPainter;
class QGlyphRun;

class Qt_RenderingContext : public RenderingContext
{
public:
    Qt_RenderingContext();
    virtual ~Qt_RenderingContext();

    void setPainter(QPainter* p) {
        m_painter = p;
    }
    QPainter* painter() const {
        return m_painter;
    }

    void fill(const scaled&, const scaled&, const BoundingBox&) const;
    void draw(const scaled&, const scaled&, const QGlyphRun& glyphs) const;

    static qreal toQtPixels(const scaled& s)
    {
        return s.toDouble();
    }
    static scaled fromQtPixels(qreal s)
    {
        return scaled((double)s);
    }

    static qreal toQtX(const scaled& x)
    {
        return toQtPixels(x);
    }
    static qreal toQtY(const scaled& y)
    {
        return toQtPixels(-y);
    }

    static scaled fromQtX(qreal x)
    {
        return fromQtPixels(x);
    }
    static scaled fromQtY(qreal y)
    {
        return fromQtPixels(-y);
    }

private:
    QPainter* m_painter;
};

#endif // __Qt_RenderingContext_hh__
