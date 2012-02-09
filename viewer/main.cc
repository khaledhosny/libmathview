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

// needed for old versions of GCC, must come before String.hh!
#include "CharTraits.icc"

#include <cassert>
#include <stdlib.h>
#include <stdio.h>

#include "defs.h"
#include "guiGTK.h"
#include "String.hh"
#include "MathMLTokenElement.hh"
#ifdef DEBUG
#include "Gtk_GraphicsContext.hh"
#endif // DEBUG
#ifdef ENABLE_PROFILE
#endif // ENABLE_PROFILE

static gboolean option_version = FALSE;
static gint option_verbose = -1;

static GOptionEntry optionEntries[] = {
  { "version", 'V', 0, G_OPTION_ARG_NONE, &option_version, "Output version information", 0 },
  { "verbose", 'v', 0, G_OPTION_ARG_INT, &option_verbose, "Display messages", "[0-3]" },
  { NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
};

static char appName[64];

extern void *parseMathMLFile(char *);

static void printVersion()
{
  printf("%s - written by Luca Padovani (C) 2000-2007.\n", appName);
  exit(0);
}

static void
usage(GOptionContext* ctxt, int exitcode, const char* msg)
{
  gchar* help = g_option_context_get_help(ctxt, TRUE, NULL);
  fprintf(stderr, "%s", help);
  if (msg) fprintf(stderr, "%s\n", msg);
  g_free(help);
  exit(exitcode);
}

int
main(int argc, char* argv[])
{
  GOptionContext* ctxt = g_option_context_new("<file> - display MathML documents");
  g_option_context_add_main_entries(ctxt, optionEntries, NULL);
  g_option_context_set_help_enabled(ctxt, TRUE);
  if (!g_option_context_parse(ctxt, &argc, &argv, NULL))
    usage(ctxt, 1, "error parsing options");

  sprintf(appName, "%s v%s", g_get_prgname(), VERSION);

  if (option_version)
    printVersion();

  if (argc <= 1) usage(ctxt, 1, "fatal error: no input document");

  g_option_context_free(ctxt);

  const gchar* file = argv[1];
  GUI_init(&argc, &argv, appName, 500, 600, option_verbose);
  if (GUI_load_document(file) < 0)
    printf("fatal error: cannot load document `%s'\n", file);
  GUI_run();
  GUI_uninit();
  exit(0);
}
