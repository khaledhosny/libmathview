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
#include <stdlib.h>

#include <sstream>

#include "defs.h"

#include <pango/pango.h>

#include <math.h>
#include <gtk/gtk.h>

#include "gtkmathview_marshal.h"

#include "gtkmathview_common.h"
#if GTKMATHVIEW_USES_CUSTOM_READER
#define GTK_MATH_VIEW_WIDGET_NAME "GtkMathView_Custom_Reader"
#include "custom_reader_MathView.hh"
typedef custom_reader_MathView MathView;
#elif GTKMATHVIEW_USES_LIBXML2_READER
#define GTK_MATH_VIEW_WIDGET_NAME "GtkMathView_libxml2_Reader"
#include "libxml2_reader_MathView.hh"
typedef libxml2_reader_MathView MathView;
#elif GTKMATHVIEW_USES_LIBXML2
#define GTK_MATH_VIEW_WIDGET_NAME "GtkMathView_libxml2"
#include "libxml2_MathView.hh"
typedef libxml2_MathView MathView;
#endif

#include "Logger.hh"
#include "Point.hh"
#include "Rectangle.hh"
#include "MathMLElement.hh"
#include "MathMLOperatorDictionary.hh"
#include "MathMLNamespaceContext.hh"
#include "MathGraphicDevice.hh"
#include "Cairo_Backend.hh"
#include "Cairo_RenderingContext.hh"
#include "WrapperArea.hh"
#include "GObjectPtr.hh"

#define CLICK_SPACE_RANGE 1
#define CLICK_TIME_RANGE  250

enum SelectState 
  {
    SELECT_STATE_NO,
    SELECT_STATE_YES,
    SELECT_STATE_ABORT
  };

enum GtkMathViewProperties
  {
    PROP_DEFAULT_FONT_SIZE,
    PROP_LOG_VERBOSITY
  };

/* structures */

struct _GtkMathViewClass
{
  GtkWidgetClass parent_class;

  GtkMathViewModelSignal click;
  GtkMathViewModelSignal select_begin;
  GtkMathViewModelSignal select_over;
  GtkMathViewModelSignal select_end;
  GtkMathViewSelectAbortSignal select_abort;
  GtkMathViewModelSignal element_over;
  GtkMathViewDecorateSignal decorate_under;
  GtkMathViewDecorateSignal decorate_over;

  AbstractLogger* logger;
  MathMLOperatorDictionary* dictionary;
};

struct _GtkMathView
{
  GtkWidget      parent;

  GtkWidget* 	 area;
  cairo_surface_t* surface;

  guint 	 hsignal; // what is this for?
  guint 	 vsignal; // what is this for?

  GtkAdjustment* hadjustment;
  GtkAdjustment* vadjustment;

  gint     	 top_x;
  gint     	 top_y;

  gint 	         old_top_x;
  gint 	         old_top_y;

  guint          freeze_counter;

  SelectState    select_state;
  gboolean       button_pressed;
  gfloat         button_press_x;
  gfloat         button_press_y;
  guint32        button_press_time;

  GtkMathViewModelId current_elem;

  MathView*      view;
  Cairo_RenderingContext* renderingContext;
  Cairo_Backend* backend;
};

/* helper functions */

static void gtk_math_view_base_class_init(GtkMathViewClass*);
static void gtk_math_view_base_class_finalize(GtkMathViewClass*);
static void gtk_math_view_class_init(GtkMathViewClass*);
static void gtk_math_view_init(GtkMathView*);

/* GObject functions */

#if 0
static void gtk_math_view_set_property(GObject*, guint, const GValue*, GParamSpec*);
static void gtk_math_view_get_property(GObject*, guint, GValue*, GParamSpec*);
#endif

/* GtkWidget functions */

static gboolean gtk_math_view_draw(GtkWidget*, cairo_t*);
static gboolean gtk_math_view_button_press_event(GtkWidget*, GdkEventButton*);
static gboolean gtk_math_view_button_release_event(GtkWidget*, GdkEventButton*);
static gboolean gtk_math_view_motion_notify_event(GtkWidget*, GdkEventMotion*);
static gboolean gtk_math_view_focus_in_event(GtkWidget*, GdkEventFocus*);
static gboolean gtk_math_view_focus_out_event(GtkWidget*, GdkEventFocus*);
static void     gtk_math_view_realize(GtkWidget*);
static void     gtk_math_view_destroy(GtkWidget*);
static void     gtk_math_view_get_preferred_height(GtkWidget*, gint*, gint*);
static void     gtk_math_view_get_preferred_width(GtkWidget*, gint*, gint*);
static void     gtk_math_view_size_allocate(GtkWidget*, GtkAllocation*);

/* GtkMathView Signals */

static void gtk_math_view_click(GtkMathView*, const GtkMathViewModelEvent*);
static void gtk_math_view_select_begin(GtkMathView*, const GtkMathViewModelEvent*);
static void gtk_math_view_select_over(GtkMathView*, const GtkMathViewModelEvent*);
static void gtk_math_view_select_end(GtkMathView*, const GtkMathViewModelEvent*);
static void gtk_math_view_select_abort(GtkMathView*);
static void gtk_math_view_element_over(GtkMathView*, const GtkMathViewModelEvent*);

/* auxiliary functions */

static void setup_adjustment(GtkAdjustment*, gfloat, gfloat);
static void setup_adjustments(GtkMathView*);

/* Local data */

