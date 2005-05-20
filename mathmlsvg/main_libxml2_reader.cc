// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include <cassert>
#include <fstream>

#if defined(HAVE_GETOPT_H) || defined(HAVE_HIDDEN_GETOPT)
#include <getopt.h>
#else
#include <../getopt/getopt.h>
#endif

#include "Logger.hh"

#include "Init.hh"
#include "Configuration.hh"
#include "libxml2_reader_MathView.hh"
#include "MathMLOperatorDictionary.hh"
#include "SVG_Backend.hh"
#include "SVG_MathGraphicDevice.hh"
#include "SVG_StreamRenderingContext.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#if ENABLE_BOXML
#include "BoxGraphicDevice.hh"
#endif // ENABLE_BOXML

typedef libxml2_reader_MathView MathView;

static double width = 21;
static double height = 29.7;
static Length::Unit unitId = Length::CM_UNIT;
static double xMargin = 2;
static double yMargin = 2;
static double fontSize = 10;
static bool   cropping = true;
static bool   cutFileName = true;
static const char* configPath = NULL;
static int logLevel = LOG_ERROR;
static bool logLevelSet = false;

enum CommandLineOptionId {
  OPTION_VERSION = 256,
  OPTION_HELP,
  OPTION_VERBOSE,
  OPTION_SIZE,
  OPTION_UNIT,
  OPTION_MARGINS,
  OPTION_FONT_SIZE,
  OPTION_CROP,
  OPTION_CUT_FILENAME,
  OPTION_CONFIG
};

static void
printVersion()
{
  std::cout << "MathML to SVG converter - written by Luca Padovani (C) 2005" << std::endl
	    << "Based on GtkMathView " << VERSION << std::endl;
#ifdef DEBUG
  std::cout << "Compiled " << __DATE__ << " " << __TIME__ << std::endl;
#endif
  exit(0);
}

static void
printHelp()
{
  static char* helpMsg = "\
Usage: mathmlsvg [options] file ...\n\n\
  -v, --version                   Output version information\n\
  -h, --help                      This small usage guide\n\
  -u, --unit=<unit>               Unit for dimensions (default='cm')\n\
  -g, --size=<float>x<float>      Page size (width x height) (default = 21 x 29.7)\n\
  -m, --margins=<float>x<float>   Margins (top x left) (default = 2 x 2)\n\
  -f, --font-size=<float>         Default font size (in pt, default=10)\n\
  -r, --crop[=yes|no]             Enable/disable cropping to bounding box (default='yes')\n\
      --cut-filename[=yes|no]     Cut the prefix dir from the output file (default='yes')\n\
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

  std::cout << helpMsg << std::endl;
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
    Length::Unit id;
  } unit[] = {
    { "mm", Length::MM_UNIT },
    { "cm", Length::CM_UNIT },
    { "in", Length::IN_UNIT },
    { "pt", Length::PT_UNIT },
    { "pc", Length::PC_UNIT },
    { "px", Length::PX_UNIT },
    { NULL, Length::UNDEFINED_UNIT }
  };

  unsigned i;
  for (i = 0; unit[i].name != NULL && strcmp(unit[i].name, s); i++) ;

  if (unit[i].name == NULL) return false;

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
  if (cutFileName && slash != NULL) in = slash + 1;

  if (dot == NULL) {
    out = new char[strlen(in) + 5];
    strcpy(out, in);
  } else {
    out = new char[strlen(in) - strlen(dot) + 5];
    strncpy(out, in, strlen(in) - strlen(dot));
    out[strlen(in) - strlen(dot)] = '\0';
  }

  strcat(out, ".svg");

  return out;
}

