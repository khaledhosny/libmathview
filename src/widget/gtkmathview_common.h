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

#ifndef __gtkmathview_common_h__
#define __gtkmathview_common_h__

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

  typedef GdkPoint GtkMathViewPoint;

  typedef struct _GtkMathViewBoundingBox {
    gint width;
    gint height;
    gint depth;
  } GtkMathViewBoundingBox;

  typedef struct _GtkMathView       GtkMathView;
  typedef struct _GtkMathViewClass  GtkMathViewClass;
  typedef struct _c_customXmlReader GtkMathViewReader;
  typedef void*                     GtkMathViewReaderData;

#if GTKMATHVIEW_USES_CUSTOM_READER
#include "c_customXmlReader.h"
#define GTKMATHVIEW_METHOD_NAME(name) gtk_math_view_##name##__##custom_reader
  typedef void*                     GtkMathViewModelId;
  typedef const char*               GtkMathViewModelString;
#elif GTKMATHVIEW_USES_LIBXML2_READER
#include <libxml/xmlreader.h>
#define GTKMATHVIEW_METHOD_NAME(name) gtk_math_view_##name##__##libxml2_reader
  typedef void*                     GtkMathViewModelId;
  typedef const xmlChar*            GtkMathViewModelString;
#elif GTKMATHVIEW_USES_LIBXML2
#include <libxml/tree.h>
#define GTKMATHVIEW_METHOD_NAME(name) gtk_math_view_##name##__##libxml2
  typedef xmlElement*               GtkMathViewModelId;
  typedef xmlDoc*                   GtkMathViewModelDocument;
  typedef const xmlChar*            GtkMathViewModelString;
#elif GTKMATHVIEW_USES_GMETADOM
#include <gdome.h>
#define GTKMATHVIEW_METHOD_NAME(name) gtk_math_view_##name##__##gmetadom
  typedef GdomeElement*             GtkMathViewModelId;
  typedef GdomeDocument*            GtkMathViewModelDocument;
  typedef GdomeDOMString*           GtkMathViewModelString;
#endif

  typedef enum _GtkMathViewCursor {
    GTKMATHVIEW_CURSOR_OFF,
    GTKMATHVIEW_CURSOR_CARET_ON,
    GTKMATHVIEW_CURSOR_FOCUS_ON,
    GTKMATHVIEW_CURSOR_ON
  } GtkMathViewCursor;

  typedef struct _GtkMathViewModelEvent {
    GtkMathViewModelId id;
    gint x;
    gint y;
    gint state;
  } GtkMathViewModelEvent;

  typedef void (*GtkMathViewModelSignal)(GtkMathView*, const GtkMathViewModelEvent*);
  typedef void (*GtkMathViewSelectAbortSignal)(GtkMathView*);

  GtkType    GTKMATHVIEW_METHOD_NAME(get_type)(void);
  GtkWidget* GTKMATHVIEW_METHOD_NAME(new)(GtkAdjustment*, GtkAdjustment*);
  gboolean   GTKMATHVIEW_METHOD_NAME(freeze)(GtkMathView*);
  gboolean   GTKMATHVIEW_METHOD_NAME(thaw)(GtkMathView*);
#if GTKMATHVIEW_USES_CUSTOM_READER
  gboolean   GTKMATHVIEW_METHOD_NAME(load_reader)(GtkMathView*, GtkMathViewReader*, GtkMathViewReaderData);
#elif GTKMATHVIEW_USES_LIBXML2_READER
  gboolean   GTKMATHVIEW_METHOD_NAME(load_reader)(GtkMathView*, xmlTextReaderPtr);
#else
  gboolean   GTKMATHVIEW_METHOD_NAME(load_uri)(GtkMathView*, const gchar*);
  gboolean   GTKMATHVIEW_METHOD_NAME(load_buffer)(GtkMathView*, const gchar*);
  gboolean   GTKMATHVIEW_METHOD_NAME(load_document)(GtkMathView*, GtkMathViewModelDocument);
  gboolean   GTKMATHVIEW_METHOD_NAME(load_root)(GtkMathView*, GtkMathViewModelId);
  GtkMathViewModelDocument GTKMATHVIEW_METHOD_NAME(get_document)(GtkMathView*);
