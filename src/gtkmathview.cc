/* (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <config.h>
#include <assert.h>
#include <stdlib.h>

#include "defs.h"

#include <gdk/gdkkeysyms.h>

#include <gtk/gtk.h>
#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkdrawingarea.h>

#include "Rectangle.hh"
#include "gtkmathview.h"
#include "MathEngine.hh"
#include "traverseAux.hh"
#include "MathMLElement.hh"
#include "PS_DrawingArea.hh"
#include "Gtk_FontManager.hh"
#include "PS_T1_FontManager.hh"
#include "T1_Gtk_DrawingArea.hh"
#include "MathMLActionElement.hh"

/* structures */

struct _GtkMathView {
  GtkEventBox    parent;

  GtkWidget* 	 frame;
  GtkWidget* 	 area;
  GdkPixmap*     pixmap;

  guint 	 hsignal;
  guint 	 vsignal;

  GtkAdjustment* hadjustment;
  GtkAdjustment* vadjustment;

  gfloat     	 top_x;
  gfloat     	 top_y;

  gfloat 	 old_top_x;
  gfloat 	 old_top_y;
  gint           old_width;

  gboolean 	 button_press;
  gboolean 	 select;
  MathMLElement* selected_first; /* first element clicked in a selection */

  mDOMNodeRef    selected_root;  /* root of the minimal subtree selected */
  mDOMNodeRef    node;           /* element the pointer is on */
  mDOMNodeRef    action_node;    /* action element the pointer is on */

  FontManagerId  font_manager_id;

  FontManager*     font_manager;
  Gtk_DrawingArea* drawing_area;
  MathEngine*      interface;
};

struct _GtkMathViewClass {
  GtkEventBoxClass parent_class;

  void (*set_scroll_adjustments) (GtkMathView *math_view,
				  GtkAdjustment *hadjustment,
				  GtkAdjustment *vadjustment);
  
  void (*clicked)           (GtkMathView*, mDOMNodeRef);
  void (*selection_changed) (GtkMathView*, mDOMNodeRef);
  void (*element_changed)   (GtkMathView*, mDOMNodeRef);
  void (*action_changed)    (GtkMathView*, mDOMNodeRef);
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
static gboolean gtk_math_view_key_press_event(GtkWidget*, GdkEventKey*, GtkMathView*);
static void     gtk_math_view_realize(GtkWidget*, GtkMathView*);

/* GtkMathView signals */

static void gtk_math_view_clicked(GtkMathView*, mDOMNodeRef);
static void gtk_math_view_selection_changed(GtkMathView*, mDOMNodeRef);
static void gtk_math_view_element_changed(GtkMathView*, mDOMNodeRef);

/* auxiliary functions */

static void setup_adjustment(GtkAdjustment*, gfloat, gfloat);
static void setup_adjustments(GtkMathView*);
static void reset_adjustments(GtkMathView*);

/* Local data */

static GtkEventBoxClass* parent_class = NULL;
static guint clicked_signal = 0;
static guint selection_changed_signal = 0;
static guint element_changed_signal = 0;
static guint action_changed_signal = 0;

/* widget implementation */

static void
paint_widget_area(GtkMathView* math_view, gint x, gint y, gint width, gint height)
{
  GtkWidget* widget;

  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->area != NULL);
  g_return_if_fail(math_view->interface != NULL);

  widget = math_view->area;

  gdk_draw_rectangle(math_view->pixmap, widget->style->white_gc, TRUE, x, y, width, height);

  Rectangle rect;
  rect.x = px2sp(x) + float2sp(math_view->top_x);
  rect.y = px2sp(y) + float2sp(math_view->top_y);
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

  paint_widget_area(math_view, 0, 0, widget->allocation.width, widget->allocation.height);
}

