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

#define MATHML2PS_VERSION "0.0.3"

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "Parser.hh"
#include "String.hh"
#include "MathEngine.hh"
#include "CharMapper.hh"
#include "EntitiesTable.hh"
#include "MathMLElement.hh"
#include "MathMLDocument.hh"
#include "PS_DrawingArea.hh"
#include "PS_T1_FontManager.hh"
#include "OperatorDictionary.hh"
#include "RenderingEnvironment.hh"

enum CommandLineOptionId {
  OPTION_VERSION = 256,
  OPTION_HELP,
  OPTION_VERBOSE,
  OPTION_SIZE,
  OPTION_UNIT,
  OPTION_MARGINS,
  OPTION_FONT_SIZE,
  OPTION_DISABLE_COLORS,
  OPTION_CONFIG
};

static char appName[64];

static double width = 21;
static double height = 29.7;
static UnitId unitId = UNIT_CM;
static double xMargin = 2;
static double yMargin = 2;
static double fontSize = 10;
static bool   colors = true;
static const char* configPath = NULL;

extern void* parseMathMLFile(char*);

static void
printVersion()
{
  printf("%s - written by Luca Padovani (C) 2000.\n", appName);
#ifdef DEBUG
  printf("Compiled %s %s\n", __DATE__, __TIME__);
#endif
  exit(0);
}

static void
printHelp()
{
  static char* helpMsg = "\
Usage: mathml2ps [options] file ...\n\n\
  -v, --version                  Output version information\n\
  -h, --help                     This small usage guide\n\
  -u, --unit=[in,mm,cm,pt,pc,px] Unit for dimensions (default='px')\n\
  -g, --size=<width>x<height>    Page size\n\
  -m, --margins=<left>x<top>     Left x Top margins\n\
  -f, --font-size=<n>            Default font size (in points, default=10)\n\
  --disable-colors               Disable colors\n\
  --config=<path>                Configuration file path\n\
  --verbose[=0-3]                Display messages\n\
";

  printf("%s\n", helpMsg);
#ifndef HAVE_LIBT1
  printf("You must compile the source code with support for Type1 font enabled\n\
in order to use this tool!\n\n");
#endif // HAVE_LIBT1
  exit(0);
}

bool
parseSize(const char* s)
{
  assert(s != NULL);

  char* nptr;

  double w = strtod(s, &nptr);
  if (nptr == s) return false;
  if (nptr == NULL || *nptr != 'x') return false;

  s = nptr + 1;
  double h = strtod(s, &nptr);
  if (nptr == s) return false;

  width = w;
  height = h;

  return true;
}

bool
parseUnit(const char* s)
{
  assert(s != NULL);

  struct {
    const char* name;
    UnitId id;
  } unit[] = {
    { "mm", UNIT_MM },
    { "cm", UNIT_CM },
    { "in", UNIT_IN },
    { "pt", UNIT_PT },
    { "pc", UNIT_PC },
    { "px", UNIT_PX },
    { NULL, UNIT_NOTVALID }
  };

  unsigned i;
  for (i = 0; unit[i].name != NULL && strcmp(unit[i].name, s); i++) ;

  if (unit[i].name == NULL) return false;

  MathEngine::logger(LOG_INFO, "Setting units to `%s'\n", unit[i].name);
  unitId = unit[i].id;

  return true;
}

bool
parseMargins(const char* s)
{
  assert(s != NULL);

  char* nptr;

  double x = strtod(s, &nptr);
  if (nptr == s) return false;
  if (nptr == NULL || *nptr != 'x') return false;
  
  s = nptr + 1;
  double y = strtod(s, &nptr);
  if (nptr == s) return false;

  xMargin = x;
  yMargin = y;

  return true;
}

