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

#define MATHML2PS_VERSION "0.0.8"

#include <assert.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif // HAVE_GETOPT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
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
  OPTION_COLORS,
  OPTION_SUBSET,
  OPTION_CROP,
  OPTION_SHOW_MISSING,
  //OPTION_KERNING,
  OPTION_CONFIG
};

static char appName[64];

static double width = 21;
static double height = 29.7;
static UnitId unitId = UNIT_CM;
static double xMargin = 2;
static double yMargin = 2;
static double fontSize = 10;
static bool   colors = false;
static bool   subsetting = true;
static bool   cropping = true;
static bool   showMissing = true;
//static bool   kerning = false;
static const char* configPath = NULL;
static int logLevel = LOG_ERROR;

extern void* parseMathMLFile(char*);

static void
printVersion()
{
  printf("%s - written by Luca Padovani (C) 2000 - 2001.\n", appName);
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
  -v, --version                   Output version information\n\
  -h, --help                      This small usage guide\n\
  -u, --unit=<unit>               Unit for dimensions (default='cm')\n\
  -g, --size=<float>x<float>      Page size (width x height) (default = 21 x 29.7)\n\
  -m, --margins=<float>x<float>   Margins (top x left) (default = 2 x 2)\n\
  -f, --font-size=<float>         Default font size (in pt, default=10)\n\
  -k, --kerning[=yes|no]          Enable/disable kerning (default='no')\n\
  -c, --colors[=yes|no]           Enable/disable colors (default='no')\n\
  -s, --subset[=yes|no]           Enable/disable font subsetting (default='yes')\n\
  -r, --crop[=yes|no]             Enable/disable cropping to bounding box (default='yes')\n\
  -i, --show-missing[=yes|no]     Show missing characters (default='yes')\n\
  --config=<path>                 Configuration file path\n\
  --verbose[=0-3]                 Display messages\n\n\
Valid units are:\n\n\
  cm    centimeter\n\
  mm    millimeter\n\
  in    inch (1 in = 2.54 cm)\n\
  pt    point (1 in = 72.27 pt)\n\
  pc    pica (1 pc = 12 pt)\n\
  px    pixel (1 in = 72 px)\n\
";

  printf("%s\n", helpMsg);
#ifndef HAVE_LIBT1
  printf("You must compile the source code with support for Type1 font enabled\n\
in order to use this tool!\n\n");
#endif // HAVE_LIBT1
  exit(0);
}

static void
parseError(const char* option)
{
  assert(option != NULL);
  fprintf(stderr, "error while parsing option `%s'\n\n", option);
  printHelp();
}

static bool
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

static bool
parseBoolean(const char* s, bool& res)
{
  assert(s != NULL);
  if (!strcmp(s, "yes")) {
    res = true;
    return true;
  } else if (!strcmp(s, "no")) {
    res = false;
    return true;
  }

  return false;
}

static bool
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

  MathEngine::logger(LOG_INFO, "Setting units to `%s'", unit[i].name);
  unitId = unit[i].id;

  return true;
}

static bool
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

