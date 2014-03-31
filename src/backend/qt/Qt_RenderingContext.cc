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

#include <cassert>
#include <QGlyphRun>
#include <QColor>
#include <QPointF>
#include <QRectF>
#include <QPainter>
#include <QDebug>
#include "Qt_RenderingContext.hh"

Qt_RenderingContext::Qt_RenderingContext(void)
    : style(NORMAL_STYLE)
{ }

Qt_RenderingContext::~Qt_RenderingContext()
{ }

void
Qt_RenderingContext::fill(const scaled& x, const scaled& y, const BoundingBox& box) const
{
    m_painter->save();

    RGBColor fg = getForegroundColor();
    m_painter->fillRect(QRectF(toQtX(x), toQtY(y+box.height),
                        toQtPixels(box.width), toQtPixels(box.height+box.depth)),
                        QColor(fg.red, fg.green, fg.blue, fg.alpha));
    qDebug() << "rect " << "\n" << "QRectF: "
             << QRectF(toQtX(x), toQtY(y+box.height), toQtPixels(box.width), toQtPixels(box.height+box.depth));
    m_painter->restore();
}

void
Qt_RenderingContext::draw(const scaled& x, const scaled& y, const QGlyphRun& glyphs) const
{
    m_painter->save();

    RGBColor fg = getForegroundColor();
    m_painter->setPen(QColor(fg.red, fg.green, fg.blue, fg.alpha));
    m_painter->drawGlyphRun(QPointF(toQtX(x), toQtY(y)), glyphs);
    qDebug() << "glyph " << glyphs.glyphIndexes()[0] << "\n"
             << "x: " << toQtX(x) << "\ty: " << toQtY(y)
             << "QRectF: " << glyphs.boundingRect()
             << "Size: " << glyphs.rawFont().pixelSize();

    m_painter->restore();
}
