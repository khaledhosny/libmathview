// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
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
#include <QRawFont>
#include <QChar>
#include <QString>
#include <QGlyphRun>
#include <QVector>
#include <QPointF>
#include <QDebug>
#include "Qt_GlyphArea.hh"
#include "Qt_Shaper.hh"
#include "Qt_RenderingContext.hh"
#include "ShapingContext.hh"
#include <cstdio>

Qt_Shaper::Qt_Shaper(QRawFont& rawFont,
                     const SmartPtr<MathFont>& mathFont)
    : MathShaper(mathFont)
    , m_rawFont(rawFont)
{}

Qt_Shaper::~Qt_Shaper() {}

unsigned
Qt_Shaper::shapeChar(Char32 ch) const
{
    QString str;
    if (QChar::requiresSurrogates(ch)) {
        str.push_back(QChar::highSurrogate(ch));
        str.push_back(QChar::lowSurrogate(ch));
    } else {
        str.push_back(QChar(ch));
    }
    QVector<quint32> chars = m_rawFont.glyphIndexesForString(str);
    if (chars.size() != 1) {
        qDebug() << "wrong vector size:" << chars.size();
        return 0;
    }
    return chars[0];
}

AreaRef
Qt_Shaper::getGlyphArea(unsigned glyph, const scaled& sp_size) const
{
    qreal size = Qt_RenderingContext::toQtPixels(sp_size);
    return Qt_GlyphArea::create(glyph, m_rawFont, size);
}