static GtkWidgetClass* parent_class = NULL;
static guint click_signal = 0;
static guint select_begin_signal = 0;
static guint select_over_signal = 0;
static guint select_end_signal = 0;
static guint select_abort_signal = 0;
static guint element_over_signal = 0;
static guint decorate_under_signal = 0;
static guint decorate_over_signal = 0;

/* auxiliary C++ functions */

static RGBColor
RGBColorOfGdkRGBA(const GdkRGBA c)
{
  return RGBColor(c.red * 0xff, c.green * 0xff, c.blue * 0xff, c.alpha * 0xff);
}

static SmartPtr<const WrapperArea>
findGtkWrapperArea(GtkMathView* math_view, GtkMathViewModelId node)
{
  if (SmartPtr<Element> elem = math_view->view->elementOfModelElement(node))
    if (SmartPtr<const WrapperArea> area = smart_cast<const WrapperArea>(elem->getArea()))
      return area;
  return 0;
}

/* auxiliary C functions */

static void
from_view_coords(GtkMathView* math_view, GtkMathViewPoint* point)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(point != NULL);
  point->x -= math_view->top_x;
  point->y -= math_view->top_y - math_view->view->getBoundingBox().height.toDouble();
}

static void
to_view_coords(GtkMathView* math_view, gint* x, gint* y)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(x != NULL);
  g_return_if_fail(y != NULL);
  *x += math_view->top_x;
  *y += math_view->top_y - math_view->view->getBoundingBox().height.toDouble();
}

/* GObject implementation */

#if 0
static extern "C" void
gtk_math_view_set_property (GObject* object,
			    guint prop_id,
			    GValue* value,
			    GParamSpec* pspec)
{
  GtkMathView *math_view = GTK_MATH_VIEW(object);
  
  switch (prop_id)
    {
    case PROP_DEFAULT_FONT_SIZE:
    case PROP_LOG_VERBOSITY:
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static extern "C" void
gtk_math_view_get_property (GObject* object,
			    guint prop_id,
			    const GValue* value,
			    GParamSpec* pspec)
{ }
#endif

/* widget implementation */

static void
gtk_math_view_update(GtkMathView* math_view, gint x0, gint y0, gint width, gint height)
{
  GtkWidget* widget = GTK_WIDGET(math_view);
  GdkWindow* window = gtk_widget_get_window(widget);

  if (!gtk_widget_get_mapped(GTK_WIDGET(math_view)) || math_view->freeze_counter > 0) return;

  cairo_t *cr = gdk_cairo_create(window);

  if (math_view->surface != NULL)
    cairo_set_source_surface(cr, math_view->surface, 0, 0);
  else
    cairo_set_source_rgb(cr, 1, 1, 1);

  cairo_rectangle(cr, x0, y0, width, height);
  cairo_fill(cr);

  g_signal_emit(math_view, decorate_over_signal, 0, cr);

  cairo_destroy(cr);
}

static void
gtk_math_view_paint(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);

  GtkMathViewClass* math_view_class = GTK_MATH_VIEW_CLASS(G_OBJECT_GET_CLASS(G_OBJECT(math_view)));
  g_return_if_fail(math_view_class != NULL);

  if (!gtk_widget_get_mapped(GTK_WIDGET(math_view)) || math_view->freeze_counter > 0) return;

  GtkWidget* widget = GTK_WIDGET(math_view);
  GdkWindow* window = gtk_widget_get_window(widget);
  GtkStyleContext* context = gtk_widget_get_style_context(widget);
  gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);
  
  setup_adjustments(math_view);

  const gint width = allocation.width;
  const gint height = allocation.height;

  if (math_view->surface == NULL)
    {
      math_view->surface = gdk_window_create_similar_surface(window, CAIRO_CONTENT_COLOR, width, height);
      math_view->renderingContext = new Cairo_RenderingContext(cairo_create(math_view->surface));
    }

  Cairo_RenderingContext* rc = math_view->renderingContext;
  g_return_if_fail(rc != 0);

  GdkRGBA fore, back;
  gtk_style_context_get_color(context, GTK_STATE_FLAG_SELECTED, &fore);
  gtk_style_context_get_background_color(context, GTK_STATE_FLAG_SELECTED, &back);
  rc->setStyle(RenderingContext::SELECTED_STYLE);
  rc->setForegroundColor(RGBColorOfGdkRGBA(fore));
  rc->setBackgroundColor(RGBColorOfGdkRGBA(back));

  gtk_style_context_get_color(context, GTK_STATE_FLAG_NORMAL, &fore);
  gtk_style_context_get_background_color(context, GTK_STATE_FLAG_NORMAL, &back);
  rc->setStyle(RenderingContext::NORMAL_STYLE);
  rc->setForegroundColor(RGBColorOfGdkRGBA(fore));
  rc->setBackgroundColor(RGBColorOfGdkRGBA(back));

  cairo_t *cr = cairo_create(math_view->surface);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // WARNING: setAvailableWidth must be invoked BEFORE any coordinate conversion
  math_view->view->setAvailableWidth(scaled(width));
  gint x = 0;
  gint y = 0;
  to_view_coords(math_view, &x, &y);
  g_signal_emit(math_view, decorate_under_signal, 0, cr);
  math_view->view->render(*rc, scaled(-x), scaled(y));

  gtk_math_view_update(math_view, 0, 0, width, height);

  cairo_destroy(cr);
}

