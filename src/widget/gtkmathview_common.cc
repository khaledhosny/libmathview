// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>
#include <stdlib.h>

#include <sstream>

#include "defs.h"

// don't know why this is needed!!!
#define PANGO_ENABLE_BACKEND
#include <pango/pango.h>

#include <math.h>
#include <gtk/gtk.h>
#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>

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
#elif GTKMATHVIEW_USES_GMETADOM
#include "gmetadom.hh"
#define GTK_MATH_VIEW_WIDGET_NAME "GtkMathView_GMetaDOM"
#include "gmetadom_MathView.hh"
typedef gmetadom_MathView MathView;
#endif

#include "Init.hh"
#include "Logger.hh"
#include "Configuration.hh"
#include "Point.hh"
#include "Rectangle.hh"
#include "MathMLElement.hh"
#include "MathMLOperatorDictionary.hh"
#include "MathMLNamespaceContext.hh"
#if GMV_ENABLE_BOXML
#include "BoxMLNamespaceContext.hh"
#endif // GMV_ENABLE_BOXML
#include "MathGraphicDevice.hh"
#include "BoxGraphicDevice.hh"
#include "Gtk_Backend.hh"
#include "Gtk_RenderingContext.hh"
#include "Gtk_WrapperArea.hh"

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

  void (*set_scroll_adjustments) (GtkMathView *math_view,
				  GtkAdjustment *hadjustment,
				  GtkAdjustment *vadjustment);

  GtkMathViewModelSignal click;
  GtkMathViewModelSignal select_begin;
  GtkMathViewModelSignal select_over;
  GtkMathViewModelSignal select_end;
  GtkMathViewSelectAbortSignal select_abort;
  GtkMathViewModelSignal element_over;
  GtkMathViewDecorateSignal decorate_under;
  GtkMathViewDecorateSignal decorate_over;

  AbstractLogger* logger;
  gint defaultFontSize;
  bool defaultT1OpaqueMode;
  bool defaultT1AntiAliasedMode;
  MathMLOperatorDictionary* dictionary;
  Gtk_Backend* backend;
};

struct _GtkMathView
{
  GtkWidget      parent;

  GtkWidget* 	 area;
  GdkPixmap*     pixmap;

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
  Gtk_RenderingContext* renderingContext;
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

/* GtkObject functions */

static void gtk_math_view_destroy(GtkObject*);

/* GtkWidget functions */

static gboolean gtk_math_view_expose_event(GtkWidget*, GdkEventExpose*);
static gboolean gtk_math_view_button_press_event(GtkWidget*, GdkEventButton*);
static gboolean gtk_math_view_button_release_event(GtkWidget*, GdkEventButton*);
static gboolean gtk_math_view_motion_notify_event(GtkWidget*, GdkEventMotion*);
static gboolean gtk_math_view_focus_in_event(GtkWidget*, GdkEventFocus*);
static gboolean gtk_math_view_focus_out_event(GtkWidget*, GdkEventFocus*);
static void     gtk_math_view_realize(GtkWidget*);
static void     gtk_math_view_size_request(GtkWidget*, GtkRequisition*);
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

// static RGBColor
// RGBColorOfGdkColor(const GdkColor& c)
// { return RGBColor(c.red >> 8, c.green >> 8, c.blue >> 8); }

static SmartPtr<const Gtk_WrapperArea>
findGtkWrapperArea(GtkMathView* math_view, GtkMathViewModelId node)
{
  if (SmartPtr<Element> elem = math_view->view->elementOfModelElement(node))
    if (SmartPtr<const Gtk_WrapperArea> area = smart_cast<const Gtk_WrapperArea>(elem->getArea()))
      return area;
  return 0;
}

#if 0
static GtkMathViewModelId
findAction(GtkMathView* math_view, GtkMathViewModelId node)
{
  SmartPtr<Element> elem = math_view->view->elementOfModelElement(node);
  while (elem && !is_a<MathMLActionElement>(elem) && !is_a<BoxMLActionElement>(elem))
    elem = elem->getParent();
  return elem ? math_view->view->modelElementOfElement(elem) : GtkMathViewModelId();
}
#endif

/* auxiliary C functions */

static void
from_view_coords(GtkMathView* math_view, GtkMathViewPoint* point)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(point != NULL);
  point->x -= math_view->top_x;
  point->y -= math_view->top_y - Gtk_RenderingContext::toGtkPixels(math_view->view->getBoundingBox().height);
}