static void
hadjustment_value_changed(GtkAdjustment* adj, GtkMathView* math_view)
{
  g_return_if_fail(adj != NULL);
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->drawing_area != NULL);

  if (adj->value > adj->upper - adj->page_size) adj->value = adj->upper - adj->page_size;
  if (adj->value < adj->lower) adj->value = adj->lower;

  math_view->old_top_x = math_view->top_x;
  math_view->top_x = adj->value;
  math_view->drawing_area->SetTopX(float2sp(adj->value));

  if (math_view->old_top_x != math_view->top_x) {
#if 0
    gint change = sp2ipx(float2sp(fabs(math_view->old_top_x - math_view->top_x)));
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
  math_view->top_y = adj->value;
  math_view->drawing_area->SetTopY(float2sp(adj->value));
  
  if (math_view->old_top_y != math_view->top_y) {
#if 0
    gint change = sp2ipx(float2sp(fabs(math_view->old_top_y - math_view->top_y)));
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

extern "C" GtkType
gtk_math_view_get_type()
{
  static guint math_view_type = 0;

  if (math_view_type == 0) {
    GtkTypeInfo math_view_info = {
      "GtkMathView",
      sizeof(GtkMathView),
      sizeof(GtkMathViewClass),
      (GtkClassInitFunc) gtk_math_view_class_init,
      (GtkObjectInitFunc) gtk_math_view_init,
      NULL,
      NULL,
      NULL
    };

    math_view_type = gtk_type_unique(gtk_event_box_get_type(), &math_view_info);
  }

  return math_view_type;
}

static void
gtk_math_view_class_init(GtkMathViewClass* klass)
{
  GtkObjectClass* object_class = (GtkObjectClass*) klass;
  GtkWidgetClass* widget_class = (GtkWidgetClass*) klass;

  klass->clicked            = gtk_math_view_clicked;
  klass->selection_changed  = gtk_math_view_selection_changed;
  klass->element_changed    = gtk_math_view_element_changed;

  klass->set_scroll_adjustments = gtk_math_view_set_adjustments;

  parent_class = (GtkEventBoxClass*) gtk_type_class(gtk_event_box_get_type());

  object_class->destroy = gtk_math_view_destroy;

  widget_class->set_scroll_adjustments_signal =
    gtk_signal_new("set_scroll_adjustments",
		   GTK_RUN_LAST,
		   object_class->type,
		   GTK_SIGNAL_OFFSET (GtkMathViewClass, set_scroll_adjustments),
		   gtk_marshal_NONE__POINTER_POINTER,
		   GTK_TYPE_NONE, 2, GTK_TYPE_ADJUSTMENT, GTK_TYPE_ADJUSTMENT);

  clicked_signal =
    gtk_signal_new("clicked",
		   GTK_RUN_FIRST,
		   object_class->type,
		   GTK_SIGNAL_OFFSET (GtkMathViewClass, clicked),
		   gtk_marshal_NONE__POINTER,
		   GTK_TYPE_NONE, 1, GTK_TYPE_POINTER);

  gtk_object_class_add_signals(object_class, &clicked_signal, 1);

  selection_changed_signal =
    gtk_signal_new("selection_changed",
		   GTK_RUN_FIRST,
		   object_class->type,
		   GTK_SIGNAL_OFFSET (GtkMathViewClass, selection_changed),
		   gtk_marshal_NONE__POINTER,
		   GTK_TYPE_NONE, 1, GTK_TYPE_POINTER);

  gtk_object_class_add_signals(object_class, &selection_changed_signal, 1);

  element_changed_signal =
    gtk_signal_new("element_changed",
		   GTK_RUN_FIRST,
		   object_class->type,
		   GTK_SIGNAL_OFFSET (GtkMathViewClass, element_changed),
		   gtk_marshal_NONE__POINTER,
		   GTK_TYPE_NONE, 1, GTK_TYPE_POINTER);

  gtk_object_class_add_signals(object_class, &element_changed_signal, 1);

  action_changed_signal =
    gtk_signal_new("action_changed",
		   GTK_RUN_FIRST,
		   object_class->type,
		   GTK_SIGNAL_OFFSET (GtkMathViewClass, action_changed),
		   gtk_marshal_NONE__POINTER,
		   GTK_TYPE_NONE, 1, GTK_TYPE_POINTER);

  gtk_object_class_add_signals(object_class, &action_changed_signal, 1);

  MathEngine::InitGlobalData(getenv("MATHENGINECONF"));
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
  math_view->select          = FALSE;
  math_view->button_press    = FALSE;

  math_view->frame = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(math_view->frame), GTK_SHADOW_IN);
  gtk_container_add(GTK_CONTAINER(math_view), math_view->frame);
  gtk_widget_show(math_view->frame);

  math_view->area = gtk_drawing_area_new();
  GTK_WIDGET_SET_FLAGS(GTK_WIDGET(math_view->area), GTK_CAN_FOCUS);
  gtk_container_add(GTK_CONTAINER(math_view->frame), math_view->area);
  gtk_widget_show(math_view->area);

  gtk_signal_connect(GTK_OBJECT(math_view->area), "configure_event",
		     GTK_SIGNAL_FUNC(gtk_math_view_configure_event), math_view);

  gtk_signal_connect(GTK_OBJECT(math_view->area), "expose_event",
		     GTK_SIGNAL_FUNC(gtk_math_view_expose_event), math_view);

  gtk_signal_connect(GTK_OBJECT(math_view->area), "button_press_event",
		     GTK_SIGNAL_FUNC(gtk_math_view_button_press_event), math_view);

  gtk_signal_connect(GTK_OBJECT(math_view->area), "button_release_event",
		     GTK_SIGNAL_FUNC(gtk_math_view_button_release_event), math_view);

  gtk_signal_connect(GTK_OBJECT(math_view->area), "motion_notify_event",
		     GTK_SIGNAL_FUNC(gtk_math_view_motion_notify_event), math_view);

  gtk_signal_connect(GTK_OBJECT(math_view->area), "key_press_event",
		     GTK_SIGNAL_FUNC(gtk_math_view_key_press_event), math_view);

  gtk_signal_connect(GTK_OBJECT(math_view->area), "realize",
		     GTK_SIGNAL_FUNC(gtk_math_view_realize), math_view);

  math_view->hadjustment = NULL;
  math_view->vadjustment = NULL;

  gtk_widget_add_events(GTK_WIDGET(math_view->area),
			GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK
			| GDK_POINTER_MOTION_MASK
			| GDK_KEY_PRESS_MASK);
}

extern "C" GtkWidget*
gtk_math_view_new(GtkAdjustment* hadj, GtkAdjustment* vadj)
{
  GtkMathView* math_view = (GtkMathView*) gtk_type_new(gtk_math_view_get_type());
  
  g_return_val_if_fail(math_view != NULL, NULL);

  math_view->top_x = math_view->top_y = 0;
  math_view->old_top_x = math_view->old_top_y = 0;
  math_view->interface = new MathEngine();

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
  g_return_if_fail(math_view->interface != NULL);

  MathMLElement* root = math_view->interface->GetRoot();
  if (root != NULL) root->ReleaseGCs();

  MathEngine::logger(LOG_DEBUG, "destroying the widget");

  delete math_view->interface;
  delete math_view->font_manager;
  delete math_view->drawing_area;

  /* FIXME: sometimes the frame has been destroyed already
   * if you disable the delete_event and close the window you'll see
   * some messages
   */

  /* ATTEMPT: since this class is derived from a container
   * then contained object will be destroyed by the parent class'
   * method
   */

  gtk_signal_disconnect_by_data(GTK_OBJECT(math_view->hadjustment), math_view);
  gtk_signal_disconnect_by_data(GTK_OBJECT(math_view->vadjustment), math_view);

  if (GTK_OBJECT_CLASS(parent_class)->destroy != NULL)
    (*GTK_OBJECT_CLASS(parent_class)->destroy)(object);
}

static gboolean
gtk_math_view_key_press_event(GtkWidget* widget,
			      GdkEventKey* event,
			      GtkMathView* math_view)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->hadjustment != NULL, FALSE);
  g_return_val_if_fail(math_view->vadjustment != NULL, FALSE);

  if (event->type != GDK_KEY_PRESS) return FALSE;

  switch (event->keyval) {
  case GDK_Up:
  case GDK_KP_Up:
    math_view->vadjustment->value -= math_view->vadjustment->step_increment;
    gtk_adjustment_value_changed(math_view->vadjustment);
    break;
  case GDK_Down:
  case GDK_KP_Down:
    math_view->vadjustment->value += math_view->vadjustment->step_increment;
    gtk_adjustment_value_changed(math_view->vadjustment);
    break;
  case GDK_Left:
  case GDK_KP_Left:
    math_view->hadjustment->value -= math_view->hadjustment->step_increment;
    gtk_adjustment_value_changed(math_view->hadjustment);
    break;
  case GDK_Right:
  case GDK_KP_Right:
    math_view->hadjustment->value += math_view->hadjustment->step_increment;
    gtk_adjustment_value_changed(math_view->hadjustment);
    break;
  case GDK_Page_Up:
  case GDK_KP_Page_Up:
    math_view->vadjustment->value -= math_view->vadjustment->page_increment;
    gtk_adjustment_value_changed(math_view->vadjustment);
    break;
  case GDK_Page_Down:
  case GDK_KP_Page_Down:
    math_view->vadjustment->value += math_view->vadjustment->page_increment;
    gtk_adjustment_value_changed(math_view->vadjustment);
    break;
  case GDK_Home:
  case GDK_KP_Home:
    math_view->vadjustment->value = math_view->vadjustment->lower;
    gtk_adjustment_value_changed(math_view->vadjustment);
    break;
  case GDK_End:
  case GDK_KP_End:
    math_view->vadjustment->value = math_view->vadjustment->upper;
    gtk_adjustment_value_changed(math_view->vadjustment);
    break;
  default:
    return FALSE;
  }

  return TRUE;
}