static void
hadjustment_value_changed(GtkAdjustment* adj, GtkMathView* math_view)
{
  g_return_if_fail(adj != NULL);
  g_return_if_fail(math_view != NULL);

  gdouble value = gtk_adjustment_get_value(adj);
  gdouble upper = gtk_adjustment_get_upper(adj);
  gdouble lower = gtk_adjustment_get_lower(adj);
  gdouble page_size = gtk_adjustment_get_page_size(adj);
  if (value > upper - page_size)
    gtk_adjustment_set_value(adj, upper - page_size);
  if (value < lower)
    gtk_adjustment_set_value(adj, lower);

  math_view->old_top_x = math_view->top_x;
  math_view->top_x = static_cast<int>(value);

  if (math_view->old_top_x != math_view->top_x)
    gtk_math_view_paint(math_view);
}

static void
vadjustment_value_changed(GtkAdjustment* adj, GtkMathView* math_view)
{
  g_return_if_fail(adj != NULL);
  g_return_if_fail(math_view != NULL);

  gdouble value = gtk_adjustment_get_value(adj);
  gdouble upper = gtk_adjustment_get_upper(adj);
  gdouble lower = gtk_adjustment_get_lower(adj);
  gdouble page_size = gtk_adjustment_get_page_size(adj);
  if (value > upper - page_size)
    gtk_adjustment_set_value(adj, upper - page_size);
  if (value < lower)
    gtk_adjustment_set_value(adj, lower);

  math_view->old_top_y = math_view->top_y;
  math_view->top_y = static_cast<int>(value);

  if (math_view->old_top_y != math_view->top_y)
    gtk_math_view_paint(math_view);
}

extern "C" GType
GTKMATHVIEW_METHOD_NAME(get_type)(void)
{
  static GType math_view_type = 0;

  if (!math_view_type)
    {
      static const GTypeInfo math_view_info =
	{
	  sizeof(GtkMathViewClass),
	  (GBaseInitFunc) gtk_math_view_base_class_init,
	  (GBaseFinalizeFunc) gtk_math_view_base_class_finalize,
	  (GClassInitFunc) gtk_math_view_class_init,
	  NULL,
	  NULL,
	  sizeof(GtkMathView),
	  0,
	  (GInstanceInitFunc) gtk_math_view_init,
	  NULL
	};
      math_view_type = g_type_register_static(GTK_TYPE_WIDGET, GTK_MATH_VIEW_WIDGET_NAME,
					      &math_view_info, (GTypeFlags) 0);
    }

  return math_view_type;
}

static void
gtk_math_view_base_class_init(GtkMathViewClass* math_view_class)
{
  g_return_if_fail(math_view_class != NULL);

  SmartPtr<AbstractLogger> logger = Logger::create();
  logger->ref();
  math_view_class->logger = logger;

  SmartPtr<MathMLOperatorDictionary> dictionary = MathMLOperatorDictionary::create();
  dictionary->ref();
  math_view_class->dictionary = dictionary;
}

static void
gtk_math_view_base_class_finalize(GtkMathViewClass* math_view_class)
{
  g_return_if_fail(math_view_class != NULL);

  if (math_view_class->dictionary)
    {
      math_view_class->dictionary->unref();
      math_view_class->dictionary = 0;
    }

  if (math_view_class->logger)
    {
      math_view_class->logger->unref();
      math_view_class->logger = 0;
    }
}

static void
gtk_math_view_class_init(GtkMathViewClass* math_view_class)
{
  g_return_if_fail(math_view_class != NULL);

  GObjectClass* object_class = G_OBJECT_CLASS(math_view_class);
  GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(math_view_class);
	
  math_view_class->click = gtk_math_view_click;
  math_view_class->select_begin = gtk_math_view_select_begin;
  math_view_class->select_over = gtk_math_view_select_over;
  math_view_class->select_end = gtk_math_view_select_end;
  math_view_class->select_abort = gtk_math_view_select_abort;
  math_view_class->element_over = gtk_math_view_element_over;

  parent_class = GTK_WIDGET_CLASS(g_type_class_ref(gtk_widget_get_type()));

  widget_class->realize = gtk_math_view_realize;
  widget_class->destroy = gtk_math_view_destroy;
  widget_class->draw = gtk_math_view_draw;
  widget_class->get_preferred_width = gtk_math_view_get_preferred_width;
  widget_class->get_preferred_height = gtk_math_view_get_preferred_height;
  widget_class->size_allocate = gtk_math_view_size_allocate;
  widget_class->button_press_event = gtk_math_view_button_press_event;
  widget_class->button_release_event = gtk_math_view_button_release_event;
  widget_class->motion_notify_event = gtk_math_view_motion_notify_event;
  widget_class->focus_in_event = gtk_math_view_focus_in_event;
  widget_class->focus_out_event = gtk_math_view_focus_out_event;

  click_signal = 
    g_signal_new("click",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, click),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__POINTER,
		 G_TYPE_NONE, 1, G_TYPE_POINTER);

  select_begin_signal=
    g_signal_new("select_begin",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, select_begin),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__POINTER,
		 G_TYPE_NONE, 1, G_TYPE_POINTER);
	
  select_over_signal = 
    g_signal_new("select_over",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, select_over),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__POINTER,
		 G_TYPE_NONE, 1, G_TYPE_POINTER);
	
  select_end_signal = 
    g_signal_new("select_end",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, select_end),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__POINTER,
		 G_TYPE_NONE, 1, G_TYPE_POINTER);
	
  select_abort_signal = 
    g_signal_new("select_abort",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, select_abort),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
	
  element_over_signal = 
    g_signal_new("element_over",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, element_over),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__POINTER,
		 G_TYPE_NONE, 1, G_TYPE_POINTER);

  decorate_under_signal =
    g_signal_new("decorate_under",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, decorate_under),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__POINTER,
		 G_TYPE_NONE, 1, G_TYPE_POINTER);

  decorate_over_signal =
    g_signal_new("decorate_over",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, decorate_over),
		 NULL, NULL, 
		 g_cclosure_marshal_VOID__POINTER,
		 G_TYPE_NONE, 1, G_TYPE_POINTER);
}

