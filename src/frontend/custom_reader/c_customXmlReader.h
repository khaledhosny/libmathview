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
