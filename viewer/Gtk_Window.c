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

#include <config.h>

#include "defs.h"
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "gtkmathview_libxml2.h"

#include "Gtk_Window.h"

static GtkBuilder *builder;
static GtkWindow *window;
static GtkMathView *math_view;
static gchar *doc_name = NULL;

static gboolean    is_semantic_selection = FALSE;
static xmlElement *first_selected = NULL;
static xmlElement *root_selected = NULL;

static GtkMathView *setup_widgets (void);
static gboolean load_document (GtkMathView*, char*);
static void unload_document (GtkMathView *view);

static gboolean cursor_blink (GtkMathViewDefaultCursorDecorator*);

static void on_file_open (GtkWidget*, GtkMathView*);
static void on_file_reopen (GtkWidget*, GtkMathView*);
static void on_file_close (GtkWidget*, GtkMathView*);
static void on_set_font_size (GtkWidget*, GtkMathView*);
static void on_larger_font_size (GtkWidget*, GtkMathView*);
static void on_smaller_font_size (GtkWidget*, GtkMathView*);
static void on_errors_verbosity (GtkWidget*, GtkMathView*);
static void on_warnings_verbosity (GtkWidget*, GtkMathView*);
static void on_info_verbosity (GtkWidget*, GtkMathView*);
static void on_debug_verbosity (GtkWidget*, GtkMathView*);
static void on_semantics_selection (GtkWidget*, GtkMathView*);
static void on_structure_selection (GtkWidget*, GtkMathView*);
static void on_selection_delete (GtkWidget*, GtkMathView*);
static void on_selection_parent (GtkWidget*, GtkMathView*);
static void on_selection_reset (GtkWidget*, GtkMathView*);
static void help_about (GtkWidget*, GtkMathView*);

void
GUI_init (int *argc, char ***argv, char *title, char *file, gint logLevel)
{
  GtkMathView *view;

  gtk_init (argc, argv);

  GError *error = NULL;

  builder = gtk_builder_new ();
  gtk_builder_add_from_resource (builder, "/org/mathview/mml-view-gtk.ui", &error);
  if (error) {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }

  gtk_builder_connect_signals (builder, NULL);

  window = GTK_WINDOW (gtk_builder_get_object (builder, "mml-view-window"));

  gtk_window_set_title (window, title);
  
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  
  view = setup_widgets ();
  g_assert (view != NULL);
  g_assert (GTK_IS_MATH_VIEW (view));
  gtk_math_view_set_log_verbosity (view, logLevel);

  if (load_document (view, file) < 0)
    printf ("fatal error: cannot load document `%s'\n", file);
}

static gboolean
load_document (GtkMathView *view,
               char *name)
{
  g_return_val_if_fail (name != NULL, FALSE);
  g_return_val_if_fail (view != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_MATH_VIEW (view), FALSE);

  GtkStatusbar *status_bar;
  guint statusbar_context;

  gtk_math_view_load_uri (view, name);

  if (name != doc_name) {
    if (doc_name != NULL) g_free (doc_name);
    doc_name = g_strdup (name);
  }

  status_bar = GTK_STATUSBAR (gtk_builder_get_object (builder, "statusbar"));
  statusbar_context = gtk_statusbar_get_context_id (status_bar, "filename");

  gtk_statusbar_pop (status_bar, statusbar_context);
  if (strlen (name) > 40) name += strlen (name) - 40;
  gtk_statusbar_push (status_bar, statusbar_context, name);
    
  return TRUE;
}

static void
unload_document (GtkMathView *view)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));

  GtkStatusbar *status_bar;
  guint statusbar_context;

  gtk_math_view_unload (view);

  if (doc_name != NULL) g_free (doc_name);
  doc_name = NULL;

  status_bar = GTK_STATUSBAR (gtk_builder_get_object (builder, "statusbar"));
  statusbar_context = gtk_statusbar_get_context_id (status_bar, "filename");

  gtk_statusbar_pop (status_bar, statusbar_context);
  gtk_statusbar_push (status_bar, statusbar_context, "");
}

static void
on_file_close (GtkWidget   *widget,
               GtkMathView *view)
{
  unload_document (view);
}

static void
on_file_reopen (GtkWidget   *widget,
                GtkMathView *view)
{
  if (doc_name != NULL) {
    load_document (view, doc_name);
  }
}

static void
on_file_open (GtkWidget   *widget,
              GtkMathView *view)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new ("Open File",
                                       window,
                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                       "_Open", GTK_RESPONSE_ACCEPT,
                                       NULL);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    load_document (view, filename);
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}

