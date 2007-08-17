/* Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
 *
 * This file is part of GtkMathView, a flexible, high-quality rendering
 * engine for MathML documents.
 * 
 * GtkMathView is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * GtkMathView is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __gtkmathview_h__
#define __gtkmathview_h__

#define gtk_math_view_get_type                 GTKMATHVIEW_METHOD_NAME(get_type)
#define gtk_math_view_new                      GTKMATHVIEW_METHOD_NAME(new)
#define gtk_math_view_update                   GTKMATHVIEW_METHOD_NAME(update)
#define gtk_math_view_freeze                   GTKMATHVIEW_METHOD_NAME(freeze)
#define gtk_math_view_thaw                     GTKMATHVIEW_METHOD_NAME(thaw)
#define gtk_math_view_load_reader              GTKMATHVIEW_METHOD_NAME(load_reader)
#define gtk_math_view_load_uri                 GTKMATHVIEW_METHOD_NAME(load_uri)
#define gtk_math_view_load_buffer              GTKMATHVIEW_METHOD_NAME(load_buffer)
#define gtk_math_view_load_document            GTKMATHVIEW_METHOD_NAME(load_document)
#define gtk_math_view_load_root                GTKMATHVIEW_METHOD_NAME(load_root)
#define gtk_math_view_unload                   GTKMATHVIEW_METHOD_NAME(unload)
#define gtk_math_view_structure_changed        GTKMATHVIEW_METHOD_NAME(structure_changed)
#define gtk_math_view_attribute_changed        GTKMATHVIEW_METHOD_NAME(attribute_changed)
#define gtk_math_view_select                   GTKMATHVIEW_METHOD_NAME(select)
#define gtk_math_view_unselect                 GTKMATHVIEW_METHOD_NAME(unselect)
#define gtk_math_view_is_selected              GTKMATHVIEW_METHOD_NAME(is_selected)
#define gtk_math_view_get_document             GTKMATHVIEW_METHOD_NAME(get_document)
#define gtk_math_view_get_element_at           GTKMATHVIEW_METHOD_NAME(get_element_at)
#define gtk_math_view_get_element_extents      GTKMATHVIEW_METHOD_NAME(get_element_extents)
#define gtk_math_view_get_element_extents_ref  GTKMATHVIEW_METHOD_NAME(get_element_extents_ref)
#define gtk_math_view_get_element_length       GTKMATHVIEW_METHOD_NAME(get_element_length)
#define gtk_math_view_get_char_at              GTKMATHVIEW_METHOD_NAME(get_char_at)
#define gtk_math_view_get_char_extents         GTKMATHVIEW_METHOD_NAME(get_char_extents)
#define gtk_math_view_get_char_extents_ref     GTKMATHVIEW_METHOD_NAME(get_char_extents_ref)
#define gtk_math_view_get_size                 GTKMATHVIEW_METHOD_NAME(get_size)
#define gtk_math_view_get_bounding_box         GTKMATHVIEW_METHOD_NAME(get_bounding_box)
#define gtk_math_view_get_top                  GTKMATHVIEW_METHOD_NAME(get_top)
#define gtk_math_view_set_top                  GTKMATHVIEW_METHOD_NAME(set_top)
#define gtk_math_view_set_adjustments          GTKMATHVIEW_METHOD_NAME(set_adjustments)
#define gtk_math_view_get_adjustments          GTKMATHVIEW_METHOD_NAME(get_adjustments)
#define gtk_math_view_get_buffer               GTKMATHVIEW_METHOD_NAME(get_buffer)
#define gtk_math_view_set_font_size            GTKMATHVIEW_METHOD_NAME(set_font_size)
#define gtk_math_view_get_font_size            GTKMATHVIEW_METHOD_NAME(get_font_size)
#define gtk_math_view_set_log_verbosity        GTKMATHVIEW_METHOD_NAME(set_log_verbosity)
#define gtk_math_view_get_log_verbosity        GTKMATHVIEW_METHOD_NAME(get_log_verbosity)
#define gtk_math_view_get_t1_opaque_mode       GTKMATHVIEW_METHOD_NAME(get_t1_opaque_mode)
#define gtk_math_view_set_t1_opaque_mode       GTKMATHVIEW_METHOD_NAME(set_t1_opaque_mode)
#define gtk_math_view_get_t1_anti_aliased_mode GTKMATHVIEW_METHOD_NAME(get_t1_anti_aliased_mode)
#define gtk_math_view_set_t1_anti_aliased_mode GTKMATHVIEW_METHOD_NAME(set_t1_anti_aliased_mode)
#define gtk_math_view_add_configuration_path   GTKMATHVIEW_METHOD_NAME(add_configuration_path)

#endif /* __gtkmathview_h__ */