static void
to_view_coords(GtkMathView* math_view, gint* x, gint* y)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(x != NULL);
  g_return_if_fail(y != NULL);
  *x += math_view->top_x;
  *y += math_view->top_y - Gtk_RenderingContext::toGtkPixels(math_view->view->getBoundingBox().height);
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

  if (!GTK_WIDGET_MAPPED(GTK_WIDGET(math_view)) || math_view->freeze_counter > 0) return;

  if (math_view->pixmap != NULL)
    gdk_draw_pixmap(widget->window,
		    widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		    math_view->pixmap,
		    x0, y0, x0, y0, width, height);
  else
    gdk_draw_rectangle(widget->window,
		       widget->style->white_gc,
		       TRUE,
		       x0, y0, width, height);

  g_signal_emit(GTK_OBJECT(math_view), decorate_over_signal, 0, widget->window);
}

static void
gtk_math_view_paint(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);

  GtkMathViewClass* math_view_class = GTK_MATH_VIEW_CLASS(G_OBJECT_GET_CLASS(G_OBJECT(math_view)));
  g_return_if_fail(math_view_class != NULL);

  if (!GTK_WIDGET_MAPPED(GTK_WIDGET(math_view)) || math_view->freeze_counter > 0) return;

  GtkWidget* widget = GTK_WIDGET(math_view);
  
  setup_adjustments(math_view);

  const gint width = widget->allocation.width;
  const gint height = widget->allocation.height;

  Gtk_RenderingContext* rc = math_view->renderingContext;
  g_return_if_fail(rc != 0);

  if (math_view->pixmap == NULL)
    {
      math_view->pixmap = gdk_pixmap_new(widget->window, width, height, -1);
      rc->setDrawable(math_view->pixmap);
    }

  rc->setStyle(Gtk_RenderingContext::SELECTED_STYLE);
  if (GTK_WIDGET_HAS_FOCUS(math_view))
    {
      rc->setForegroundColor(widget->style->text[GTK_STATE_SELECTED]);
      rc->setBackgroundColor(widget->style->base[GTK_STATE_SELECTED]);
    }
  else
    {
      rc->setForegroundColor(widget->style->text[GTK_STATE_ACTIVE]);
      rc->setBackgroundColor(widget->style->base[GTK_STATE_ACTIVE]);
    }
  rc->setStyle(Gtk_RenderingContext::NORMAL_STYLE);
  rc->setForegroundColor(widget->style->fg[GTK_STATE_NORMAL]);
  rc->setBackgroundColor(widget->style->bg[GTK_STATE_NORMAL]);

  gdk_draw_rectangle(math_view->pixmap, widget->style->white_gc, TRUE, 0, 0, width, height);

  // WARNING: setAvailableWidth must be invoked BEFORE any coordinate conversion
  math_view->view->setAvailableWidth(Gtk_RenderingContext::fromGtkX(width));
  gint x = 0;
  gint y = 0;
  to_view_coords(math_view, &x, &y);
  g_signal_emit(GTK_OBJECT(math_view), decorate_under_signal, 0, math_view->pixmap);
  math_view->view->render(*rc,
			  Gtk_RenderingContext::fromGtkX(-x),
			  Gtk_RenderingContext::fromGtkY(-y));

  gtk_math_view_update(math_view, 0, 0, width, height);
}

static void
hadjustment_value_changed(GtkAdjustment* adj, GtkMathView* math_view)
{
  g_return_if_fail(adj != NULL);
  g_return_if_fail(math_view != NULL);

  if (adj->value > adj->upper - adj->page_size) adj->value = adj->upper - adj->page_size;
  if (adj->value < adj->lower) adj->value = adj->lower;

  math_view->old_top_x = math_view->top_x;
  math_view->top_x = static_cast<int>(adj->value);

  if (math_view->old_top_x != math_view->top_x)
    gtk_math_view_paint(math_view);
}

