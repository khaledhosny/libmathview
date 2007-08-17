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

#include <config.h>

#include "defs.h"
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <gdome.h>
#include "gtkmathview_gmetadom.h"
#include "guiGTK.h"

extern GdomeDOMString* find_hyperlink(GdomeElement*, const char*, const char*);
extern GdomeElement* find_xref_element(GdomeElement*);
extern GdomeElement* find_common_ancestor(GdomeElement*, GdomeElement*);
extern GdomeElement* find_self_or_ancestor(GdomeElement*, const char*, const char*);
extern GdomeElement* find_action_element(GdomeElement*);
extern void action_toggle(GdomeElement*);
extern void delete_element(GdomeElement*);

static GtkWidget* window;
static GtkWidget* main_area;
static GtkWidget* scrolled_area;
static GtkWidget* status_bar;
static GdkCursor* normal_cursor;
static GdkCursor* link_cursor;  
static gboolean   semantic_selection = FALSE;

static gchar* doc_name = NULL;
static GdomeElement* first_selected = NULL;
static GdomeElement* root_selected = NULL;
static GtkMathViewDefaultCursorDecorator* cursor = NULL;

static guint statusbar_context;

static void create_widget_set(void);
static GtkWidget* get_main_menu(void);
static void file_open(GtkWidget*, gpointer);
static void file_re_open(GtkWidget*, gpointer);
static void file_close(GtkWidget*, gpointer);
static void options_set_font_size(GtkWidget*, gpointer);
static void options_change_font_size(GtkWidget*, gboolean);
static void options_verbosity(GtkWidget*, guint);
static void options_transparent(GtkWidget*, gpointer);
static void options_anti_aliased(GtkWidget*, gpointer);
static void options_selection(GtkWidget*, gboolean);
static void selection_delete(GtkWidget*, gpointer);
static void selection_parent(GtkWidget*, gpointer);
static void selection_reset(GtkWidget*, gpointer);
static void help_about(GtkWidget*, gpointer);

static GtkItemFactoryEntry menu_items[] = {
  { "/_File",                          NULL,         NULL,          0, "<Branch>", 0 },
  { "/File/_Open...",                  "<control>O", file_open,     0, NULL, 0 },
  { "/File/_Reopen",                   NULL,         file_re_open,  0, NULL, 0 },
  { "/File/_Close",                    "<control>W", file_close,    0, NULL, 0 },
  { "/File/sep1",                      NULL,         NULL,          0, "<Separator>", 0 },
  { "/File/_Quit",                     "<control>Q", gtk_main_quit, 0, NULL, 0 },

  { "/_Selection",                     NULL, NULL,                  0,  "<Branch>", 0 },
  { "/Selection/Reset",                NULL, selection_reset,       0, NULL, 0 },
  { "/Selection/Delete",               NULL, selection_delete,      0, NULL, 0 },
  { "/Selection/Select Parent",        NULL, selection_parent,      0, NULL, 0 },

  { "/_Options",                       NULL, NULL,                  0,  "<Branch>", 0 },
  { "/Options/Default _Font Size",     NULL, NULL,                  0,  "<Branch>", 0 },
  { "/Options/Default Font Size/Set...", NULL, options_set_font_size, 0,  NULL, 0 },
  { "/Options/Default Font Size/sep1", NULL, NULL,                  0,  "<Separator>", 0 },
  { "/Options/Default Font Size/Larger", "<control>2", options_change_font_size, TRUE, NULL, 0 },
  { "/Options/Default Font Size/Smaller", "<control>1", options_change_font_size, FALSE, NULL, 0 },
  { "/Options/Verbosity",              NULL, NULL,                  0,  "<Branch>", 0 },
  { "/Options/Verbosity/_Errors",      NULL, options_verbosity,     0,  "<RadioItem>", 0 },
  { "/Options/Verbosity/_Warnings",    NULL, options_verbosity,     1,  "/Options/Verbosity/Errors", 0 },
  { "/Options/Verbosity/_Info",        NULL, options_verbosity,     2,  "/Options/Verbosity/Errors", 0 },
  { "/Options/Verbosity/_Debug",       NULL, options_verbosity,     3,  "/Options/Verbosity/Errors", 0 },
  { "/Options/Type 1",                 NULL, NULL,                  0,  "<Branch>", 0 },
  { "/Options/Type 1/_Transparent",    NULL, options_transparent,   TRUE,  "<CheckItem>", 0 },
  { "/Options/Type 1/_Anti Aliased",   NULL, options_anti_aliased,  0,  "<CheckItem>", 0 },
  { "/Options/sep1",                   NULL, NULL,                  0,  "<Separator>", 0 },
  { "/Options/Selection/Structure",    NULL, options_selection,     0,  "<RadioItem>", 0 },
  { "/Options/Selection/Semantics",    NULL, options_selection,     1,  "/Options/Selection/Structure", 0 },

  { "/_Help" ,        NULL,         NULL,          0, "<Branch>", 0 },
  { "/Help/About...", NULL,         help_about,    0, NULL, 0 }
};