int
main(int argc, char* argv[])
{
  while (true)
    {
      int option_index = 0;
      static struct option long_options[] =
	{
	  { "version", 	       no_argument,       NULL, OPTION_VERSION },
	  { "help",    	       no_argument,       NULL, OPTION_HELP },
	  { "verbose",         optional_argument, NULL, OPTION_VERBOSE },
	  { "size",            required_argument, NULL, OPTION_SIZE },
	  { "unit",            required_argument, NULL, OPTION_UNIT },
	  { "margins",         required_argument, NULL, OPTION_MARGINS },
	  { "font-size",       required_argument, NULL, OPTION_FONT_SIZE },
	  { "crop",            optional_argument, NULL, OPTION_CROP },
	  { "cut-filename",    optional_argument, NULL, OPTION_CUT_FILENAME },
	  { "config",          required_argument, NULL, OPTION_CONFIG },

	  { NULL,              no_argument, NULL, 0 }
	};

      int c = getopt_long(argc, argv, "vhg:u:m:f:r::", long_options, &option_index);

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
	if (optarg && (*optarg < '0' || *optarg > '3')) printHelp();
	else if (optarg == NULL) logLevel = LOG_ERROR;
	else logLevel = *optarg - '0';
	logLevelSet = true;
	break;

      case OPTION_FONT_SIZE:
      case 'f':
	if (optarg == NULL) printHelp();
	fontSize = atof(optarg);
	break;

      case OPTION_CROP:
      case 'r':
	if (optarg == NULL) cropping = true;
	else if (!parseBoolean(optarg, cropping)) parseError("crop");
	break;

      case OPTION_CUT_FILENAME:
	if (optarg == NULL) cutFileName = true;
	else if (!parseBoolean(optarg, cutFileName)) parseError("cut-filename");
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

  if (configPath == NULL) configPath = getenv("GTKMATHVIEWCONF");

  if (optind >= argc)
    {
      printHelp();
      exit(1);
    }

  SmartPtr<AbstractLogger> logger = Logger::create();
  logger->setLogLevel(LogLevelId(logLevel));
  SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, configPath);
  if (logLevelSet) logger->setLogLevel(LogLevelId(logLevel));
  SmartPtr<Backend> backend = SVG_Backend::create(logger, configuration);
  SmartPtr<MathGraphicDevice> mgd = backend->getMathGraphicDevice();
  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary<MathView>(logger, configuration);

  logger->out(LOG_INFO, "Font size : %f", fontSize);
  logger->out(LOG_INFO, "Paper size: %fx%f", width, height);
  logger->out(LOG_INFO, "Margins   : %fx%f", xMargin, yMargin);

  SmartPtr<MathView> view = MathView::create();
  view->setLogger(logger);
  view->setOperatorDictionary(dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, mgd));
  view->setDefaultFontSize(static_cast<unsigned>(fontSize));

#if ENABLE_BOXML
  FormattingContext context(mgd, 0);
#else
  FormattingContext context(mgd);
#endif
  const scaled widthS = mgd->evaluate(context, Length(width, unitId), scaled::zero());
  const scaled heightS = mgd->evaluate(context, Length(height, unitId), scaled::zero());
  const scaled xMarginS = mgd->evaluate(context, Length(xMargin, unitId), scaled::zero());
  const scaled yMarginS = mgd->evaluate(context, Length(yMargin, unitId), scaled::zero());

  view->setAvailableWidth(widthS - xMarginS * 2);

  while (optind < argc)
    {
      logger->out(LOG_INFO, "Processing `%s'...", argv[optind]);

      char* outName = getOutputFileName(argv[optind]);
      assert(outName != NULL);
      xmlTextReaderPtr reader = xmlNewTextReaderFilename(argv[optind]);

      assert(reader);
      view->loadReader(reader);
      const BoundingBox box = view->getBoundingBox();

      std::ofstream os(outName);
      SVG_StreamRenderingContext rc(logger, os);
      if (cropping)
	{
	  rc.documentStart(box.horizontalExtent(), box.verticalExtent());
	  view->render(rc, 0, -box.height);
	}
      else
	{
	  rc.documentStart(widthS, heightS);
	  view->render(rc, xMarginS, -(yMarginS + box.height));
	}
      rc.documentEnd();
      view->resetRootElement();
      os.close();
      // WARNING: currently the text reader is freed by libxmlXmlReader
      // not sure this is what we want
      //xmlFreeTextReader(reader);

      optind++;
    }

  return 0;
}
