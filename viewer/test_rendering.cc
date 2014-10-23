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

#include "defs.h"
#include "Logger.hh"
#include "libxml2_MathView.hh"
#include "MathMLOperatorDictionary.hh"
#include "Cairo_Backend.hh"
#include "Cairo_RenderingContext.hh"
#include "MathGraphicDevice.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#include "GObjectPtr.hh"

typedef libxml2_MathView MathView;

SmartPtr<AbstractLogger> logger;
SmartPtr<MathView> view;
GtkWidget* window = 0;
cairo_surface_t* surface = 0;

extern "C" gboolean
draw(GtkWidget* widget, cairo_t *cr)
{
  GdkWindow* window = gtk_widget_get_window(widget);
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);

  const gint width = allocation.width;
  const gint height = allocation.height;
  if (!surface)
    {
      const BoundingBox box = view->getBoundingBox();
      surface = gdk_window_create_similar_surface(window, CAIRO_CONTENT_COLOR, width, height);
      cairo_t *cr2 = cairo_create(surface);
      // needed to cleanup the surface
      cairo_set_source_rgb(cr2, 1, 1, 1);
      cairo_rectangle(cr2, 0, 0, width, height);
      cairo_fill(cr2);
      Cairo_RenderingContext* rc = new Cairo_RenderingContext(cr2);
      view->render(*rc, scaled::zero(), -box.height);
    }

  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);

  return TRUE;
}

int main(int argc, char *argv[]) {
  gtk_init (&argc, &argv);
    
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_show  (window);

  g_signal_connect(window,
		   "draw",
		   G_CALLBACK(draw), NULL);

  g_signal_connect(window,
		   "delete_event", 
		   G_CALLBACK(gtk_main_quit), NULL);

  logger = Logger::create();

  GObjectPtr<PangoContext> pango_context = gtk_widget_create_pango_context(window);
  PangoFontDescription* description = pango_font_description_new();
  pango_font_description_set_family(description, DEFAULT_FONT_FAMILY);
  pango_font_description_set_size(description, DEFAULT_FONT_SIZE * PANGO_SCALE);
  PangoFont* font = pango_context_load_font(pango_context, description);
  cairo_scaled_font_t* cairo_font = pango_cairo_font_get_scaled_font (PANGO_CAIRO_FONT(font));

  SmartPtr<Backend> backend = Cairo_Backend::create(cairo_font);
  SmartPtr<MathGraphicDevice> mgd = backend->getMathGraphicDevice();
  SmartPtr<MathMLOperatorDictionary> dictionary = MathMLOperatorDictionary::create();

  view = MathView::create(logger);
  view->setOperatorDictionary(dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, mgd));
  //view->setDefaultFontSize(static_cast<unsigned>(DEFAULT_FONT_SIZE));

  FormattingContext context(mgd);

  view->loadURI(argv[1]);
  gtk_main ();
  view->resetRootElement();
  
  return 0;
}

