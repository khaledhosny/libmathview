/* Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
 *
 * This file is part of GtkMathView, a flexible, high-quality rendering
 * engine for MathML documents.
 * 
 * GtkMathView is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * GtkMathView is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <assert.h>

#include "gtkmathview_libxml2_reader.h"

int
main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget* math_view;
  xmlTextReader* reader;
  gtk_init (&argc, &argv);
    
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_show  (window);

  g_signal_connect(GTK_OBJECT(window), 
		   "delete_event", 
		   (GtkSignalFunc) gtk_main_quit, NULL);

  math_view = gtk_math_view_new(NULL, NULL);
  reader = xmlReaderForFile(argv[1], NULL, 0);
  /*xmlTextReaderRead(reader);*/
  assert(reader);
  gtk_math_view_load_reader(GTK_MATH_VIEW(math_view), reader);
  gtk_container_add (GTK_CONTAINER (window), math_view);
    
  gtk_widget_show (math_view);
    
  gtk_main ();
  
  return 0;
}