#endif
  void       GTKMATHVIEW_METHOD_NAME(unload)(GtkMathView*);
  gboolean   GTKMATHVIEW_METHOD_NAME(structure_changed)(GtkMathView*, GtkMathViewModelId);
  gboolean   GTKMATHVIEW_METHOD_NAME(attribute_changed)(GtkMathView*, GtkMathViewModelId, GtkMathViewModelString);
  gboolean   GTKMATHVIEW_METHOD_NAME(select)(GtkMathView*, GtkMathViewModelId);
  gboolean   GTKMATHVIEW_METHOD_NAME(unselect)(GtkMathView*, GtkMathViewModelId);
  gboolean   GTKMATHVIEW_METHOD_NAME(is_selected)(GtkMathView*, GtkMathViewModelId);
  gboolean   GTKMATHVIEW_METHOD_NAME(get_bounding_box)(GtkMathView*, GtkMathViewBoundingBox*);
  gboolean   GTKMATHVIEW_METHOD_NAME(get_element_at)(GtkMathView*, gint, gint, GtkMathViewModelId*,
						     GtkMathViewPoint*, GtkMathViewBoundingBox*);
  gboolean   GTKMATHVIEW_METHOD_NAME(get_element_extents)(GtkMathView*, GtkMathViewModelId,
							  GtkMathViewPoint*, GtkMathViewBoundingBox*);
  gboolean   GTKMATHVIEW_METHOD_NAME(get_char_at)(GtkMathView*, gint, gint, GtkMathViewModelId*, gint*,
						  GtkMathViewPoint*, GtkMathViewBoundingBox*);
  gboolean   GTKMATHVIEW_METHOD_NAME(get_char_extents)(GtkMathView*, GtkMathViewModelId, gint,
						       GtkMathViewPoint*, GtkMathViewBoundingBox*);
  void       GTKMATHVIEW_METHOD_NAME(set_cursor)(GtkMathView*, GtkMathViewModelId, gint);
  void       GTKMATHVIEW_METHOD_NAME(get_cursor)(GtkMathView*, GtkMathViewModelId*, gint*);
  void       GTKMATHVIEW_METHOD_NAME(set_cursor_visible)(GtkMathView*, GtkMathViewCursor);
  GtkMathViewCursor GTKMATHVIEW_METHOD_NAME(get_cursor_visible)(GtkMathView*);
  void       GTKMATHVIEW_METHOD_NAME(get_size)(GtkMathView*, gint*, gint*);
  void       GTKMATHVIEW_METHOD_NAME(get_top)(GtkMathView*, gint*, gint*);
  void       GTKMATHVIEW_METHOD_NAME(set_top)(GtkMathView*, gint, gint);
  void       GTKMATHVIEW_METHOD_NAME(set_adjustments)(GtkMathView*, GtkAdjustment*, GtkAdjustment*);
  void       GTKMATHVIEW_METHOD_NAME(get_adjustments)(GtkMathView*, GtkAdjustment**, GtkAdjustment**);
  GdkPixmap* GTKMATHVIEW_METHOD_NAME(get_buffer)(GtkMathView*);
  void       GTKMATHVIEW_METHOD_NAME(set_font_size)(GtkMathView*, guint);
  guint      GTKMATHVIEW_METHOD_NAME(get_font_size)(GtkMathView*);
  void       GTKMATHVIEW_METHOD_NAME(set_log_verbosity)(GtkMathView*, gint);
  gint       GTKMATHVIEW_METHOD_NAME(get_log_verbosity)(GtkMathView*);
  gboolean   GTKMATHVIEW_METHOD_NAME(get_t1_opaque_mode)(GtkMathView*);
  void       GTKMATHVIEW_METHOD_NAME(set_t1_opaque_mode)(GtkMathView*, gboolean);
  gboolean   GTKMATHVIEW_METHOD_NAME(get_t1_anti_aliased_mode)(GtkMathView*);
  void       GTKMATHVIEW_METHOD_NAME(set_t1_anti_aliased_mode)(GtkMathView*, gboolean);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __gtkmathview_common_h__ */