static void
gtk_math_view_realize(GtkWidget* widget, GtkMathView* math_view)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->drawing_area != NULL);

  math_view->drawing_area->Realize();
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

  scaled x0 = math_view->drawing_area->GetTopX();
  scaled y0 = math_view->drawing_area->GetTopY();

  if (event->button == 1) {
    if (math_view->selected_root != NULL) {
      math_view->selected_root = NULL;
      gtk_signal_emit(GTK_OBJECT(math_view), selection_changed_signal, NULL);
    }

    math_view->selected_first = math_view->interface->GetElementAt(px2sp((gint) event->x) + x0, px2sp((gint) event->y) + y0);
    math_view->button_press = TRUE;
    math_view->select = FALSE;
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

  scaled x0 = math_view->drawing_area->GetTopX();
  scaled y0 = math_view->drawing_area->GetTopY();

  if (event->button == 1) {
    if (math_view->button_press == TRUE && math_view->select == FALSE) {
      MathMLElement* elem = math_view->interface->GetElementAt(px2sp((gint) event->x) + x0, px2sp((gint) event->y) + y0);

      if (elem != NULL) {
	mDOMNodeRef node = (elem != NULL) ? findDOMNode(elem) : NULL;

	if (node != NULL)
	  gtk_signal_emit(GTK_OBJECT(math_view), clicked_signal, node);
      }
    }
    
    math_view->button_press = math_view->select = FALSE;
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

  scaled x0 = math_view->drawing_area->GetTopX();
  scaled y0 = math_view->drawing_area->GetTopY();

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

  math_view->select = math_view->button_press;

  MathMLElement* elem = math_view->interface->GetElementAt(px2sp((gint) event->x) + x0, px2sp((gint) event->y) + y0);

  if (math_view->select == TRUE && math_view->selected_first != NULL && elem != NULL) {
    mDOMNodeRef oldRoot = math_view->selected_root;
    math_view->selected_root = findDOMNode(findCommonAncestor(math_view->selected_first, elem));

    if (math_view->selected_root != oldRoot)
      gtk_signal_emit(GTK_OBJECT(math_view), selection_changed_signal, math_view->selected_root);
  } else {
    mDOMNodeRef node = findDOMNode(elem);
    /* BEWARE: should this branch be tested anyway, even if a selection
     * is being made?
     */
    if (node != math_view->node) {
      math_view->node = node;
      gtk_signal_emit(GTK_OBJECT(math_view), element_changed_signal, node);
    }

    mDOMNodeRef action = findDOMNode(findActionElement(elem));
    if (action != math_view->action_node) {
      math_view->action_node = action;
      gtk_signal_emit(GTK_OBJECT(math_view), action_changed_signal, action);
    }
  }

  return FALSE;
}

static gint
gtk_math_view_configure_event(GtkWidget* widget,
			      GdkEventConfigure* event,
			      GtkMathView* math_view)
{
  gboolean do_layout;

  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(event != NULL, FALSE);
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);
  g_return_val_if_fail(math_view->drawing_area != NULL, FALSE);

  do_layout = math_view->old_width != event->width;
  math_view->old_width = event->width;

  if (math_view->pixmap != NULL) gdk_pixmap_unref(math_view->pixmap);
  math_view->pixmap = gdk_pixmap_new(widget->window, event->width, event->height, -1);
  math_view->drawing_area->SetSize(px2sp(event->width), px2sp(event->height));
  math_view->drawing_area->SetPixmap(math_view->pixmap);

  if (do_layout) math_view->interface->Layout();

  setup_adjustments(math_view);

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

  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		  math_view->pixmap,
		  event->area.x, event->area.y,
		  event->area.x, event->area.y,
		  event->area.width, event->area.height);

  return FALSE;
}

