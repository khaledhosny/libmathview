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

  typedef struct _GtkMathView       GtkMathView;
  typedef struct _GtkMathViewClass  GtkMathViewClass;
  typedef struct _c_customXmlReader GtkMathViewReader;
  typedef void*                     GtkMathViewReaderData;

#if GTKMATHVIEW_USES_CUSTOM_READER
#include "c_customXmlReader.h"
#define GTKMATHVIEW_METHOD_NAME(name) gtk_math_view_##name##__##custom_reader
  typedef void*                     GtkMathViewElementId;
  typedef const char*               GtkMathViewModelString;
#elif GTKMATHVIEW_USES_LIBXML2
#include <libxml/tree.h>
#define GTKMATHVIEW_METHOD_NAME(name) gtk_math_view_##name##__##libxml2
  typedef xmlElement*               GtkMathViewElementId;
  typedef const xmlChar*            GtkMathViewModelString;
#elif GTKMATHVIEW_USES_GMETADOM
#include <gdome.h>
#define GTKMATHVIEW_METHOD_NAME(name) gtk_math_view_##name##__##gmetadom
  typedef GdomeElement*             GtkMathViewElementId;
  typedef GdomeDOMString*           GtkMathViewModelString;
#endif

  GtkType    	 GTKMATHVIEW_METHOD_NAME(get_type)(void);
  GtkWidget* 	 GTKMATHVIEW_METHOD_NAME(new)(GtkAdjustment*, GtkAdjustment*);
  gboolean       GTKMATHVIEW_METHOD_NAME(freeze)(GtkMathView*);
  gboolean       GTKMATHVIEW_METHOD_NAME(thaw)(GtkMathView*);
#if GTKMATHVIEW_USES_CUSTOM_READER
  gboolean       GTKMATHVIEW_METHOD_NAME(load_reader)(GtkMathView*, GtkMathViewReader*, GtkMathViewReaderData);
#else
  gboolean       GTKMATHVIEW_METHOD_NAME(load_uri)(GtkMathView*, const gchar*);
  gboolean       GTKMATHVIEW_METHOD_NAME(load_root)(GtkMathView*, GtkMathViewElementId);
#endif
  void           GTKMATHVIEW_METHOD_NAME(unload)(GtkMathView*);
  void           GTKMATHVIEW_METHOD_NAME(structure_changed)(GtkMathView*, GtkMathViewElementId);
  void           GTKMATHVIEW_METHOD_NAME(attribute_changed)(GtkMathView*, GtkMathViewElementId, GtkMathViewModelString);
  void           GTKMATHVIEW_METHOD_NAME(select)(GtkMathView*, GtkMathViewElementId);
  void           GTKMATHVIEW_METHOD_NAME(unselect)(GtkMathView*, GtkMathViewElementId);
  gboolean       GTKMATHVIEW_METHOD_NAME(is_selected)(GtkMathView*, GtkMathViewElementId);
  gboolean       GTKMATHVIEW_METHOD_NAME(get_bounding_box)(GtkMathView*, gint*, gint*, gint*);
  gboolean       GTKMATHVIEW_METHOD_NAME(get_element_at)(GtkMathView*, gint, gint, GtkMathViewElementId*);
  gboolean       GTKMATHVIEW_METHOD_NAME(get_element_location)(GtkMathView*, GtkMathViewElementId, gint*, gint*, GdkRectangle*);
  gboolean       GTKMATHVIEW_METHOD_NAME(get_char_at)(GtkMathView*, gint, gint, GtkMathViewElementId*, gint*);
  gboolean       GTKMATHVIEW_METHOD_NAME(get_char_location)(GtkMathView*, GtkMathViewElementId, gint, gint*, gint*, GdkRectangle*);
  void           GTKMATHVIEW_METHOD_NAME(set_cursor)(GtkMathView*, GtkMathViewElementId, gint);
  void           GTKMATHVIEW_METHOD_NAME(get_cursor)(GtkMathView*, GtkMathViewElementId*, gint*);
  void           GTKMATHVIEW_METHOD_NAME(set_cursor_visible)(GtkMathView*, gboolean);
  gboolean       GTKMATHVIEW_METHOD_NAME(get_cursor_visible)(GtkMathView*);
  gint      	 GTKMATHVIEW_METHOD_NAME(get_width)(GtkMathView*);
  gint      	 GTKMATHVIEW_METHOD_NAME(get_height)(GtkMathView*);
  void       	 GTKMATHVIEW_METHOD_NAME(get_top)(GtkMathView*, gint*, gint*);
  void       	 GTKMATHVIEW_METHOD_NAME(set_top)(GtkMathView*, gint, gint);
  void       	 GTKMATHVIEW_METHOD_NAME(set_adjustments)(GtkMathView*, GtkAdjustment*, GtkAdjustment*);
  GtkAdjustment* GTKMATHVIEW_METHOD_NAME(get_hadjustment)(GtkMathView*);
  GtkAdjustment* GTKMATHVIEW_METHOD_NAME(get_vadjustment)(GtkMathView*);
  GdkPixmap*     GTKMATHVIEW_METHOD_NAME(get_buffer)(GtkMathView*);
  GtkDrawingArea* GTKMATHVIEW_METHOD_NAME(get_drawing_area)(GtkMathView*);
  void           GTKMATHVIEW_METHOD_NAME(set_font_size)(GtkMathView*, guint);
  guint          GTKMATHVIEW_METHOD_NAME(get_font_size)(GtkMathView*);
  void           GTKMATHVIEW_METHOD_NAME(set_log_verbosity)(GtkMathView*, gint);
  gint           GTKMATHVIEW_METHOD_NAME(get_log_verbosity)(GtkMathView*);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* gtkmathview_h */

