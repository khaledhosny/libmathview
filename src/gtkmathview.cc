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
#include <assert.h>
#include <stdlib.h>

#include <sstream>

#include "defs.h"

#include <math.h>
#include <gtk/gtk.h>
#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkdrawingarea.h>

#include "Globals.hh"
#include "Rectangle.hh"
#include "gtkmathview.h"
#include "traverseAux.hh"
#include "MathMLElement.hh"
#include "PS_DrawingArea.hh"
#include "Gtk_FontManager.hh"
#include "PS_T1_FontManager.hh"
#include "T1_Gtk_DrawingArea.hh"
#include "MathMLActionElement.hh"
#include "MathMLRenderingEngine.hh"

#define CLICK_SPACE_RANGE 1
#define CLICK_TIME_RANGE  250
#define MARGIN            1

enum SelectState 
  {
    SELECT_STATE_NO,
    SELECT_STATE_YES,
    SELECT_STATE_ABORT
  };

/* structures */

struct _GtkMathView {
  GtkEventBox    parent;

  GtkWidget* 	 area;
  GdkPixmap*     pixmap;

  guint 	 hsignal;
  guint 	 vsignal;

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

#if defined(HAVE_GMETADOM)
  GdomeElement*  current_elem;
#endif

  FontManagerId  font_manager_id;

  FontManager*     font_manager;
  Gtk_DrawingArea* drawing_area;
  MathMLRenderingEngine* interface;
};

struct _GtkMathViewClass {
  GtkEventBoxClass parent_class;

  void (*set_scroll_adjustments) (GtkMathView *math_view,
				  GtkAdjustment *hadjustment,
				  GtkAdjustment *vadjustment);

#if defined(HAVE_GMETADOM)
  void (*click)         (GtkMathView*, GdomeElement*, int);
  void (*select_begin)  (GtkMathView*, GdomeElement*, int);
  void (*select_over)   (GtkMathView*, GdomeElement*, int);
  void (*select_end)    (GtkMathView*, GdomeElement*, int);
  void (*select_abort)  (GtkMathView*);
  void (*element_over)  (GtkMathView*, GdomeElement*, int);
#endif
};

/* helper functions */

static void gtk_math_view_class_init(GtkMathViewClass*);
static void gtk_math_view_init(GtkMathView*);

/* GtkObject functions */

static void gtk_math_view_destroy(GtkObject*);

/* GtkWidget functions */

static gboolean gtk_math_view_configure_event(GtkWidget*, GdkEventConfigure*, GtkMathView*);
static gboolean gtk_math_view_expose_event(GtkWidget*, GdkEventExpose*, GtkMathView*);
static gboolean gtk_math_view_button_press_event(GtkWidget*, GdkEventButton*, GtkMathView*);
static gboolean gtk_math_view_button_release_event(GtkWidget*, GdkEventButton*, GtkMathView*);
static gboolean gtk_math_view_motion_notify_event(GtkWidget*, GdkEventMotion*, GtkMathView*);
static void     gtk_math_view_realize(GtkWidget*, GtkMathView*);
static void     gtk_math_view_size_request(GtkWidget*, GtkRequisition*);

/* GtkMathView Signals */

#if defined(HAVE_GMETADOM)
static void gtk_math_view_click(GtkMathView*, GdomeElement*, int);
static void gtk_math_view_select_begin(GtkMathView*, GdomeElement*, int);
static void gtk_math_view_select_over(GtkMathView*, GdomeElement*, int);
static void gtk_math_view_select_end(GtkMathView*, GdomeElement*, int);
static void gtk_math_view_select_abort(GtkMathView*);
static void gtk_math_view_element_over(GtkMathView*, GdomeElement*, int);
#endif

/* auxiliary functions */

static void setup_adjustment(GtkAdjustment*, gfloat, gfloat);
static void setup_adjustments(GtkMathView*);
static void reset_adjustments(GtkMathView*);

/* Local data */

static GtkEventBoxClass* parent_class = NULL;
static guint click_signal = 0;
static guint select_begin_signal = 0;
static guint select_over_signal = 0;
static guint select_end_signal = 0;
static guint select_abort_signal = 0;
static guint element_over_signal = 0;

/* widget implementation */

static void
paint_widget_area(GtkMathView* math_view, gint x, gint y, gint width, gint height)
{
  GtkWidget* widget;

  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->area != NULL);
  g_return_if_fail(math_view->interface != NULL);

  if (!GTK_WIDGET_MAPPED(GTK_WIDGET(math_view)) || math_view->freeze_counter > 0) return;

  widget = math_view->area;

  //printf("sto per cancellare: %d %d %d %d\n", x, y,width, height);
  gdk_draw_rectangle(math_view->pixmap, widget->style->white_gc, TRUE, x, y, width, height);

  Rectangle rect;
  rect.x = px2sp(x + math_view->top_x);
  rect.y = px2sp(y + math_view->top_y);
  rect.width = px2sp(width);
  rect.height = px2sp(height);

  math_view->interface->Render(&rect);
}