#if 0
static void
quick_message(const char* msg)
{
  GtkWidget* dialog;
  GtkWidget* label;
  GtkWidget* okay_button;
     
  /* Create the widgets */
     
  dialog = gtk_dialog_new();
  label = gtk_label_new (msg);
  okay_button = gtk_button_new_with_label("OK");

  gtk_widget_set_usize(dialog, 300, 100);

  /* Ensure that the dialog box is destroyed when the user clicks ok. */
     
  /* g_signal_connect_object(GTK_OBJECT (okay_button), 
		  "clicked",
		  GTK_SIGNAL_FUNC(gtk_widget_destroy), 
		  dialog,0);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->action_area),
		     okay_button);
		     */
  
  g_signal_connect_swapped(GTK_OBJECT(okay_button),
		  "clicked",
		  G_CALLBACK(gtk_widget_destroy),
		  dialog);
  
  /* Add the label, and show everything we've added to the dialog. */
  
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);
  gtk_widget_show_all (dialog);
}

static void
load_error_msg(const char* name)
{
  char* msg = g_strdup_printf("Could not load\n`%s'", name);
  quick_message(msg);
  g_free(msg);
}
#endif

void
GUI_init(int* argc, const char*** argv, char* title, guint width, guint height, gint logLevel)
{
  gtk_init(argc, argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), title);
  gtk_window_set_default_size(GTK_WINDOW(window), width, height);
  
  g_signal_connect(GTK_OBJECT(window), 
		  "delete_event", 
		  (GtkSignalFunc) gtk_main_quit, NULL);
  
  create_widget_set();
  g_assert(main_area != NULL);
  g_assert(GTK_IS_MATH_VIEW(main_area));
  gtk_math_view_set_log_verbosity(GTK_MATH_VIEW(main_area), logLevel);

  gtk_widget_show(window);

  normal_cursor = gdk_cursor_new(GDK_TOP_LEFT_ARROW);
  link_cursor = gdk_cursor_new(GDK_HAND2);
}

void
GUI_uninit()
{
  /*gtk_widget_unref(GTK_WIDGET(window));*/
}

int
GUI_load_document(const char* name)
{
  GtkMathView* math_view;

  g_return_val_if_fail(name != NULL, -1);
  g_return_val_if_fail(main_area != NULL, -1);
  g_return_val_if_fail(GTK_IS_MATH_VIEW(main_area), -1);

  math_view = GTK_MATH_VIEW(main_area);

  gtk_math_view_load_uri(math_view, name);

  if (name != doc_name) {
    if (doc_name != NULL) g_free(doc_name);
    doc_name = g_strdup(name);
  }

  gtk_statusbar_pop(GTK_STATUSBAR(status_bar), statusbar_context);
  if (strlen(name) > 40) name += strlen(name) - 40;
  gtk_statusbar_push(GTK_STATUSBAR(status_bar), statusbar_context, name);
    
  return 0;
}

void
GUI_unload_document()
{
  GtkMathView* math_view;

  g_return_if_fail(main_area != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(main_area));

  math_view = GTK_MATH_VIEW(main_area);

  gtk_math_view_unload(math_view);

  if (doc_name != NULL) g_free(doc_name);
  doc_name = NULL;

  gtk_statusbar_pop(GTK_STATUSBAR(status_bar), statusbar_context);
  gtk_statusbar_push(GTK_STATUSBAR(status_bar), statusbar_context, "");
}

void
GUI_run()
{
  gtk_main();
}

