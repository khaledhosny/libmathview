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

#include <gtk/gtk.h>

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Logger.hh"
#include "Init.hh"
#include "Configuration.hh"
#include "libxml2_MathView.hh"
#include "MathMLOperatorDictionary.hh"
#include "Gtk_Backend.hh"
#include "Gtk_MathGraphicDevice.hh"
#include "Gtk_RenderingContext.hh"
#include "MathMLNamespaceContext.hh"
#include "FormattingContext.hh"
#if ENABLE_BOXML
#include "BoxMLNamespaceContext.hh"
#include "BoxGraphicDevice.hh"
#endif // ENABLE_BOXML

typedef libxml2_MathView MathView;

SmartPtr<AbstractLogger> logger;
SmartPtr<MathView> view;
GtkWidget* window = 0;
GdkPixmap* pixmap = 0;

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
      Gtk_RenderingContext rc(logger);
      rc.setColorMap(gtk_widget_get_colormap(GTK_WIDGET(widget)));
      rc.setDrawable(pixmap);
      view->render(rc, scaled::zero(), -box.height);
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
  SmartPtr<Backend> backend = Gtk_Backend::create(logger, configuration);
  SmartPtr<MathGraphicDevice> mgd = backend->getMathGraphicDevice();
  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary<MathView>(logger, configuration);

  view = MathView::create();
  view->setLogger(logger);
  view->setOperatorDictionary(dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, mgd));
#if ENABLE_BOXML
  SmartPtr<BoxGraphicDevice> bgd = backend->getBoxGraphicDevice();
  view->setBoxMLNamespaceContext(BoxMLNamespaceContext::create(view, bgd));
#endif
  //view->setDefaultFontSize(static_cast<unsigned>(fontSize));

#if ENABLE_BOXML
  FormattingContext context(mgd, bgd);
#else
  FormattingContext context(mgd);
#endif

  view->loadURI(argv[1]);
  gtk_main ();
  view->resetRootElement();
  
  return 0;
}