static void
gtk_math_view_release_document_resources(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  math_view->current_elem = NULL;
}

static void
gtk_math_view_init(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);

  math_view->surface         = NULL;
  math_view->view            = 0;
  math_view->renderingContext = 0;
  math_view->backend         = 0;
  math_view->freeze_counter  = 0;
  math_view->select_state    = SELECT_STATE_NO;
  math_view->button_pressed  = FALSE;
  math_view->current_elem    = NULL;
  math_view->hadjustment = NULL;
  math_view->vadjustment = NULL;
  math_view->top_x = math_view->top_y = 0;
  math_view->old_top_x = math_view->old_top_y = 0;

  GtkMathViewClass* math_view_class = GTK_MATH_VIEW_CLASS(g_type_class_ref(gtk_math_view_get_type()));
  g_assert(math_view_class != NULL);

  SmartPtr<MathView> view = MathView::create(math_view_class->logger);
  view->ref();
  math_view->view = view;

  GObjectPtr<PangoContext> pango_context = gtk_widget_create_pango_context(GTK_WIDGET(math_view));
  PangoFontDescription* description = pango_font_description_new();
  pango_font_description_set_family(description, DEFAULT_FONT_FAMILY);
  pango_font_description_set_size(description, DEFAULT_FONT_SIZE * PANGO_SCALE);

  PangoFont* font = pango_context_load_font(pango_context, description);
  cairo_scaled_font_t* cairo_font = pango_cairo_font_get_scaled_font (PANGO_CAIRO_FONT(font));

  SmartPtr<Cairo_Backend> backend = Cairo_Backend::create(cairo_font);
  backend->ref();
  math_view->backend = backend;

  view->setDefaultFontSize(DEFAULT_FONT_SIZE);
  view->setOperatorDictionary(math_view_class->dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, backend->getMathGraphicDevice()));
}

extern "C" GtkWidget*
GTKMATHVIEW_METHOD_NAME(new)(GtkAdjustment* hadj, GtkAdjustment* vadj)
{
  GtkMathView* math_view = (GtkMathView*) g_object_new(GTKMATHVIEW_METHOD_NAME(get_type)(), NULL);
  g_return_val_if_fail(math_view != NULL, NULL);
  GTKMATHVIEW_METHOD_NAME(set_adjustments)(math_view, hadj, vadj);
  return GTK_WIDGET(math_view);
}

static void
gtk_math_view_destroy(GtkWidget* widget)
{
  GtkMathView* math_view;

  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(widget));

  math_view = GTK_MATH_VIEW(widget);
  g_assert(math_view != NULL);

  if (math_view->view)
    {
      math_view->view->resetRootElement();
      math_view->view->unref();
      math_view->view = 0;
    }

  if (math_view->renderingContext)
    {
      delete math_view->renderingContext;
      math_view->renderingContext = 0;
    }

  if (math_view->backend)
    {
      math_view->backend->unref();
      math_view->backend = 0;
    }

  if (math_view->hadjustment != NULL)
    {
      g_object_unref(math_view->hadjustment);
      math_view->hadjustment = NULL;
    }

  if (math_view->vadjustment != NULL)
    {
      g_object_unref(math_view->vadjustment);
      math_view->vadjustment = NULL;
    }

  if (math_view->surface != NULL)
    {
      cairo_surface_destroy(math_view->surface);
      math_view->surface = NULL;
    }

  gtk_math_view_release_document_resources(math_view);

  if (GTK_WIDGET_CLASS(parent_class)->destroy != NULL)
    GTK_WIDGET_CLASS(parent_class)->destroy(widget);
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(freeze)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  return (math_view->freeze_counter++ > 0);
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(thaw)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->freeze_counter > 0, FALSE);
  if (--math_view->freeze_counter == 0)
    {
      gtk_math_view_paint(math_view);
      return TRUE;
    }
  else
    return FALSE;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(update)(GtkMathView* math_view, GdkRectangle* rect)
{
  GtkWidget* widget = GTK_WIDGET(math_view);
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);

  if (rect)
    gtk_math_view_update(math_view, rect->x, rect->y, rect->width, rect->height);
  else
    gtk_math_view_update(math_view, 0, 0, allocation.width, allocation.height);
}

static void
gtk_math_view_realize(GtkWidget* widget)
{
  GtkWidget* parent;
  GdkWindowAttr attributes;
  GdkWindow* window;
  GtkAllocation allocation;
  gint attributes_mask;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (widget));

  gtk_widget_set_realized(widget, TRUE);
  gtk_widget_set_can_focus(widget, TRUE);

  gtk_widget_get_allocation(widget, &allocation);

  attributes.x = allocation.x;
  attributes.y = allocation.y;
  attributes.width = allocation.width;
  attributes.height = allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask = gtk_widget_get_events (widget) | 
    GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | 
    GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK |
    GDK_POINTER_MOTION_HINT_MASK;
  attributes.visual = gtk_widget_get_visual (widget);

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;
  parent = gtk_widget_get_parent(widget);
  window = gdk_window_new(gtk_widget_get_window(parent), &attributes, attributes_mask);
  gdk_window_set_user_data(window, widget);

  gtk_style_context_set_background(gtk_widget_get_style_context(widget), window);

  gtk_widget_set_window(widget, window);
}