int
main(int argc, char *argv[])
{
  sprintf(appName, "MathML to PostScript v%s", MATHML2PS_VERSION);

  while (TRUE) {
    int option_index = 0;
    static struct option long_options[] =
    {
      { "version", 	 no_argument,       NULL, OPTION_VERSION },
      { "help",    	 no_argument,       NULL, OPTION_HELP },
      { "verbose",       optional_argument, NULL, OPTION_VERBOSE },
      { "size",          required_argument, NULL, OPTION_SIZE },
      { "unit",          required_argument, NULL, OPTION_UNIT },
      { "margins",       required_argument, NULL, OPTION_MARGINS },
      { "font-size",     required_argument, NULL, OPTION_FONT_SIZE },
      { "disable-colors",no_argument,       NULL, OPTION_DISABLE_COLORS },
      { "config",        required_argument, NULL, OPTION_CONFIG },

      { NULL,            no_argument, NULL, 0 }
    };

    int c = getopt_long(argc, argv, "vhg:u:m:f:", long_options, &option_index);

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

    case OPTION_SIZE:
    case 'g':
      if (optarg == NULL) printHelp();
      if (!parseSize(optarg)) fprintf(stderr, "error while parsing size\n");
      break;

    case OPTION_UNIT:
    case 'u':
      if (optarg == NULL) printHelp();
      if (!parseUnit(optarg)) fprintf(stderr, "error while parsing unit\n");
      break;

    case OPTION_MARGINS:
    case 'm':
      if (optarg == NULL) printHelp();
      if (!parseMargins(optarg)) fprintf(stderr, "error while parsing margins\n");
      break;

    case OPTION_VERBOSE:
      if (optarg == NULL) MathEngine::logger.SetLogLevel(0);
      else MathEngine::logger.SetLogLevel(*optarg - '0');
      break;

    case OPTION_FONT_SIZE:
    case 'f':
      if (optarg == NULL) printHelp();
      fontSize = atof(optarg);
      break;

    case OPTION_DISABLE_COLORS:
      colors = false;
      break;

    case OPTION_CONFIG:
      configPath = optarg;
      break;

    case '?':
      break;

    default:
      fprintf(stderr, "*** getopt returned %o value\n", c);
      break;
    }
  }

  MathEngine::logger(LOG_INFO, "Font size : %f", fontSize);
  MathEngine::logger(LOG_INFO, "Paper size: %fx%f", width, height);
  MathEngine::logger(LOG_INFO, "Margins   : %fx%f", xMargin, yMargin);

#ifdef HAVE_LIBT1
  if (optind < argc) {
    MathEngine::InitGlobalData(configPath);

    MathMLParser parser(argv[optind]);
    MathMLDocument* document = parser.Parse();
    document->Normalize();

    if (document != NULL) {
      MathMLElement* root = document->GetRoot();
      assert(root != NULL);

      PS_T1_FontManager fm;
      CharMapper cm(fm);
      cm.Load("/usr/local/share/gtkmathview/font-configuration.xml");
      cm.Load("config/font-configuration.xml");

      UnitValue size;
      size.Set(fontSize, UNIT_PT);
      RenderingEnvironment env(cm);
      env.SetFontSize(size);

      root->Setup(&env);
      root->DoBoxedLayout(LAYOUT_MIN);
      root->DoBoxedLayout(LAYOUT_MAX);

      UnitValue uWidth;
      UnitValue uHeight;
      UnitValue uXMargin;
      UnitValue uYMargin;
      uWidth.Set(width, unitId);
      uHeight.Set(height, unitId);
      uXMargin.Set(xMargin, unitId);
      uYMargin.Set(yMargin, unitId);

      scaled w = uWidth.ToScaledPoints();
      scaled h = uHeight.ToScaledPoints();
      scaled x0 = uXMargin.ToScaledPoints();
      scaled y0 = uYMargin.ToScaledPoints();

      root->DoBoxedLayout(LAYOUT_AUTO, BREAK_GOOD, w - 2 * x0);
      const BoundingBox& box = root->GetBoundingBox();
      root->SetPosition(0, box.ascent);
      root->Freeze();

      GraphicsContextValues values;
      values.foreground = BLACK_COLOR;
      values.background = WHITE_COLOR;
      values.lineStyle = LINE_STYLE_SOLID;
      values.lineWidth = px2sp(1);

      PS_DrawingArea area(values, x0, y0, stdout);
      area.SetSize(w, h);
      if (!colors) area.DisableColors();
      fm.DumpFontDictionary(stdout);

      area.DumpPreamble();
      //area.DumpGrid();
      root->SetDirty();
      root->Render(area);
      area.DumpEpilogue();
    }
  } else
#endif // HAVE_LIBT1
    printHelp();

  exit(0);
}
