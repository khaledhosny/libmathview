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
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif // HAVE_GETOPT_H
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H
#include <stdlib.h>

#include "defs.h"
#include "guiGTK.h"
#include "String.hh"
#include "MathEngine.hh"
#include "MathMLTokenElement.hh"
#ifdef DEBUG
#include "Gtk_GraphicsContext.hh"
#endif // DEBUG
#ifdef ENABLE_PROFILE
#include "CharMapper.hh"
#endif // ENABLE_PROFILE

enum CommandLineOptionId {
  OPTION_VERSION = 256,
  OPTION_DEBUG,
  OPTION_HELP,
  OPTION_VERBOSE
};

// global options
bool entityWarning = false;

static char appName[64];

extern void *parseMathMLFile(char *);

static void printVersion()
{
  printf("%s - written by Luca Padovani (C) 2000-2001.\n", appName);
#ifdef DEBUG
  //printf("Compiled %s %s\n", __DATE__, __TIME__);
#endif // DEBUG
  exit(0);
}

static void
printHelp()
{
  static char* helpMsg = "\
Usage: mathmlviewer [options] file ...\n\n\
  -V, --version                 Output version information\n\
  -d, --debug                   Debug mode\n\
  -h, --help                    This small usage guide\n\
  -v, --verbose[=0-3]           Display messages\n\
";

  printf("%s\n", helpMsg);
}

#ifdef DEBUG
static void
checkCounters()
{
  MathEngine::dictionary.Unload();
  MathEngine::logger(LOG_DEBUG, "Elements : %d", MathMLElement::GetCounter());
  MathEngine::logger(LOG_DEBUG, "GCs      : %d", Gtk_GraphicsContext::GetCounter());
  MathEngine::logger(LOG_DEBUG, "Strings  : %d", String::GetCounter());
  MathEngine::logger(LOG_DEBUG, "Values   : %d (cached %d)", Value::GetCounter(), Value::GetCached());
}
#endif // DEBUG

#ifdef ENABLE_PROFILE
static void
checkProfileCounters()
{
  MathEngine::logger(LOG_DEBUG, "Total : %d", CharMapper::GetChars());
  MathEngine::logger(LOG_DEBUG, "alnum : %d", CharMapper::GetAlnumChars());
}
#endif // ENABLE_PROFILE

int
main(int argc, char *argv[])
{
  bool debugMode = false;

  sprintf(appName, "mathmlviewer v%s", VERSION);

  while (TRUE) {
    int option_index = 0;
    static struct option long_options[] =
    {
      { "version", 	 no_argument, NULL, OPTION_VERSION },
      { "debug",         no_argument, NULL, OPTION_DEBUG },
      { "help",    	 no_argument, NULL, OPTION_HELP },
      { "verbose",       optional_argument, NULL, OPTION_VERBOSE },

      { NULL,            no_argument, NULL, 0 }
    };

    int c = getopt_long(argc, argv, "Vv:dh", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
    case OPTION_VERSION:
    case 'V':
      printVersion();
      break;

    case OPTION_DEBUG:
    case 'd':
      debugMode = true;
      break;

    case OPTION_HELP:
    case 'h':
      printHelp();
      exit(0);
      break;

    case OPTION_VERBOSE:
    case 'v':
      if (optarg == NULL) MathEngine::logger.SetLogLevel(0);
      else MathEngine::logger.SetLogLevel(*optarg - '0');
      break;

    case '?':
      break;

    default:
      printf("*** getopt returned %c value\n", c);
      break;
    }
  }

#ifdef DEBUG
  atexit(checkCounters);
#endif // DEBUG

#ifdef ENABLE_PROFILE
  atexit(checkProfileCounters);
#endif // ENABLE_PROFILE

  if (optind < argc) {
    GUI_init(&argc, &argv, appName, 500, 600);

    while (optind < argc) {

      if (GUI_load_document(argv[optind]) < 0)
	printf("mathmlviewer: fatal error: cannot load document `%s'\n", argv[optind]);

      if (debugMode) optind++;
      else {
	GUI_run();
	break;
      }
    }

    GUI_uninit();
    GUI_unload_document();
  } else printHelp();

  Value::Flush();

  exit(0);
}