static void
gtk_math_view_get_preferred_width(GtkWidget* widget, gint* minimal, gint* natural)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(widget));

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view != 0);

  if (BoundingBox box = math_view->view->getBoundingBox())
    {
      *minimal = *natural = box.horizontalExtent().toDouble();
    }
}

static void
gtk_math_view_get_preferred_height(GtkWidget* widget, gint* minimal, gint* natural)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(widget));

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view != 0);

  if (BoundingBox box = math_view->view->getBoundingBox())
    {
      *minimal = *natural = box.verticalExtent().toDouble();
    }
}

static void
gtk_math_view_size_allocate(GtkWidget* widget, GtkAllocation* allocation)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW (widget));
  g_return_if_fail(allocation != NULL);

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  
  if (math_view->surface != NULL)
    {
      cairo_surface_destroy(math_view->surface);
      math_view->surface = NULL;
    }

  gtk_widget_set_allocation (widget, allocation);

  if (gtk_widget_get_realized(widget))
    {
      gdk_window_move_resize(gtk_widget_get_window(widget),
			     allocation->x, allocation->y,
			     allocation->width, allocation->height);

      //gtk_math_view_paint(math_view);
    }
}

static gboolean
gtk_math_view_focus_in_event(GtkWidget* widget, GdkEventFocus*)
{
  gtk_math_view_paint(GTK_MATH_VIEW(widget));
  return FALSE;
}

static gboolean
gtk_math_view_focus_out_event(GtkWidget* widget, GdkEventFocus*)
{
  gtk_math_view_paint(GTK_MATH_VIEW(widget));
  return FALSE;
}

static gint
gtk_math_view_button_press_event(GtkWidget* widget, GdkEventButton* event)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_MATH_VIEW(widget), FALSE);

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  g_return_val_if_fail(math_view->view, FALSE);

  if (event->button == 1)
    {
      math_view->select_state = SELECT_STATE_NO;
      math_view->button_pressed = TRUE;
      math_view->button_press_x = event->x;
      math_view->button_press_y = event->y;
      math_view->button_press_time = event->time;
    }
  else if (math_view->select_state == SELECT_STATE_YES)
    {
      math_view->select_state = SELECT_STATE_ABORT;
      g_signal_emit(math_view, select_abort_signal, 0);
    }
  
  return FALSE;
}

static gint
gtk_math_view_button_release_event(GtkWidget* widget, GdkEventButton* event)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_MATH_VIEW(widget), FALSE);

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  g_return_val_if_fail(math_view->view, FALSE);

  if (event->button == 1)
    {
      GtkMathViewModelId elem = NULL;

      GTKMATHVIEW_METHOD_NAME(get_element_at)(math_view, (gint) event->x, (gint) event->y, &elem, NULL, NULL);

      GtkMathViewModelEvent me;
      me.id = elem;
      me.x = (gint) event->x;
      me.y = (gint) event->y;
      me.state = event->state;

      if (math_view->button_pressed == TRUE &&
	  math_view->select_state == SELECT_STATE_NO &&
	  fabs(math_view->button_press_x - event->x) <= CLICK_SPACE_RANGE &&
	  fabs(math_view->button_press_y - event->y) <= CLICK_SPACE_RANGE &&
	  abs(int(math_view->button_press_time - event->time)) <= CLICK_TIME_RANGE)
	{
	  // the mouse should have not moved more than one pixel in each direction
	  // and the time elapsed from the press event should be no more than 250ms
	  g_signal_emit(math_view, click_signal, 0, &me);
	}

      if (math_view->select_state == SELECT_STATE_YES)      
	g_signal_emit(math_view, select_end_signal, 0, &me);
      
      math_view->button_pressed = FALSE;
      math_view->select_state = SELECT_STATE_NO;
    }
  
  return FALSE;
}

static gint
gtk_math_view_motion_notify_event(GtkWidget* widget, GdkEventMotion* event)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_MATH_VIEW(widget), FALSE);

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  GdkWindow* window = gtk_widget_get_window(widget);
  g_return_val_if_fail(math_view->view, FALSE);

  GdkModifierType mods;
  gint x = (gint) event->x;
  gint y = (gint) event->y;
  // using GDK hints is convenient since this handler is rather heavy
  // this way we notify GDK when we are ready to process another event
  if (event->is_hint || (event->window != window))
    gdk_window_get_device_position(window, event->device, &x, &y, &mods);

  GtkMathViewModelId elem = NULL;

  GTKMATHVIEW_METHOD_NAME(get_element_at)(math_view, x, y, &elem, NULL, NULL);

  GtkMathViewModelEvent me;
  me.id = elem;
  me.x = x;
  me.y = y;
  me.state = event->state;

  if (math_view->button_pressed == TRUE &&
      (math_view->select_state == SELECT_STATE_YES ||
       fabs(math_view->button_press_x - x) > CLICK_SPACE_RANGE ||
       fabs(math_view->button_press_y - y) > CLICK_SPACE_RANGE ||
       abs(int(math_view->button_press_time - event->time)) > CLICK_TIME_RANGE))
    {
      if (math_view->select_state == SELECT_STATE_NO)
	{
	  g_signal_emit(math_view, select_begin_signal, 0, &me);
	  math_view->select_state = SELECT_STATE_YES;
	}
      else if (math_view->select_state == SELECT_STATE_YES && math_view->current_elem != elem)
	g_signal_emit(math_view, select_over_signal, 0, &me);
    }

  if (math_view->current_elem != elem)
    {
      math_view->current_elem = elem;

      g_signal_emit(math_view, element_over_signal, 0, &me);
    }

  return FALSE;
}

