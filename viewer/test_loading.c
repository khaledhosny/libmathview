// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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

  g_signal_connect(GTK_OBJECT(window), 
		   "delete_event", 
		   (GtkSignalFunc) gtk_main_quit, NULL);

  math_view = gtk_math_view_new(NULL, NULL);
  gtk_math_view_load_uri(GTK_MATH_VIEW(math_view), argv[1]);
  gtk_container_add (GTK_CONTAINER (window), math_view);
    
  gtk_widget_show (math_view);
    
  gtk_main ();
  
  return 0;
}

