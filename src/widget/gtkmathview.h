/*
 * Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
 * 
 * This file is part of GtkMathView, a Gtk widget for MathML.
 * 
 * GtkMathView is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * GtkMathView is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GtkMathView; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * For details, see the GtkMathView World-Wide-Web page,
 * http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
 * <luca.padovani@cs.unibo.it>
 */

#ifndef gtkmathview_h
#define gtkmathview_h

#include "defs.h"

#include <gtk/gtkframe.h>
#include <gtk/gtkeventbox.h>
#include <gtk/gtkdrawingarea.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_TYPE_MATH_VIEW         (gtk_math_view_get_type())
#define GTK_MATH_VIEW(obj)         GTK_CHECK_CAST(obj, GTK_TYPE_MATH_VIEW, GtkMathView)
#define GTK_MATH_VIEW_CLASS(klass) GTK_CHECK_CLASS_CAST(klass, GTK_TYPE_MATH_VIEW, GtkMathViewClass)
#define GTK_IS_MATH_VIEW(obj)      GTK_CHECK_TYPE(obj, GTK_TYPE_MATH_VIEW)

#define USE_GMETADOM 1
#define USE_LIBXML2  (1 - USE_GMETADOM)

#if USE_GMETADOM
#include <gdome.h>
typedef GdomeElement* GtkMathView_Model_Element;
#elif USE_LIBXML2
#include <libxml/tree.h>
typedef xmlElement* GtkMathView_Model_Element;
#endif // USE_LIBXML2

  typedef struct _GtkMathView      GtkMathView;
  typedef struct _GtkMathViewClass GtkMathViewClass;

  GtkType    	 gtk_math_view_get_type(void);
  GtkWidget* 	 gtk_math_view_new(GtkAdjustment*, GtkAdjustment*);
  gboolean       gtk_math_view_freeze(GtkMathView*);
  gboolean       gtk_math_view_thaw(GtkMathView*);
  gboolean       gtk_math_view_load_uri(GtkMathView*, const gchar*);
#if USE_GMETADOM
  gboolean       gtk_math_view_load_doc(GtkMathView*, GdomeDocument*);
#elif USE_LIBXML2
  gboolean       gtk_math_view_load_doc(GtkMathView*, xmlDoc*);
#endif // USE_LIBXML2
  gboolean       gtk_math_view_load_root(GtkMathView*, GtkMathView_Model_Element);
  void           gtk_math_view_unload(GtkMathView*);
  void           gtk_math_view_select(GtkMathView*, GtkMathView_Model_Element);
  void           gtk_math_view_unselect(GtkMathView*, GtkMathView_Model_Element);
  gboolean       gtk_math_view_is_selected(GtkMathView*, GtkMathView_Model_Element);
  gboolean       gtk_math_view_get_element_at(GtkMathView*, gint, gint, GtkMathView_Model_Element*);
  gboolean       gtk_math_view_get_element_location(GtkMathView*, GtkMathView_Model_Element, gint*, gint*, GdkRectangle*);
  gboolean       gtk_math_view_get_char_at(GtkMathView*, gint, gint, GtkMathView_Model_Element*, gint*);
  gboolean       gtk_math_view_get_char_location(GtkMathView*, GtkMathView_Model_Element, gint, gint*, gint*, GdkRectangle*);
  gint      	 gtk_math_view_get_width(GtkMathView*);
  gint      	 gtk_math_view_get_height(GtkMathView*);
  void       	 gtk_math_view_get_top(GtkMathView*, gint*, gint*);
  void       	 gtk_math_view_set_top(GtkMathView*, gint, gint);
  void       	 gtk_math_view_set_adjustments(GtkMathView*, GtkAdjustment*, GtkAdjustment*);
  GtkAdjustment* gtk_math_view_get_hadjustment(GtkMathView*);
  GtkAdjustment* gtk_math_view_get_vadjustment(GtkMathView*);
  GdkPixmap*     gtk_math_view_get_buffer(GtkMathView*);
  GtkDrawingArea* gtk_math_view_get_drawing_area(GtkMathView*);
  void           gtk_math_view_set_font_size(GtkMathView*, guint);
  guint          gtk_math_view_get_font_size(GtkMathView*);
  void           gtk_math_view_set_log_verbosity(GtkMathView*, gint);
  gint           gtk_math_view_get_log_verbosity(GtkMathView*);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* gtkmathview_h */

