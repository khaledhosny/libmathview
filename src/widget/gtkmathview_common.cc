// Copyright (C) 2000-2002, Luca Padovani <luca.padovani@cs.unibo.it>.
//
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

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
#include <gtk/gtkdrawingarea.h>

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

#include "Logger.hh"
#include "Configuration.hh"
#include "Point.hh"
#include "Rectangle.hh"
#include "MathMLElement.hh"
#include "MathMLOperatorDictionary.hh"
#include "MathMLNamespaceContext.hh"
#include "Gtk_MathGraphicDevice.hh"
#if ENABLE_BOXML
#include "BoxMLNamespaceContext.hh"
#include "Gtk_BoxGraphicDevice.hh"
#endif // ENABLE_BOXML
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
    PROP_DEFAULT_FOREGROUND_COLOR,
    PROP_DEFAULT_BACKGROUND_COLOR,
    PROP_DEFAULT_SELECTED_FOREGROUND_COLOR,
    PROP_DEFAULT_SELECTED_BACKGROUND_COLOR,
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

  AbstractLogger* logger;
  Configuration* configuration;
  MathMLOperatorDictionary* dictionary;
  Gtk_MathGraphicDevice* math_graphic_device;
#if defined(ENABLE_BOXML)
  Gtk_BoxGraphicDevice* box_graphic_device;
#endif // ENABLE_BOXML
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

  GtkMathViewCursor cursor_visible;
  GtkMathViewModelId cursor_elem;
  gint           cursor_index;

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

/* auxiliary C++ functions */

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
    case PROP_DEFAULT_FOREGROUND_COLOR:
    case PROP_DEFAULT_BACKGROUND_COLOR:
    case PROP_DEFAULT_SELECTED_FOREGROUND_COLOR:
    case PROP_DEFAULT_SELECTED_BACKGROUND_COLOR:
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

  if (math_view->cursor_visible != GTKMATHVIEW_CURSOR_OFF && math_view->cursor_elem != NULL)
    {
      GtkMathViewPoint focus_orig;
      GtkMathViewBoundingBox focus_box;
      if (!GTKMATHVIEW_METHOD_NAME(get_element_extents)(math_view, math_view->cursor_elem, &focus_orig, &focus_box))
	return;

      if (math_view->cursor_visible != GTKMATHVIEW_CURSOR_CARET_ON)
	{
	  gtk_paint_focus(widget->style,
			  widget->window,
			  GTK_STATE_NORMAL,
			  NULL,
			  widget,
			  "?",
			  focus_orig.x,
			  focus_orig.y - focus_box.height,
			  focus_box.width,
			  focus_box.height + focus_box.depth);
	}
 
      if (math_view->cursor_visible != GTKMATHVIEW_CURSOR_FOCUS_ON && math_view->cursor_index >= 0)
	{
	  GdkRectangle crect;
	  GtkMathViewPoint char_orig;
	  GtkMathViewBoundingBox char_box;
	  if (GTKMATHVIEW_METHOD_NAME(get_char_extents)(math_view, math_view->cursor_elem, math_view->cursor_index,
							&char_orig, &char_box))
	    crect.x = char_orig.x;
	  else
	    {
	      crect.x = focus_orig.x;
	      if (math_view->cursor_index > 0) crect.x += focus_box.width;
	    }
	  crect.y = focus_orig.y - focus_box.height;
	  crect.height = focus_box.height + focus_box.depth;
	  gtk_draw_insertion_cursor(widget,
				    widget->window,
				    NULL,
				    &crect,
				    TRUE,
				    GTK_TEXT_DIR_LTR,
				    FALSE);
	}
    }
}

