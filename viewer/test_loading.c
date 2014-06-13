/* Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
 *
 * This file is part of GtkMathView, a flexible, high-quality rendering
 * engine for MathML documents.
 * 
 * GtkMathView is free software; you can redistribute it and/or modify it
 * either under the terms of the GNU Lesser General Public License version
 * 3 as published by the Free Software Foundation (the "LGPL") or, at your
 * option, under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation (the "GPL").  If you do not
 * alter this notice, a recipient may use your version of this file under
 * either the GPL or the LGPL.
 *
 * GtkMathView is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
 * the GPL for more details.
 * 
 * You should have received a copy of the LGPL and of the GPL along with
 * this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>

#include "gtkmathview_libxml2.h"

int
main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget* math_view;

  gtk_init (&argc, &argv);
    
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_show  (window);

  g_signal_connect(window,
		   "delete_event", 
		   G_CALLBACK(gtk_main_quit), NULL);

  math_view = gtk_math_view_new(NULL, NULL);
  gtk_math_view_load_uri(GTK_MATH_VIEW(math_view), argv[1]);
  gtk_container_add (GTK_CONTAINER (window), math_view);
    
  gtk_widget_show (math_view);
    
  gtk_main ();
  
  return 0;
}