static void
store_filename(GtkFileSelection* selector, GtkWidget* user_data)
{
  const gchar* selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION(user_data));
  if (selected_filename != NULL)
    GUI_load_document(selected_filename);
}

static void
file_close(GtkWidget* widget, gpointer data)
{
  GUI_unload_document();
}

static void
file_re_open(GtkWidget* widget, gpointer data)
{
  if (doc_name != NULL) {
    GUI_load_document(doc_name);
  }
}

static void
file_open(GtkWidget* widget, gpointer data)
{
  GtkWidget* fs = gtk_file_selection_new("Open File");

  g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION(fs)->ok_button),
		      "clicked", 
		      GTK_SIGNAL_FUNC(store_filename), (gpointer) fs);
                             
  /* Ensure that the dialog box is destroyed when the user clicks a button. */
     
  g_signal_connect_swapped(GTK_OBJECT (GTK_FILE_SELECTION(fs)->ok_button),
			     "clicked", 
			     G_CALLBACK(gtk_widget_destroy),
			     (gpointer) fs);

  /* gtk_signal_connect_object(GTK_OBJECT (GTK_FILE_SELECTION(fs)->cancel_button),
			     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     (gpointer) fs);
			     */

  g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(fs)->cancel_button),
		  "clicked",
		  G_CALLBACK(gtk_widget_destroy),
		  (gpointer) fs);
     
  /* Display that dialog */
     
  gtk_widget_show (fs);
}

static void
options_verbosity(GtkWidget* widget, guint level)
{
  gtk_math_view_set_log_verbosity(GTK_MATH_VIEW(main_area), level);
}

static void
options_transparent(GtkWidget* widget, gpointer data)
{
  GtkMathView* math_view = GTK_MATH_VIEW(main_area);
  g_return_if_fail(math_view != NULL);
  gtk_math_view_set_t1_opaque_mode(math_view, !gtk_math_view_get_t1_opaque_mode(math_view));
}

static void
options_anti_aliased(GtkWidget* widget, gpointer data)
{
  GtkMathView* math_view = GTK_MATH_VIEW(main_area);
  g_return_if_fail(math_view != NULL);
  gtk_math_view_set_t1_anti_aliased_mode(math_view, !gtk_math_view_get_t1_anti_aliased_mode(math_view));
}

static void
options_selection(GtkWidget* widget, gboolean semantic)
{
  semantic_selection = semantic;
  selection_reset(widget, NULL);
}

static void
selection_delete(GtkWidget* widget, gpointer data)
{
  if (root_selected != NULL)
    {
      GdomeException exc;
      gtk_math_view_freeze(GTK_MATH_VIEW(main_area));
      delete_element(root_selected);
      gdome_el_unref(root_selected, &exc);
      g_assert(exc == 0);
      root_selected = NULL;
      gtk_math_view_thaw(GTK_MATH_VIEW(main_area));
    }
}

static void
selection_parent(GtkWidget* widget, gpointer data)
{
  if (root_selected != NULL)
    {
      GdomeException exc = 0;
      GdomeElement* parent = (GdomeElement*) gdome_n_parentNode((GdomeNode*) root_selected, &exc);
      g_assert(exc == 0);
      gdome_el_unref(root_selected, &exc);
      g_assert(exc == 0);
      root_selected = parent;
      gtk_math_view_select(GTK_MATH_VIEW(main_area), root_selected);
    }
}

static void
selection_reset(GtkWidget* widget, gpointer data)
{
  if (root_selected != NULL)
    {
      GdomeException exc = 0;
      gtk_math_view_unselect(GTK_MATH_VIEW(main_area), root_selected);
      gdome_el_unref(root_selected, &exc);
      g_assert(exc == 0);
      root_selected = NULL;
    }
}

static void
help_about(GtkWidget* widget, gpointer data)
{
  GtkWidget* dialog;
  GtkWidget* label;
  GtkWidget* ok;

  dialog = gtk_dialog_new();
  label = gtk_label_new("\n    MathML Viewer    \n    Copyright (C) 2000-2004 Luca Padovani    \n");
  ok = gtk_button_new_with_label("Close");

  /* gtk_signal_connect_object (GTK_OBJECT (ok), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy), (gpointer) dialog); */

  g_signal_connect_swapped(GTK_OBJECT(ok),
		  "clicked",
		  G_CALLBACK(gtk_widget_destroy),
		  (gpointer) dialog);
  
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->action_area),
		     ok);

  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);

  gtk_widget_show_all (dialog);
}