static void
gtk_math_view_update(GtkMathView* math_view)
{
  GtkWidget* widget = GTK_WIDGET(math_view);
  gtk_math_view_update(math_view, 0, 0, widget->allocation.width, widget->allocation.height);
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
  rc->setForegroundColor(math_view_class->configuration->getSelectForeground());
  rc->setBackgroundColor(math_view_class->configuration->getSelectBackground());
  rc->setStyle(Gtk_RenderingContext::NORMAL_STYLE);
  rc->setForegroundColor(math_view_class->configuration->getForeground());
  rc->setBackgroundColor(math_view_class->configuration->getBackground());

  gdk_draw_rectangle(math_view->pixmap, widget->style->white_gc, TRUE, 0, 0, width, height);

  gint x = 0;
  gint y = 0;
  to_view_coords(math_view, &x, &y);
  math_view->view->render(*rc,
			  Gtk_RenderingContext::fromGtkX(x),
			  Gtk_RenderingContext::fromGtkY(y));

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

static SmartPtr<Configuration>
initConfiguration(SmartPtr<AbstractLogger>& logger, const char* confPath)
{
  SmartPtr<Configuration> configuration = Configuration::create();

  bool res = false;
  if (confPath != NULL) res = MathView::loadConfiguration(logger, configuration, confPath);
  if (!res) res = MathView::loadConfiguration(logger, configuration, MathView::getDefaultConfigurationPath());
  if (!res) res = MathView::loadConfiguration(logger, configuration, "config/gtkmathview.conf.xml");
  if (!res)
    {
      logger->out(LOG_ERROR, "could not load configuration file");
      exit(-1);
    }

  return configuration;
}

static SmartPtr<MathMLOperatorDictionary>
initOperatorDictionary(const SmartPtr<AbstractLogger>& logger, const SmartPtr<Configuration> configuration)
{
  SmartPtr<MathMLOperatorDictionary> dictionary = MathMLOperatorDictionary::create();
  if (!configuration->getDictionaries().empty())
    for (std::vector<std::string>::const_iterator dit = configuration->getDictionaries().begin();
	 dit != configuration->getDictionaries().end();
	 dit++)
      {
	logger->out(LOG_DEBUG, "loading dictionary `%s'", (*dit).c_str());
	if (!MathView::loadOperatorDictionary(logger, dictionary, (*dit).c_str()))
	  logger->out(LOG_WARNING, "could not load `%s'", (*dit).c_str());
      }
  else
    {
      const bool res = MathView::loadOperatorDictionary(logger, dictionary, "config/dictionary.xml");
      if (!res) MathView::loadOperatorDictionary(logger, dictionary, MathView::getDefaultOperatorDictionaryPath());
    }

  return dictionary;
}

static void
gtk_math_view_base_class_init(GtkMathViewClass* math_view_class)
{
  g_return_if_fail(math_view_class != NULL);

  SmartPtr<AbstractLogger> logger = Logger::create();
  logger->ref();
  math_view_class->logger = logger;

  SmartPtr<Configuration> configuration = initConfiguration(logger, getenv("GTKMATHVIEWCONF"));
  configuration->ref();
  math_view_class->configuration = configuration;

  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary(logger, configuration);
  dictionary->ref();
  math_view_class->dictionary = dictionary;

  SmartPtr<Gtk_MathGraphicDevice> mathGraphicDevice = Gtk_MathGraphicDevice::create();
  mathGraphicDevice->ref();
  math_view_class->math_graphic_device = mathGraphicDevice;

#if defined(ENABLE_BOXML)
  SmartPtr<Gtk_BoxGraphicDevice> boxGraphicDevice = Gtk_BoxGraphicDevice::create();
  boxGraphicDevice->ref();
  math_view_class->box_graphic_device = boxGraphicDevice;
#endif // ENABLE_BOXML
}

static void
gtk_math_view_base_class_finalize(GtkMathViewClass* math_view_class)
{
  g_return_if_fail(math_view_class != NULL);

#if defined(ENABLE_BOXML)
  if (math_view_class->box_graphic_device)
    {
      math_view_class->box_graphic_device->unref();
      math_view_class->box_graphic_device = 0;
    }
#endif // ENABLE_BOXML

  if (math_view_class->math_graphic_device)
    {
      math_view_class->math_graphic_device->unref();
      math_view_class->math_graphic_device = 0;
    }

  if (math_view_class->dictionary)
    {
      math_view_class->dictionary->unref();
      math_view_class->dictionary = 0;
    }

  if (math_view_class->configuration)
    {
      math_view_class->configuration->unref();
      math_view_class->configuration = 0;
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
		 NULL,NULL,
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
				  PROP_DEFAULT_FOREGROUND_COLOR,
				  g_param_spec_string("foreground_color",
						      "Foreground color",
						      "The default foreground color",
						      NULL,
						      G_PARAM_READWRITE));

  g_object_class_install_property(gobject_class,
				  PROP_DEFAULT_BACKGROUND_COLOR,
				  g_param_spec_string("background_color",
						      "Background color",
						      "The default background color",
						      NULL,
						      G_PARAM_READWRITE));

  g_object_class_install_property(gobject_class,
				  PROP_DEFAULT_SELECT_FOREGROUND_COLOR,
				  g_param_spec_string("select_foreground_color",
						      "Select foreground color",
						      "The default foreground color for selected markup",
						      NULL,
						      G_PARAM_READWRITE));

  g_object_class_install_property(gobject_class,
				  PROP_DEFAULT_SELECT_BACKGROUND_COLOR,
				  g_param_spec_string("select_background_color",
						      "Select background color",
						      "The default background color for selected markup",
						      NULL,
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

  if (math_view->cursor_elem != NULL)
    {
      gdome_el_unref(math_view->cursor_elem, &exc);
      g_assert(exc == 0);
      math_view->cursor_elem = NULL;
    }
}

#else

static void
gtk_math_view_release_document_resources(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);

  math_view->current_elem = NULL;
  math_view->cursor_elem = NULL;
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
  math_view->cursor_elem     = NULL;
  math_view->cursor_index    = 0;
  math_view->cursor_visible  = GTKMATHVIEW_CURSOR_OFF;
  math_view->hadjustment = NULL;
  math_view->vadjustment = NULL;
  math_view->top_x = math_view->top_y = 0;
  math_view->old_top_x = math_view->old_top_y = 0;

#if 1
  SmartPtr<MathView> view = MathView::create();
  view->ref();
  math_view->view = view;

  GtkMathViewClass* math_view_class = GTK_MATH_VIEW_CLASS(G_OBJECT_GET_CLASS(G_OBJECT(math_view)));
  g_assert(math_view_class != NULL);

  view->setLogger(math_view_class->logger);
  view->setOperatorDictionary(math_view_class->dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, math_view_class->math_graphic_device));
#if ENABLE_BOXML
  view->setBoxMLNamespaceContext(BoxMLNamespaceContext::create(view, math_view_class->box_graphic_device));
#endif // ENABLE_BOXML

  math_view->renderingContext = new Gtk_RenderingContext;
  math_view->renderingContext->setColorMap(gtk_widget_get_colormap(GTK_WIDGET(math_view)));
#endif
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

static void
gtk_math_view_realize(GtkWidget* widget)
{
  GtkMathView* math_view;
  GdkWindowAttr attributes;
  gint attributes_mask;

  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_MATH_VIEW (widget));

  GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);
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

#include <iostream>

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

      gtk_math_view_paint(math_view);
    }
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

      gtk_math_view_get_element_at(math_view, (gint) event->x, (gint) event->y, &elem, NULL, NULL);

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
    gtk_math_view_update(math_view, event->area.x, event->area.y, event->area.width, event->area.height);

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
  g_return_val_if_fail(math_view->view != NULL, NULL);
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
  math_view->view->resetRootElement();
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

