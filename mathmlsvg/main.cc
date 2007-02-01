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

#include <glib.h>

// needed for old versions of GCC, must come before String.hh!
#include "CharTraits.icc"

#include "Logger.hh"

#include "Init.hh"
#include "Configuration.hh"
#include "libxml2_MathView.hh"
#include "MathMLOperatorDictionary.hh"
#include "SVG_Backend.hh"
#include "SVG_MathGraphicDevice.hh"
#include "SVG_libxml2_StreamRenderingContext.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#if GMV_ENABLE_BOXML
#include "BoxMLNamespaceContext.hh"
#include "BoxGraphicDevice.hh"
#endif // GMV_ENABLE_BOXML
#include "SMS.hh"
#include "Fragment.hh"

typedef libxml2_MathView MathView;

static double width = 21;
static double height = 29.7;
static Length::Unit unitId = Length::CM_UNIT;
static double xMargin = 2;
static double yMargin = 2;
static gint fontSize = DEFAULT_FONT_SIZE;
static gboolean cropping = TRUE;
static gboolean cutFileName = TRUE;
static gchar* configS = 0;
static gchar* unitS = 0;
static gchar* pageSizeS = 0;
static gchar* marginsS = 0;
static gint logLevel = LOG_ERROR;
static gboolean logLevelSet = FALSE;
static gboolean extendedSVG = FALSE;
static gboolean version = FALSE;
static gchar** filenameA = 0;

static void
printVersion()
{
  std::cout << "MathML to SVG converter - written by Luca Padovani (C) 2007" << std::endl
	    << "Based on GtkMathView " << VERSION << std::endl;
#ifdef DEBUG
  std::cout << "Compiled " << __DATE__ << " " << __TIME__ << std::endl;
#endif
  exit(0);
}

static gboolean
cropCB(const gchar* name, const gchar* value, gpointer, GError**)
{
  std::cout << "option " << name << " = " << value << std::endl;
}

static GOptionEntry optionsTable[] = {
  { "version", 'V', 0, G_OPTION_ARG_NONE, &version, "Output version information", 0 },
  { "verbose", 'v', 0, G_OPTION_ARG_INT, &logLevel, "Display messages", "[0-3]" },
  { "unit", 'u', 0, G_OPTION_ARG_STRING, &unitS, "Unit for dimensions (default='cm')", "<unit>" },
  { "page-size", 'p', 0, G_OPTION_ARG_STRING, &pageSizeS, "Page size (width x height) (default = 21 x 29.7)", "<float>x<float>" },
  { "margins", 'm', 0, G_OPTION_ARG_STRING, &marginsS, "Margins (top x left) (default = 2 x 2)", "<float>x<float>" },
  { "font-size", 'f', 0, G_OPTION_ARG_INT, &fontSize, "Default font size (in pt, default=10)", "<float>" },
  { "config", 0, 0, G_OPTION_ARG_FILENAME, &configS, "Configuration file path", "<path>" },
  { "crop", 'r', 0, G_OPTION_ARG_CALLBACK, (gpointer) &cropCB, "Enable/disable cropping to bounding box (default='yes')", "[yes,no]" },

#if 0
  { "cut-filename", G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_CALLBACK, cutFilenameCB, "Cut the prefix dir from the output file (default='yes')", "[yes,no]" },

  { "extended-svg", 'x', G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_CALLBACK, extendedSVGCB, "Extended SVG processing (default='no')", "[yes,no]" },
#endif

  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &filenameA, 0, 0 },
  { 0, 0, 0, G_OPTION_ARG_NONE, 0, 0, 0 }
};

static void
usage(const char* msg, const char* arg)
{
  fprintf(stderr, "\
Valid units are:\n\n\
  cm    centimeter\n\
  mm    millimeter\n\
  in    inch (1 in = 2.54 cm)\n\
  pt    point (1 in = 72.27 pt)\n\
  pc    pica (1 pc = 12 pt)\n\
  px    pixel (1 in = 72 px)\n\
");
  if (msg && arg) fprintf(stderr, "%s %s\n", msg, arg);
  exit(0);
}