static void
paint_widget(GtkMathView* math_view)
{
  GtkWidget* widget;

  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->area != NULL);

  widget = math_view->area;

  setup_adjustments(math_view);
  paint_widget_area(math_view, 0, 0, widget->allocation.width, widget->allocation.height);
}

static void
hadjustment_value_changed(GtkAdjustment* adj, GtkMathView* math_view)
{
  g_return_if_fail(adj != NULL);
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->drawing_area != NULL);

  //  printf("val: %f upper: %f lower: %f ps: %f upper-ps: %f\n", adj->value, adj->upper, adj->lower, adj->page_size, adj->upper - adj->page_size);

  if (adj->value > adj->upper - adj->page_size) adj->value = adj->upper - adj->page_size;
  if (adj->value < adj->lower) adj->value = adj->lower;

  math_view->old_top_x = math_view->top_x;
  math_view->top_x = static_cast<int>(adj->value);
  math_view->drawing_area->SetTopX(px2sp(static_cast<int>(adj->value)));

  if (math_view->old_top_x != math_view->top_x) {
#if 0
    gint change = abs(math_view->old_top_x - math_view->top_x));
    GtkWidget* widget = math_view->area;
    if (change < widget->allocation.width) {
      if (math_view->old_top_x < math_view->top_x) {
	// the window scrolled right
	gdk_draw_pixmap(math_view->pixmap,
			widget->style->white_gc,
			math_view->pixmap,
			change, 0, 0, 0,
			widget->allocation.width - change,
			widget->allocation.height);

	paint_widget_area(math_view,
			  widget->allocation.width - change, 0,
			  change, widget->allocation.height);
      } else {
	// the window scrolled left
	gdk_draw_pixmap(math_view->pixmap,
			widget->style->white_gc,
			math_view->pixmap,
			0, 0, change, 0,
			widget->allocation.width - change,
			widget->allocation.height);

	paint_widget_area(math_view,
			  0, 0,
			  change, widget->allocation.height);
      }
      
      gtk_widget_draw(math_view->area, NULL);
    } else
#endif
      paint_widget(math_view);
  }
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
  math_view->drawing_area->SetTopY(px2sp(static_cast<int>(adj->value)));
  
  if (math_view->old_top_y != math_view->top_y) {
#if 0
    gint change = abs(math_view->old_top_y - math_view->top_y);
    GtkWidget* widget = math_view->area;
    if (change < widget->allocation.height) {
      if (math_view->old_top_y < math_view->top_y) {
	// the window scrolled down
	gdk_draw_pixmap(math_view->pixmap,
			widget->style->white_gc,
			math_view->pixmap,
			0, change, 0, 0,
			widget->allocation.width,
			widget->allocation.height - change);

	paint_widget_area(math_view,
			  0, widget->allocation.height - change,
			  widget->allocation.width, change);
      } else {
	// the window scrolled up
	gdk_draw_pixmap(math_view->pixmap,
			widget->style->white_gc,
			math_view->pixmap,
			0, 0, 0, change,
			widget->allocation.width,
			widget->allocation.height - change);

	paint_widget_area(math_view,
			  0, 0,
			  widget->allocation.width, change);
      }

      gtk_widget_draw(math_view->area, NULL);
    } else
#endif
      paint_widget(math_view);
  }
}

extern "C" GType
gtk_math_view_get_type(void)
{
  static GType math_view_type = 0;

  if (!math_view_type) {
    static const GTypeInfo math_view_info = {
      sizeof(GtkMathViewClass),
      NULL,
      NULL,
      (GClassInitFunc) gtk_math_view_class_init,
      NULL,
      NULL,
      sizeof(GtkMathView),
      0,
      (GInstanceInitFunc) gtk_math_view_init,
    };
    math_view_type = g_type_register_static(GTK_TYPE_EVENT_BOX, "GtkMathView",
		    &math_view_info, (GTypeFlags) 0);
  }

  return math_view_type;
}