static void
change_default_font_size(GtkSpinButton* widget, GtkSpinButton* spin)
{
  g_return_if_fail(spin != NULL);
  gtk_math_view_set_font_size( GTK_MATH_VIEW(main_area), gtk_spin_button_get_value_as_int(spin));
}

static void
options_change_font_size(GtkWidget* widget, gboolean larger)
{
  gfloat size = gtk_math_view_get_font_size (GTK_MATH_VIEW(main_area));
  if (larger) size = size / 0.71;
  else size = size * 0.71;
  if (size < 1) size = 1;
  gtk_math_view_set_font_size (GTK_MATH_VIEW(main_area), (gint) size + 0.5);
}

static void
options_set_font_size(GtkWidget* widget, gpointer data)
{
  GtkWidget* dialog;
  GtkWidget* label;
  GtkWidget* ok;
  GtkWidget* cancel;
  GtkWidget* spin;
  GtkObject* adj;

  dialog = gtk_dialog_new();
  label = gtk_label_new("Default font size:");
  ok = gtk_button_new_with_label("OK");
  cancel = gtk_button_new_with_label("Cancel");

  adj = gtk_adjustment_new (gtk_math_view_get_font_size (GTK_MATH_VIEW(main_area)), 1, 200, 1, 1, 1);
  spin = gtk_spin_button_new (GTK_ADJUSTMENT(adj), 1, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin), TRUE);

  /* gtk_signal_connect (GTK_OBJECT (ok), "clicked",
		      GTK_SIGNAL_FUNC (change_default_font_size), (gpointer) spin); */

  g_signal_connect(GTK_OBJECT(ok),
		  "clicked",
		  G_CALLBACK(change_default_font_size),
		  (gpointer) spin);

  /* gtk_signal_connect_object (GTK_OBJECT (ok), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy), (gpointer) dialog); */

  g_signal_connect_swapped(GTK_OBJECT (ok), 
		  "clicked",
		  G_CALLBACK(gtk_widget_destroy), 
		  (gpointer) dialog);


  /* gtk_signal_connect_object (GTK_OBJECT (ok), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy), (gpointer) dialog);*/

  /* gtk_signal_connect_object (GTK_OBJECT (cancel), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy), (gpointer) dialog); */
  
  g_signal_connect_swapped(GTK_OBJECT (cancel),
		  "clicked",
		  G_CALLBACK(gtk_widget_destroy), 
		  (gpointer) dialog);


  gtk_container_set_border_width (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), 5);

  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->action_area), ok);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->action_area), cancel);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), spin);

  gtk_widget_show_all (dialog);
}

#if defined(HAVE_GMETADOM)
static void
element_over(GtkMathView* math_view, const GtkMathViewModelEvent* event)
{
  GdomeElement* action = NULL;
  GdomeDOMString* link = NULL;

  g_return_if_fail(math_view != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(math_view));
  g_return_if_fail(event != NULL);

  link = find_hyperlink(event->id, XLINK_NS_URI, "href");
  if (link != NULL)
    gdk_window_set_cursor(GTK_WIDGET(math_view)->window, link_cursor);
  else
    gdk_window_set_cursor(GTK_WIDGET(math_view)->window, normal_cursor);

  if (link != NULL)
    gdome_str_unref(link);

  action = find_action_element(event->id);
#if 0
  if (action != NULL) 
    {
      GdomeException exc = 0;
      gtk_math_view_decor_default_cursor_set(cursor, TRUE, action, TRUE, -1, FALSE);
      gdome_el_unref(action, &exc);
      g_assert(exc == 0);
    }
#endif
}

