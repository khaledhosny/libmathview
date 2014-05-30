// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2013, Khaled Hosny <khaledhosny@eglug.org>.
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

#include <cairo.h>
#ifdef CAIRO_HAS_SVG_SURFACE
#  include <cairo-svg.h>
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
#  include <cairo-pdf.h>
#endif
#ifdef CAIRO_HAS_PS_SURFACE
#  include <cairo-ps.h>
#endif
#include <glib.h>
#include <pango/pangocairo.h>

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Logger.hh"
#include "Init.hh"
#include "Configuration.hh"
#include "libxml2_MathView.hh"
#include "MathMLOperatorDictionary.hh"
#include "Cairo_Backend.hh"
#include "Cairo_MathGraphicDevice.hh"
#include "Cairo_RenderingContext.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#include "GObjectPtr.hh"

typedef libxml2_MathView MathView;

static char **remaining_args = NULL;
static GOptionEntry entries[] = {
  { G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_FILENAME_ARRAY, &remaining_args, NULL, "[FILE...]" },
  { NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
};

static cairo_surface_t*
create_surface(const char* filename, double width, double height)
{
  cairo_surface_t* surface = NULL;
  const char *extension = strrchr(filename, '.');
  if (extension)
  {
    extension++;
    if (strcasecmp(extension, "png") == 0)
      surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
#ifdef CAIRO_HAS_SVG_SURFACE
    else if (strcasecmp(extension, "svg") == 0)
      surface = cairo_svg_surface_create(filename, width, height);
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
    else if (strcasecmp(extension, "pdf") == 0)
      surface = cairo_pdf_surface_create(filename, width, height);
#endif
#ifdef CAIRO_HAS_PS_SURFACE
    else if (strcasecmp(extension, "ps") == 0)
      surface = cairo_ps_surface_create(filename, width, height);
#endif
    else
      g_print("can't output files of format: %s\n", extension);
  }

  return surface;
}

int
main(int argc, char *argv[])
{
  GError *error = NULL;
  GOptionContext *context = g_option_context_new(NULL);
  g_option_context_add_main_entries(context, entries, NULL);

  if (!g_option_context_parse(context, &argc, &argv, &error))
  {
    g_print("option parsing failed: %s\n", error->message);
    exit(1);
  }

  if (remaining_args == NULL || remaining_args[1] == NULL)
  {
    g_print("no input or output files specified\n");
    exit(1);
  }

  const char* input_file = remaining_args[0];
  const char* output_file = remaining_args[1];

  const char* configPath = getenv("GTKMATHVIEWCONF");
  SmartPtr<AbstractLogger> logger = Logger::create();
  SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, configPath);

  const String fontname = configuration->getString(logger, "default/font-family", DEFAULT_FONT_FAMILY);
  const int fontsize = configuration->getInt(logger, "default/font-size", DEFAULT_FONT_SIZE);

  PangoFontMap* fontmap = pango_cairo_font_map_get_default();
  GObjectPtr<PangoContext> pango_context = pango_font_map_create_context(fontmap);
  PangoFontDescription* description = pango_font_description_new();
  pango_font_description_set_family(description, fontname.c_str());
  PangoFont* font = pango_context_load_font(pango_context, description);
  cairo_scaled_font_t* cairo_font = pango_cairo_font_get_scaled_font (PANGO_CAIRO_FONT(font));

  SmartPtr<Backend> backend = Cairo_Backend::create(cairo_font);
  SmartPtr<MathGraphicDevice> device = backend->getMathGraphicDevice();
  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary<MathView>(logger, configuration);

  SmartPtr<MathView> view = MathView::create(logger);
  view->setOperatorDictionary(dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, device));
  view->setDefaultFontSize(fontsize);

  view->loadURI(input_file);

  const BoundingBox box = view->getBoundingBox();
  double width = Cairo_RenderingContext::toCairoPixels(box.horizontalExtent());
  double height = Cairo_RenderingContext::toCairoPixels(box.verticalExtent());
  cairo_surface_t* surface = create_surface(output_file, width, height);
  if (surface)
  {
    cairo_t* cr = cairo_create(surface);
    Cairo_RenderingContext* rc = new Cairo_RenderingContext();
    rc->setCairo(cr);

    view->render(*rc, scaled::zero(), -box.height);

    if (cairo_surface_get_type(surface) == CAIRO_SURFACE_TYPE_IMAGE)
      cairo_surface_write_to_png(surface, output_file);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
  }

  view->resetRootElement();

  return 0;
}