static void gtk_math_view_class_init(GtkMathViewClass* klass)
{

	GtkObjectClass* object_class;
	GtkWidgetClass* widget_class;
	
	object_class = (GtkObjectClass*) klass;
	widget_class = (GtkWidgetClass*) klass;
	
	klass->click = gtk_math_view_click;
  	klass->select_begin = gtk_math_view_select_begin;
  	klass->select_over = gtk_math_view_select_over;
  	klass->select_end = gtk_math_view_select_end;
  	klass->select_abort = gtk_math_view_select_abort;
  	klass->element_over = gtk_math_view_element_over;
  	klass->set_scroll_adjustments = gtk_math_view_set_adjustments;

  	parent_class = (GtkEventBoxClass*) gtk_type_class(gtk_event_box_get_type());

  	object_class->destroy = gtk_math_view_destroy;

  	widget_class->size_request = gtk_math_view_size_request;
  	
	widget_class->set_scroll_adjustments_signal = 
		g_signal_new("set_scroll_adjustments",
				G_OBJECT_CLASS_TYPE(object_class),
				G_SIGNAL_RUN_LAST,
				G_STRUCT_OFFSET(GtkMathViewClass,set_scroll_adjustments),
				NULL, NULL,
				gtk_marshal_NONE__POINTER_POINTER,
				G_TYPE_NONE , 2 , GTK_TYPE_ADJUSTMENT,GTK_TYPE_ADJUSTMENT); 
	
	click_signal = 
		g_signal_new("click",
				G_OBJECT_CLASS_TYPE(object_class),
				G_SIGNAL_RUN_FIRST,
				G_STRUCT_OFFSET(GtkMathViewClass, click),
				NULL, NULL,
				gtk_marshal_NONE__POINTER_INT,
				G_TYPE_NONE, 2, GTK_TYPE_POINTER, GTK_TYPE_INT);

	select_begin_signal=
		g_signal_new("select_begin",
				G_OBJECT_CLASS_TYPE(object_class),
				G_SIGNAL_RUN_FIRST,
				G_STRUCT_OFFSET(GtkMathViewClass,select_begin),
				NULL,NULL,
				gtk_marshal_NONE__POINTER_INT,
				G_TYPE_NONE, 2 ,GTK_TYPE_POINTER, GTK_TYPE_INT);
	
	select_over_signal = 
		g_signal_new("select_over",
				G_OBJECT_CLASS_TYPE(object_class),
				G_SIGNAL_RUN_FIRST,
				G_STRUCT_OFFSET(GtkMathViewClass,select_over),
				NULL,NULL,
				gtk_marshal_NONE__POINTER_INT,
				G_TYPE_NONE, 2 ,GTK_TYPE_POINTER,GTK_TYPE_INT);
	
	select_end_signal = 
		g_signal_new("select_end",
				G_OBJECT_CLASS_TYPE(object_class),
				G_SIGNAL_RUN_FIRST,
				G_STRUCT_OFFSET(GtkMathViewClass,select_end),
				NULL,NULL,
				gtk_marshal_NONE__POINTER_INT,
				G_TYPE_NONE, 2 ,GTK_TYPE_POINTER, GTK_TYPE_INT);
	
	select_abort_signal = 
		g_signal_new("select_abort",
				G_OBJECT_CLASS_TYPE(object_class),
				G_SIGNAL_RUN_FIRST,
				G_STRUCT_OFFSET(GtkMathViewClass,select_abort),
				NULL,NULL,
				gtk_marshal_NONE__NONE,
				G_TYPE_NONE,0);
	
	element_over_signal = 
		g_signal_new("element_over",
				G_OBJECT_CLASS_TYPE(object_class),
				G_SIGNAL_RUN_FIRST,
				G_STRUCT_OFFSET(GtkMathViewClass,element_over),
				NULL,NULL,
				gtk_marshal_NONE__POINTER_INT,
				G_TYPE_NONE, 2 , GTK_TYPE_POINTER,GTK_TYPE_INT);
	
	Globals::InitGlobalData(getenv("MATHENGINECONF"));
}

static void
gtk_math_view_init(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);

  math_view->pixmap          = NULL;
  math_view->font_manager_id = FONT_MANAGER_UNKNOWN;
  math_view->font_manager    = NULL;
  math_view->drawing_area    = NULL;
  math_view->interface       = NULL;
  math_view->freeze_counter  = 0;
  math_view->select_state    = SELECT_STATE_NO;
  math_view->button_pressed  = FALSE;
  math_view->current_elem    = NULL;
  math_view->hadjustment = NULL;
  math_view->vadjustment = NULL;

  math_view->area = gtk_drawing_area_new();
  GTK_WIDGET_SET_FLAGS(GTK_WIDGET(math_view->area), GTK_CAN_FOCUS);
  gtk_container_add(GTK_CONTAINER(math_view), math_view->area);
  gtk_widget_show(math_view->area);

  g_signal_connect(GTK_OBJECT(math_view->area),
		  "configure_event",
		  G_CALLBACK(gtk_math_view_configure_event), 
		  math_view);

  g_signal_connect(GTK_OBJECT(math_view->area), 
		  "expose_event",
		  G_CALLBACK(gtk_math_view_expose_event), 
		  math_view);

  g_signal_connect(GTK_OBJECT(math_view->area), 
		  "realize",
		  G_CALLBACK(gtk_math_view_realize), 
		  math_view);

  g_signal_connect(GTK_OBJECT(math_view->area), 
		  "button_press_event",
		  G_CALLBACK(gtk_math_view_button_press_event), 
		  math_view);

  g_signal_connect(GTK_OBJECT(math_view->area), 
		  "button_release_event",
		  G_CALLBACK(gtk_math_view_button_release_event), 
		  math_view);

  g_signal_connect(GTK_OBJECT(math_view->area), 
		  "motion_notify_event",
		  G_CALLBACK(gtk_math_view_motion_notify_event), 
		  math_view);

  gtk_widget_add_events(GTK_WIDGET(math_view->area),
			GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
}