static gint
gtk_math_view_draw(GtkWidget* widget,
		   cairo_t *cr)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_MATH_VIEW(widget), FALSE);

  GtkMathView* math_view = GTK_MATH_VIEW(widget);

  // It may be that the first expose event the double-buffer has not
  // been allocated yet. In this case the paint method should be used first.
  if (math_view->surface == NULL)
    gtk_math_view_paint(math_view);
  else
    GTKMATHVIEW_METHOD_NAME(update)(math_view, NULL);

  return FALSE;
}

static void
gtk_math_view_click(GtkMathView* math_view, const GtkMathViewModelEvent*)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_select_begin(GtkMathView* math_view, const GtkMathViewModelEvent*)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_select_over(GtkMathView* math_view, const GtkMathViewModelEvent*)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_select_end(GtkMathView* math_view, const GtkMathViewModelEvent*)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_select_abort(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_element_over(GtkMathView* math_view, const GtkMathViewModelEvent*)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
setup_adjustment(GtkAdjustment* adj, gfloat size, gfloat page_size)
{
  g_return_if_fail(adj != NULL);

  if (size < 0)
    size = 0.0;

  gtk_adjustment_set_lower (adj, 0.0);
  gtk_adjustment_set_page_size (adj, page_size);
  gtk_adjustment_set_step_increment (adj, 10);
  gtk_adjustment_set_page_increment (adj, page_size);
  gtk_adjustment_set_upper (adj, size);

  gdouble value = gtk_adjustment_get_value(adj);

  if (value > size - page_size) {
    gtk_adjustment_set_value(adj, std::max(0.0, static_cast<double>(size - page_size)));
#if !GTK_CHECK_VERSION(3,18,0)
    gtk_adjustment_value_changed(adj);
#endif
  }

#if !GTK_CHECK_VERSION(3,18,0)
  gtk_adjustment_changed(adj);
#endif
}

static void
setup_adjustments(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view);

  const BoundingBox box = math_view->view->getBoundingBox();

  GtkAllocation allocation;
  gtk_widget_get_allocation(GTK_WIDGET(math_view), &allocation);

  if (math_view->hadjustment != NULL) {
    const gint page_width = allocation.width;
    const gint width = box.defined() ? box.width.toDouble() : page_width;
    
    if (math_view->top_x > width - page_width)
      math_view->top_x = std::max(0, width - page_width);

    setup_adjustment(math_view->hadjustment, width, page_width);
  }

  if (math_view->vadjustment != NULL) {
    const gint page_height = allocation.height;
    const gint height = box.defined() ? box.verticalExtent().toDouble() : page_height;

    if (math_view->top_y > height - page_height)
      math_view->old_top_y = math_view->top_y = std::max(0, height - page_height);

    setup_adjustment(math_view->vadjustment, height, page_height);
  }
}

#if GTKMATHVIEW_USES_LIBXML2

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_uri)(GtkMathView* math_view, const gchar* name)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(name != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadURI(name);
  gtk_math_view_paint(math_view);
  return res;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_buffer)(GtkMathView* math_view, const gchar* buffer)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(buffer != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadBuffer(buffer);
  gtk_math_view_paint(math_view);
  return res;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_document)(GtkMathView* math_view, xmlDoc* doc)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(doc != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadDocument(doc);
  gtk_math_view_paint(math_view);
  return res;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_root)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadRootElement(elem);
  gtk_math_view_paint(math_view);
  return res;
}

extern "C" xmlDoc*
GTKMATHVIEW_METHOD_NAME(get_document)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);
  g_return_val_if_fail(math_view->view != NULL, NULL);
  return math_view->view->getDocument();
}

#elif GTKMATHVIEW_USES_CUSTOM_READER

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_reader)(GtkMathView* math_view,
				     GtkMathViewReader* reader,
				     GtkMathViewReaderData user_data)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(reader != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadReader(reader, user_data);
  gtk_math_view_paint(math_view);
  return res;
}

#elif GTKMATHVIEW_USES_LIBXML2_READER

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_reader)(GtkMathView* math_view, xmlTextReaderPtr reader)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(reader != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadReader(reader);
  gtk_math_view_paint(math_view);
  return res;
}

#endif // GTKMATHVIEW_USES_LIBXML2_READER

extern "C" void
GTKMATHVIEW_METHOD_NAME(unload)(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view != NULL);
  math_view->view->unload();
  gtk_math_view_release_document_resources(math_view);
  gtk_math_view_paint(math_view);
}

extern "C" cairo_surface_t*
GTKMATHVIEW_METHOD_NAME(get_buffer)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);

  return math_view->surface;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_adjustments)(GtkMathView* math_view,
					 GtkAdjustment* hadj,
					 GtkAdjustment* vadj)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(math_view));

#if 0
  if (hadj != NULL)
    g_return_if_fail(GTK_IS_ADJUSTMENT(hadj));
  else
    hadj = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0));

  if (vadj != NULL)
    g_return_if_fail(GTK_IS_ADJUSTMENT(vadj));
  else
    vadj = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