static void
setup_adjustment(GtkAdjustment* adj, gfloat size, gfloat page_size)
{
  g_return_if_fail(adj != NULL);

  adj->lower = 0.0;
  adj->page_size = page_size;
  adj->step_increment = 10 * SCALED_POINTS_PER_PX;
  adj->page_increment = page_size;
  adj->upper = size + 10 * SCALED_POINTS_PER_PX;
  if (adj->upper < 0) adj->upper = 0.0;

  if (adj->value > adj->upper - page_size) {
    adj->value = floatMax(0, adj->upper - page_size);
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
    gfloat width = sp2float(box.width);
    gfloat page_width = sp2float(math_view->drawing_area->GetWidth());
    
    if (math_view->top_x > width - page_width)
      math_view->top_x = floatMax(0, width - page_width);

    setup_adjustment(math_view->hadjustment, width, page_width);
  }

  if (math_view->vadjustment != NULL) {
    gfloat height = sp2float(box.GetHeight());
    gfloat page_height = sp2float(math_view->drawing_area->GetHeight());

    if (math_view->top_y > height - page_height)
      math_view->old_top_y = math_view->top_y = floatMax(0, height - page_height);

    setup_adjustment(math_view->vadjustment, height, page_height);
  }
}

extern "C" gboolean
gtk_math_view_load(GtkMathView* math_view, const gchar* name)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(name != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  bool res = math_view->interface->Load(name);
  if (!res) return FALSE;

  math_view->interface->Layout();

  setup_adjustments(math_view);
  reset_adjustments(math_view);
  paint_widget(math_view);

  return TRUE;
}