extern "C" GtkWidget*
gtk_math_view_new(GtkAdjustment*, GtkAdjustment*)
{
  GtkMathView* math_view = (GtkMathView*) gtk_type_new(gtk_math_view_get_type());
  
  g_return_val_if_fail(math_view != NULL, NULL);

  math_view->top_x = math_view->top_y = 0;
  math_view->old_top_x = math_view->old_top_y = 0;
  math_view->interface = new MathMLRenderingEngine();

  gtk_math_view_set_font_manager_type(math_view, FONT_MANAGER_GTK);

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

  Globals::logger(LOG_DEBUG, "destroying the widget");

  delete math_view->interface;
  math_view->interface = 0;
  delete math_view->font_manager;
  math_view->font_manager = 0;
  delete math_view->drawing_area;
  math_view->drawing_area = 0;

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

  if (math_view->current_elem != NULL)
    {
      GdomeException exc = 0;
      gdome_el_unref(math_view->current_elem, &exc);
      g_assert(exc == 0);
      math_view->current_elem = NULL;
    }

  /* ATTEMPT: since this class is derived from a container
   * then contained object will be destroyed by the parent class'
   * method
   */

  if (GTK_OBJECT_CLASS(parent_class)->destroy != NULL)
    (*GTK_OBJECT_CLASS(parent_class)->destroy)(object);
}


extern "C" gboolean
gtk_math_view_freeze(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  return (math_view->freeze_counter++ > 0);
}

extern "C" gboolean
gtk_math_view_thaw(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->freeze_counter > 0, FALSE);
  if (--math_view->freeze_counter == 0)
    {
      paint_widget(math_view);
      return TRUE;
    }
  else
    return FALSE;
}

static void
gtk_math_view_realize(GtkWidget* widget, GtkMathView* math_view)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->drawing_area != NULL);

  math_view->drawing_area->Realize();
}

static void
gtk_math_view_size_request(GtkWidget* widget, GtkRequisition* requisition)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(requisition != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(widget));

  GtkMathView* math_view = GTK_MATH_VIEW(widget);
  g_assert(math_view != NULL);
  g_assert(math_view->interface != NULL);

  BoundingBox box;
  math_view->interface->GetDocumentBoundingBox(box);

  // the 10 is for the border, the frame thickness is missing. How can I get it?
  requisition->width = sp2ipx(box.horizontalExtent()) + 10;
  requisition->height = sp2ipx(box.verticalExtent()) + 10;
}

static gint
gtk_math_view_button_press_event(GtkWidget* widget,
				 GdkEventButton* event,
				 GtkMathView* math_view)
{
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);
  g_return_val_if_fail(math_view->drawing_area != NULL, FALSE);

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
      g_signal_emit(GTK_OBJECT(math_view), select_abort_signal,0);
    }
  
  return FALSE;
}

static gint
gtk_math_view_button_release_event(GtkWidget* widget,
				   GdkEventButton* event,
				   GtkMathView* math_view)
{
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);
  g_return_val_if_fail(math_view->drawing_area != NULL, FALSE);

  if (event->button == 1)
    {
      GdomeException exc = 0;
      GdomeElement* elem = gtk_math_view_get_element_at(math_view, (gint) event->x, (gint) event->y);

      if (math_view->button_pressed == TRUE &&
	  math_view->select_state == SELECT_STATE_NO &&
	  fabs(math_view->button_press_x - event->x) <= CLICK_SPACE_RANGE &&
	  fabs(math_view->button_press_y - event->y) <= CLICK_SPACE_RANGE &&
	  abs(math_view->button_press_time - event->time) <= CLICK_TIME_RANGE)
	{
	  // the mouse should have not moved more than one pixel in each direction
	  // and the time elapsed from the press event should be no more than 250ms
	  g_signal_emit(GTK_OBJECT(math_view),
			  click_signal,
			  0,
			  elem,
			  event->state);
	}

      if (math_view->select_state == SELECT_STATE_YES)      
	      g_signal_emit(GTK_OBJECT(math_view),
			      select_end_signal,
			      0,
			      elem,
			      event->state);

      if (elem != NULL)
	{
	  gdome_el_unref(elem, &exc);
	  g_assert(exc == 0);
	}

      math_view->button_pressed = FALSE;
      math_view->select_state = SELECT_STATE_NO;
    }
  
  return FALSE;
}