#endif

  if (math_view->hadjustment != hadj)
    {
      if (math_view->hadjustment != NULL)
	{
	  g_signal_handlers_disconnect_matched(math_view->hadjustment, G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, math_view);
	  g_object_unref(math_view->hadjustment);
	}

      math_view->hadjustment = hadj;

      if (hadj != NULL)
	{
	  g_object_ref(math_view->hadjustment);
	  g_object_ref_sink(math_view->hadjustment);
	  
	  math_view->hsignal = 
	    g_signal_connect(hadj,
			     "value_changed",
			     G_CALLBACK(hadjustment_value_changed),
			     math_view);
	}
    }

  if (math_view->vadjustment != vadj)
    {
      if (math_view->vadjustment != NULL)
	{
	  g_signal_handlers_disconnect_matched(math_view->hadjustment, G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, math_view);
	  g_object_unref(math_view->vadjustment);
	}

      math_view->vadjustment = vadj;

      if (vadj != NULL)
	{
	  g_object_ref(math_view->vadjustment);
	  g_object_ref_sink(math_view->vadjustment);
	  
	  math_view->vsignal =
	    g_signal_connect(vadj,
			     "value_changed",
			     G_CALLBACK(vadjustment_value_changed),
			     math_view);
	}
    }
  
  setup_adjustments(math_view);
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(get_adjustments)(GtkMathView* math_view,
					 GtkAdjustment** hadjustment,
					 GtkAdjustment** vadjustment)
{
  g_return_if_fail(math_view != NULL);

  if (hadjustment) *hadjustment = math_view->hadjustment;
  if (vadjustment) *vadjustment = math_view->vadjustment;
}

extern "C" GtkAdjustment*
GTKMATHVIEW_METHOD_NAME(get_vadjustment)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);

  return math_view->vadjustment;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_font_size)(GtkMathView* math_view, guint size)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view != NULL);
  g_return_if_fail(size > 0);
  math_view->view->setDefaultFontSize(size);
  gtk_math_view_paint(math_view);
}

