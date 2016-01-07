// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// Copyright (C) 2012-2014, Khaled Hosny <khaledhosny@eglug.org>.
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

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "defs.h"
#include "gtkmathview_libxml2.h"

static gboolean cursor_blink (GtkMathViewDefaultCursorDecorator*);

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
  GdkDisplay *display;
  GdkWindow *window;
  GdkCursor *cursor;

  display = gtk_widget_get_display (GTK_WIDGET (view));
  window = gtk_widget_get_window (GTK_WIDGET (view));

  link = find_hyperlink (event->id, "href");
  if (link != NULL)
    cursor = gdk_cursor_new_for_display (display, GDK_HAND2);
  else
    cursor = gdk_cursor_new_for_display (display, GDK_TOP_LEFT_ARROW);

  gdk_window_set_cursor (window, cursor);
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

static void
select_begin (GtkMathView                 *view,
              const GtkMathViewModelEvent *event)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));
  g_return_if_fail (event != NULL);

  if (event->id != NULL)
    {
      xmlElement *root_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "root-selected"));
      xmlElement *first_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "first-selected"));

      gtk_math_view_freeze (view);

      if (root_selected != NULL)
        {
          gtk_math_view_unselect (view, root_selected);
        }

      first_selected = root_selected = event->id;
      gtk_math_view_select (view, root_selected);

      g_object_set_data (G_OBJECT (view), "root-selected", root_selected);
      g_object_set_data (G_OBJECT (view), "first-selected", first_selected);

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

  xmlElement *root_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "root-selected"));
  xmlElement *first_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "first-selected"));

  if (first_selected != NULL && event->id != NULL)
    {
      gtk_math_view_freeze (view);

      if (root_selected != NULL)
        {
          gtk_math_view_unselect (view, root_selected);
        }

      root_selected = find_common_ancestor (first_selected, event->id);

      while (root_selected != NULL && !gtk_math_view_select (view, root_selected))
        {
          xmlElement *new_root = (xmlElement*) ((xmlNodePtr) root_selected)->parent;
          root_selected = new_root;
        }

      gtk_math_view_thaw (view);
    }

  g_object_set_data (G_OBJECT (view), "root-selected", root_selected);
  g_object_set_data (G_OBJECT (view), "first-selected", first_selected);
}

static void
select_end (GtkMathView                 *view,
            const GtkMathViewModelEvent *event)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));
  g_return_if_fail (event != NULL);

  xmlElement *first_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "first-selected"));

  if (first_selected != NULL)
    {
      g_object_set_data (G_OBJECT (view), "first-selected", NULL);
    }
}

static void
select_abort (GtkMathView *view)
{
  g_return_if_fail (view != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (view));

  xmlElement *root_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "root-selected"));
  xmlElement *first_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "first-selected"));

  if (first_selected != NULL)
    {
      g_object_set_data (G_OBJECT (view), "first-selected", NULL);
    }

  if (root_selected != NULL)
    {
      gtk_math_view_freeze (view);
      gtk_math_view_unselect (view, root_selected);
      gtk_math_view_thaw (view);
      g_object_set_data (G_OBJECT (view), "root-selected", NULL);
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

  xmlElement *root_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "root-selected"));

  gint w, h;
  gtk_math_view_get_size (view, &w, &h);
  g_debug ("click signal %d %d\n", w, h);

  guint timer = GPOINTER_TO_UINT (g_object_get_data (G_OBJECT (view), "cursor-timer"));
  if (timer)
    g_source_remove (timer);

  cursor = gtk_math_view_decor_default_cursor_new (view);
  timer = g_timeout_add_full (G_PRIORITY_DEFAULT, 500,
                              (GSourceFunc) cursor_blink, cursor,
                              (GDestroyNotify) gtk_math_view_decor_default_cursor_free);
  g_object_set_data (G_OBJECT (view), "cursor-timer", GUINT_TO_POINTER (timer));

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
      g_object_set_data (G_OBJECT (view), "root-selected", NULL);
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

static void
window_selection_reset (GSimpleAction *action,
                        GVariant      *parameter,
                        gpointer       user_data)
{
  GtkMathView *view = GTK_MATH_VIEW (user_data);
  xmlElement *root_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "root-selected"));

  if (root_selected != NULL)
    {
      gtk_math_view_unselect (view, root_selected);
      g_object_set_data (G_OBJECT (view), "root-selected", NULL);
    }
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
window_selection_delete (GSimpleAction *action,
                         GVariant      *parameter,
                         gpointer       user_data)
{
  GtkMathView *view = GTK_MATH_VIEW (user_data);
  xmlElement *root_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "root-selected"));

  if (root_selected != NULL)
    {
      gtk_math_view_freeze (view);
      delete_element (view, root_selected);
      g_object_set_data (G_OBJECT (view), "root-selected", NULL);
      gtk_math_view_thaw (view);
    }
}

static void
window_select_parent (GSimpleAction *action,
                      GVariant      *parameter,
                      gpointer       user_data)
{
  GtkMathView *view = GTK_MATH_VIEW (user_data);
  xmlElement *root_selected = static_cast<xmlElement*>(g_object_get_data (G_OBJECT (view), "root-selected"));

  if (root_selected != NULL)
    {
      xmlElement *parent = (xmlElement*) ((xmlNodePtr) root_selected)->parent;
      g_object_set_data (G_OBJECT (view), "root-selected", parent);
      gtk_math_view_select (view, parent);
    }
}

static void
window_zoom_in (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       user_data)
{
  GtkMathView *view = GTK_MATH_VIEW (user_data);
  gfloat size = gtk_math_view_get_font_size (view) / 0.71;

  if (size < 1)
    size = 1;

  gtk_math_view_set_font_size (view, (gint) size + 0.5);
}