static gint
gtk_math_view_motion_notify_event(GtkWidget* widget,
				  GdkEventMotion* event,
				  GtkMathView* math_view)
{
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);
  g_return_val_if_fail(math_view->drawing_area != NULL, FALSE);

  if (event->x < 0) {
    math_view->hadjustment->value -= math_view->hadjustment->step_increment;
    gtk_adjustment_value_changed(math_view->hadjustment);
  } else if (event->x > widget->allocation.width) {
    math_view->hadjustment->value += math_view->hadjustment->step_increment;
    gtk_adjustment_value_changed(math_view->hadjustment);
  }

  if (event->y < 0) {
    math_view->vadjustment->value -= math_view->vadjustment->step_increment;
    gtk_adjustment_value_changed(math_view->vadjustment);
  } else if (event->y > widget->allocation.height) {
    math_view->vadjustment->value += math_view->vadjustment->step_increment;
    gtk_adjustment_value_changed(math_view->vadjustment);
  }

  GdomeException exc = 0;
  GdomeElement* elem = gtk_math_view_get_element_at(math_view, (gint) event->x, (gint) event->y);

  if (math_view->button_pressed == TRUE &&
      (math_view->select_state == SELECT_STATE_YES ||
       fabs(math_view->button_press_x - event->x) > CLICK_SPACE_RANGE ||
       fabs(math_view->button_press_y - event->y) > CLICK_SPACE_RANGE ||
       abs(math_view->button_press_time - event->time) > CLICK_TIME_RANGE))
    {
      if (math_view->select_state == SELECT_STATE_NO)
	{
		g_signal_emit(GTK_OBJECT(math_view),
				select_begin_signal,
				0,
				elem,
				event->state);
	  
	  math_view->select_state = SELECT_STATE_YES;
	}
      else if (math_view->select_state == SELECT_STATE_YES && math_view->current_elem != elem)
	      g_signal_emit(GTK_OBJECT(math_view),
			      select_over_signal,
			      0,
			      elem,
			      event->state);
    }

  if (math_view->current_elem != elem)
    {
      if (math_view->current_elem != NULL)
	{
	  gdome_el_unref(math_view->current_elem, &exc);
	  g_assert(exc == 0);
	}

      if (elem != NULL)
	{
	  gdome_el_ref(elem, &exc);
	  g_assert(exc == 0);
	}

      math_view->current_elem = elem;
      g_signal_emit(GTK_OBJECT(math_view), 
		      element_over_signal, 
		      0, elem,
		      event->state);
    }

  if (elem != NULL)
    {
      gdome_el_unref(elem, &exc);
      g_assert(exc == 0);
    }

  return FALSE;
}

static gint
gtk_math_view_configure_event(GtkWidget* widget,
			      GdkEventConfigure* event,
			      GtkMathView* math_view)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);
  g_return_val_if_fail(math_view->drawing_area != NULL, FALSE);

  if (math_view->pixmap != NULL) g_object_unref(math_view->pixmap);
  math_view->pixmap = gdk_pixmap_new(widget->window, event->width, event->height, -1);
  math_view->drawing_area->SetSize(px2sp(event->width), px2sp(event->height));
  math_view->drawing_area->SetPixmap(math_view->pixmap);
  paint_widget(math_view);

  return TRUE;
}

static gint
gtk_math_view_expose_event(GtkWidget* widget,
			   GdkEventExpose* event,
			   GtkMathView* math_view)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(math_view != NULL, FALSE);

  //printf("widget expose %f %f %f %f\n", (double) event->area.x, (double) event->area.y, (double) event->area.width, (double) event->area.height);

  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		  math_view->pixmap,
		  event->area.x, event->area.y,
		  event->area.x, event->area.y,
		  event->area.width, event->area.height);

  return FALSE;
}

static void
gtk_math_view_click(GtkMathView* math_view, GdomeElement*, gint)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_select_begin(GtkMathView* math_view, GdomeElement*, gint)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_select_over(GtkMathView* math_view, GdomeElement*, gint)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_select_end(GtkMathView* math_view, GdomeElement*, gint)
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
gtk_math_view_element_over(GtkMathView* math_view, GdomeElement*, gint)
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
reset_adjustments(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);

  math_view->old_top_x = math_view->old_top_y = math_view->top_x = math_view->top_y = 0;

  if (math_view->hadjustment != NULL)
    gtk_adjustment_set_value(math_view->hadjustment, 0.0);

  if (math_view->vadjustment != NULL)
    gtk_adjustment_set_value(math_view->vadjustment, 0.0);
}

static void
setup_adjustments(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->area != NULL);
  g_return_if_fail(math_view->interface != NULL);

  BoundingBox box;
  math_view->interface->GetDocumentBoundingBox(box);

  if (math_view->hadjustment != NULL) {
    gint width = sp2ipx(box.width) + 2 * MARGIN;
    gint page_width = sp2ipx(math_view->drawing_area->GetWidth());
    
    if (math_view->top_x > width - page_width)
      math_view->top_x = std::max(0, width - page_width);

    setup_adjustment(math_view->hadjustment, width, page_width);
  }

  if (math_view->vadjustment != NULL) {
    gint height = sp2ipx(box.verticalExtent()) + 2 * MARGIN;
    gint page_height = sp2ipx(math_view->drawing_area->GetHeight());

    if (math_view->top_y > height - page_height)
      math_view->old_top_y = math_view->top_y = std::max(0, height - page_height);

    setup_adjustment(math_view->vadjustment, height, page_height);
  }
}