static void
select_begin(GtkMathView* math_view, const GtkMathViewModelEvent* event)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(math_view));
  g_return_if_fail(event != NULL);

  if (event->id != NULL)
    {
      GdomeException exc = 0;

      gtk_math_view_freeze(math_view);

      if (root_selected != NULL)
	{
	  gtk_math_view_unselect(math_view, root_selected);
	  gdome_el_unref(root_selected, &exc);
	  g_assert(exc == 0);
	  root_selected = NULL;
	}

      if (semantic_selection)
	{
	  GdomeElement* new_elem = find_xref_element(event->id);
	  if (new_elem != NULL)
            {
	      gdome_el_ref(new_elem, &exc);
	      g_assert(exc == 0);
	    }
          first_selected = root_selected = new_elem;
	}
      else
	{
	  gdome_el_ref(event->id, &exc);
	  g_assert(exc == 0);
	  gdome_el_ref(event->id, &exc);
	  g_assert(exc == 0);
          first_selected = root_selected = event->id;
	}

      if (root_selected != NULL)
	gtk_math_view_select(math_view, root_selected);

      gtk_math_view_thaw(math_view);
    }
}

static void
select_over(GtkMathView* math_view, const GtkMathViewModelEvent* event)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(math_view));
  g_return_if_fail(event != NULL);

  if (first_selected != NULL && event->id != NULL)
    {
      GdomeException exc = 0;

      gtk_math_view_freeze(math_view);

      if (root_selected != NULL)
	{
	  gtk_math_view_unselect(math_view, root_selected);
	  gdome_el_unref(root_selected, &exc);
	  g_assert(exc == 0);
	  root_selected = NULL;
	}

      if (semantic_selection)
	{
	  GdomeElement* new_root = find_common_ancestor(first_selected, event->id);
	  if (new_root != NULL)
	    {
	      root_selected = find_xref_element(new_root);
	      gdome_el_unref(new_root, &exc);
	      g_assert(exc == 0);
	    }
	  else
	    root_selected = NULL;
	}
      else
        root_selected = find_common_ancestor(first_selected, event->id);

      while (root_selected != NULL && !gtk_math_view_select(math_view, root_selected))
	{
	  GdomeElement* new_root = (GdomeElement*) gdome_el_parentNode(root_selected, &exc);
	  g_assert(exc == 0);
	  gdome_el_unref(root_selected, &exc);
	  g_assert(exc == 0);
	  root_selected = new_root;
	}

      gtk_math_view_thaw(math_view);
    }
}

static void
select_end(GtkMathView* math_view, const GtkMathViewModelEvent* event)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(math_view));
  g_return_if_fail(event != NULL);

  if (first_selected != NULL)
    {
      GdomeException exc = 0;
      gdome_el_unref(first_selected, &exc);
      g_assert(exc == 0);
      first_selected = NULL;
    }
}

static void
select_abort(GtkMathView* math_view)
{
  GdomeException exc = 0;

  g_return_if_fail(math_view != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(math_view));

  if (first_selected != NULL)
    {
      gdome_el_unref(first_selected, &exc);
      g_assert(exc == 0);
      first_selected = NULL;
    }

  if (root_selected != NULL)
    {
      gtk_math_view_freeze(math_view);
      gtk_math_view_unselect(math_view, root_selected);
      gtk_math_view_thaw(math_view);
      gdome_el_unref(root_selected, &exc);
      g_assert(exc == 0);
      root_selected = NULL;
    }
}

static void
click(GtkMathView* math_view, const GtkMathViewModelEvent* event)
{
  gboolean res;
  gint index;
  GdomeElement* elem;
  GdomeException exc;
  GdomeDOMString* name;
  GdomeDOMString* ns_uri;
  GdomeElement* p;

  gint w, h;
  gtk_math_view_get_size(math_view, &w, &h);
  printf("click signal %d %d\n", w, h);

  g_return_if_fail(math_view != NULL);
  g_return_if_fail(event != NULL);

#if 1
  index = -1;
  if (gtk_math_view_get_char_at(math_view, event->x, event->y, &elem, &index, NULL, NULL))
    gtk_math_view_decor_default_cursor_set(cursor, TRUE, event->id, TRUE, index, TRUE);
  else
    gtk_math_view_decor_default_cursor_set(cursor, FALSE, NULL, FALSE, -1, FALSE);
#endif

  if (event->id != NULL)
    {
      name = gdome_el_nodeName(event->id, &exc);
      g_assert(exc == 0);
      printf("node name: %s\n", name->str);
    }

  if (event->id != NULL)
    {
      GdomeElement* action;
      GdomeDOMString* href = find_hyperlink(event->id, XLINK_NS_URI, "href");
      if (href != NULL)
	{
 	  printf("hyperlink %s\n", href->str);
	  gdome_str_unref(href);
	}

      action = find_action_element(event->id);
      printf("action? %p\n", action);
      if (action != NULL)
	{
	  gtk_math_view_freeze(math_view);
	  action_toggle(action);
	  gtk_math_view_thaw(math_view);
	  gdome_el_unref(action, &exc);
	  g_assert(exc == 0);
	  return;
	}
    }

  if (root_selected != NULL)
    {
      gtk_math_view_freeze(math_view);
      gtk_math_view_unselect(math_view, root_selected);
      gtk_math_view_thaw(math_view);
      gdome_el_unref(root_selected, &exc);
      g_assert(exc == 0);
      root_selected = NULL;
    }
}

