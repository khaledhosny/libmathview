// Copyright (C) 2000-2001, Luca Padovani <luca.padovani@cs.unibo.it>.
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
#include <ctype.h>

#include <gdome.h>
#include "gdomeAux.h"

const gchar*
gdome_str_c(GdomeDOMString* s)
{
  g_return_val_if_fail(s != NULL, NULL);
  return s->str;
}

gboolean
gdome_str_equal_c(GdomeDOMString* s1, const gchar* s2)
{
  g_return_val_if_fail(s1 != NULL, FALSE);
  g_return_val_if_fail(s2 != NULL, FALSE);

  return xmlStrEqual(s1->str, s2);
}

gboolean
gdome_str_isEmpty(GdomeDOMString* s)
{
  const gchar* cs;

  g_return_val_if_fail(s != NULL, TRUE);

  cs = s->str;
  g_assert(cs != NULL);
  while (*cs != '\0') {
    if (!isspace(*cs)) return FALSE;
    cs++;
  }

  return TRUE;
}

gboolean
gdome_n_name_is(GdomeNode* node, const gchar* name)
{
  gboolean res;
  GdomeDOMString* node_name;
  GdomeException exc;

  g_return_val_if_fail(node != NULL, FALSE);
  g_return_val_if_fail(name != NULL, FALSE);

  node_name = gdome_n_nodeName(node, &exc);
  g_assert(node_name != NULL);
  
  res = xmlStrEqual(node_name->str, name);

  gdome_str_unref(node_name);

  return res;
}

GdomeNode*
gdome_n_nextSibling_unref(GdomeNode* n)
{
  GdomeNode* next;
  GdomeException exc;

  g_return_val_if_fail(n != NULL, NULL);
  next = gdome_n_nextSibling(n, &exc);
  gdome_n_unref(n, &exc);

  return next;
}

gboolean
gdome_el_hasAttribute_c(GdomeElement* el, const gchar* c_name)
{
  gboolean res;
  GdomeDOMString* name;
  GdomeException exc;

  g_return_val_if_fail(el != NULL, FALSE);
  g_return_val_if_fail(c_name != NULL, FALSE);

  name = gdome_str_mkref(c_name);
  g_assert(name != NULL);

  res = gdome_el_hasAttribute(el, name, &exc);

  gdome_str_unref(name);

  return res;
}

GdomeDOMString*
gdome_el_getAttribute_c(GdomeElement* el, const gchar* c_name)
{
  GdomeDOMString* res;
  GdomeDOMString* name;
  GdomeException exc;

  g_return_val_if_fail(el != NULL, FALSE);
  g_return_val_if_fail(c_name != NULL, FALSE);

  name = gdome_str_mkref(c_name);
  g_assert(name != NULL);

  res = gdome_el_getAttribute(el, name, &exc);

  gdome_str_unref(name);

  return res;
}

GdomeDOMString*
gdome_n_content(GdomeNode* node)
{
  GdomeNode* child;
  GdomeDOMString* value;
  GdomeException exc;

  g_return_val_if_fail(node != NULL, NULL);

  gdome_n_normalize(node, &exc);
  if (!gdome_n_hasChildNodes(node, &exc)) return NULL;

  child = gdome_n_firstChild(node, &exc);
  if (child == NULL) return NULL;

  value = gdome_n_nodeValue(child, &exc);
  gdome_n_unref(child, &exc);

  return value;
}

gboolean
gdome_n_isFirst(GdomeNode* node)
{
  GdomeNode* prev;
  GdomeException exc;

  g_return_val_if_fail(node != NULL, FALSE);

  prev = gdome_n_previousSibling(node, &exc);

  if (prev != NULL) {
    gdome_n_unref(prev, &exc);
    return FALSE;
  } else
    return TRUE;
}

gboolean
gdome_n_isLast(GdomeNode* node)
{
  GdomeNode* next;
  GdomeException exc;

  g_return_val_if_fail(node != NULL, FALSE);

  next = gdome_n_nextSibling(node, &exc);

  if (next != NULL) {
    gdome_n_unref(next, &exc);
    return FALSE;
  } else
    return TRUE;
}

gboolean
gdome_n_isBlank(GdomeNode* node)
{
  GdomeDOMString* s;
  GdomeException exc;
  gboolean res;

  g_return_val_if_fail(node != NULL, FALSE);

  if (gdome_n_nodeType(node, &exc) == GDOME_COMMENT_NODE) return TRUE;
  if (gdome_n_nodeType(node, &exc) != GDOME_TEXT_NODE) return FALSE;

  s = gdome_n_nodeValue(node, &exc);
  g_assert(s != NULL);

  res = gdome_str_isEmpty(s);
  gdome_str_unref(s);

  return res;
}

void
gdome_n_setUserData(GdomeNode* node, gpointer user_data)
{
  g_return_if_fail(node != NULL);
  node->user_data = user_data;
}

gpointer
gdome_n_getUserData(GdomeNode* node)
{
  g_return_val_if_fail(node != NULL, NULL);
  return node->user_data;
}

void
gdome_doc_unref(GdomeDocument* node, GdomeException* exc)
{
  printf("null doc unref!\n");
}