extern "C" gboolean
gtk_math_view_load_uri(GtkMathView* math_view, const gchar* name)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(name != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  bool res = math_view->interface->Load(name);
  if (!res) return FALSE;

  reset_adjustments(math_view);
  paint_widget(math_view);

  return TRUE;
}

extern "C" gboolean
gtk_math_view_load_doc(GtkMathView* math_view, GdomeDocument* doc)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(doc != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  bool res = math_view->interface->Load(DOM::Document(doc));
  if (!res) return FALSE;

  reset_adjustments(math_view);
  paint_widget(math_view);

  return TRUE;
}

#if 0
extern "C" gboolean
gtk_math_view_load_tree(GtkMathView* math_view, GdomeElement* elem)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(elem != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  bool res = math_view->interface->Load(DOM::Element(elem));
  if (!res) return FALSE;

  reset_adjustments(math_view);
  paint_widget(math_view);

  return TRUE;
}
#endif

extern "C" void
gtk_math_view_unload(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);

  math_view->interface->Unload();
  reset_adjustments(math_view);
  paint_widget(math_view);
}

extern "C" GdkPixmap*
gtk_math_view_get_buffer(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);

  return math_view->pixmap;
}

extern "C" void
gtk_math_view_set_adjustments(GtkMathView* math_view,
			      GtkAdjustment* hadj,
			      GtkAdjustment* vadj)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(GTK_IS_MATH_VIEW(math_view));

  if (hadj != NULL)
    g_return_if_fail(GTK_IS_ADJUSTMENT(hadj));
  else
    hadj = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0));

  if (vadj != NULL)
    g_return_if_fail(GTK_IS_ADJUSTMENT(vadj));
  else
    vadj = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0));

  if (math_view->hadjustment != NULL && (math_view->hadjustment != hadj)) {
    gtk_signal_disconnect_by_data(GTK_OBJECT(math_view->hadjustment), math_view);
    gtk_object_unref(GTK_OBJECT(math_view->hadjustment));
  }

  if (math_view->vadjustment != NULL && (math_view->vadjustment != vadj)) {
    gtk_signal_disconnect_by_data(GTK_OBJECT(math_view->vadjustment), math_view);
    gtk_object_unref(GTK_OBJECT(math_view->vadjustment));
  }

  if (math_view->hadjustment != hadj) {
    math_view->hadjustment = hadj;
    gtk_object_ref(GTK_OBJECT(math_view->hadjustment));
    gtk_object_sink(GTK_OBJECT(math_view->hadjustment));
    
    math_view->hsignal = 
	    g_signal_connect(GTK_OBJECT(hadj), 
			    "value_changed",
			    G_CALLBACK(hadjustment_value_changed),
			    math_view);
  }

  if (math_view->vadjustment != vadj) {
    math_view->vadjustment = vadj;
    gtk_object_ref(GTK_OBJECT(math_view->vadjustment));
    gtk_object_sink(GTK_OBJECT(math_view->vadjustment));
    
    math_view->vsignal =
	    g_signal_connect(GTK_OBJECT(vadj), 
			    "value_changed",
			    G_CALLBACK(vadjustment_value_changed),
			    math_view);
  }

  setup_adjustments(math_view);
}

extern "C" GtkAdjustment*
gtk_math_view_get_hadjustment(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);

  return math_view->hadjustment;
}

extern "C" GtkAdjustment*
gtk_math_view_get_vadjustment(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);

  return math_view->vadjustment;
}

extern "C" GtkDrawingArea*
gtk_math_view_get_drawing_area(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);
  return math_view->area != NULL ? GTK_DRAWING_AREA(math_view->area) : NULL;
}

extern "C" void
gtk_math_view_set_font_size(GtkMathView* math_view, guint size)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->area != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(size > 0);

  if (size == math_view->interface->GetDefaultFontSize()) return;

  math_view->interface->SetDefaultFontSize(size);
  paint_widget(math_view);
}

extern "C" guint
gtk_math_view_get_font_size(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, 0);
  g_return_val_if_fail(math_view->interface != NULL, 0);

  return math_view->interface->GetDefaultFontSize();
}

extern "C" void
gtk_math_view_select(GtkMathView* math_view, GdomeElement* elem)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(elem != NULL);

  if (Ptr<MathMLElement> el = findMathMLElement(math_view->interface->GetDocument(), DOM::Element(elem)))
    {
      el->SetSelected();
      paint_widget(math_view);
    }
}

extern "C" void
gtk_math_view_unselect(GtkMathView* math_view, GdomeElement* elem)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(elem != NULL);

  if (Ptr<MathMLElement> el = findMathMLElement(math_view->interface->GetDocument(), DOM::Element(elem)))
    {
      el->ResetSelected();
      paint_widget(math_view);
    }
}

