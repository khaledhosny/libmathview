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

#include <gtk/gtk.h>

#include <stdio.h>
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

typedef libxml2_MathView MathView;

SmartPtr<AbstractLogger> logger;
SmartPtr<MathView> view;
GtkWidget* window = 0;
GdkPixmap* pixmap = 0;
cairo_t* cr = 0;

extern "C" gboolean
expose(GtkWidget* widget, GdkEventExpose *event, gpointer user_data)
{
  printf("expose %p\n", pixmap);

  const gint width = widget->allocation.width;
  const gint height = widget->allocation.height;
  if (!pixmap)
    {
      const BoundingBox box = view->getBoundingBox();
      pixmap = gdk_pixmap_new(widget->window, width, height, -1);
      // needed to cleanup the pixmap
      gdk_draw_rectangle(pixmap, widget->style->white_gc, TRUE, 0, 0, width, height);
      Cairo_RenderingContext* rc = new Cairo_RenderingContext();
      cr = gdk_cairo_create(pixmap);
      rc->setCairo(cr);
      view->render(*rc, scaled::zero(), -box.height);
    }

  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		  pixmap,
		  0, 0, 0, 0, width, height);

  return TRUE;
}

int main(int argc, char *argv[]) {
  gtk_init (&argc, &argv);
    
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_show  (window);

  g_signal_connect(GTK_OBJECT(window),
		   "expose_event",
		   (GtkSignalFunc) expose, NULL);

  g_signal_connect(GTK_OBJECT(window), 
		   "delete_event", 
		   (GtkSignalFunc) gtk_main_quit, NULL);

  const char* configPath = getenv("GTKMATHVIEWCONF");

  logger = Logger::create();
  SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, configPath);

  const String fontname = configuration->getString(logger, "default/font-family", DEFAULT_FONT_FAMILY);
  const int fontsize = configuration->getInt(logger, "default/font-size", DEFAULT_FONT_SIZE);

  GObjectPtr<PangoContext> pango_context = gtk_widget_create_pango_context(window);
  PangoFontDescription* description = pango_font_description_new();
  pango_font_description_set_family(description, fontname.c_str());
  pango_font_description_set_size(description, fontsize * PANGO_SCALE);
  PangoFont* font = pango_context_load_font(pango_context, description);
  cairo_scaled_font_t* cairo_font = pango_cairo_font_get_scaled_font (PANGO_CAIRO_FONT(font));

  SmartPtr<Backend> backend = Cairo_Backend::create(cairo_font);
  SmartPtr<MathGraphicDevice> mgd = backend->getMathGraphicDevice();
  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary<MathView>(logger, configuration);

  view = MathView::create(logger);
  view->setOperatorDictionary(dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, mgd));
  //view->setDefaultFontSize(static_cast<unsigned>(fontSize));

  FormattingContext context(mgd);

  view->loadURI(argv[1]);
  gtk_main ();
  view->resetRootElement();
  
  return 0;
}