extern "C" guint
GTKMATHVIEW_METHOD_NAME(get_font_size)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, 0);
  g_return_val_if_fail(math_view->view != NULL, 0);
  return math_view->view->getDefaultFontSize();
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(structure_changed)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  if (math_view->view->notifyStructureChanged(elem))
    {
      gtk_math_view_paint(math_view);
      return TRUE;
    }
  else
    return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(attribute_changed)(GtkMathView* math_view, GtkMathViewModelId elem, GtkMathViewModelString name)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  if (math_view->view->notifyAttributeChanged(elem, name))
    {
      gtk_math_view_paint(math_view);
      return TRUE;
    }
  else
    return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(select)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(elem != NULL, FALSE);

  if (SmartPtr<const WrapperArea> area = findGtkWrapperArea(math_view, elem))
    {
      area->setSelected(1);
      gtk_math_view_paint(math_view);
      return TRUE;
    }
  else
    return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(unselect)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(elem != NULL, FALSE);

  if (SmartPtr<const WrapperArea> area = findGtkWrapperArea(math_view, elem))
    {
      area->setSelected(0);
      gtk_math_view_paint(math_view);
      return TRUE;
    }
  else
    return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(is_selected)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(elem != NULL, FALSE);

  if (SmartPtr<const WrapperArea> area = findGtkWrapperArea(math_view, elem))
    return area->getSelected();
  else
    return FALSE;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(get_size)(GtkMathView* math_view, gint* width, gint* height)
{
  g_return_if_fail(math_view != NULL);

  GtkAllocation allocation;
  gtk_widget_get_allocation(GTK_WIDGET(math_view), &allocation);

  if (width)
    *width = allocation.width;
  if (height)
    *height = allocation.height;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_bounding_box)(GtkMathView* math_view, GtkMathViewBoundingBox* result_box)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  if (BoundingBox box = math_view->view->getBoundingBox())
    {
      if (result_box)
	{
	  result_box->width = box.width.toDouble();
	  result_box->height = box.height.toDouble();
	  result_box->depth = box.depth.toDouble();
	}

      return TRUE;
    }
  else
    return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_element_at)(GtkMathView* math_view, gint x, gint y,
					GtkMathViewModelId* result, GtkMathViewPoint* result_orig,
					GtkMathViewBoundingBox* result_box)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);

  Point elemOrig;
  BoundingBox elemBox;
  to_view_coords(math_view, &x, &y);
  if (SmartPtr<Element> elem = math_view->view->getElementAt(scaled(x), -scaled(y),
							     result_orig ? &elemOrig : 0,
							     result_box ? &elemBox : 0))
    if (GtkMathViewModelId el = math_view->view->modelElementOfElement(elem))
      {
	if (result) *result = el;

	if (result_orig)
	  {
	    result_orig->x = elemOrig.x.toDouble();
	    result_orig->y = -elemOrig.y.toDouble();
	    from_view_coords(math_view, result_orig);
	  }

	if (result_box)
	  {
	    result_box->width = elemBox.width.toDouble();
	    result_box->height = elemBox.height.toDouble();
	    result_box->depth = elemBox.depth.toDouble();
	  }
	
	return TRUE;
      }

  return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_element_extents_ref)(GtkMathView* math_view, GtkMathViewModelId refEl, GtkMathViewModelId el,
						 GtkMathViewPoint* result_orig, GtkMathViewBoundingBox* result_box)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(el != NULL, FALSE);

  SmartPtr<Element> refElem;
  if (refEl)
    {
      refElem = math_view->view->elementOfModelElement(refEl);
      if (!refElem) return FALSE;
    }

  if (SmartPtr<Element> elem = math_view->view->elementOfModelElement(el))
    {
      Point elemOrig;
      BoundingBox elemBox;

      if ((refElem && math_view->view->getElementExtents(refElem, elem, result_orig ? &elemOrig : 0, result_box ? &elemBox : 0))
	  || math_view->view->getElementExtents(elem, result_orig ? &elemOrig : 0, result_box ? &elemBox : 0))
	{
	  if (result_orig)
	    {
	      result_orig->x = elemOrig.x.toDouble();
	      result_orig->y = -elemOrig.y.toDouble();
	      from_view_coords(math_view, result_orig);
	    }
	  
	  if (result_box)
	    {
	      result_box->width = elemBox.width.toDouble();
	      result_box->height = elemBox.height.toDouble();
	      result_box->depth = elemBox.depth.toDouble();
	    }
	  
	  return TRUE;
	}
    }
  return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_element_extents)(GtkMathView* math_view, GtkMathViewModelId el,
					     GtkMathViewPoint* result_orig, GtkMathViewBoundingBox* result_box)
{
  return GTKMATHVIEW_METHOD_NAME(get_element_extents_ref)(math_view, NULL, el, result_orig, result_box);
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_char_at)(GtkMathView* math_view, gint x, gint y,
				     GtkMathViewModelId* result_elem, gint* result_index,
				     GtkMathViewPoint* result_orig, GtkMathViewBoundingBox* result_box)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);

  CharIndex charIndex;
  Point charOrig;
  BoundingBox charBox;
  to_view_coords(math_view, &x, &y);
  if (SmartPtr<Element> elem = math_view->view->getCharAt(scaled(x), -scaled(y), charIndex,
							  result_orig ? &charOrig : 0,
							  result_box ? &charBox : 0))
    if (GtkMathViewModelId el = math_view->view->modelElementOfElement(elem))
      {
	if (result_elem) *result_elem = el;

	if (result_index) *result_index = charIndex;

	if (result_orig)
	  {
	    result_orig->x = charOrig.x.toDouble();
	    result_orig->y = -charOrig.y.toDouble();
	    from_view_coords(math_view, result_orig);
	  }
	
	if (result_box)
	  {
	    result_box->width = charBox.width.toDouble();
	    result_box->height = charBox.height.toDouble();
	    result_box->depth = charBox.depth.toDouble();
	  }
	
	return TRUE;
      }
  
  return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_char_extents_ref)(GtkMathView* math_view, GtkMathViewModelId refEl, GtkMathViewModelId el,
					      gint index, GtkMathViewPoint* result_orig, GtkMathViewBoundingBox* result_box)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(el != NULL, FALSE);
  g_return_val_if_fail(index >= 0, FALSE);

  SmartPtr<Element> refElem;
  if (refEl)
    {
      refElem = math_view->view->elementOfModelElement(refEl);
      if (!refElem) return FALSE;
    }

  if (SmartPtr<Element> elem = math_view->view->elementOfModelElement(el))
    {
      Point charOrig;
      BoundingBox charBox;
      if ((refElem && math_view->view->getCharExtents(refElem, elem, index, result_orig ? &charOrig : 0, result_box ? &charBox : 0))
	  || math_view->view->getCharExtents(elem, index, result_orig ? &charOrig : 0, result_box ? &charBox : 0))
	{
	  if (result_orig)
	    {
	      result_orig->x = charOrig.x.toDouble();
	      result_orig->y = -charOrig.y.toDouble();
	      from_view_coords(math_view, result_orig);
	    }
	  
	  if (result_box)
	    {
	      result_box->width = charBox.width.toDouble();
	      result_box->height = charBox.height.toDouble();
	      result_box->depth = charBox.depth.toDouble();
	    }
	
	  return TRUE;
	}
    }

  return FALSE;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_char_extents)(GtkMathView* math_view, GtkMathViewModelId el,
					  gint index, GtkMathViewPoint* result_orig, GtkMathViewBoundingBox* result_box)
{
  return GTKMATHVIEW_METHOD_NAME(get_char_extents_ref)(math_view, NULL, el, index, result_orig, result_box);
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(get_top)(GtkMathView* math_view, gint* x, gint* y)
{
  g_return_if_fail(math_view != NULL);
  gdouble h_value = gtk_adjustment_get_value(math_view->hadjustment);
  gdouble v_value = gtk_adjustment_get_value(math_view->vadjustment);
  if (x != NULL)
    *x = math_view->vadjustment ? h_value : 0;
  if (y != NULL)
    *y = math_view->hadjustment ? v_value : 0;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_top)(GtkMathView* math_view, gint x, gint y)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->vadjustment != NULL);
  g_return_if_fail(math_view->hadjustment != NULL);

  gtk_adjustment_set_value(math_view->hadjustment, x);
  gtk_adjustment_set_value(math_view->vadjustment, y);

#if !GTK_CHECK_VERSION(3,18,0)
  gtk_adjustment_value_changed(math_view->hadjustment);
  gtk_adjustment_value_changed(math_view->vadjustment);
#endif
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_log_verbosity)(GtkMathView* math_view, gint level)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view != 0);
  g_return_if_fail(math_view->renderingContext != 0);
  math_view->view->getLogger()->setLogLevel(LogLevelId(level));
}

extern "C" gint
GTKMATHVIEW_METHOD_NAME(get_log_verbosity)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, LOG_ERROR);
  g_return_val_if_fail(math_view->view != 0, LOG_ERROR);
  return math_view->view->getLogger()->getLogLevel();
}
