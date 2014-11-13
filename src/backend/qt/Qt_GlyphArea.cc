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
#include <QRawFont>
#include <QRect>
#include <cstdio>

#include "Qt_GlyphArea.hh"
#include "Qt_RenderingContext.hh"

Qt_GlyphArea::Qt_GlyphArea(unsigned glyph, const QRawFont& templateRawFont, qreal size)
    : m_rawFont(templateRawFont)
{
    m_glyphRun.setGlyphIndexes(QVector<quint32>() << glyph);
    m_glyphRun.setPositions(QVector<QPointF>() << QPointF(0,0));
    m_rawFont.setPixelSize(size);
    m_glyphRun.setRawFont(m_rawFont);
}

Qt_GlyphArea::~Qt_GlyphArea()
{
}

BoundingBox
Qt_GlyphArea::box() const
{
    QRectF rect = m_glyphRun.boundingRect();
    QVector<QPointF> advances = m_glyphRun.rawFont().advancesForGlyphIndexes(m_glyphRun.glyphIndexes());
    qreal baseline = m_glyphRun.positions()[0].y();
    return BoundingBox(Qt_RenderingContext::fromQtPixels(advances.at(0).x()),
                       Qt_RenderingContext::fromQtPixels(baseline - rect.top()),
                       Qt_RenderingContext::fromQtPixels(rect.bottom() - baseline));
}

scaled
Qt_GlyphArea::leftEdge() const
{
    QRectF rect = m_glyphRun.boundingRect();
    return Qt_RenderingContext::fromQtPixels(rect.left());
}

scaled
Qt_GlyphArea::rightEdge() const
{
    QRectF rect = m_glyphRun.boundingRect();
    return Qt_RenderingContext::fromQtPixels(rect.right());
}

void
Qt_GlyphArea::render(RenderingContext& c, const scaled& x, const scaled& y) const
{
    Qt_RenderingContext& context = dynamic_cast<Qt_RenderingContext&>(c);
    context.draw(x, y, m_glyphRun);
}
