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
  GtkTextBuffer* buffer;
  GtkTextIter start, end;
  GtkWidget* text_view;
  GtkWidget* math_view;
  GtkTextChildAnchor* math_anchor;
  GtkTextTag* rise_tag;
  GtkMathViewBoundingBox math_box;

  gtk_init (&argc, &argv);
    
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_show  (window);

  g_signal_connect(window,
		   "delete_event", 
		    G_CALLBACK(gtk_main_quit), NULL);

  buffer = gtk_text_buffer_new(NULL);
  gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), "This kind of annotation is useful when something other than the \
default rendering of the content encoding is desired. For example, by \
default, some renderers might layout the integrand something like \
. Specifying that the integrand should by preference \
render as \"dx/x\" instead can be accomplished with the use of a \
MathML Presentation annotation as shown. Be aware, however, that \
renderers are not required to take into account information contained \
in annotations, and what use is made of them, if any, will depend on \
the renderer.", -1);

  gtk_text_buffer_get_iter_at_offset(buffer, &start, 199);
  math_anchor = gtk_text_buffer_create_child_anchor(buffer, &start);

  text_view = gtk_text_view_new_with_buffer(buffer);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);

  math_view = gtk_math_view_new(NULL, NULL);
  gtk_math_view_load_uri(GTK_MATH_VIEW(math_view), "mini.xml");
#if 0
  gtk_math_view_set_cursor(GTK_MATH_VIEW(math_view), root, 0);
  gtk_math_view_set_cursor_visible(GTK_MATH_VIEW(math_view), GTKMATHVIEW_CURSOR_FOCUS_ON);
#endif
  gtk_math_view_get_bounding_box(GTK_MATH_VIEW(math_view), &math_box);
  gtk_text_buffer_get_iter_at_offset(buffer, &start, 199);
  gtk_text_buffer_get_iter_at_offset(buffer, &end, 200);
  rise_tag = gtk_text_buffer_create_tag(buffer, NULL, "rise", -PANGO_SCALE * math_box.depth, NULL);
  gtk_text_buffer_apply_tag(buffer, rise_tag, &start, &end);

  gtk_text_view_add_child_at_anchor(GTK_TEXT_VIEW(text_view), math_view, math_anchor);

  gtk_container_add (GTK_CONTAINER (window), text_view);
    
  gtk_widget_show (text_view);
  gtk_widget_show (math_view);
    
  gtk_main ();
  
  return 0;
}

