// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __gtkmathview_h__
#define __gtkmathview_h__

#define gtk_math_view_get_type             GTKMATHVIEW_METHOD_NAME(get_type)
#define gtk_math_view_new                  GTKMATHVIEW_METHOD_NAME(new)
#define gtk_math_view_freeze               GTKMATHVIEW_METHOD_NAME(freeze)
#define gtk_math_view_thaw                 GTKMATHVIEW_METHOD_NAME(thaw)
#define gtk_math_view_load_reader          GTKMATHVIEW_METHOD_NAME(load_reader)
#define gtk_math_view_load_uri             GTKMATHVIEW_METHOD_NAME(load_uri)
#define gtk_math_view_load_root            GTKMATHVIEW_METHOD_NAME(load_root)
#define gtk_math_view_unload               GTKMATHVIEW_METHOD_NAME(unload)
#define gtk_math_view_get_bounding_box     GTKMATHVIEW_METHOD_NAME(get_bounding_box)
#define gtk_math_view_structure_changed    GTKMATHVIEW_METHOD_NAME(structure_changed)
#define gtk_math_view_attribute_changed    GTKMATHVIEW_METHOD_NAME(attribute_changed)
#define gtk_math_view_select               GTKMATHVIEW_METHOD_NAME(select)
#define gtk_math_view_unselect             GTKMATHVIEW_METHOD_NAME(unselect)
#define gtk_math_view_is_selected          GTKMATHVIEW_METHOD_NAME(is_selected)
#define gtk_math_view_get_element_at       GTKMATHVIEW_METHOD_NAME(get_element_at)
#define gtk_math_view_get_element_location GTKMATHVIEW_METHOD_NAME(get_element_location)
#define gtk_math_view_get_char_at          GTKMATHVIEW_METHOD_NAME(get_char_at)
#define gtk_math_view_get_char_location    GTKMATHVIEW_METHOD_NAME(get_char_location)
#define gtk_math_view_get_width            GTKMATHVIEW_METHOD_NAME(get_width)
#define gtk_math_view_get_height           GTKMATHVIEW_METHOD_NAME(get_height)
#define gtk_math_view_get_top              GTKMATHVIEW_METHOD_NAME(get_top)
#define gtk_math_view_set_top              GTKMATHVIEW_METHOD_NAME(set_top)
#define gtk_math_view_set_adjustments      GTKMATHVIEW_METHOD_NAME(set_adjustments)
#define gtk_math_view_get_hadjustment      GTKMATHVIEW_METHOD_NAME(get_hadjustment)
#define gtk_math_view_get_vadjustment      GTKMATHVIEW_METHOD_NAME(get_vadjustment)
#define gtk_math_view_get_buffer           GTKMATHVIEW_METHOD_NAME(get_buffer)
#define gtk_math_view_get_drawing_area     GTKMATHVIEW_METHOD_NAME(get_drawing_area)
#define gtk_math_view_set_font_size        GTKMATHVIEW_METHOD_NAME(set_font_size)
#define gtk_math_view_get_font_size        GTKMATHVIEW_METHOD_NAME(get_font_size)
#define gtk_math_view_set_log_verbosity    GTKMATHVIEW_METHOD_NAME(set_log_verbosity)
#define gtk_math_view_get_log_verbosity    GTKMATHVIEW_METHOD_NAME(get_log_verbosity)
#define gtk_math_view_set_cursor           GTKMATHVIEW_METHOD_NAME(set_cursor)
#define gtk_math_view_get_cursor           GTKMATHVIEW_METHOD_NAME(get_cursor)
#define gtk_math_view_set_cursor_visible   GTKMATHVIEW_METHOD_NAME(set_cursor_visible)
#define gtk_math_view_get_cursor_visible   GTKMATHVIEW_METHOD_NAME(get_cursor_visible)
     
#endif /* __gtkmathview_h__ */
