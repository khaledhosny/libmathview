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

#include <stdio.h>
#include <gdome.h>

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

  enum _FontManagerId {
    FONT_MANAGER_UNKNOWN = -1,
    FONT_MANAGER_GTK,
    FONT_MANAGER_T1
  };

  typedef enum   _FontManagerId    FontManagerId;
  typedef struct _GtkMathView      GtkMathView;
  typedef struct _GtkMathViewClass GtkMathViewClass;

  GtkType    	 gtk_math_view_get_type(void);
  GtkWidget* 	 gtk_math_view_new(GtkAdjustment*, GtkAdjustment*);
  gboolean     	 gtk_math_view_load(GtkMathView*, const gchar*);
  gboolean       gtk_math_view_load_tree(GtkMathView*, GdomeDocument*);
  void           gtk_math_view_unload(GtkMathView*);
  GdomeElement*  gtk_math_view_get_selection(GtkMathView*);
  void           gtk_math_view_set_selection(GtkMathView*, GdomeElement*);
  gint      	 gtk_math_view_get_width(GtkMathView*);
  gint      	 gtk_math_view_get_height(GtkMathView*);
  void       	 gtk_math_view_get_top(GtkMathView*, gint*, gint*);
  void       	 gtk_math_view_set_top(GtkMathView*, gint, gint);
  void       	 gtk_math_view_set_adjustments(GtkMathView*, GtkAdjustment*, GtkAdjustment*);
  GtkAdjustment* gtk_math_view_get_hadjustment(GtkMathView*);
  GtkAdjustment* gtk_math_view_get_vadjustment(GtkMathView*);
  GdkPixmap*     gtk_math_view_get_buffer(GtkMathView*);
  GtkFrame*      gtk_math_view_get_frame(GtkMathView*);
  void           gtk_math_view_set_font_size(GtkMathView*, guint);
  guint          gtk_math_view_get_font_size(GtkMathView*);
  void           gtk_math_view_set_anti_aliasing(GtkMathView*, gboolean);
  gboolean       gtk_math_view_get_anti_aliasing(GtkMathView*);
  void           gtk_math_view_set_kerning(GtkMathView*, gboolean);
  gboolean       gtk_math_view_get_kerning(GtkMathView*);
  void           gtk_math_view_set_transparency(GtkMathView*, gboolean);
  gboolean       gtk_math_view_get_transparency(GtkMathView*);
  void           gtk_math_view_set_log_verbosity(GtkMathView*, gint);
  gint           gtk_math_view_get_log_verbosity(GtkMathView*);
  void           gtk_math_view_export_to_postscript(GtkMathView*, gint, gint, gint, gint, gboolean, FILE*);
  void           gtk_math_view_set_font_manager_type(GtkMathView*, FontManagerId);
  FontManagerId  gtk_math_view_get_font_manager_type(GtkMathView*);

  /* the following are *unstable*, *experimental* APIs */
  GdomeElement*  gtk_math_view_get_element(GtkMathView*);
  GdomeElement*  gtk_math_view_get_action(GtkMathView*);
  guint          gtk_math_view_action_get_selected(GtkMathView*);
  void           gtk_math_view_action_set_selected(GtkMathView*, guint);
  void           gtk_math_view_action_toggle(GtkMathView*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* gtkmathview_h */