static void
on_errors_verbosity (GtkWidget   *widget,
                     GtkMathView *view)
{
  gtk_math_view_set_log_verbosity (view, 0);
}

static void
on_warnings_verbosity (GtkWidget   *widget,
                       GtkMathView *view)
{
  gtk_math_view_set_log_verbosity (view, 1);
}

static void
on_info_verbosity (GtkWidget   *widget,
                   GtkMathView *view)
{
  gtk_math_view_set_log_verbosity (view, 2);
}

static void
on_debug_verbosity (GtkWidget   *widget,
                    GtkMathView *view)
{
  gtk_math_view_set_log_verbosity (view, 3);
}

static void
on_semantics_selection (GtkWidget   *widget,
                        GtkMathView *view)
{
  is_semantic_selection = TRUE;
  on_selection_reset (widget, view);
}

static void
on_structure_selection (GtkWidget   *widget,
                        GtkMathView *view)
{
  is_semantic_selection = FALSE;
  on_selection_reset (widget, view);
}

static void
delete_element (GtkMathView *view,
                xmlElement  *elem)
{
  xmlNodePtr node = (xmlNodePtr) elem;
  gtk_math_view_structure_changed (view, (GtkMathViewModelId) node->parent);
  xmlUnlinkNode (node);
  xmlFreeNode (node);
}

static void
on_selection_delete (GtkWidget   *widget,
                     GtkMathView *view)
{
  if (root_selected != NULL)
    {
      gtk_math_view_freeze (view);
      delete_element (view, root_selected);
      root_selected = NULL;
      gtk_math_view_thaw (view);
    }
}

static void
on_selection_parent (GtkWidget   *widget,
                     GtkMathView *view)
{
  if (root_selected != NULL)
    {
      xmlElement *parent = (xmlElement*) ((xmlNodePtr) root_selected)->parent;
      root_selected = parent;
      gtk_math_view_select (view, root_selected);
    }
}

static void
on_selection_reset (GtkWidget   *widget,
                    GtkMathView *view)
{
  if (root_selected != NULL)
    {
      gtk_math_view_unselect (view, root_selected);
      root_selected = NULL;
    }
}

static void
help_about (GtkWidget   *widget,
            GtkMathView *view)
{

  gtk_show_about_dialog (window,
                        "program-name", "MathML Viewer",
                        "copyright", "Copyright (C) 2000-2004 Luca Padovani",
                        NULL);
}

static void
change_default_font_size (GtkDialog     *dialog,
                          gint           response_id,
                          GtkSpinButton *spin)
{
  g_return_if_fail (spin != NULL);
  if (response_id == GTK_RESPONSE_ACCEPT)
    gtk_math_view_set_font_size (math_view, gtk_spin_button_get_value_as_int (spin));
  gtk_widget_destroy (GTK_WIDGET (dialog));
}

static void
change_font_size (GtkMathView *view,
                  gboolean     larger)
{
  gfloat size = gtk_math_view_get_font_size (view);
  if (larger)
    size = size / 0.71;
  else
    size = size * 0.71;

  if (size < 1)
    size = 1;

  gtk_math_view_set_font_size (view, (gint) size + 0.5);
}

static void
on_larger_font_size (GtkWidget   *widget,
                     GtkMathView *view)
{
  change_font_size (view, TRUE);
}

static void
on_smaller_font_size (GtkWidget   *widget,
                      GtkMathView *view)
{
  change_font_size (view, FALSE);
}

static void
on_set_font_size (GtkWidget   *widget,
                  GtkMathView *view)
{
  GtkWidget *dialog;
  GtkWidget *spin;
  GtkAdjustment *adj;

  dialog = gtk_dialog_new_with_buttons ("Set default font size",
                                        window,
                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                        "_OK", GTK_RESPONSE_ACCEPT,
                                        "_Cancel", GTK_RESPONSE_REJECT,
                                        NULL);

  adj = gtk_adjustment_new (gtk_math_view_get_font_size (view), 1, 200, 1, 1, 0);
  spin = gtk_spin_button_new (GTK_ADJUSTMENT (adj), 1, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin), TRUE);

  g_signal_connect (dialog, "response", G_CALLBACK (change_default_font_size), spin);

  GtkContainer *content_area = GTK_CONTAINER (gtk_dialog_get_content_area (GTK_DIALOG (dialog)));
  gtk_container_set_border_width (content_area, 5);
  gtk_container_add (content_area, spin);

  gtk_widget_show_all (dialog);
}

