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

#include <config.h>
#include <glib.h>
#include <parser.h>
#include <parserInternals.h>

#include "minidom.h"

gboolean
mdom_string_eq(mDOMConstStringRef s1, mDOMConstStringRef s2)
{
  g_return_val_if_fail(s1 != NULL, FALSE);
  g_return_val_if_fail(s2 != NULL, FALSE);

  return strcmp(C_CONST_STRING(s1), C_CONST_STRING(s2)) ? FALSE : TRUE;
}

mDOMStringRef
mdom_string_dup(mDOMConstStringRef s)
{
  g_return_val_if_fail(s != NULL, NULL);
  return xmlStrdup(s);
}

void
mdom_string_free(mDOMStringRef s)
{
  g_free(s);
}

mDOMDocRef
mdom_load(const char* file_name, gboolean subst, mDOMEntityResolver entity_resolver)
{
  xmlSAXHandler sax;
  xmlParserCtxtPtr ctxt;
  mDOMDocRef doc;

  g_return_val_if_fail(file_name != NULL, NULL);

  xmlSubstituteEntitiesDefault(subst);
  
  sax = xmlDefaultSAXHandler;
  sax.getEntity = entity_resolver;
  
  ctxt = xmlCreateFileParserCtxt(file_name);
  if (ctxt == NULL) return NULL;
  
  ctxt->sax = &sax;
  
  xmlParseDocument(ctxt);
  
  doc = ctxt->myDoc;
  ctxt->sax = NULL;
  xmlFreeParserCtxt(ctxt);

  return doc;
}

void
mdom_unload(mDOMDocRef doc)
{
  g_return_if_fail(doc != NULL);
  g_assert_not_reached();
}

mDOMDocRef
mdom_doc_new(mDOMConstStringRef version)
{
  mDOMDocRef doc;
  
  g_return_val_if_fail(version != NULL, NULL);
  doc = xmlNewDoc(version);
  g_return_val_if_fail(doc != NULL, NULL);

  doc->extSubset = xmlNewDtd(doc, DOM_CONST_STRING("Entities"), NULL, NULL);

  return doc;
}

void
mdom_doc_free(mDOMDocRef doc)
{
  g_return_if_fail(doc != NULL);
  xmlFreeDoc(doc);
}

mDOMEntityRef
mdom_doc_add_entity(mDOMDocRef doc, mDOMConstStringRef name, mDOMConstStringRef content)
{
  g_return_val_if_fail(doc != NULL, NULL);
  g_return_val_if_fail(name != NULL, NULL);
  g_return_val_if_fail(content != NULL, NULL);
#ifdef HAVE_LIBXML2
  return xmlAddDtdEntity(doc, name, XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, content);
#else
  xmlAddDtdEntity(doc, name, XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, content);
  return NULL;
#endif
}

mDOMEntityRef
mdom_doc_get_entity(mDOMDocRef doc, mDOMConstStringRef name)
{
  g_return_val_if_fail(doc != NULL, NULL);
  g_return_val_if_fail(name != NULL, NULL);
  return xmlGetDtdEntity(doc, name);
}

mDOMEntityRef
mdom_get_predefined_entity(mDOMConstStringRef name)
{
  g_return_val_if_fail(name != NULL, NULL);
  return xmlGetPredefinedEntity(name);
}

mDOMConstStringRef
mdom_entity_get_content(mDOMEntityRef entity)
{
  g_return_val_if_fail(entity != NULL, NULL);
  return entity->content;
}

mDOMNodeRef
mdom_doc_get_root_node(mDOMDocRef doc)
{
  g_return_val_if_fail(doc != NULL, NULL);
  return xmlDocGetRootElement(doc);
}

void
mdom_node_set_user_data(mDOMNodeRef node, gpointer user_data)
{
  g_return_if_fail(node != NULL);
#ifndef XML_WITHOUT_CORBA
  node->_private = user_data;
#else
  g_assert_not_reached();
#endif
}

gpointer
mdom_node_get_user_data(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
#ifndef XML_WITHOUT_CORBA
  return node->_private;
#else
  g_assert_not_reached();
#endif
}

gboolean
mdom_node_is_text(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, FALSE);
  return xmlNodeIsText(node) ? TRUE : FALSE;
}

gboolean
mdom_node_is_element(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, FALSE);
  return node->type == XML_ELEMENT_NODE;
}

gboolean
mdom_node_is_blank(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, FALSE);
#ifdef HAVE_LIBXML2
  return xmlIsBlankNode(node) ? TRUE : FALSE;  