static char*
getOutputFileName(const char* in)
{
  char* out;

  assert(in != NULL);
  const char* dot = strrchr(in, '.');
  const char* slash = strrchr(in, '/');
  if (slash != NULL) in = slash + 1;

  if (dot == NULL) {
    out = new char[strlen(in) + 5];
    strcpy(out, in);
  } else {
    out = new char[strlen(in) - strlen(dot) + 5];
    strncpy(out, in, strlen(in) - strlen(dot));
    out[strlen(in) - strlen(dot)] = '\0';
  }

  strcat(out, ".eps");

  return out;
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
      { "colors",        optional_argument, NULL, OPTION_COLORS },
      { "subset",        optional_argument, NULL, OPTION_SUBSET },
      { "crop",          optional_argument, NULL, OPTION_CROP },
      { "show-missing",  optional_argument, NULL, OPTION_SHOW_MISSING },
      //{ "kerning",       optional_argument, NULL, OPTION_KERNING },
      { "config",        required_argument, NULL, OPTION_CONFIG },

      { NULL,            no_argument, NULL, 0 }
    };

    int c = getopt_long(argc, argv, "vhg:u:m:f:k::c::s::", long_options, &option_index);

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
      if (!parseSize(optarg)) parseError("size");
      break;

    case OPTION_UNIT:
    case 'u':
      if (optarg == NULL) printHelp();
      if (!parseUnit(optarg)) parseError("unit");
      break;

    case OPTION_MARGINS:
    case 'm':
      if (optarg == NULL) printHelp();
      if (!parseMargins(optarg)) parseError("margins");
      break;

    case OPTION_VERBOSE:
      if (optarg == NULL) logLevel = LOG_ERROR;
      else logLevel = *optarg - '0';
      break;

    case OPTION_FONT_SIZE:
    case 'f':
      if (optarg == NULL) printHelp();
      fontSize = atof(optarg);
      break;

    case OPTION_COLORS:
    case 'c':
      if (optarg == NULL) colors = true;
      else if (!parseBoolean(optarg, colors)) parseError("colors");
      break;

    case OPTION_SUBSET:
    case 's':
      if (optarg == NULL) subsetting = true;
      else if (!parseBoolean(optarg, subsetting)) parseError("subset");
      break;

    case OPTION_CROP:
    case 'r':
      if (optarg == NULL) cropping = true;
      else if (!parseBoolean(optarg, cropping)) parseError("crop");
      break;

    case OPTION_SHOW_MISSING:
    case 'i':
      if (optarg == NULL) showMissing = true;
      else if (!parseBoolean(optarg, showMissing)) parseError("show-missing");
      break;

#if 0
    case OPTION_KERNING:
    case 'k':
      if (optarg == NULL) kerning = true;
      else if (!parseBoolean(optarg, kerning)) parseError("kerning");
      break;
#endif

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

  if (configPath == NULL) configPath = getenv("MATHENGINECONF");
  
  MathEngine::logger.SetLogLevel(logLevel);
  MathEngine::logger(LOG_INFO, "Font size : %f", fontSize);
  MathEngine::logger(LOG_INFO, "Paper size: %fx%f", width, height);
  MathEngine::logger(LOG_INFO, "Margins   : %fx%f", xMargin, yMargin);

#ifdef HAVE_LIBT1
  if (optind >= argc) {
    printHelp();
    exit(1);
  }
  
  MathEngine engine;
  MathEngine::InitGlobalData(configPath);

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

  GraphicsContextValues values;
  values.foreground = BLACK_COLOR;
  values.background = WHITE_COLOR;
  values.lineStyle = LINE_STYLE_SOLID;
  values.lineWidth = px2sp(1);

  PS_DrawingArea area(values, x0, y0);
  area.SetSize(w, h);
  if (!colors) area.DisableColors();

  PS_T1_FontManager fm;
  engine.Init(&area, &fm);

  while (optind < argc) {
    MathEngine::logger(LOG_INFO, "Processing `%s'...", argv[optind]);

    char* outName = getOutputFileName(argv[optind]);
    assert(outName != NULL);

    FILE* outFile = fopen(outName, "wt");
    if (outFile == NULL) {
      MathEngine::logger(LOG_ERROR, "could not open output file `%s' for writing", outName);
      exit(1);
    }

    area.SetOutputFile(outFile);

    engine.SetVerbosity(logLevel);
    engine.SetDefaultFontSize(fontSize);
    MathEngine::DrawMissingCharacter(showMissing);
#if 0
    engine.SetKerning(kerning);
#endif

    fm.ResetUsedFonts();
    if (engine.Load(argv[optind])) {
      engine.Layout();

      Rectangle rect;
      engine.GetDocumentRectangle(rect);

      Rectangle sheet;
      sheet.x = 0;
      sheet.y = 0;
      sheet.width = w;
      sheet.height = h;

      // the following invocations are needed just to mark the chars actually used :(
      fm.ResetUsedChars();
      area.SetOutputFile(NULL);
      engine.Render(&sheet);
      area.SetOutputFile(outFile);

      area.DumpHeader(appName, argv[optind], cropping ? rect : sheet);
      fm.DumpFontDictionary(outFile, subsetting);

      area.DumpPreamble();
      //area.DumpGrid();
      engine.Render(&sheet);
      area.DumpEpilogue();
      fclose(outFile);
    } else {
      fclose(outFile);
      remove(outName);
      fprintf(stderr, "cannot open input document `%s' (skipped)\n", argv[optind]);
    }

    delete [] outName;

    optind++;
  }
#else 
  printHelp();
#endif

  exit(0);
}
