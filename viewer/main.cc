// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
// 
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "guiGTK.h"
#include "Parser.hh"
#include "String.hh"
#include "MathEngine.hh"
#include "MathMLTokenElement.hh"

enum CommandLineOptionId {
  OPTION_VERSION = 256,
  OPTION_HELP,
  OPTION_VERBOSE
};

// global options
bool entityWarning = false;

static char appName[64];

extern void *parseMathMLFile(char *);

static void printVersion()
{
#ifdef HAVE_IOSTREAM
  cout << appName << " - written by Luca Padovani (C) 2000." << endl;
#ifdef DEBUG
  cout << "Compiled " << __DATE__ << ' ' << __TIME__ << endl;
#endif
#else
  printf("%s - written by Luca Padovani (C) 2000.\n", appName);
#ifdef DEBUG
  printf("Compiled %s %s\n", __DATE__, __TIME__);
#endif
#endif
  exit(0);
}

static void
printHelp()
{
  static char* helpMsg = "\
Usage: viewer [options] file ...\n\n\
  -v, --version                 Output version information\n\
  -h, --help                    This small usage guide\n\
  --verbose[=0-3]               Display messages\n\
";

#ifdef HAVE_IOSTREAM
  cout << helpMsg << endl;
#else
  printf("%s\n", helpMsg);
#endif

  exit(0);
}

int
main(int argc, char *argv[])
{
  sprintf(appName, "MathML Viewer v%s", VERSION);

  while (TRUE) {
    int option_index = 0;
    static struct option long_options[] =
    {
      { "version", 	 no_argument, NULL, OPTION_VERSION },
      { "help",    	 no_argument, NULL, OPTION_HELP },
      { "verbose",       optional_argument, NULL, OPTION_VERBOSE },

      { NULL,            no_argument, NULL, 0 }
    };

    int c = getopt_long(argc, argv, "vh", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
    case OPTION_VERSION:
    case 'v':
      printVersion();
      break;

    case OPTION_HELP:
    case 'h':
      printHelp();
      break;

    case OPTION_VERBOSE:
      if (optarg == NULL) MathEngine::logger.SetLogLevel(0);
      else MathEngine::logger.SetLogLevel(*optarg - '0');
      break;

    case '?':
      break;

    default:
#ifdef HAVE_IOSTREAM
      cout << "*** getopt returned " << c << " value" << endl;
#else
      printf("*** getopt returned %o value\n", c);
#endif
      break;
    }
  }

  if (optind < argc) {
    GUI_init(&argc, &argv, appName, 500, 500);

    if (GUI_load_document(argv[optind]) < 0)
#ifdef HAVE_IOSTREAM
      cout << "viewer: fatal error: cannot load document `" << argv[optind] << '\'' << endl;
#else      
      printf("viewer: fatal error: cannot load document `%s'\n", argv[optind]);
#endif
    else {
      GUI_run();
      GUI_uninit();
      GUI_unload_document();
    }
  } else printHelp();

  exit(0);
}