#if 0
static void
parseError(poptContext optCon, const char* option)
{
  assert(option != NULL);
  usage(optCon, 1, "error while parsing option", option);
}
#endif

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
parseBoolean(const String& s, bool& res)
{
  std::cout << "option = " << s << std::endl;
  if (s == "yes") {
    res = true;
    return true;
  } else if (s == "no") {
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
  GError* error = 0;
  GOptionContext* ctxt = g_option_context_new("<filename> ...");
  g_option_context_set_help_enabled(ctxt, TRUE);
  g_option_context_add_main_entries(ctxt, optionsTable, 0);
  g_option_context_parse(ctxt, &argc, &argv, &error);

#if 0  
  int c;
  while ((c = poptGetNextOpt(ctxt)) >= 0)
    {
      const char* arg = poptGetOptArg(ctxt);
      switch (c)
	{
	case OPTION_VERSION:
	  printVersion();
	  break;
	case OPTION_VERBOSE:
	  if (logLevel < 0 || logLevel > 3) parseError(ctxt, "verbose");
	  logLevelSet = true;
	  break;
	case OPTION_PAGE_SIZE:
	  assert(arg != 0);
	  if (!parseSize(arg)) parseError(ctxt, "size");
	  break;
	case OPTION_UNIT:
	  assert(arg != 0);
	  if (!parseUnit(arg)) parseError(ctxt, "unit");
	  break;
	case OPTION_MARGINS:
	  assert(arg != 0);
	  if (!parseMargins(arg)) parseError(ctxt, "margins");
	  break;
	case OPTION_FONT_SIZE:
	  break;
	case OPTION_CROP:
	  if (arg == 0) cropping = true;
	  else if (!parseBoolean(arg, cropping)) parseError(ctxt, "crop");
	  break;
	case OPTION_CUT_FILENAME:
	  if (arg == 0) cutFileName = true;
	  else if (!parseBoolean(arg, cutFileName)) parseError(ctxt, "cut-filename");
	  break;
	case OPTION_CONFIG:
	  assert(arg != 0);
	  configPath = strdup(arg);
	  break;
	case OPTION_XSVG:
	  if (arg == 0) extendedSVG = true;
	  else if (!parseBoolean(arg, extendedSVG)) parseError(ctxt, "extended-svg");
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
#endif

  if (configS == 0) configS = getenv("GTKMATHVIEWCONF");

  SmartPtr<AbstractLogger> logger = Logger::create();
  logger->setLogLevel(LogLevelId(logLevel));
  SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, configS);
  if (logLevelSet) logger->setLogLevel(LogLevelId(logLevel));
  SmartPtr<Backend> backend = SVG_Backend::create(logger, configuration);
  SmartPtr<MathGraphicDevice> mgd = backend->getMathGraphicDevice();
  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary<MathView>(logger, configuration);

  logger->out(LOG_INFO, "Font size : %f", fontSize);
  logger->out(LOG_INFO, "Page size : %fx%f", width, height);
  logger->out(LOG_INFO, "Margins   : %fx%f", xMargin, yMargin);

  SmartPtr<MathView> view = MathView::create(logger);
  view->setOperatorDictionary(dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, mgd));
#if GMV_ENABLE_BOXML
  SmartPtr<BoxGraphicDevice> bgd = backend->getBoxGraphicDevice();
  view->setBoxMLNamespaceContext(BoxMLNamespaceContext::create(view, bgd));
#endif
  view->setDefaultFontSize(static_cast<unsigned>(fontSize));

#if GMV_ENABLE_BOXML
  FormattingContext context(mgd, bgd);
#else
  FormattingContext context(mgd);
#endif
  const scaled widthS = mgd->evaluate(context, Length(width, unitId), scaled::zero());
  const scaled heightS = mgd->evaluate(context, Length(height, unitId), scaled::zero());
  const scaled xMarginS = mgd->evaluate(context, Length(xMargin, unitId), scaled::zero());
  const scaled yMarginS = mgd->evaluate(context, Length(yMargin, unitId), scaled::zero());

  view->setAvailableWidth(widthS - xMarginS * 2);

  for (int i = 0; filenameA != 0 && filenameA[i] != 0; i++)
    {
      const gchar* file = filenameA[i];
      logger->out(LOG_INFO, "Processing `%s'...", file);

      char* outName = getOutputFileName(file);
      assert(outName != NULL);

      if (!extendedSVG)
	{
#if 0
	  xmlTextReaderPtr reader = xmlNewTextReaderFilename(file);
	
	  assert(reader);
	  view->loadReader(reader);
#endif
	  view->loadURI(file);
	  const BoundingBox box = view->getBoundingBox();
	
	  std::ofstream os(outName);
	  //SVG_StreamRenderingContext rc(logger, os);
	  SVG_libxml2_StreamRenderingContext rc(logger, os, view);
	  if (cropping)
	    {
	      rc.documentStart(box);
	      view->render(rc, 0, -box.height);
	    }
	  else
	    {
	      rc.documentStart(BoundingBox(widthS, box.height, heightS - box.height));
	      view->render(rc, xMarginS, -(yMarginS + box.height));
	    }
	  rc.documentEnd();
	  view->resetRootElement();
	  os.close();
	  // WARNING: currently the text reader is freed by libxmlXmlReader
	  // not sure this is what we want
	  //xmlFreeTextReader(reader);
	}
      else
	{
	  SMS sms(logger, view);
	  sms.process(file, outName);
	}

      optind++;
    }

  g_option_context_free(ctxt);

  return 0;
}