extern "C" gboolean
gtk_math_view_load_tree(GtkMathView* math_view, mDOMDocRef doc)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(doc != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  bool res = math_view->interface->Load(doc);
  if (!res) return FALSE;

  math_view->interface->Layout();

  setup_adjustments(math_view);
  reset_adjustments(math_view);
  paint_widget(math_view);

  return TRUE;
}

extern "C" void
gtk_math_view_unload(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);

  math_view->interface->Unload();
  math_view->interface->Update();

  setup_adjustments(math_view);
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

    math_view->hsignal = gtk_signal_connect(GTK_OBJECT(hadj), "value_changed",
					    GTK_SIGNAL_FUNC(hadjustment_value_changed),
					    math_view);
  }

  if (math_view->vadjustment != vadj) {
    math_view->vadjustment = vadj;
    gtk_object_ref(GTK_OBJECT(math_view->vadjustment));
    gtk_object_sink(GTK_OBJECT(math_view->vadjustment));

    math_view->vsignal = gtk_signal_connect(GTK_OBJECT(vadj), "value_changed",
					    GTK_SIGNAL_FUNC(vadjustment_value_changed),
					    math_view);
  }

  setup_adjustments(math_view);
}

static void
gtk_math_view_clicked(GtkMathView* math_view, mDOMNodeRef node)
{
  g_return_if_fail(math_view != NULL);
  // noop
}

static void
gtk_math_view_selection_changed(GtkMathView* math_view, mDOMNodeRef node)
{
  g_return_if_fail(math_view != NULL);
  // _math_view_set_selection(math_view, node);
}

static void
gtk_math_view_element_changed(GtkMathView* math_view, mDOMNodeRef node)
{
  g_return_if_fail(math_view != NULL);
  // noop
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

extern "C" GtkFrame*
gtk_math_view_get_frame(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);

  return math_view->frame != NULL ? GTK_FRAME(math_view->frame) : NULL;
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
  math_view->interface->Setup();
  math_view->interface->Layout();
  setup_adjustments(math_view);

  if (GTK_WIDGET_MAPPED(GTK_WIDGET(math_view))) paint_widget(math_view);
}

extern "C" guint
gtk_math_view_get_font_size(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, 0);
  g_return_val_if_fail(math_view->interface != NULL, 0);

  return math_view->interface->GetDefaultFontSize();
}

