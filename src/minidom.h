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

#ifndef minidom_h
#define minidom_h

#include <glib.h>
#include <parser.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  typedef xmlChar*     mDOMStringRef;
  typedef const xmlChar* mDOMConstStringRef;
  typedef xmlDocPtr    mDOMDocRef;
  typedef xmlNodePtr   mDOMNodeRef;
  typedef xmlAttrPtr   mDOMAttrRef;
  typedef xmlEntityPtr mDOMEntityRef;

  typedef xmlEntityPtr (*mDOMEntityResolver)(void*, mDOMConstStringRef);

#define C_STRING(s)         ((char*) (s))
#define C_CONST_STRING(s)   ((const char*) (s))
#define DOM_STRING(s)       ((mDOMStringRef) (s))
#define DOM_CONST_STRING(s) ((mDOMConstStringRef) (s))

  mDOMDocRef    mdom_load(const char*, gboolean, mDOMEntityResolver);
  void          mdom_unload(mDOMDocRef);

  gboolean      mdom_string_eq(mDOMConstStringRef, mDOMConstStringRef);
  void          mdom_string_free(mDOMStringRef);
  mDOMStringRef mdom_string_dup(mDOMConstStringRef);

  mDOMDocRef    mdom_doc_new(mDOMConstStringRef);
  void          mdom_doc_free(mDOMDocRef);
  mDOMNodeRef   mdom_doc_get_root_node(mDOMDocRef);
  mDOMEntityRef mdom_doc_add_entity(mDOMDocRef, mDOMConstStringRef, mDOMConstStringRef);
  mDOMEntityRef mdom_doc_get_entity(mDOMDocRef, mDOMConstStringRef);
  mDOMEntityRef mdom_get_predefined_entity(mDOMConstStringRef);
  mDOMConstStringRef mdom_entity_get_content(mDOMEntityRef);

  void          mdom_node_set_user_data(mDOMNodeRef, gpointer);
  gpointer      mdom_node_get_user_data(mDOMNodeRef);
  gboolean      mdom_node_is_text(mDOMNodeRef);
  gboolean      mdom_node_is_element(mDOMNodeRef);
  gboolean      mdom_node_is_entity_ref(mDOMNodeRef);
  gboolean      mdom_node_is_blank(mDOMNodeRef);
  guint         mdom_node_get_type(mDOMNodeRef);
  mDOMConstStringRef mdom_node_get_name(mDOMNodeRef);
  mDOMConstStringRef mdom_node_get_ns_uri(mDOMNodeRef);
  mDOMStringRef mdom_node_get_attribute(mDOMNodeRef, mDOMConstStringRef);
  mDOMStringRef mdom_node_get_attribute_ns(mDOMNodeRef, mDOMConstStringRef, mDOMConstStringRef);
  gboolean      mdom_node_has_attribute(mDOMNodeRef, mDOMConstStringRef);
  mDOMStringRef mdom_node_get_content(mDOMNodeRef);
  mDOMNodeRef   mdom_node_get_parent(mDOMNodeRef);
  mDOMNodeRef   mdom_node_get_prev_sibling(mDOMNodeRef);
  mDOMNodeRef   mdom_node_get_next_sibling(mDOMNodeRef);
  mDOMNodeRef   mdom_node_get_first_child(mDOMNodeRef);
  mDOMAttrRef   mdom_node_get_first_attribute(mDOMNodeRef);
  gboolean      mdom_node_is_first(mDOMNodeRef);
  gboolean      mdom_node_is_last(mDOMNodeRef);
  
  mDOMConstStringRef mdom_attr_get_name(mDOMAttrRef);
  mDOMConstStringRef mdom_attr_get_ns_uri(mDOMAttrRef);
  mDOMStringRef mdom_attr_get_value(mDOMAttrRef);
  mDOMAttrRef   mdom_attr_get_prev_sibling(mDOMAttrRef);
  mDOMAttrRef   mdom_attr_get_next_sibling(mDOMAttrRef);
  mDOMNodeRef   mdom_attr_get_parent(mDOMAttrRef);
  
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* minidom_h */