static xmlNodePtr
find_node_with_attribute (xmlNodePtr     node,
                          const xmlChar *name)
{
  while (node && !xmlHasProp (node, name))
    node = node->parent;
  return node;
}

static xmlChar*
find_hyperlink (xmlElement  *elem,
                const gchar *name)
{
  xmlNodePtr node = find_node_with_attribute ((xmlNodePtr) elem, BAD_CAST(name));
  if (node)
    return xmlGetProp (node, BAD_CAST(name));
  else
    return NULL;
}

static void
element_over (GtkMathView                 *view,
              const GtkMathViewModelEvent *event)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));
  g_return_if_fail (event != NULL);

  xmlChar *link = NULL;
  GdkCursor *cursor;

  link = find_hyperlink (event->id, "href");
  if (link != NULL)
    cursor = gdk_cursor_new (GDK_HAND2);
  else
    cursor = gdk_cursor_new (GDK_TOP_LEFT_ARROW);

  gdk_window_set_cursor (gtk_widget_get_window (GTK_WIDGET (view)), cursor);
  g_object_unref (cursor);

  if (link != NULL)
    xmlFree (link);

#if 0
  xmlElement *action = find_action_element (event->id);
  if (action != NULL)
    {
      gtk_math_view_decor_default_cursor_set (cursor, TRUE, action, TRUE, -1, FALSE);
    }
#endif
}

static xmlElement*
find_xref_element (xmlElement *elem)
{
  xmlNodePtr node = find_node_with_attribute ((xmlNodePtr) elem, BAD_CAST("xref"));
  return (xmlElement*) node;
}

static void
select_begin (GtkMathView                 *view,
              const GtkMathViewModelEvent *event)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));
  g_return_if_fail (event != NULL);

  if (event->id != NULL)
    {
      gtk_math_view_freeze (view);

      if (root_selected != NULL)
        {
          gtk_math_view_unselect (view, root_selected);
          root_selected = NULL;
        }

      if (is_semantic_selection)
        {
          xmlElement *new_elem = find_xref_element (event->id);
          first_selected = root_selected = new_elem;
        }
      else
        {
          first_selected = root_selected = event->id;
        }

      if (root_selected != NULL)
        gtk_math_view_select (view, root_selected);

      gtk_math_view_thaw (view);
    }
}

static unsigned
get_depth (xmlNodePtr node)
{
  unsigned length = 0;
  while (node)
    {
      node = node->parent;
      length++;
    }

  return length;
}

static xmlElement*
find_common_ancestor (xmlElement *first,
                      xmlElement *last)
{
  if (!first || !last)
    return NULL;

  xmlNodePtr p = (xmlNodePtr) first;
  xmlNodePtr q = (xmlNodePtr) last;

  if (p != q)
    {
      unsigned p_depth = get_depth (p);
      unsigned q_depth  = get_depth (q);

      while (p && p_depth > q_depth)
        {
          p = p->parent;
          p_depth--;
        }

      while (q && q_depth > p_depth)
        {
          q = q->parent;
          q_depth--;
        }

      g_assert (p_depth == q_depth);

      while (p && q && p != q)
        {
          p = p->parent;
          q = q->parent;
        }
    }

  return (xmlElement*) p;
}

static void
select_over (GtkMathView                 *view,
             const GtkMathViewModelEvent *event)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));
  g_return_if_fail (event != NULL);

  if (first_selected != NULL && event->id != NULL)
    {
      gtk_math_view_freeze (view);

      if (root_selected != NULL)
        {
          gtk_math_view_unselect (view, root_selected);
          root_selected = NULL;
        }

      if (is_semantic_selection)
        {
          xmlElement *new_root = find_common_ancestor (first_selected, event->id);
          if (new_root != NULL)
            root_selected = find_xref_element (new_root);
          else
            root_selected = NULL;
        }
      else
        root_selected = find_common_ancestor (first_selected, event->id);

      while (root_selected != NULL && !gtk_math_view_select (view, root_selected))
        {
          xmlElement *new_root = (xmlElement*) ((xmlNodePtr) root_selected)->parent;
          root_selected = new_root;
        }

      gtk_math_view_thaw (view);
    }
}

static void
select_end (GtkMathView                 *view,
            const GtkMathViewModelEvent *event)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));
  g_return_if_fail (event != NULL);

  if (first_selected != NULL)
    {
      first_selected = NULL;
    }
}