extern "C" gboolean
gtk_math_view_is_selected(GtkMathView* math_view, GdomeElement* elem)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);
  g_return_val_if_fail(elem != NULL, FALSE);

  Ptr<MathMLElement> el = findMathMLElement(math_view->interface->GetDocument(),
					    DOM::Element(elem));
  if (!el) return FALSE;

  return el->Selected() ? TRUE : FALSE;
}

extern "C" gint
gtk_math_view_get_width(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->area != NULL, FALSE);

  return math_view->area->allocation.width;
}

extern "C" gint
gtk_math_view_get_height(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->area != NULL, FALSE);

  return math_view->area->allocation.height;
}

extern "C" void
gtk_math_view_set_anti_aliasing(GtkMathView* math_view, gboolean anti_aliasing)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  
  math_view->interface->SetAntiAliasing(anti_aliasing != FALSE);
  paint_widget(math_view);
}

extern "C" gboolean
gtk_math_view_get_anti_aliasing(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  return math_view->interface->GetAntiAliasing() ? TRUE : FALSE;
}

extern "C" void
gtk_math_view_set_transparency(GtkMathView* math_view, gboolean transparency)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  
  math_view->interface->SetTransparency(transparency != FALSE);
  paint_widget(math_view);
}

extern "C" gboolean
gtk_math_view_get_transparency(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  return math_view->interface->GetTransparency() ? TRUE : FALSE;
}

extern "C" GdomeElement*
gtk_math_view_get_element_at(GtkMathView* math_view, gint x, gint y)
{
  g_return_val_if_fail(math_view != NULL, NULL);
  g_return_val_if_fail(math_view->interface != NULL, NULL);

  gint x0 = (math_view->vadjustment != NULL) ? static_cast<int>(math_view->hadjustment->value) : 0;
  gint y0 = (math_view->hadjustment != NULL) ? static_cast<int>(math_view->vadjustment->value) : 0;

  Ptr<MathMLElement> at = math_view->interface->GetElementAt(px2sp(x0 + x), px2sp(y0 + y));
  return gdome_cast_el(findDOMNode(at).gdome_object());
}

extern "C" gboolean
gtk_math_view_get_element_coords(GtkMathView* math_view, GdomeElement* elem, gint* x, gint* y)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);
  g_return_val_if_fail(elem != NULL, FALSE);

  Ptr<MathMLElement> el = findMathMLElement(math_view->interface->GetDocument(),
					    DOM::Element(elem));
  if (!el) return FALSE;

  if (x != NULL) *x = static_cast<gint>(sp2px(el->GetX()));
  if (y != NULL) *y = static_cast<gint>(sp2px(el->GetY()));

  return TRUE;
}

extern "C" gboolean
gtk_math_view_get_element_rectangle(GtkMathView* math_view, GdomeElement* elem, GdkRectangle* rect)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);
  g_return_val_if_fail(elem != NULL, FALSE);
  g_return_val_if_fail(rect != NULL, FALSE);

  Ptr<MathMLElement> el = findMathMLElement(math_view->interface->GetDocument(),
					    DOM::Element(elem));
  if (!el) return FALSE;

  const BoundingBox& box = el->GetBoundingBox();
  rect->x = sp2ipx(el->GetX());
  rect->y = sp2ipx(el->GetY() - box.height);
  rect->width = sp2ipx(box.horizontalExtent());
  rect->height = sp2ipx(box.verticalExtent());

  return TRUE;
}

extern "C" void
gtk_math_view_get_top(GtkMathView* math_view, gint* x, gint* y)
{
  g_return_if_fail(math_view != NULL);
  if (x != NULL) *x = math_view->vadjustment ? sp2ipx(math_view->hadjustment->value) : 0;
  if (y != NULL) *y = math_view->hadjustment ? sp2ipx(math_view->vadjustment->value) : 0;
}

extern "C" void
gtk_math_view_set_top(GtkMathView* math_view, gint x, gint y)
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
gtk_math_view_set_log_verbosity(GtkMathView*, gint level)
{
  Globals::SetVerbosity(level);
}

extern "C" gint
gtk_math_view_get_log_verbosity(GtkMathView*)
{
  return Globals::GetVerbosity();
}