extern "C" void
GTKMATHVIEW_METHOD_NAME(structure_changed)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view != NULL);
  if (math_view->view->notifyStructureChanged(elem)) gtk_math_view_paint(math_view);
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(attribute_changed)(GtkMathView* math_view, GtkMathViewModelId elem, GtkMathViewModelString name)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->view != NULL);
  if (math_view->view->notifyAttributeChanged(elem, name)) gtk_math_view_paint(math_view);
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(select)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_if_fail(math_view);
  g_return_if_fail(math_view->view);
  g_return_if_fail(elem);

  if (SmartPtr<const Gtk_WrapperArea> area = findGtkWrapperArea(math_view, elem))
    {
      area->setSelected(1);
      gtk_math_view_paint(math_view);
    }
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(unselect)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_if_fail(math_view);
  g_return_if_fail(math_view->view);
  g_return_if_fail(elem);

  if (SmartPtr<const Gtk_WrapperArea> area = findGtkWrapperArea(math_view, elem))
    {
      area->setSelected(0);
      gtk_math_view_paint(math_view);
    }
}

extern "C" gboolean
GTKMATHVIEW_METHOD_NAME(is_selected)(GtkMathView* math_view, GtkMathViewModelId elem)
{
  g_return_val_if_fail(math_view, FALSE);
  g_return_val_if_fail(math_view->view, FALSE);
  g_return_val_if_fail(elem, FALSE);

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
GTKMATHVIEW_METHOD_NAME(get_element_extents)(GtkMathView* math_view, GtkMathViewModelId el,
					     GtkMathViewPoint* result_orig, GtkMathViewBoundingBox* result_box)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(el != NULL, FALSE);

  if (SmartPtr<Element> elem = math_view->view->elementOfModelElement(el))
    {
      Point elemOrig;
      BoundingBox elemBox;
      if (math_view->view->getElementExtents(elem, result_orig ? &elemOrig : 0, result_box ? &elemBox : 0))
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
GTKMATHVIEW_METHOD_NAME(get_char_extents)(GtkMathView* math_view, GtkMathViewModelId el,
					  gint index, GtkMathViewPoint* result_orig, GtkMathViewBoundingBox* result_box)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->view != NULL, FALSE);
  g_return_val_if_fail(el != NULL, FALSE);
  g_return_val_if_fail(index >= 0, FALSE);

  if (SmartPtr<Element> elem = math_view->view->elementOfModelElement(el))
    {
      Point charOrig;
      BoundingBox charBox;
      if (math_view->view->getCharExtents(elem, index, result_orig ? &charOrig : 0, result_box ? &charBox : 0))
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

extern "C" void
GTKMATHVIEW_METHOD_NAME(get_cursor)(GtkMathView* math_view, GtkMathViewModelId* elem, gint* index)
{
  g_return_if_fail(math_view != NULL);
  if (elem != NULL)
    {
      *elem = math_view->cursor_elem;
#if GTKMATHVIEW_USES_GMETADOM
      if (*elem)
	{
	  GdomeException exc = 0;
	  gdome_el_ref(*elem, &exc);
	  g_assert(exc == 0);
	}
#endif // GTKMATHVIEW_USES_GMETADOM
    }

  if (index != NULL) *index = math_view->cursor_index;
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_cursor)(GtkMathView* math_view, GtkMathViewModelId elem, gint index)
{
  g_return_if_fail(math_view != NULL);
  if (math_view->cursor_elem != elem || math_view->cursor_index != index)
    {
      math_view->cursor_index = index;
#if GTKMATHVIEW_USES_GMETADOM
      if (math_view->cursor_elem != NULL)
	{
	  GdomeException exc = 0;
	  gdome_el_unref(math_view->cursor_elem, &exc);
	  g_assert(exc == 0);
	}
#endif // GTKMATHVIEW_USES_GMETADOM
      math_view->cursor_elem = elem;
#if GTKMATHVIEW_USES_GMETADOM
      if (math_view->cursor_elem != NULL)
	{
	  GdomeException exc = 0;
	  gdome_el_ref(math_view->cursor_elem, &exc);
	  g_assert(exc == 0);
	}
#endif // GTKMATHVIEW_USES_GMETADOM
      if (math_view->cursor_visible) gtk_math_view_update(math_view);
    }
}

extern "C" void
GTKMATHVIEW_METHOD_NAME(set_cursor_visible)(GtkMathView* math_view, GtkMathViewCursor visible)
{
  g_return_if_fail(math_view != NULL);
  if (math_view->cursor_visible != visible)
    {
      math_view->cursor_visible = visible;
      if (math_view->cursor_visible) gtk_math_view_update(math_view);
    }
}

extern "C" GtkMathViewCursor
GTKMATHVIEW_METHOD_NAME(get_cursor_visible)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, GTKMATHVIEW_CURSOR_OFF);
  return math_view->cursor_visible;
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
  math_view->view->getLogger()->setLogLevel(LogLevelId(level));
}

extern "C" gint
GTKMATHVIEW_METHOD_NAME(get_log_verbosity)(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, LOG_ERROR);
  g_return_val_if_fail(math_view->view != 0, LOG_ERROR);
  return math_view->view->getLogger()->getLogLevel();
}