static void
vadjustment_value_changed(GtkAdjustment* adj, GtkMathView* math_view)
{
  g_return_if_fail(adj != NULL);
  g_return_if_fail(math_view != NULL);

  if (adj->value > adj->upper - adj->page_size) adj->value = adj->upper - adj->page_size;
  if (adj->value < adj->lower) adj->value = adj->lower;

  math_view->old_top_y = math_view->top_y;
  math_view->top_y = static_cast<int>(adj->value);

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

  SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, getenv("GTKMATHVIEWCONF"));
  configuration->ref();

  math_view_class->defaultFontSize = configuration->getInt(logger, "default/font-size", DEFAULT_FONT_SIZE);
  math_view_class->defaultT1OpaqueMode = configuration->getBool(logger, "default/t1lib/opaque-mode", false);
  math_view_class->defaultT1AntiAliasedMode = configuration->getBool(logger, "default/t1lib/anti-aliasing", false);

  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary<MathView>(logger, configuration);
  dictionary->ref();
  math_view_class->dictionary = dictionary;

  SmartPtr<Gtk_Backend> backend = Gtk_Backend::create(logger, configuration);
  backend->ref();
  math_view_class->backend = backend;
}

static void
gtk_math_view_base_class_finalize(GtkMathViewClass* math_view_class)
{
  g_return_if_fail(math_view_class != NULL);

  if (math_view_class->backend)
    {
      math_view_class->backend->unref();
      math_view_class->backend = 0;
    }

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

#if 0
  GObjectClass* gobject_class = G_OBJECT_CLASS(math_view_class);
#endif
  GtkObjectClass* object_class = (GtkObjectClass*) math_view_class;
  GtkWidgetClass* widget_class = (GtkWidgetClass*) math_view_class;
	
  math_view_class->click = gtk_math_view_click;
  math_view_class->select_begin = gtk_math_view_select_begin;
  math_view_class->select_over = gtk_math_view_select_over;
  math_view_class->select_end = gtk_math_view_select_end;
  math_view_class->select_abort = gtk_math_view_select_abort;
  math_view_class->element_over = gtk_math_view_element_over;
  math_view_class->set_scroll_adjustments = gtk_math_view_set_adjustments;

  parent_class = (GtkWidgetClass*) gtk_type_class(gtk_widget_get_type());

#if 0
  gobject_class->set_property = gtk_math_view_set_property;
  gobject_class->get_property = gtk_math_view_get_property;
#endif

  object_class->destroy = gtk_math_view_destroy;

  widget_class->realize = gtk_math_view_realize;
  widget_class->expose_event = gtk_math_view_expose_event;
  widget_class->size_request = gtk_math_view_size_request;
  widget_class->size_allocate = gtk_math_view_size_allocate;
  widget_class->button_press_event = gtk_math_view_button_press_event;
  widget_class->button_release_event = gtk_math_view_button_release_event;
  widget_class->motion_notify_event = gtk_math_view_motion_notify_event;
  widget_class->focus_in_event = gtk_math_view_focus_in_event;
  widget_class->focus_out_event = gtk_math_view_focus_out_event;

  widget_class->set_scroll_adjustments_signal = 
    g_signal_new("set_scroll_adjustments",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(GtkMathViewClass,set_scroll_adjustments),
		 NULL, NULL,
		 gtk_marshal_NONE__POINTER_POINTER,
		 G_TYPE_NONE , 2, GTK_TYPE_ADJUSTMENT, GTK_TYPE_ADJUSTMENT); 
	
  click_signal = 
    g_signal_new("click",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, click),
		 NULL, NULL,
		 gtk_marshal_NONE__POINTER,
		 G_TYPE_NONE, 1, GTK_TYPE_POINTER);

  select_begin_signal=
    g_signal_new("select_begin",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, select_begin),
		 NULL, NULL,
		 gtk_marshal_NONE__POINTER,
		 G_TYPE_NONE, 1, GTK_TYPE_POINTER);
	
  select_over_signal = 
    g_signal_new("select_over",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, select_over),
		 NULL, NULL,
		 gtk_marshal_NONE__POINTER,
		 G_TYPE_NONE, 1, GTK_TYPE_POINTER);
	
  select_end_signal = 
    g_signal_new("select_end",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, select_end),
		 NULL, NULL,
		 gtk_marshal_NONE__POINTER,
		 G_TYPE_NONE, 1, GTK_TYPE_POINTER);
	
  select_abort_signal = 
    g_signal_new("select_abort",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, select_abort),
		 NULL, NULL,
		 gtk_marshal_NONE__NONE,
		 G_TYPE_NONE, 0);
	
  element_over_signal = 
    g_signal_new("element_over",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, element_over),
		 NULL, NULL,
		 gtk_marshal_NONE__POINTER,
		 G_TYPE_NONE, 1, GTK_TYPE_POINTER);

  decorate_under_signal =
    g_signal_new("decorate_under",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, decorate_under),
		 NULL, NULL,
		 gtk_marshal_NONE__POINTER,
		 G_TYPE_NONE, 1, GTK_TYPE_POINTER);

  decorate_over_signal =
    g_signal_new("decorate_over",
		 G_OBJECT_CLASS_TYPE(object_class),
		 G_SIGNAL_RUN_FIRST,
		 G_STRUCT_OFFSET(GtkMathViewClass, decorate_over),
		 NULL, NULL, 
		 gtk_marshal_NONE__POINTER,
		 G_TYPE_NONE, 1, GTK_TYPE_POINTER);