static void
select_abort (GtkMathView *view)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));

  if (first_selected != NULL)
    {
      first_selected = NULL;
    }

  if (root_selected != NULL)
    {
      gtk_math_view_freeze (view);
      gtk_math_view_unselect (view, root_selected);
      gtk_math_view_thaw (view);
      root_selected = NULL;
    }
}

static xmlElement*
find_action_element (xmlElement *elem)
{
  xmlNodePtr node;
  for (node = (xmlNodePtr) elem; node && node->name; node = node->parent)
  {
    if (g_strcmp0((const char*) node->ns->href, MATHML_NS_URI) == 0
     && g_strcmp0((const char*) node->name, "maction") == 0)
      return (xmlElement*) node;
  }
  return NULL;
}

static void
action_toggle (xmlElement *elem)
{
  xmlNodePtr node = (xmlNodePtr) elem;
  const xmlChar selection[] = "selection";
  guint idx = 1;

  if (xmlHasProp (node, selection))
    {
      xmlChar *prop = xmlGetProp (node, selection);
      idx = atoi ((const char*) prop);
      xmlFree (prop);
    }

  idx++;

  gchar *prop = g_strdup_printf ("%d", idx);
  xmlSetProp (node, selection, (const xmlChar*) prop);
  g_free (prop);
}

static void
click (GtkMathView                 *view,
       const GtkMathViewModelEvent *event)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));
  g_return_if_fail (event != NULL);

  gint index;
  xmlElement *elem;
  GtkMathViewDefaultCursorDecorator *cursor;

  gint w, h;
  gtk_math_view_get_size (view, &w, &h);
  g_debug ("click signal %d %d\n", w, h);

  cursor = gtk_math_view_decor_default_cursor_new (view);
  g_timeout_add (500, (GSourceFunc) cursor_blink, cursor);

  index = -1;
  if (gtk_math_view_get_char_at (view, event->x, event->y, &elem, &index, NULL, NULL))
    gtk_math_view_decor_default_cursor_set (cursor, TRUE, event->id, TRUE, index, TRUE);
  else
    gtk_math_view_decor_default_cursor_set (cursor, FALSE, NULL, FALSE, -1, FALSE);

  if (event->id != NULL)
    {
      g_debug ("node name: %s\n", ((xmlNodePtr) event->id)->name);
    }

  if (event->id != NULL)
    {
      xmlElement *action;
      xmlChar *href = find_hyperlink (event->id, "href");
      if (href != NULL)
        {
          g_debug ("hyperlink %s\n", href);
          xmlFree (href);
        }

      action = find_action_element (event->id);
      g_debug ("action? %p\n", action);
      if (action != NULL)
        {
          gtk_math_view_freeze (view);
          action_toggle (action);
          gtk_math_view_attribute_changed (view, event->id, (GtkMathViewModelString) "selection");
          gtk_math_view_thaw (view);
          return;
        }
    }

  if (root_selected != NULL)
    {
      gtk_math_view_freeze (view);
      gtk_math_view_unselect (view, root_selected);
      gtk_math_view_thaw (view);
      root_selected = NULL;
    }
}

static gboolean
cursor_blink (GtkMathViewDefaultCursorDecorator *cursor)
{
  gboolean enabled;
  xmlElement *focus;
  gboolean draw_focus;
  gint index;
  gboolean char_index;
  g_return_val_if_fail (cursor != NULL, FALSE);

  gtk_math_view_decor_default_cursor_get (cursor, &enabled, &focus, &draw_focus, &index, &char_index);
  gtk_math_view_decor_default_cursor_set (cursor, !enabled, focus, draw_focus, index, char_index);

  return TRUE;
}

