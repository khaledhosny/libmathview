// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

// needed for old versions of GCC, must come before String.hh!
#include "CharTraits.icc"

#include <cassert>
#include <stdlib.h>

#include <popt.h>

#include "defs.h"
#include "guiGTK.h"
#include "String.hh"
#include "MathMLTokenElement.hh"
#ifdef DEBUG
#include "Gtk_GraphicsContext.hh"
#endif // DEBUG
#ifdef ENABLE_PROFILE
#endif // ENABLE_PROFILE

enum CommandLineOptionId {
  OPTION_VERSION = 256,
  OPTION_VERBOSE
};

// global options
bool entityWarning = false;

static char appName[64];

extern void *parseMathMLFile(char *);

static void printVersion()
{
  printf("%s - written by Luca Padovani (C) 2000-2005.\n", appName);
  exit(0);
}

static gint logLevel = -1;
static struct poptOption optionsTable[] = {
  { "version", 'V', POPT_ARG_NONE, 0, OPTION_VERSION, "Output version information", 0 },
  { "verbose", 'v', POPT_ARG_INT, &logLevel, OPTION_VERBOSE, "Display messages", "[0-3]" },
  POPT_AUTOHELP
  { 0, 0, 0, 0, 0, 0, 0 }
};

static void
usage(poptContext optCon, int exitcode, const char* msg)
{
  poptPrintUsage(optCon, stderr, 0);
  if (msg) fprintf(stderr, "%s\n", msg);
  exit(exitcode);
}

int
main(int argc, const char *argv[])
{
  poptContext ctxt = poptGetContext(NULL, argc, argv, optionsTable, 0);
  sprintf(appName, "mathmlviewer v%s", VERSION);

  gint c;
  while ((c = poptGetNextOpt(ctxt)) >= 0)
    {
      switch (c) {
      case OPTION_VERSION:
	printVersion();
	break;
      case OPTION_VERBOSE:
	break;
      default:
	assert(false);
      }
    }

  if (c < -1)
    {
      /* an error occurred during option processing */
      fprintf(stderr, "%s: %s\n",
	      poptBadOption(ctxt, POPT_BADOPTION_NOALIAS),
	      poptStrerror(c));
      return 1;
    }

  const gchar* file = poptGetArg(ctxt);
  if (file == 0 || !(poptPeekArg(ctxt) == 0))
    usage(ctxt, 1, "fatal error: no input document");

  poptFreeContext(ctxt);

  GUI_init(&argc, &argv, appName, 500, 600, logLevel);
  if (GUI_load_document(file) < 0)
    printf("fatal error: cannot load document `%s'\n", file);
  GUI_run();
  GUI_uninit();
  exit(0);
}
