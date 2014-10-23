// Copyright (C) 2014, Yue Liu <yue.liu@mail.com>.
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

#include "Qt_RenderArea.hh"
#include <QPainter>
#include <QRawFont>
#include <QDebug>

Qt_RenderArea::Qt_RenderArea(SmartPtr<AbstractLogger> logger,
                       QWidget* parent)
    : QWidget(parent)
{
    m_rawFont = QRawFont::fromFont(QFont(DEFAULT_FONT_FAMILY, DEFAULT_FONT_SIZE));
    m_backend = Qt_Backend::create(m_rawFont);
    m_device = m_backend->getMathGraphicDevice();
    m_dictionary = MathMLOperatorDictionary::create();
    m_view = MathView::create(logger);
    m_view->setOperatorDictionary(m_dictionary);
    m_view->setMathMLNamespaceContext(MathMLNamespaceContext::create(m_view, m_device));
    m_view->setDefaultFontSize(DEFAULT_FONT_SIZE);
}

Qt_RenderArea::~Qt_RenderArea()
{
    m_view->resetRootElement();
}

void Qt_RenderArea::loadURI(const char* mml_file) {
    m_view->loadURI(mml_file);
    const BoundingBox box = m_view->getBoundingBox();
    qreal width = Qt_RenderingContext::toQtPixels(box.horizontalExtent());
    qreal height = Qt_RenderingContext::toQtPixels(box.verticalExtent());
    qDebug() << width << height;
    setMinimumSize(QSizeF(width, height).toSize());
    repaint();
}

void Qt_RenderArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    m_rc.setPainter(&painter);
    m_view->render(m_rc, scaled::zero(), -m_view->getBoundingBox().height);
}