#if 0	
  g_object_class_install_property(gobject_class,
				  PROP_DEFAULT_FONT_SIZE,
				  g_param_spec_int("font_size",
						   "Font Size",
						   "The default font size",
						   -1,
						   100,
						   -1,
						   G_PARAM_READWRITE));

  g_object_class_install_property(gobject_class,
				  PROP_LOG_VERBOSITY,
				  g_param_spec_int("log_verbosity",
						   "Logger verbosity",
						   "Verbosity level of the logger",
						   LOG_ERROR,
						   LOG_DEBUG,
						   LOG_ERROR,
						   G_PARAM_READWRITE));
#endif
}

#if GTKMATHVIEW_USES_GMETADOM

static void
gtk_math_view_release_document_resources(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);

  GdomeException exc = 0;

  if (math_view->current_elem != NULL)
    {
      gdome_el_unref(math_view->current_elem, &exc);
      g_assert(exc == 0);
      math_view->current_elem = NULL;
    }
}

#else

static void
gtk_math_view_release_document_resources(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  math_view->current_elem = NULL;
}

#endif

static void
gtk_math_view_init(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);

  math_view->pixmap          = NULL;
  math_view->view            = 0;
  math_view->renderingContext = 0;
  math_view->freeze_counter  = 0;
  math_view->select_state    = SELECT_STATE_NO;
  math_view->button_pressed  = FALSE;
  math_view->current_elem    = NULL;
  math_view->hadjustment = NULL;
  math_view->vadjustment = NULL;
  math_view->top_x = math_view->top_y = 0;
  math_view->old_top_x = math_view->old_top_y = 0;

  GtkMathViewClass* math_view_class = GTK_MATH_VIEW_CLASS(gtk_type_class(gtk_math_view_get_type()));
  g_assert(math_view_class != NULL);

  SmartPtr<MathView> view = MathView::create(math_view_class->logger);
  view->ref();
  math_view->view = view;

  view->setDefaultFontSize(math_view_class->defaultFontSize);
  view->setOperatorDictionary(math_view_class->dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, math_view_class->backend->getMathGraphicDevice()));
#if GMV_ENABLE_BOXML
  view->setBoxMLNamespaceContext(BoxMLNamespaceContext::create(view, math_view_class->backend->getBoxGraphicDevice()));
#endif // GMV_ENABLE_BOXML

  math_view->renderingContext = new Gtk_RenderingContext(math_view_class->logger);
  math_view->renderingContext->setColorMap(gtk_widget_get_colormap(GTK_WIDGET(math_view)));
  math_view->renderingContext->setT1OpaqueMode(math_view_class->defaultT1OpaqueMode);
  math_view->renderingContext->setT1AntiAliasedMode(math_view_class->defaultT1AntiAliasedMode);
}