extern "C" void
gtk_math_view_set_font_manager_type(GtkMathView* math_view, FontManagerId id)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(id != FONT_MANAGER_UNKNOWN);

  if (id == math_view->font_manager_id) return;

  Ptr<MathMLDocument> document = math_view->interface->GetDocument();
  if (document) document->ReleaseGCs();

  delete math_view->font_manager;
  delete math_view->drawing_area;

  math_view->font_manager = NULL;
  math_view->drawing_area = NULL;

  math_view->font_manager_id = id;

  GraphicsContextValues values;
  values.foreground = Globals::configuration.GetForeground();
  values.background = Globals::configuration.GetBackground();
  values.lineStyle  = LINE_STYLE_SOLID;
  values.lineWidth  = px2sp(1);

  switch (id) {
  case FONT_MANAGER_T1:
#ifdef HAVE_LIBT1
    math_view->font_manager = new PS_T1_FontManager;
    math_view->drawing_area = new T1_Gtk_DrawingArea(values, px2sp(MARGIN), px2sp(MARGIN),
						     GTK_WIDGET(math_view->area),
						     Globals::configuration.GetSelectForeground(),
						     Globals::configuration.GetSelectBackground());
    math_view->drawing_area->SetPixmap(math_view->pixmap);
    break;
#else
    Globals::logger(LOG_WARNING, "the widget was compiled without support for T1 fonts, falling back to GTK fonts");
#endif // HAVE_LIBT1
  case FONT_MANAGER_GTK:
    math_view->font_manager = new Gtk_FontManager;
    math_view->drawing_area = new Gtk_DrawingArea(values, px2sp(MARGIN), px2sp(MARGIN),
						  GTK_WIDGET(math_view->area),
						  Globals::configuration.GetSelectForeground(),
						  Globals::configuration.GetSelectBackground());
    math_view->drawing_area->SetPixmap(math_view->pixmap);
    break;
  default:
    Globals::logger(LOG_ERROR, "could not switch to font manager type %d", id);
    break;
  }

  math_view->interface->Init(math_view->drawing_area, math_view->font_manager);
  if (GTK_WIDGET_REALIZED(GTK_WIDGET(math_view))) math_view->drawing_area->Realize();
  paint_widget(math_view);
}

extern "C" FontManagerId
gtk_math_view_get_font_manager_type(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FONT_MANAGER_UNKNOWN);
  return math_view->font_manager_id;
}

extern "C" void
gtk_math_view_export_to_postscript(GtkMathView* math_view,
				   gint w, gint h,
				   gint x0, gint y0,
				   gboolean disable_colors,
				   FILE* f)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(math_view->drawing_area != NULL);

  if (math_view->font_manager_id != FONT_MANAGER_T1) {
    Globals::logger(LOG_ERROR, "cannot export to PostScript if the Type1 Font Manager is not available");
    return;
  }

#ifdef HAVE_LIBT1
  PS_T1_FontManager* fm = TO_PS_T1_FONT_MANAGER(math_view->font_manager);
  g_assert(fm != NULL);

  PS_DrawingArea area(math_view->drawing_area->GetDefaultGraphicsContextValues(),
		      px2sp(x0), px2sp(y0), f);
  area.SetSize(px2sp(w), px2sp(h));
  if (disable_colors) area.DisableColors();

  if (Ptr<MathMLDocument> document = math_view->interface->GetDocument())
    {
      // the following invocations are needed just to mark the chars actually used :(
      fm->ResetUsedChars();
      area.SetOutputFile(NULL);
      document->SetDirty();
      document->Render(area);
      area.SetOutputFile(f);

      Rectangle rect;
      math_view->interface->GetDocumentRectangle(rect);
      area.DumpHeader(PACKAGE, "(no title)", rect);
      fm->DumpFontDictionary(f);
      area.DumpPreamble();
      document->SetDirty();
      document->Render(area);
      area.DumpEpilogue();
    }
#else
  g_assert_not_reached();
#endif // HAVE_LIBT1
}

#if 0
extern "C" guint
gtk_math_view_action_get_selected(GtkMathView* math_view, GdomeElement* elem)
{
  g_return_val_if_fail(math_view != NULL, 0);
  g_return_val_if_fail(math_view->interface != NULL, 0);
  g_return_val_if_fail(elem != NULL, 0);

  DOM::Element el(elem);
  if (el.get_namespaceURI() != MATHML_NS_URI || el.get_localName() != "maction") return 0;

  Ptr<MathMLDocument> doc = math_view->interface->GetDocument();
  assert(doc);

  Ptr<MathMLElement> e = doc->getFormattingNodeNoCreate(DOM::Element(elem));
  if (!e) return 0;

  Ptr<MathMLActionElement> action = smart_cast<MathMLActionElement>(e);
  assert(action);

  return action->GetSelectedIndex();
}

extern "C" void
gtk_math_view_action_set_selected(GtkMathView* math_view, GdomeElement* elem, guint idx)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(elem != NULL);

  DOM::Element el(elem);
  if (el.get_namespaceURI() != MATHML_NS_URI || el.get_localName() != "maction") return;

  Ptr<MathMLDocument> doc = math_view->interface->GetDocument();
  assert(doc);

  Ptr<MathMLElement> e = doc->getFormattingNodeNoCreate(DOM::Element(elem));
  if (!e) return;

  Ptr<MathMLActionElement> action = smart_cast<MathMLActionElement>(e);
  assert(action);

  return action->SetSelectedIndex(idx);
}

extern "C" void
gtk_math_view_action_toggle(GtkMathView* math_view, GdomeElement* elem)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(elem != NULL);
  gtk_math_view_action_set_selected(math_view, elem, gtk_math_view_action_get_selected(math_view, elem) + 1);
}
#endif
