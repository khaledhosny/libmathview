// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <stdlib.h>

#if defined(HAVE_MINIDOM)

#include <minidom.h>

#include <assert.h>
#include <stdio.h>

#include "stringAux.hh"
#include "Globals.hh"
#include "EntitiesTable.hh"
#include "MathMLParseFile.hh"

#define XML_ERROR_ENTITY  (DOM_CONST_STRING("ThisIsMyVerySillyErrorEntity"))

static struct {
  const char* name;
  const char* content;
  const char* publicID;
  const char* systemID;
} internalTable[] = {
#include "../auto/entitiesTable.inc"
  { NULL, NULL, NULL, NULL }
};

EntitiesTable::EntitiesTable()
{
  repository = mdom_doc_new(DOM_CONST_STRING("1.0"));
  assert(repository != NULL);
  // this is a default entity used to avoid parser errors while
  // parsing unknown entities (see command line options)
  mdom_doc_add_entity(repository, XML_ERROR_ENTITY, DOM_CONST_STRING("?"));
}

EntitiesTable::~EntitiesTable()
{
  mdom_doc_free(repository);
  repository = NULL;
}

bool
EntitiesTable::Load(const char* fileName)
{
  mDOMDocRef doc = mdom_load(fileName, FALSE, NULL);
  if (doc == NULL) return false;
  
  mDOMNodeRef root = mdom_doc_get_root_node(doc);
  if (root == NULL) {
    mdom_unload(doc);
    return false;
  }

  mDOMNodeRef p = mdom_node_get_first_child(root);
  while (p != NULL) {
    if (!mdom_node_is_blank(p)) {
      mDOMStringRef name = mdom_node_get_attribute(p, DOM_CONST_STRING("name"));
      mDOMStringRef value = mdom_node_get_attribute(p, DOM_CONST_STRING("value"));

      if (name != NULL && value != NULL) mdom_doc_add_entity(repository, name, value);

      mdom_string_free(name);
      mdom_string_free(value);
    }

    p = mdom_node_get_next_sibling(p);
  }

  mdom_unload(doc);

  return true;
}

void
EntitiesTable::LoadInternalTable()
{
  for (unsigned i = 0; internalTable[i].name != NULL; i++) {
    mdom_doc_add_entity(repository,
			DOM_CONST_STRING(internalTable[i].name),
			DOM_CONST_STRING(internalTable[i].content));
  }
}

mDOMEntityRef
EntitiesTable::GetEntity(mDOMConstStringRef name) const
{
  return mdom_doc_get_entity(repository, name);
}

mDOMEntityRef
EntitiesTable::GetErrorEntity() const
{
  return GetEntity(XML_ERROR_ENTITY);
}

String*
EntitiesTable::GetEntityContent(mDOMConstStringRef name) const
{
  mDOMEntityRef ep = GetEntity(name);
  if (ep == NULL) return NULL;

  return allocString(mdom_entity_get_content(ep));
}

String*
EntitiesTable::GetErrorEntityContent() const
{
  return GetEntityContent(XML_ERROR_ENTITY);
}

#elif defined(HAVE_GMETADOM)

#include <gdome.h>

static const GdomeEntitiesTableEntry mathmlEntities[] = {
#include "../auto/entitiesTable.inc"
  { NULL, NULL, NULL, NULL }
};

const GdomeEntitiesTableEntry*
getMathMLEntities(void)
{
  return mathmlEntities;
}

#endif // HAVE_GMETADOM