#else
  return FALSE;
#endif
}

gboolean
mdom_node_is_entity_ref(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, FALSE);
  return node->type == XML_ENTITY_REF_NODE;
}

guint 
mdom_node_get_type(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, 0);
  return node->type;
}

mDOMConstStringRef
mdom_node_get_name(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
  return node->name;
}

mDOMConstStringRef
mdom_node_get_ns_uri(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
  if (node->ns == NULL) return NULL;
  return node->ns->href;
}

mDOMStringRef
mdom_node_get_attribute(mDOMNodeRef node, mDOMConstStringRef name)
{
  g_return_val_if_fail(node != NULL, NULL);
  g_return_val_if_fail(name != NULL, NULL);
  return xmlGetProp(node, name);
}

mDOMStringRef
mdom_node_get_attribute_ns(mDOMNodeRef node, mDOMConstStringRef uri, mDOMConstStringRef name)
{
  g_return_val_if_fail(node != NULL, NULL);
  g_return_val_if_fail(uri != NULL, NULL);
  g_return_val_if_fail(name != NULL, NULL);
  g_return_val_if_fail(mdom_node_is_element(node), NULL);
  return xmlGetNsProp(node, name, uri);
}

gboolean
mdom_node_has_attribute(mDOMNodeRef node, mDOMConstStringRef name)
{
#ifndef HAVE_LIBXML2
  mDOMStringRef s = NULL;
#endif /* HAVE_LIBXML2 */
  g_return_val_if_fail(node != NULL, FALSE);
  g_return_val_if_fail(name != NULL, FALSE);
#ifdef HAVE_LIBXML2
  return (xmlHasProp(node, name) != NULL) ? TRUE : FALSE;
#else
  s = xmlGetProp(node, name);
  if (s != NULL) {
    mdom_string_free(s);
    return TRUE;
  }
  return FALSE;
#endif /* HAVE_LIBXML2 */
}

mDOMStringRef
mdom_node_get_content(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
  return xmlNodeGetContent(node);
}

mDOMNodeRef
mdom_node_get_parent(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
  return node->parent;
}

mDOMNodeRef
mdom_node_get_prev_sibling(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
  return node->prev;
}

mDOMNodeRef
mdom_node_get_next_sibling(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
  return node->next;
}

mDOMNodeRef
mdom_node_get_first_child(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
#ifdef HAVE_LIBXML2
  return node->children;
#else
  return node->childs;
#endif
}

mDOMAttrRef
mdom_node_get_first_attribute(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, NULL);
  return node->properties;
}

gboolean
mdom_node_is_first(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, FALSE);
  return node->prev == NULL;
}

gboolean
mdom_node_is_last(mDOMNodeRef node)
{
  g_return_val_if_fail(node != NULL, FALSE);
  return node->next == NULL;
}

mDOMConstStringRef
mdom_attr_get_name(mDOMAttrRef attr)
{
  g_return_val_if_fail(attr != NULL, NULL);
  return attr->name;
}

mDOMConstStringRef
mdom_attr_get_ns_uri(mDOMAttrRef attr)
{
  g_return_val_if_fail(attr != NULL, NULL);
  if (attr->ns == NULL) return NULL;
  return attr->ns->href;
}

mDOMStringRef
mdom_attr_get_value(mDOMAttrRef attr)
{
#ifdef HAVE_LIBXML2
  g_return_val_if_fail(attr != NULL, NULL);
  return xmlNodeListGetString(attr->doc, attr->children, 1);
#else
  mDOMNodeRef child;
  g_return_val_if_fail(attr != NULL, NULL);

  child = attr->val;
  if (child == NULL) return NULL;
  else return xmlNodeListGetString(child->doc, attr->val, 1);
#endif
}

mDOMAttrRef
mdom_attr_get_prev_sibling(mDOMAttrRef attr)
{
#ifdef HAVE_LIBXML2
  g_return_val_if_fail(attr != NULL, NULL);
  return attr->prev;
#else
  /* in libxml 1.x there is no prev link in attributes */
  return NULL;
#endif
}

mDOMAttrRef
mdom_attr_get_next_sibling(mDOMAttrRef attr)
{
  g_return_val_if_fail(attr != NULL, NULL);
  return attr->next;
}

mDOMNodeRef
mdom_attr_get_parent(mDOMAttrRef attr)
{
  g_return_val_if_fail(attr != NULL, NULL);
#ifdef HAVE_LIBXML2
  return attr->parent;
#else
  return attr->node;
#endif
}