static void
window_zoom_out (GSimpleAction *action,
                 GVariant      *parameter,
                 gpointer       user_data)
{
  GtkMathView *view = GTK_MATH_VIEW (user_data);
  gfloat size = gtk_math_view_get_font_size (view) * 0.71;

  if (size < 1)
    size = 1;

  gtk_math_view_set_font_size (view, (gint) size + 0.5);
}

static void
mml_view_new_window (GApplication *app,
                     GFile        *file)
{
  GtkWidget *window;
  GtkWidget *view;

  window = gtk_application_window_new (GTK_APPLICATION (app));
  view = gtk_math_view_new (NULL, NULL);

  g_object_set_data (G_OBJECT (view), "root-selected", NULL);
  g_object_set_data (G_OBJECT (view), "first-selected", NULL);

  GActionEntry entries[] = {
    { "selection-reset", window_selection_reset, NULL, NULL, NULL, { 0 } },
    { "selection-delete", window_selection_delete, NULL, NULL, NULL, { 0 } },
    { "select-parent", window_select_parent, NULL, NULL, NULL, { 0 } },
    { "zoom-in", window_zoom_in, NULL, NULL, NULL, { 0 } },
    { "zoom-out", window_zoom_out, NULL, NULL, NULL, { 0 } },
  };

  g_action_map_add_action_entries (G_ACTION_MAP (window), entries, G_N_ELEMENTS (entries), view);

  if (file != NULL)
    {
      gchar *buffer;
      gsize length;

      if (g_file_load_contents (file, NULL, &buffer, &length, NULL, NULL))
        {
          gtk_math_view_load_buffer (GTK_MATH_VIEW (view), buffer);
        }
    }

  g_signal_connect (view, "select_begin", G_CALLBACK (select_begin), NULL);
  g_signal_connect (view, "select_over", G_CALLBACK (select_over), NULL);
  g_signal_connect (view, "select_end", G_CALLBACK (select_end), NULL);
  g_signal_connect (view, "select_abort", G_CALLBACK (select_abort), NULL);
  g_signal_connect (view, "element_over", G_CALLBACK (element_over), NULL);
  g_signal_connect (view, "click", G_CALLBACK (click), NULL);

  gtk_math_view_set_font_size (GTK_MATH_VIEW (view), 24);

  gtk_container_add (GTK_CONTAINER (window), view);
  gtk_widget_show_all (window);
}

static void
open_activated (GSimpleAction *action,
               GVariant      *parameter,
               gpointer       user_data)
{
  GApplication *app = G_APPLICATION (user_data);
  GtkWidget* dialog;
  GFile **files = { NULL };
  gint n_files = 0;

  dialog = gtk_file_chooser_dialog_new("Open File",
                                       NULL,
                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                       "_Open", GTK_RESPONSE_ACCEPT,
                                       NULL);

  gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER (dialog), TRUE);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    GSList *list;

    list = gtk_file_chooser_get_files (GTK_FILE_CHOOSER (dialog));
    n_files = g_slist_length (list);
    files = g_new (GFile*, n_files);

    for (gint i = 0; i < n_files; i++)
      files[i] = G_FILE (g_slist_nth_data (list, i));
  }

  gtk_widget_destroy(dialog);

  g_application_open (app, files, n_files, "");
}

static void
about_activated (GSimpleAction *action,
                 GVariant      *parameter,
                 gpointer       user_data)
{
  static const gchar * const authors[] = {
    "Luca Padovani <padovani@sti.uniurb.it>",
    "Khaled Hosny <khaledhosny@eglug.org>",
    NULL
  };

  static const gchar copyright[] = "Copyright \xc2\xa9 2000-2012 Luca Padovani\n"
                                   "Copyright \xc2\xa9 2012-2014 Khaled Hosny";
  gtk_show_about_dialog(NULL,
                        "program-name", "Math View",
                        "version", PACKAGE_VERSION,
                        "comments", "A simple MathML viewer",
                        "copyright", copyright,
                        "authors", authors,
                        "license-type", GTK_LICENSE_LGPL_3_0,
                        NULL);
}

static void
quit_activated (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       user_data)
{
  GApplication *app = G_APPLICATION (user_data);

  g_application_quit (app);
}

static void
mml_view_startup (GApplication *application)
{
  GtkApplication* app = GTK_APPLICATION (application);
  GtkBuilder* builder;

  GActionEntry entries[] = {
    { "open", open_activated, NULL, NULL, NULL, { 0 } },
    { "about", about_activated, NULL, NULL, NULL, { 0 } },
    { "quit", quit_activated, NULL, NULL, NULL, { 0 } },
  };

  g_action_map_add_action_entries (G_ACTION_MAP (application), entries, G_N_ELEMENTS (entries), application);

  builder = gtk_builder_new();
  gtk_builder_add_from_resource(builder, "/org/mathview/viewer/mml-view-gtk.ui", NULL);

  gtk_application_set_app_menu (app, G_MENU_MODEL (gtk_builder_get_object (builder, "app-menu")));
  gtk_application_set_menubar (app, G_MENU_MODEL (gtk_builder_get_object (builder, "menubar")));
}

static void
mml_view_open (GApplication  *app,
               GFile        **files,
               gint           n_files)
{
  for (gint i = 0; i < n_files; i++)
    mml_view_new_window (app, files[i]);
}

static void
mml_view_activate (GApplication *app)
{
  mml_view_new_window (app, NULL);
}

int
main (int argc, char *argv[])
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.mathview.viewer", G_APPLICATION_HANDLES_OPEN);

  g_signal_connect (app, "startup", G_CALLBACK (mml_view_startup), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (mml_view_activate), NULL);
  g_signal_connect (app, "open", G_CALLBACK (mml_view_open), NULL);

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