extern "C" mDOMNodeRef
gtk_math_view_get_selection(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);
  g_return_val_if_fail(math_view->interface != NULL, 0);

  MathMLElement* elem = math_view->interface->GetSelected();
  return (elem != NULL) ? findDOMNode(elem) : NULL;
}

extern "C" void
gtk_math_view_set_selection(GtkMathView* math_view, mDOMNodeRef node)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);

  MathMLElement* elem = (node != NULL) ? getMathMLElement(node) : NULL;
  math_view->interface->SetSelected(elem);
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

  if (GTK_WIDGET_MAPPED(GTK_WIDGET(math_view))) paint_widget(math_view);
}

extern "C" gboolean
gtk_math_view_get_anti_aliasing(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  return math_view->interface->GetAntiAliasing() ? TRUE : FALSE;
}

extern "C" void
gtk_math_view_set_kerning(GtkMathView* math_view, gboolean kerning)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);

  math_view->interface->SetKerning(kerning != FALSE);
  math_view->interface->Setup();
  math_view->interface->Layout();
  setup_adjustments(math_view);

  if (GTK_WIDGET_MAPPED(GTK_WIDGET(math_view))) paint_widget(math_view);
}

extern "C" gboolean
gtk_math_view_get_kerning(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  return math_view->interface->GetKerning() ? TRUE : FALSE;
}

extern "C" void
gtk_math_view_set_transparency(GtkMathView* math_view, gboolean transparency)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  
  math_view->interface->SetTransparency(transparency != FALSE);

  if (GTK_WIDGET_MAPPED(GTK_WIDGET(math_view))) paint_widget(math_view);
}

extern "C" gboolean
gtk_math_view_get_transparency(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, FALSE);
  g_return_val_if_fail(math_view->interface != NULL, FALSE);

  return math_view->interface->GetTransparency() ? TRUE : FALSE;
}

extern "C" void
gtk_math_view_get_top(GtkMathView* math_view, gint* x, gint* y)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->vadjustment != NULL);
  g_return_if_fail(math_view->hadjustment != NULL);

  if (x != NULL) *x = (gint) (math_view->hadjustment->value / SCALED_POINTS_PER_PX);
  if (y != NULL) *y = (gint) (math_view->vadjustment->value / SCALED_POINTS_PER_PX);
}

extern "C" void
gtk_math_view_set_top(GtkMathView* math_view, gint x, gint y)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->vadjustment != NULL);
  g_return_if_fail(math_view->hadjustment != NULL);

  math_view->hadjustment->value = px2sp(x);
  math_view->vadjustment->value = px2sp(y);

  // FIXME: this is inefficient, since the widget will be repainted twice!!!
  gtk_adjustment_value_changed(math_view->hadjustment);
  gtk_adjustment_value_changed(math_view->vadjustment);
}

extern "C" void
gtk_math_view_set_log_verbosity(GtkMathView* math_view, gint level)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  
  math_view->interface->SetVerbosity(level);
}

extern "C" gint
gtk_math_view_get_log_verbosity(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, 0);
  g_return_val_if_fail(math_view->interface != NULL, 0);

  return math_view->interface->GetVerbosity();
}