extern "C" GtkWidget*
GTKMATHVIEW_METHOD_NAME(new)(GtkAdjustment* hadj, GtkAdjustment* vadj)
{
  GtkMathView* math_view = (GtkMathView*) gtk_type_new(GTKMATHVIEW_METHOD_NAME(get_type)());
  g_return_val_if_fail(math_view != NULL, NULL);
  GTKMATHVIEW_METHOD_NAME(set_adjustments)(math_view, hadj, vadj);
  return GTK_WIDGET(math_view);
}

static void
gtk_math_view_destroy(GtkObject* object)
{
  GtkMathView* math_view;

  g_return_if_fail(object != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(object));

  math_view = GTK_MATH_VIEW(object);
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

  if (math_view->hadjustment != NULL)
    {
      gtk_object_unref(GTK_OBJECT(math_view->hadjustment));
      math_view->hadjustment = NULL;
    }

  if (math_view->vadjustment != NULL)
    {
      gtk_object_unref(GTK_OBJECT(math_view->vadjustment));
      math_view->vadjustment = NULL;
    }

  if (math_view->pixmap != NULL)
    {
      g_object_unref(G_OBJECT(math_view->pixmap));
      math_view->pixmap = NULL;
    }

  gtk_math_view_release_document_resources(math_view);

  if (GTK_OBJECT_CLASS(parent_class)->destroy != NULL)
    (*GTK_OBJECT_CLASS(parent_class)->destroy)(object);
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
  if (rect)
    gtk_math_view_update(math_view, rect->x, rect->y, rect->width, rect->height);
  else
    gtk_math_view_update(math_view, 0, 0, widget->allocation.width, widget->allocation.height);
}

static void
gtk_math_view_realize(GtkWidget* widget)
{
  GtkMathView* math_view;
  GdkWindowAttr attributes;
  gint attributes_mask;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (widget));

  GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED | GTK_CAN_FOCUS);
  math_view = GTK_MATH_VIEW (widget);

  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask = gtk_widget_get_events (widget) | 
    GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | 
    GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK |
    GDK_POINTER_MOTION_HINT_MASK;
  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window = gdk_window_new (widget->parent->window, &attributes, attributes_mask);

  widget->style = gtk_style_attach (widget->style, widget->window);

  gdk_window_set_user_data (widget->window, widget);

  gtk_style_set_background (widget->style, widget->window, GTK_STATE_ACTIVE);
}

static void
gtk_math_view_size_request(GtkWidget* widget, GtkRequisition* requisition)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(requisition != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(widget));

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view != 0);

  if (BoundingBox box = math_view->view->getBoundingBox())
    {
      requisition->width = Gtk_RenderingContext::toGtkPixels(box.horizontalExtent());
      requisition->height = Gtk_RenderingContext::toGtkPixels(box.verticalExtent());
    }
}

