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

#ifndef Qt_RenderArea_hh
#define Qt_RenderArea_hh

#include "libxml2_MathView.hh"

#include "Logger.hh"
#include "MathMLOperatorDictionary.hh"
#include "Qt_Backend.hh"
#include "MathGraphicDevice.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#include "Qt_RenderingContext.hh"

#include <QWidget>
#include <QSize>

typedef libxml2_MathView MathView;

class Qt_RenderArea : public QWidget
{
    Q_OBJECT
public:
    Qt_RenderArea(SmartPtr<AbstractLogger> logger,
               QWidget* parent = 0);
    virtual ~Qt_RenderArea();
    void loadURI(const char* mml_file);

protected:
    void paintEvent(QPaintEvent *event);

private:
    SmartPtr<Backend> m_backend;
    SmartPtr<MathGraphicDevice> m_device;
    SmartPtr<MathMLOperatorDictionary> m_dictionary;
    SmartPtr<MathView> m_view;
    Qt_RenderingContext m_rc;
    QRawFont m_rawFont;
};

#endif // Qt_RenderArea_hh
