// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
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

#include <config.h>

#include "Qt_RenderArea.hh"
#include "Logger.hh"

#include <QApplication>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QVBoxLayout>

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("mml-view-qt");
    QApplication::setApplicationVersion(PACKAGE_VERSION);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("input", "MathML file to view.");
    parser.process(a);

    const QStringList args = parser.positionalArguments();

    const QString input_file = args.at(0);

    SmartPtr<AbstractLogger> logger = Logger::create();

    QMainWindow w;
    Qt_RenderArea* ra = new Qt_RenderArea(logger);
    ra->loadURI(input_file.toUtf8());
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ra);
    QWidget* window = new QWidget();
    window->setLayout(layout);
    w.setCentralWidget(window);
    w.show();
    return a.exec();
}
