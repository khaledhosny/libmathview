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
#include "Init.hh"
#include "Logger.hh"
#include "Configuration.hh"

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
static char **remaining_args = NULL;
static GOptionEntry entries[] = {
    { G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_FILENAME_ARRAY, &remaining_args, NULL, "[FILE...]" },
    { NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
};

int
main(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context = g_option_context_new(NULL);
    g_option_context_add_main_entries(context, entries, NULL);

    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_print("option parsing failed: %s\n", error->message);
        exit(1);
    }

    if (remaining_args == NULL)
    {
        g_print("no input file specified\n");
        exit(1);
    }

    const char* input_file = remaining_args[0];

    const char* configPath = getenv("GTKMATHVIEWCONF");
    SmartPtr<AbstractLogger> logger = Logger::create();
    SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, configPath);

    QApplication a( argc, argv );
    QMainWindow w;
    Qt_RenderArea* ra = new Qt_RenderArea(logger, configuration);
    ra->loadURI(input_file);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ra);
    QWidget* window = new QWidget();
    window->setLayout(layout);
    w.setCentralWidget(window);
    w.show();
    return a.exec();
}