#endif // HAVE_GMETADOM

static gboolean
cursor_blink(GtkMathViewDefaultCursorDecorator* cursor)
{
  gboolean enabled;
  GdomeElement* focus;
  gboolean draw_focus;
  gint index;
  gboolean char_index;
  g_return_val_if_fail(cursor != NULL, FALSE);

  gtk_math_view_decor_default_cursor_get(cursor, &enabled, &focus, &draw_focus, &index, &char_index);
  gtk_math_view_decor_default_cursor_set(cursor, !enabled, focus, draw_focus, index, char_index);

  return TRUE;
}

static void
create_widget_set()
{
  GtkWidget* main_vbox;
  GtkWidget* menu_bar;

  main_vbox = gtk_vbox_new(FALSE, 1);
  gtk_container_border_width(GTK_CONTAINER(main_vbox), 1);
  gtk_container_add(GTK_CONTAINER(window), main_vbox);
  gtk_widget_show(main_vbox);

  menu_bar = get_main_menu();
  gtk_box_pack_start(GTK_BOX(main_vbox), menu_bar, FALSE, TRUE, 0);
  gtk_widget_show(menu_bar);

  scrolled_area = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_area),
				 GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_area),
				      GTK_SHADOW_IN);
  gtk_widget_show(scrolled_area);
  gtk_box_pack_start(GTK_BOX(main_vbox), scrolled_area, TRUE, TRUE, 0);

  main_area = gtk_math_view_new(NULL, NULL);
  gtk_widget_show(main_area);
  gtk_container_add(GTK_CONTAINER(scrolled_area), main_area);
  gtk_widget_set_double_buffered(main_area, FALSE);

  g_signal_connect(GTK_OBJECT (main_area),
		   "select_begin", 
		   G_CALLBACK(select_begin),
		   (gpointer) main_area);

  g_signal_connect(GTK_OBJECT (main_area),
		   "select_over", 
		   G_CALLBACK(select_over),
		   (gpointer) main_area);

  g_signal_connect(GTK_OBJECT (main_area),
		   "select_end", 
		   G_CALLBACK(select_end),
		   (gpointer) main_area);
 
  g_signal_connect(GTK_OBJECT (main_area),
		   "select_abort", 
		   G_CALLBACK(select_abort),
		   (gpointer) main_area);

  g_signal_connect(GTK_OBJECT (main_area),
		   "element_over", 
		   G_CALLBACK(element_over),
		   (gpointer) main_area);
  
  g_signal_connect(GTK_OBJECT (main_area),
		   "click", 
		   G_CALLBACK(click),
		   (gpointer) main_area);

  cursor = gtk_math_view_decor_default_cursor_new(GTK_MATH_VIEW(main_area));
  g_timeout_add(500, cursor_blink, cursor);

  status_bar = gtk_statusbar_new();
  gtk_widget_show(status_bar);
  gtk_box_pack_start(GTK_BOX(main_vbox), status_bar, FALSE, TRUE, 0);
  statusbar_context = gtk_statusbar_get_context_id(GTK_STATUSBAR(status_bar), "filename");

  gtk_widget_show(main_vbox);
}

GtkWidget*
get_main_menu()
{
  GtkItemFactory* item_factory;
  GtkAccelGroup* accel_group;

  gint nmenu_items = sizeof(menu_items) / sizeof(menu_items[0]);

  accel_group = gtk_accel_group_new();

  item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<main>", accel_group);

  gtk_item_factory_create_items(item_factory, nmenu_items, menu_items, NULL);

  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

  return gtk_item_factory_get_widget(item_factory, "<main>");
}
