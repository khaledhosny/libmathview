// Copyright (C) 2015 Yue Liu <yue.liu@mail.com>.
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

#ifndef QMathView_hh
#define QMathView_hh

#include <QSizeF>
#include <QLoggingCategory>
#include <QColor>
#include <QtGlobal>

class QMathViewPrivate;
class QPainter;

class QMathView
{
public:
#if QT_VERSION >= 0x050300
    QMathView(const QFont& font, const QLoggingCategory& category);
#endif
    QMathView(const QFont& font, QLoggingCategory& category);
    ~QMathView();
    void loadURI(const char* mathml_url);
    void loadBuffer(const char* mathml_buf);
    void setForegroundColor(const QColor& color);
    void setBackgroundColor(const QColor& color);
    void setFont(const QFont& font);
    void update(QPainter* painter);
    QSizeF sizeF() const;

private:
    QMathViewPrivate* const d;
};

#endif // QMathView_hh