static GtkMathView*
setup_widgets ()
{
  GtkWidget *scrolled_area;
  GtkWidget *file_open;
  GtkWidget *file_close;
  GtkWidget *file_reopen;
  GtkWidget *quit;
  GtkWidget *set_font_size;
  GtkWidget *larger_font_size;
  GtkWidget *smaller_font_size;
  GtkWidget *selection_reset;
  GtkWidget *selection_delete;
  GtkWidget *selection_parent;
  GtkWidget *semantics_selection;
  GtkWidget *structure_selection;
  GtkWidget *errors_verbosity;
  GtkWidget *warnings_verbosity;
  GtkWidget *info_verbosity;
  GtkWidget *debug_verbosity;
  GtkWidget *about;

  math_view = GTK_MATH_VIEW (gtk_math_view_new (NULL, NULL));

  file_open = GTK_WIDGET (gtk_builder_get_object (builder, "fileopen"));
  g_signal_connect (file_open, "activate", G_CALLBACK (on_file_open), math_view);

  file_close = GTK_WIDGET (gtk_builder_get_object (builder, "fileclose"));
  g_signal_connect (file_close, "activate", G_CALLBACK (on_file_close), math_view);

  file_reopen = GTK_WIDGET (gtk_builder_get_object (builder, "filereopen"));
  g_signal_connect (file_reopen, "activate", G_CALLBACK (on_file_reopen), math_view);

  quit = GTK_WIDGET (gtk_builder_get_object (builder, "quit"));
  g_signal_connect (quit, "activate", G_CALLBACK (gtk_main_quit), math_view);

  selection_reset = GTK_WIDGET (gtk_builder_get_object (builder, "selectionreset"));
  g_signal_connect (selection_reset, "activate", G_CALLBACK (on_selection_reset), math_view);

  selection_delete = GTK_WIDGET (gtk_builder_get_object (builder, "selectiondelete"));
  g_signal_connect (selection_delete, "activate", G_CALLBACK (on_selection_delete), math_view);

  selection_parent = GTK_WIDGET (gtk_builder_get_object (builder, "selectionparent"));
  g_signal_connect (selection_parent, "activate", G_CALLBACK (on_selection_parent), math_view);

  set_font_size = GTK_WIDGET (gtk_builder_get_object (builder, "setfontsize"));
  g_signal_connect (set_font_size, "activate", G_CALLBACK (on_set_font_size), math_view);

  larger_font_size = GTK_WIDGET (gtk_builder_get_object (builder, "largerfontsize"));
  g_signal_connect (larger_font_size, "activate", G_CALLBACK (on_larger_font_size), math_view);

  smaller_font_size = GTK_WIDGET (gtk_builder_get_object (builder, "smallerfontsize"));
  g_signal_connect (smaller_font_size, "activate", G_CALLBACK (on_smaller_font_size), math_view);

  errors_verbosity = GTK_WIDGET (gtk_builder_get_object (builder, "errorsverbosity"));
  g_signal_connect (errors_verbosity, "activate", G_CALLBACK (on_errors_verbosity), math_view);

  warnings_verbosity = GTK_WIDGET (gtk_builder_get_object (builder, "warningsverbosity"));
  g_signal_connect (warnings_verbosity, "activate", G_CALLBACK (on_warnings_verbosity), math_view);

  info_verbosity = GTK_WIDGET (gtk_builder_get_object (builder, "infoverbosity"));
  g_signal_connect (info_verbosity, "activate", G_CALLBACK (on_info_verbosity), math_view);

  debug_verbosity = GTK_WIDGET (gtk_builder_get_object (builder, "debugverbosity"));
  g_signal_connect (debug_verbosity, "activate", G_CALLBACK (on_debug_verbosity), math_view);

  semantics_selection = GTK_WIDGET (gtk_builder_get_object (builder, "semanticsselection"));
  g_signal_connect (semantics_selection, "activate", G_CALLBACK (on_semantics_selection), math_view);

  structure_selection = GTK_WIDGET (gtk_builder_get_object (builder, "structureselection"));
  g_signal_connect (structure_selection, "activate", G_CALLBACK (on_structure_selection), math_view);

  about = GTK_WIDGET (gtk_builder_get_object (builder, "about"));
  g_signal_connect (about, "activate", G_CALLBACK (help_about), math_view);

  scrolled_area = GTK_WIDGET (gtk_builder_get_object (builder, "scrolledwindow"));

  gtk_container_add (GTK_CONTAINER (scrolled_area), GTK_WIDGET (math_view));
  gtk_widget_set_double_buffered (GTK_WIDGET (math_view), FALSE);
  gtk_widget_show (GTK_WIDGET (math_view));

  g_signal_connect (math_view, "select_begin", G_CALLBACK (select_begin), NULL);
  g_signal_connect (math_view, "select_over", G_CALLBACK (select_over), NULL);
  g_signal_connect (math_view, "select_end", G_CALLBACK (select_end), NULL);
  g_signal_connect (math_view, "select_abort", G_CALLBACK (select_abort), NULL);
  g_signal_connect (math_view, "element_over", G_CALLBACK (element_over), NULL);
  g_signal_connect (math_view, "click", G_CALLBACK (click), NULL);

  return math_view;
}
