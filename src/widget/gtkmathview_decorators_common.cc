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

typedef struct _GtkMathViewDefaultCursorDecorator {
  GtkMathView* math_view;
  gboolean enabled;
  gulong handler_id;
  GtkMathViewModelId element;
  gint index;
  gboolean draw_focus;
  gboolean char_index;
} GtkMathViewDefaultCursorDecorator;

// This is a (simplified) implementation of gtk_draw_insertion_cursor() that
// make the caret stem width relative to font size not caret height, as the
// later is variable in our case and gives us variable and often ugly carets
static void
draw_insertion_cursor(GtkMathView* math_view, GdkDrawable *drawable,
		      double x, double y, double height,
		      GtkTextDirection direction)
{
  cairo_t* cr;
  GdkColor *cursor_color;
  double stem_width, offset, cursor_aspect_ratio;
  int font_size;

  cr = gdk_cairo_create(drawable);

  gtk_widget_style_get(GTK_WIDGET(math_view),
	               "cursor-aspect-ratio",
		       &cursor_aspect_ratio,
	               NULL);
  gtk_widget_style_get(GTK_WIDGET(math_view),
		       "cursor-color",
		       &cursor_color,
		       NULL);

  if (cursor_color)
    gdk_cairo_set_source_color(cr, cursor_color);

  font_size = GTKMATHVIEW_METHOD_NAME(get_font_size)(math_view);
  stem_width = font_size * cursor_aspect_ratio + 1;

  if (direction == GTK_TEXT_DIR_LTR)
    offset = stem_width / 2;
  else
    offset = stem_width - stem_width / 2;

  cairo_rectangle(cr, x - offset, y, stem_width, height);
  cairo_fill(cr);
  cairo_destroy(cr);
}

extern "C" void
default_cursor_handler(GtkMathView* math_view, GdkDrawable* drawable,
		       GtkMathViewDefaultCursorDecorator* cursor)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(drawable != NULL);
  g_return_if_fail(cursor != NULL);

  if (cursor->enabled && cursor->element != NULL)
    {
      GtkMathViewPoint focus_orig;
      GtkMathViewBoundingBox focus_box;
      if (!GTKMATHVIEW_METHOD_NAME(get_element_extents)(math_view, cursor->element, &focus_orig, &focus_box))
	return;

      if (cursor->draw_focus)
	gtk_paint_focus(GTK_WIDGET(math_view)->style,
			drawable,
			GTK_STATE_NORMAL,
			NULL,
			GTK_WIDGET(math_view),
			"?",
			focus_orig.x,
			focus_orig.y - focus_box.height,
			focus_box.width,
			focus_box.height + focus_box.depth);
 
      if (cursor->index >= 0)
	{
	  GtkMathViewPoint char_orig;
	  GtkMathViewBoundingBox char_box;
	  double x;
	  if (cursor->char_index &&
	      GTKMATHVIEW_METHOD_NAME(get_char_extents)(math_view, cursor->element, cursor->index,
							&char_orig, &char_box))
	    {
	      x = char_orig.x;
	    }
	  else
	    {
	      x = focus_orig.x;
	      if (cursor->index > 0) x += focus_box.width;
	    }
	  double y = focus_orig.y - focus_box.height;
	  double height = focus_box.height + focus_box.depth;

	  draw_insertion_cursor(math_view, drawable,
				x, y, height,
				GTK_TEXT_DIR_LTR);

	}
    }
}

extern "C" GtkMathViewDefaultCursorDecorator*
GTKMATHVIEW_METHOD_NAME(decor_default_cursor_new)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  GtkMathViewDefaultCursorDecorator* cursor = g_new(GtkMathViewDefaultCursorDecorator, 1);
  g_assert(cursor != NULL);
  cursor->math_view = math_view;
  gtk_widget_ref(GTK_WIDGET(math_view));
  cursor->enabled = FALSE;
  cursor->element = 0;
  cursor->index = -1;
  cursor->draw_focus = cursor->char_index = FALSE;
  cursor->handler_id = g_signal_connect(math_view, "decorate_over",
					G_CALLBACK(default_cursor_handler),
					(gpointer) cursor);

  return cursor;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(decor_default_cursor_free)(GtkMathViewDefaultCursorDecorator* cursor)
{
  g_return_if_fail(cursor != NULL);
  gtk_widget_unref(GTK_WIDGET(cursor->math_view));

  g_signal_handler_disconnect(cursor->math_view, cursor->handler_id);

  cursor->math_view = NULL;
#if GTKMATHVIEW_USES_GMETADOM
  if (cursor->element)
    {
      GdomeException exc = 0;
      gdome_el_unref(cursor->element, &exc);
      g_assert(exc == 0);
      cursor->element = NULL;
    }
#else
  cursor->element = 0;
#endif 
  g_free(cursor);
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(decor_default_cursor_set)(GtkMathViewDefaultCursorDecorator* cursor,
						  gboolean enabled,
						  GtkMathViewModelId elem, gboolean draw_focus,
						  gint index, gboolean char_index)
{
  g_return_if_fail(cursor != NULL);

  bool need_update = false;

  if (cursor->enabled != enabled)
    {
      cursor->enabled = enabled;
      need_update = true;
    }

  if (cursor->element != elem)
    {
#if GTKMATHVIEW_USES_GMETADOM
      GdomeException exc = 0;
      if (cursor->element)
	{
	  gdome_el_unref(cursor->element, &exc);
	  g_assert(exc == 0);
	}
#endif
      cursor->element = elem;
#if GTKMATHVIEW_USES_GMETADOM
      if (cursor->element)
	{
	  gdome_el_ref(cursor->element, &exc);
	  g_assert(exc == 0);
	}
#endif
      need_update = true;
    }

  if (cursor->draw_focus != draw_focus)
    {
      cursor->draw_focus = draw_focus;
      need_update = true;
    }

  if (cursor->index != index)
    {
      cursor->index = index;
      need_update = true;
    }

  if (cursor->char_index != char_index)
    {
      cursor->char_index = char_index;
      need_update = true;
    }

  if (need_update)
    // should update only the region where the cursor is drawn
    GTKMATHVIEW_METHOD_NAME(update)(cursor->math_view, NULL);
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(decor_default_cursor_get)(GtkMathViewDefaultCursorDecorator* cursor,
						  gboolean* enabled,
						  GtkMathViewModelId* elem, gboolean* draw_focus,
						  gint* index, gboolean* char_index)
{
  g_return_if_fail(cursor != NULL);
  if (enabled) *enabled = cursor->enabled;
  if (elem) *elem = cursor->element;
  if (draw_focus) *draw_focus = cursor->draw_focus;
  if (index) *index = cursor->index;
  if (char_index) *char_index = cursor->char_index;
}
