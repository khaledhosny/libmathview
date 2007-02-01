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
static gdouble fontSize = DEFAULT_FONT_SIZE;
static bool cropping = true;
static bool cutFileName = true;
static gchar* configPath = 0;
static gint logLevel = -1;
static bool extendedSVG = false;
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
unitArgCB(const gchar*, const gchar* s, gpointer, GError**)
{
  assert(s != 0);

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
  for (i = 0; unit[i].name && strcmp(unit[i].name, s); i++) ;
  if (!unit[i].name) return FALSE;

  unitId = unit[i].id;

  return TRUE;
}

static gboolean
floatPairArgCB(const gchar* name, const gchar* s, gpointer, GError**)
{
  assert(name != 0);
  assert(s != 0);

  gchar* nptr;

  const double x = strtod(s, &nptr);
  if (nptr == s) return FALSE;
  if (nptr == NULL || *nptr != 'x') return FALSE;
  
  s = nptr + 1;
  const double y = strtod(s, &nptr);
  if (nptr == s) return FALSE;

  const String n = name;
  if (n == "page-size")
    {
      width = x;
      height = y;
    }
  else if (n == "margins")
    {
      xMargin = x;
      yMargin = y;
    }

  return TRUE;
}

static gboolean
booleanArgCB(const gchar* name, const gchar* value, gpointer, GError**)
{
  assert(name != 0);
  const String n = name;
  const String v = (value == 0) ? "yes" : value;

  if (n != "yes" && n != "no")
    return FALSE;

  const bool res = n == "yes";
  if (n == "crop")
    cropping = res;
  else if (n == "cut-filename")
    cutFileName = res;
  else if (n == "extended-svg")
    extendedSVG = res;

  return TRUE;
}

static GOptionEntry optionsTable[] = {
  { "version", 'V',
    0, G_OPTION_ARG_NONE, &version,
    "Output version information", 0
  },
  { "verbose", 'v',
    0, G_OPTION_ARG_INT, &logLevel,
    "Display messages", "[0-3]"
  },
  { "unit", 'u',
    0, G_OPTION_ARG_CALLBACK, (gpointer) &unitArgCB,
    "Unit for dimensions (default='cm')", "<unit>"
  },
  { "page-size", 'p',
    0, G_OPTION_ARG_CALLBACK, (gpointer) &floatPairArgCB,
    "Page size (width x height) (default = 21 x 29.7)", "<float>x<float>"
  },
  { "margins", 'm',
    0, G_OPTION_ARG_CALLBACK, (gpointer) &floatPairArgCB,
    "Margins (top x left) (default = 2 x 2)", "<float>x<float>"
  },
  { "font-size", 'f',
    0, G_OPTION_ARG_DOUBLE, &fontSize,
    "Default font size (in pt, default=10)", "<float>"
  },
  { "config", 0,
    0, G_OPTION_ARG_FILENAME, &configPath,
    "Configuration file path", "<path>"
  },
  { "crop", 'r',
    G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &booleanArgCB,
    "Enable/disable cropping to bounding box (default='yes')", "[yes,no]"
  },
  { "cut-filename", 0,
    G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &booleanArgCB,
    "Cut the prefix dir from the output file (default='yes')", "[yes,no]"
  },
  { "extended-svg", 'x', 
    G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &booleanArgCB,
    "Extended SVG processing (default='no')", "[yes,no]"
  },
  { G_OPTION_REMAINING, 0,
    0, G_OPTION_ARG_FILENAME_ARRAY, &filenameA,
    0, 0
  },
  { 0, 0, 0, G_OPTION_ARG_NONE, 0, 0, 0 }
};

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
  g_option_context_set_description(ctxt, "\
Valid units are:\n\
  cm    centimeter\n\
  mm    millimeter\n\
  in    inch (1 in = 2.54 cm)\n\
  pt    point (1 in = 72.27 pt)\n\
  pc    pica (1 pc = 12 pt)\n\
  px    pixel (1 in = 72 px)\n\
");

  g_option_context_set_help_enabled(ctxt, TRUE);
  g_option_context_add_main_entries(ctxt, optionsTable, 0);

  if (!g_option_context_parse(ctxt, &argc, &argv, &error))
    {
      std::cerr << "error: unrecognized option or syntax error, use --help for help" << std::endl;
      return 1;
    }

  if (version)
    {
      printVersion();
      return 0;
    }

  if (configPath == 0) configPath = getenv("GTKMATHVIEWCONF");

  SmartPtr<AbstractLogger> logger = Logger::create();
  logger->setLogLevel(LogLevelId(logLevel));
  SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, configPath);
  if (logLevel >= LOG_ERROR && logLevel <= LOG_DEBUG) logger->setLogLevel(LogLevelId(logLevel));
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
    }

  g_option_context_free(ctxt);

  return 0;
}
