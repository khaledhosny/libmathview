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
#if HAVE_LIBT1
#include "T1_FontDataBase.hh"
#endif //HAVE_LIBT1
#include "FontDataBase.hh"
#include "PS_Backend.hh"
#include "PS_MathGraphicDevice.hh"
#include "PS_StreamRenderingContext.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#if GMV_ENABLE_BOXML
#include "BoxMLNamespaceContext.hh"
#include "BoxGraphicDevice.hh"
#endif // GMV_ENABLE_BOXML

typedef libxml2_MathView MathView;

static double width = 21;
static double height = 29.7;
static Length::Unit unitId = Length::CM_UNIT;
static double xMargin = 2;
static double yMargin = 2;
static double fontSize = DEFAULT_FONT_SIZE;
static bool cropping = true;

static gboolean option_version = FALSE;
static gint option_verbose = -1;
static gchar* option_unit = NULL;
static gchar* option_page_size = NULL;
static gchar* option_margins = NULL;
static gdouble option_font_size = 0.0;
#ifdef HAVE_LIBT1
static gint option_font_embed = 2;
#else
static int option_font_embed = 0;
#endif
static gchar* option_config = NULL;
static gchar* option_crop = NULL;

static void
printVersion()
{
  std::cout << "MathML to PostScript converter - written by Luca Padovani & Nicola Rossi (C) 2006-2007" << std::endl
	    << "Based on GtkMathView " << VERSION << std::endl;
#ifdef DEBUG
  std::cout << "Compiled " << __DATE__ << " " << __TIME__ << std::endl;
#endif
  exit(0);
}

static GOptionEntry optionEntries[] = {
  { "version", 'V', 0, G_OPTION_ARG_NONE, &option_version, "Output version information", 0 },
  { "verbose", 'v', 0, G_OPTION_ARG_INT, &option_verbose, "Display messages", "[0-3]" },
  { "unit", 'u', 0, G_OPTION_ARG_STRING, &option_unit, "Unit for dimensions (default='cm')", "<unit>" },
  { "page-size", 'p', 0, G_OPTION_ARG_STRING, &option_page_size, "Page size (width x height) (default = 21 x 29.7)", "<float>x<float>" },
  { "margins", 'm', 0, G_OPTION_ARG_STRING, &option_margins, "Margins (top x left) (default = 2 x 2)", "<float>x<float>" },
  { "font-size", 'f', 0, G_OPTION_ARG_DOUBLE, &option_font_size, "Default font size (in pt, default=10)", "<float>" },
#ifdef HAVE_LIBT1
  { "font-embed", 0, 0, G_OPTION_ARG_INT, &option_font_embed, "Enable/disable embedding (default=2)", "[0=disable,1=embed,2=subset]" },
#endif // HAVE_LIBT1
  { "config", 0, 0, G_OPTION_ARG_FILENAME, &option_config, "Configuration file path", "<path>" },
  { "crop", 'r', G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_STRING, &option_crop, "Enable/disable cropping to bounding box", "[yes,no]" },

  { NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
};

static void
usage(GOptionContext* ctxt, int exitcode, const char* msg, const char* arg)
{
  gchar* help = g_option_context_get_help(ctxt, TRUE, NULL);
  fprintf(stderr, "%s", help);
  fprintf(stderr, "\
Valid units are:\n\
  cm    centimeter\n\
  mm    millimeter\n\
  in    inch (1 in = 2.54 cm)\n\
  pt    point (1 in = 72.27 pt)\n\
  pc    pica (1 pc = 12 pt)\n\
  px    pixel (1 in = 72 px)\n\
");
  if (msg && arg) fprintf(stderr, "%s %s\n", msg, arg);
  g_free(help);
  exit(exitcode);
}

static void
parseError(GOptionContext* ctxt, const char* option)
{
  assert(option != NULL);
  usage(ctxt, 1, "error while parsing option", option);
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
main(int argc, char* argv[])
{
  GOptionContext* ctxt = g_option_context_new("<file> - display MathML documents");
  g_option_context_add_main_entries(ctxt, optionEntries, NULL);
  g_option_context_set_help_enabled(ctxt, TRUE);
  if (!g_option_context_parse(ctxt, &argc, &argv, NULL))
    usage(ctxt, 1, "error parsing options", "");

  if (option_version)
    printVersion();

  if (option_page_size && !parseSize(option_page_size)) parseError(ctxt, "page-size");
  if (option_unit && !parseUnit(option_unit)) parseError(ctxt, "unit");
  if (option_margins && !parseMargins(option_margins)) parseError(ctxt, "margins");
#ifdef HAVE_LIBT1
  if (option_font_embed < 0 || option_font_embed > 2) parseError(ctxt, "font-embed");
#endif // HAVE_LIBT1
  if (option_crop && !parseBoolean(option_crop, cropping)) parseError(ctxt, "crop");

  if (option_config == NULL) option_config = getenv("GTKMATHVIEWCONF");

  g_option_context_free(ctxt);

  SmartPtr<AbstractLogger> logger = Logger::create();
  if (option_verbose >= 0 && option_verbose <= 3) logger->setLogLevel(LogLevelId(option_verbose));
  SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, option_config);
  if (option_verbose >= 0 && option_verbose <= 3) logger->setLogLevel(LogLevelId(option_verbose));
  SmartPtr<Backend> backend = PS_Backend::create(logger, configuration);
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

  for (int i = 1; i < argc; i++)
    {
      const char* file = argv[i];
      logger->out(LOG_INFO, "Processing `%s'...", file);

      char* outName = getOutputFileName(file);
      assert(outName != NULL);
#if 1
#if 0
      xmlTextReaderPtr reader = xmlNewTextReaderFilename(file);

      assert(reader);
      view->loadReader(reader);
#endif
      view->loadURI(file);
      const BoundingBox box = view->getBoundingBox();
     
#ifdef HAVE_LIBT1
      SmartPtr<FontDataBase> fDb;
      switch (option_font_embed) {
        case 0: fDb = FontDataBase::create(); break;
	case 1: fDb = T1_FontDataBase::create(logger, configuration, false); break;
	case 2: fDb = T1_FontDataBase::create(logger, configuration, true); break;
	default: assert(false); /* IMPOSSIBLE */
      }
#else // !HAVE_LIBT1
      SmartPtr<FontDataBase> fDb = FontDataBase::create();
#endif

      std::ofstream os(outName);
      PS_StreamRenderingContext rc(logger, os, fDb);

      if (cropping)
	{
	  rc.documentStart(0, 0, box, outName);
	  view->render(rc, 0, box.depth);
	}
      else
	{
	  rc.documentStart(xMarginS, (box.depth + yMarginS),
			   BoundingBox(widthS, heightS - box.depth - yMarginS, 
				       box.depth + yMarginS),
			   outName);
	  view->render(rc, xMarginS, (box.depth + yMarginS));
	}

      delete [] outName;

      rc.documentEnd();
      view->resetRootElement();
      os.close();
      // WARNING: currently the text reader is freed by libxmlXmlReader
      // not sure this is what we want
      //xmlFreeTextReader(reader);
#else
      SMS sms(logger, view);
      sms.process(argv[optind]);
#endif
    }

  return 0;
}