static void
gtk_math_view_size_allocate(GtkWidget* widget, GtkAllocation* allocation)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW (widget));
  g_return_if_fail(allocation != NULL);

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  
  if (math_view->pixmap != NULL)
    {
      g_object_unref(math_view->pixmap);
      math_view->pixmap = NULL;
    }

  widget->allocation = *allocation;
  if (GTK_WIDGET_REALIZED(widget))
    {
      gdk_window_move_resize(widget->window,
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
      g_signal_emit(GTK_OBJECT(math_view), select_abort_signal, 0);
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
#if GTKMATHVIEW_USES_GMETADOM
      GdomeException exc = 0;
#endif
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
	  abs(math_view->button_press_time - event->time) <= CLICK_TIME_RANGE)
	{
	  // the mouse should have not moved more than one pixel in each direction
	  // and the time elapsed from the press event should be no more than 250ms
	  g_signal_emit(GTK_OBJECT(math_view), click_signal, 0, &me);
	}

      if (math_view->select_state == SELECT_STATE_YES)      
	g_signal_emit(GTK_OBJECT(math_view), select_end_signal, 0, &me);
      
#if GTKMATHVIEW_USES_GMETADOM
      if (elem != NULL)
	{
	  gdome_el_unref(elem, &exc);
	  g_assert(exc == 0);
	}
#endif // GTKMATHVIEW_USES_GMETADOM

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
  g_return_val_if_fail(math_view->view, FALSE);

  GdkModifierType mods;
  gint x = (gint) event->x;
  gint y = (gint) event->y;
  // using GDK hints is convenient since this handler is rather heavy
  // this way we notify GDK when we are ready to process another event
  if (event->is_hint || (event->window != widget->window))
    gdk_window_get_pointer(widget->window, &x, &y, &mods);

#if GTKMATHVIEW_USES_GMETADOM
  GdomeException exc = 0;
#endif
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
       abs(math_view->button_press_time - event->time) > CLICK_TIME_RANGE))
    {
      if (math_view->select_state == SELECT_STATE_NO)
	{
	  g_signal_emit(GTK_OBJECT(math_view), select_begin_signal, 0, &me);
	  math_view->select_state = SELECT_STATE_YES;
	}
      else if (math_view->select_state == SELECT_STATE_YES && math_view->current_elem != elem)
	g_signal_emit(GTK_OBJECT(math_view), select_over_signal, 0, &me);
    }

  if (math_view->current_elem != elem)
    {
#if GTKMATHVIEW_USES_GMETADOM
      if (math_view->current_elem != NULL)
	{
	  gdome_el_unref(math_view->current_elem, &exc);
	  g_assert(exc == 0);
	}
#endif // GTKMATHVIEW_USES_GMETADOM

      math_view->current_elem = elem;

#if GTKMATHVIEW_USES_GMETADOM
      if (math_view->current_elem != NULL)
	{
	  gdome_el_ref(math_view->current_elem, &exc);
	  g_assert(exc == 0);
	}
#endif // GTKMATHVIEW_USES_GMETADOM

      g_signal_emit(GTK_OBJECT(math_view), element_over_signal, 0, &me);
    }

#if GTKMATHVIEW_USES_GMETADOM
  if (elem != NULL)
    {
      gdome_el_unref(elem, &exc);
      g_assert(exc == 0);
    }
#endif // GTKMATHVIEW_USES_GMETADOM

  return FALSE;
}

static gint
gtk_math_view_expose_event(GtkWidget* widget,
			   GdkEventExpose* event)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_MATH_VIEW(widget), FALSE);

  GtkMathView* math_view = GTK_MATH_VIEW(widget);

  // It may be that the first expose event the double-buffer has not
  // been allocated yet. In this case the paint method should be used
  if (math_view->pixmap == NULL)
    gtk_math_view_paint(math_view);
  else
    GTKMATHVIEW_METHOD_NAME(update)(math_view, event->area.x, event->area.y, event->area.width, event->area.height);

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

  adj->lower = 0.0;
  adj->page_size = page_size;
  adj->step_increment = 10;
  adj->page_increment = page_size;
  adj->upper = size;
  if (adj->upper < 0) adj->upper = 0.0;

  if (adj->value > adj->upper - page_size) {
    adj->value = std::max(0.0, adj->upper - page_size);
    gtk_adjustment_value_changed(adj);
  }

  gtk_adjustment_changed(adj);
}

static void
setup_adjustments(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view);

  const BoundingBox box = math_view->view->getBoundingBox();

  if (math_view->hadjustment != NULL) {
    const gint page_width = GTK_WIDGET(math_view)->allocation.width;
    const gint width = box.defined() ? Gtk_RenderingContext::toGtkPixels(box.width) : page_width;
    
    if (math_view->top_x > width - page_width)
      math_view->top_x = std::max(0, width - page_width);

    setup_adjustment(math_view->hadjustment, width, page_width);
  }

  if (math_view->vadjustment != NULL) {
    const gint page_height = GTK_WIDGET(math_view)->allocation.height;
    const gint height = box.defined() ? Gtk_RenderingContext::toGtkPixels(box.verticalExtent()) : page_height;

    if (math_view->top_y > height - page_height)
      math_view->old_top_y = math_view->top_y = std::max(0, height - page_height);

    setup_adjustment(math_view->vadjustment, height, page_height);
  }
}