extern "C" void
gtk_math_view_set_font_manager_type(GtkMathView* math_view, FontManagerId id)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(id != FONT_MANAGER_UNKNOWN);

  if (id == math_view->font_manager_id) return;

  MathMLElement* root = math_view->interface->GetRoot();
  if (root != NULL) root->ReleaseGCs();

  delete math_view->font_manager;
  delete math_view->drawing_area;

  math_view->font_manager = NULL;
  math_view->drawing_area = NULL;

  math_view->font_manager_id = id;

  GraphicsContextValues values;
  values.foreground = MathEngine::configuration.GetForeground();
  values.background = MathEngine::configuration.GetBackground();
  values.lineStyle  = LINE_STYLE_SOLID;
  values.lineWidth  = px2sp(1);

  switch (id) {
  case FONT_MANAGER_T1:
#ifdef HAVE_LIBT1
    math_view->font_manager = new PS_T1_FontManager;
    math_view->drawing_area = new T1_Gtk_DrawingArea(values, px2sp(5), px2sp(5),
						     GTK_WIDGET(math_view->area),
						     MathEngine::configuration.GetSelectForeground(),
						     MathEngine::configuration.GetSelectBackground());
    math_view->drawing_area->SetPixmap(math_view->pixmap);
    break;
#else
    MathEngine::logger(LOG_WARNING, "the widget was compiled without support for T1 fonts, falling back to GTK fonts");
#endif // HAVE_LIBT1
  case FONT_MANAGER_GTK:
    math_view->font_manager = new Gtk_FontManager;
    math_view->drawing_area = new Gtk_DrawingArea(values, px2sp(5), px2sp(5),
						  GTK_WIDGET(math_view->area),
						  MathEngine::configuration.GetSelectForeground(),
						  MathEngine::configuration.GetSelectBackground());
    math_view->drawing_area->SetPixmap(math_view->pixmap);
    break;
  default:
    MathEngine::logger(LOG_ERROR, "could not switch to font manager type %d", id);
    break;
  }

  math_view->interface->Init(math_view->drawing_area, math_view->font_manager);
  if (GTK_WIDGET_REALIZED(GTK_WIDGET(math_view))) math_view->drawing_area->Realize();

  math_view->interface->Setup();
  math_view->interface->Layout();
  setup_adjustments(math_view);

  if (GTK_WIDGET_MAPPED(GTK_WIDGET(math_view))) paint_widget(math_view);
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
    MathEngine::logger(LOG_ERROR, "cannot export to PostScript if the Type1 Font Manager is not available");
    return;
  }

#ifdef HAVE_LIBT1
  PS_T1_FontManager* fm = TO_PS_T1_FONT_MANAGER(math_view->font_manager);
  g_assert(fm != NULL);

  PS_DrawingArea area(math_view->drawing_area->GetDefaultGraphicsContextValues(),
		      px2sp(x0), px2sp(y0), f);
  area.SetSize(px2sp(w), px2sp(h));
  if (disable_colors) area.DisableColors();

  MathMLElement* root = math_view->interface->GetRoot();
  if (root == NULL) return;

  // the following invocations are needed just to mark the chars actually used :(
  fm->ResetUsedChars();
  area.SetOutputFile(NULL);
  root->SetDirty();
  root->Render(area);
  area.SetOutputFile(f);

  Rectangle rect;
  math_view->interface->GetDocumentRectangle(rect);
  area.DumpHeader(PACKAGE, "(no title)", rect);
  fm->DumpFontDictionary(f);
  area.DumpPreamble();
  root->SetDirty();
  root->Render(area);
  area.DumpEpilogue();
#else
  g_assert_not_reached();
#endif // HAVE_LIBT1
}

/* experimental APIs */

extern "C" mDOMNodeRef
gtk_math_view_get_element(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);
  return math_view->node;
}

extern "C" mDOMNodeRef
gtk_math_view_get_action(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, NULL);
  return math_view->action_node;
}

extern "C" guint
gtk_math_view_action_get_selected(GtkMathView* math_view)
{
  g_return_val_if_fail(math_view != NULL, 0);

  if (math_view->action_node == NULL) return 0;

  MathMLActionElement* action_element = TO_ACTION(findMathMLElement(math_view->action_node));
  assert(action_element != NULL);
  return action_element->GetSelectedIndex();
}

extern "C" void
gtk_math_view_action_set_selected(GtkMathView* math_view, guint idx)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(math_view->action_node != NULL);

  MathMLActionElement* action_element = TO_ACTION(findMathMLElement(math_view->action_node));
  assert(action_element != NULL);
  action_element->SetSelectedIndex(idx);

  math_view->interface->MinMaxLayout();
  math_view->interface->Layout();
  setup_adjustments(math_view);

  paint_widget(math_view);
}

extern "C" void
gtk_math_view_action_toggle(GtkMathView* math_view)
{
  g_return_if_fail(math_view != NULL);
  g_return_if_fail(math_view->interface != NULL);
  g_return_if_fail(math_view->action_node != NULL);

  MathMLActionElement* action_element = TO_ACTION(findMathMLElement(math_view->action_node));
  assert(action_element != NULL);
  guint idx = action_element->GetSelectedIndex();
  if (idx < action_element->content.GetSize())
    idx++;
  else
    idx = 1;

  gtk_math_view_action_set_selected(math_view, idx);
}
