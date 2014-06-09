/* Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
 *
 * This file is part of GtkMathView, a flexible, high-quality rendering
 * engine for MathML documents.
 * 
 * GtkMathView is free software; you can redistribute it and/or modify it
 * either under the terms of the GNU Lesser General Public License version
 * 3 as published by the Free Software Foundation (the "LGPL") or, at your
 * option, under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation (the "GPL").  If you do not
 * alter this notice, a recipient may use your version of this file under
 * either the GPL or the LGPL.
 *
 * GtkMathView is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
 * the GPL for more details.
 * 
 * You should have received a copy of the LGPL and of the GPL along with
 * this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef __c_customXmlReader_h__
#define __c_customXmlReader_h__

enum
  {
    C_CUSTOM_TEXT_NODE = 1,
    C_CUSTOM_ELEMENT_NODE
  };

typedef void* c_customModelElementId;
typedef void* c_customModelUserData;

struct _c_customXmlReader
{
  /* auxiliary methods */
  void  (*free_data)(c_customModelUserData);
  void  (*free_string)(char*);

  /* query methods */
  int   (*more)(c_customModelUserData);
  int   (*get_node_type)(c_customModelUserData);
  char* (*get_node_name)(c_customModelUserData);
  char* (*get_node_namespace_uri)(c_customModelUserData);
  char* (*get_node_value)(c_customModelUserData);
  void* (*get_node_id)(c_customModelUserData);
  int   (*get_attribute_count)(c_customModelUserData);
  void  (*get_attribute_by_index)(c_customModelUserData, int, char**, char**, char**);
  char* (*get_attribute)(c_customModelUserData, const char*);
  int   (*has_attribute)(c_customModelUserData, const char*);

  /* state methods */
  int   (*reset)(c_customModelUserData);
  void  (*move_to_first_child)(c_customModelUserData);
  void  (*move_to_next_sibling)(c_customModelUserData);
  void  (*move_to_parent_node)(c_customModelUserData);
};

typedef struct _c_customXmlReader c_customXmlReader;

#endif // __c_customXmlReader_h__
