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

#include "QMathView.hh"

#include "defs.h"
#include "libxml2_MathView.hh"
#include "AbstractLogger.hh"
#include "MathMLOperatorDictionary.hh"
#include "Qt_Backend.hh"
#include "MathGraphicDevice.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#include "Qt_RenderingContext.hh"
#include "RGBColor.hh"

#include <QPainter>
#include <QRawFont>

typedef libxml2_MathView MathView;

class QMathViewLogger : public AbstractLogger
{
public:
#if QT_VERSION >= 0x050300
    QMathViewLogger(const QLoggingCategory& category)
    : AbstractLogger()
    , m_category(category)
    {}
#endif
    QMathViewLogger(QLoggingCategory& category)
        : AbstractLogger()
        , m_category(category)
    {}
    ~QMathViewLogger(){}

protected:
    virtual void outString(const String& str) const {
        QString log = QString::fromStdString(str);
        if (m_category.isCriticalEnabled() && log.startsWith("[MathView] *** Error")) {
            qCCritical(m_category) << log;
        } else if (m_category.isWarningEnabled() && log.startsWith("[MathView] *** Warning")) {
            qCWarning(m_category) << log;
#if QT_VERSION >= 0x050500
        } else if (m_category.isInfoEnabled() && log.startsWith("[MathView] *** Info")) {
            qCInfo(m_category) << log;
#else
        } else if (m_category.isDebugEnabled() && log.startsWith("[MathView] *** Info")) {
            qCDebug(m_category) << log;
#endif
        } else if (m_category.isDebugEnabled() && log.startsWith("[MathView] *** Debug")) {
            qCDebug(m_category) << log;
        }
    }

private:
#if QT_VERSION >= 0x050300
    const QLoggingCategory& m_category;
#else
    QLoggingCategory& m_category;
#endif
};

class QMathViewPrivate
{
public:
    QMathViewPrivate(){}
    ~QMathViewPrivate(){}

    SmartPtr<Backend> m_backend;
    SmartPtr<MathView> m_view;
    Qt_RenderingContext m_rc;
    QRawFont m_rawFont;
    RGBColor m_foregroundColor;
    RGBColor m_backgroundColor;
};

#if QT_VERSION >= 0x050300
QMathView::QMathView(const QFont& font, const QLoggingCategory& category)
    : d(new QMathViewPrivate())
{
    d->m_rawFont = QRawFont::fromFont(font);
    d->m_backend = Qt_Backend::create(d->m_rawFont);
    d->m_view = MathView::create(new QMathViewLogger(category));
    d->m_view->setOperatorDictionary(MathMLOperatorDictionary::create());
    d->m_view->setMathMLNamespaceContext(MathMLNamespaceContext::create(
        d->m_view, d->m_backend->getMathGraphicDevice()
    ));
    d->m_view->setDefaultFontSize(DEFAULT_FONT_SIZE);
}
#endif
QMathView::QMathView(const QFont& font, QLoggingCategory& category)
    : d(new QMathViewPrivate())
{
    d->m_rawFont = QRawFont::fromFont(font);
    d->m_backend = Qt_Backend::create(d->m_rawFont);
    d->m_view = MathView::create(new QMathViewLogger(category));
    d->m_view->setOperatorDictionary(MathMLOperatorDictionary::create());
    d->m_view->setMathMLNamespaceContext(MathMLNamespaceContext::create(
        d->m_view, d->m_backend->getMathGraphicDevice()
    ));
    d->m_view->setDefaultFontSize(DEFAULT_FONT_SIZE);
}

QMathView::~QMathView()
{
    d->m_view->resetRootElement();
    delete d;
}

void QMathView::loadURI(const char* mathml_url) {
    d->m_view->loadURI(mathml_url);
}

void QMathView::loadBuffer(const char* mathml_buf) {
    d->m_view->loadBuffer(mathml_buf);
}

void QMathView::setForegroundColor(const QColor& color) {
    d->m_foregroundColor = RGBColor(color.red(), color.green(), color.blue(), color.alpha());
    d->m_rc.setForegroundColor(d->m_foregroundColor);
}

void QMathView::setBackgroundColor(const QColor& color) {
    d->m_backgroundColor = RGBColor(color.red(), color.green(), color.blue(), color.alpha());
    d->m_rc.setBackgroundColor(d->m_backgroundColor);
}

void QMathView::setFont(const QFont& font) {
    d->m_rawFont = QRawFont::fromFont(font);
    d->m_backend = Qt_Backend::create(d->m_rawFont);
    d->m_view->setMathMLNamespaceContext(MathMLNamespaceContext::create(
        d->m_view, d->m_backend->getMathGraphicDevice()
    ));
}

void QMathView::update(QPainter* painter) {
    d->m_rc.setPainter(painter);
    d->m_view->render(d->m_rc, scaled::zero(), -d->m_view->getBoundingBox().height);
}

QSizeF QMathView::sizeF() const {
    const BoundingBox box = d->m_view->getBoundingBox();
    qreal width = Qt_RenderingContext::toQtPixels(box.horizontalExtent());
    qreal height = Qt_RenderingContext::toQtPixels(box.verticalExtent());
    //qDebug() << width << height;
    return QSizeF(width, height);
}