#if GTKMATHVIEW_USES_GMETADOM

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_uri)(GtkMathView* math_view, const gchar* name)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
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
  g_return_val_if_fail(buffer != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadBuffer(buffer);
  gtk_math_view_paint(math_view);
  return res;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_document)(GtkMathView* math_view, GdomeDocument* doc)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(doc != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadDocument(DOM::Document(doc));
  gtk_math_view_paint(math_view);
  return res;
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(load_root)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);

  gtk_math_view_release_document_resources(math_view);
  const bool res = math_view->view->loadRootElement(DOM::Element(elem));
  gtk_math_view_paint(math_view);
  return res;
}

extern "C" GdomeDocument*
GTKMATHVIEW_METHOD_NAME(get_document)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);
  g_return_val_if_fail(math_view->view != NULL, NULL);
  return reinterpret_cast<GdomeDocument*>(math_view->view->getDocument().gdome_object());
}

#elif GTKMATHVIEW_USES_LIBXML2

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

extern "C" GdkPixmap*
GTKMATHVIEW_METHOD_NAME(get_buffer)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);

  return math_view->pixmap;
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
	  gtk_signal_disconnect_by_data(GTK_OBJECT(math_view->hadjustment), math_view);
	  gtk_object_unref(GTK_OBJECT(math_view->hadjustment));
	}

      math_view->hadjustment = hadj;

      if (hadj != NULL)
	{
	  gtk_object_ref(GTK_OBJECT(math_view->hadjustment));
	  gtk_object_sink(GTK_OBJECT(math_view->hadjustment));
	  
	  math_view->hsignal = 
	    g_signal_connect(GTK_OBJECT(hadj), 
			     "value_changed",
			     G_CALLBACK(hadjustment_value_changed),
			     math_view);
	}
    }

  if (math_view->vadjustment != vadj)
    {
      if (math_view->vadjustment != NULL)
	{
	  gtk_signal_disconnect_by_data(GTK_OBJECT(math_view->vadjustment), math_view);
	  gtk_object_unref(GTK_OBJECT(math_view->vadjustment));
	}

      math_view->vadjustment = vadj;

      if (vadj != NULL)
	{
	  gtk_object_ref(GTK_OBJECT(math_view->vadjustment));
	  gtk_object_sink(GTK_OBJECT(math_view->vadjustment));
	  
	  math_view->vsignal =
	    g_signal_connect(GTK_OBJECT(vadj), 
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

  if (SmartPtr<const Gtk_WrapperArea> area = findGtkWrapperArea(math_view, elem))
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

  if (SmartPtr<const Gtk_WrapperArea> area = findGtkWrapperArea(math_view, elem))
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

  if (SmartPtr<const Gtk_WrapperArea> area = findGtkWrapperArea(math_view, elem))
    return area->getSelected();
  else
    return FALSE;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(get_size)(GtkMathView* math_view, gint* width, gint* height)
{
  g_return_if_fail(math_view != NULL);

  if (width) *width = GTK_WIDGET(math_view)->allocation.width;
  if (height) *height = GTK_WIDGET(math_view)->allocation.height;
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
	  result_box->width = Gtk_RenderingContext::toGtkPixels(box.width);
	  result_box->height = Gtk_RenderingContext::toGtkPixels(box.height);
	  result_box->depth = Gtk_RenderingContext::toGtkPixels(box.depth);
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
  if (SmartPtr<Element> elem = math_view->view->getElementAt(Gtk_RenderingContext::fromGtkX(x),
							     Gtk_RenderingContext::fromGtkY(y),
							     result_orig ? &elemOrig : 0,
							     result_box ? &elemBox : 0))
    if (GtkMathViewModelId el = math_view->view->modelElementOfElement(elem))
      {
	if (result) *result = el;

	if (result_orig)
	  {
	    result_orig->x = Gtk_RenderingContext::toGtkX(elemOrig.x);
	    result_orig->y = Gtk_RenderingContext::toGtkY(elemOrig.y);
	    from_view_coords(math_view, result_orig);
	  }

	if (result_box)
	  {
	    result_box->width = Gtk_RenderingContext::toGtkPixels(elemBox.width);
	    result_box->height = Gtk_RenderingContext::toGtkPixels(elemBox.height);
	    result_box->depth = Gtk_RenderingContext::toGtkPixels(elemBox.depth);
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
	      result_orig->x = Gtk_RenderingContext::toGtkX(elemOrig.x);
	      result_orig->y = Gtk_RenderingContext::toGtkY(elemOrig.y);
	      from_view_coords(math_view, result_orig);
	    }
	  
	  if (result_box)
	    {
	      result_box->width = Gtk_RenderingContext::toGtkPixels(elemBox.width);
	      result_box->height = Gtk_RenderingContext::toGtkPixels(elemBox.height);
	      result_box->depth = Gtk_RenderingContext::toGtkPixels(elemBox.depth);
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
  if (SmartPtr<Element> elem = math_view->view->getCharAt(Gtk_RenderingContext::fromGtkX(x),
							  Gtk_RenderingContext::fromGtkY(y),
							  charIndex,
							  result_orig ? &charOrig : 0,
							  result_box ? &charBox : 0))
    if (GtkMathViewModelId el = math_view->view->modelElementOfElement(elem))
      {
	if (result_elem) *result_elem = el;

	if (result_index) *result_index = charIndex;

	if (result_orig)
	  {
	    result_orig->x = Gtk_RenderingContext::toGtkX(charOrig.x);
	    result_orig->y = Gtk_RenderingContext::toGtkY(charOrig.y);
	    from_view_coords(math_view, result_orig);
	  }
	
	if (result_box)
	  {
	    result_box->width = Gtk_RenderingContext::toGtkPixels(charBox.width);
	    result_box->height = Gtk_RenderingContext::toGtkPixels(charBox.height);
	    result_box->depth = Gtk_RenderingContext::toGtkPixels(charBox.depth);
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
	      result_orig->x = Gtk_RenderingContext::toGtkX(charOrig.x);
	      result_orig->y = Gtk_RenderingContext::toGtkY(charOrig.y);
	      from_view_coords(math_view, result_orig);
	    }
	  
	  if (result_box)
	    {
	      result_box->width = Gtk_RenderingContext::toGtkPixels(charBox.width);
	      result_box->height = Gtk_RenderingContext::toGtkPixels(charBox.height);
	      result_box->depth = Gtk_RenderingContext::toGtkPixels(charBox.depth);
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
  if (x != NULL) *x = math_view->vadjustment ? Gtk_RenderingContext::toGtkPixels(math_view->hadjustment->value) : 0;
  if (y != NULL) *y = math_view->hadjustment ? Gtk_RenderingContext::toGtkPixels(math_view->vadjustment->value) : 0;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_top)(GtkMathView* math_view, gint x, gint y)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->vadjustment != NULL);
  g_return_if_fail(math_view->hadjustment != NULL);

  math_view->hadjustment->value = x;
  math_view->vadjustment->value = y;

  gtk_adjustment_value_changed(math_view->hadjustment);
  gtk_adjustment_value_changed(math_view->vadjustment);
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

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_t1_opaque_mode)(GtkMathView* math_view, gboolean mode)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->renderingContext != 0);
  math_view->renderingContext->setT1OpaqueMode(mode == TRUE);
  gtk_math_view_paint(math_view);
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_t1_opaque_mode)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->renderingContext != 0, FALSE);
  return math_view->renderingContext->getT1OpaqueMode() ? TRUE : FALSE;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_t1_anti_aliased_mode)(GtkMathView* math_view, gboolean mode)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->renderingContext != 0);
  math_view->renderingContext->setT1AntiAliasedMode(mode == TRUE);
  gtk_math_view_paint(math_view);
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(get_t1_anti_aliased_mode)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->renderingContext != 0, FALSE);
  return math_view->renderingContext->getT1AntiAliasedMode() ? TRUE : FALSE;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(add_configuration_path)(const gchar* path)
{
  g_return_if_fail(path != NULL);
  Configuration::addConfigurationPath(path);
}
