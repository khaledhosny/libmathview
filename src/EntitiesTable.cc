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
#include <assert.h>
#include <stdio.h>

#include "minidom.h"
#include "stringAux.hh"
#include "EntitiesTable.hh"
#include "MathMLParseFile.hh"

#define XML_ERROR_ENTITY  (DOM_CONST_STRING("ThisIsMyVerySillyErrorEntity"))

static struct {
  const char* name;
  const char* content;
} internalTable[] = {
#include "../auto/entitiesTable.inc"
  { NULL, NULL }
};

EntitiesTable::EntitiesTable()
{
  repository = mdom_doc_new(DOM_CONST_STRING("1.0"));
  assert(repository != NULL);

#if 0
  entityRepository->extSubset = 
    xmlNewDtd(entityRepository, XMLSTRING("Entities Repository"), NULL, NULL);
#endif

  // this is a default entity used to avoid parser errors while
  // parsing unknown entities (see command line options)
  mdom_doc_add_entity(repository, XML_ERROR_ENTITY, DOM_CONST_STRING("?"));

#if 0
  xmlAddDtdEntity(entityRepository,
		  XML_ERROR_ENTITY,
		  XML_INTERNAL_GENERAL_ENTITY,
		  NULL,
		  NULL,
		  XMLSTRING("?"));
#endif
}

EntitiesTable::~EntitiesTable()
{
  mdom_doc_free(repository);
  repository = NULL;
}

void
EntitiesTable::Load(const char* name)
{
  assert(NOT_IMPLEMENTED);
#if 0
  printf("Loading entities from `%s'...", name);
  fflush(stdout);

  xmlDocPtr doc = MathMLParseFile(name, false);
  if (doc == NULL) {
    printf("error!\n");
    return;
  }
  
  xmlDtdPtr dtd = doc->intSubset;
  xmlEntitiesTablePtr et = static_cast<xmlEntitiesTablePtr>(dtd->entities);

  for (int i = 0; i < et->nb_entities; i++) {
#ifdef HAVE_LIBXML2
    xmlEntityPtr ep0 = et->table[i];
#else
    xmlEntityPtr ep0 = &et->table[i];
#endif
#ifdef HAVE_LIBXML2
    xmlEntityPtr ep =
#endif
      xmlAddDtdEntity(entityRepository,
		      ep0->name,
#ifdef HAVE_LIBXML2
		      ep0->etype,
#else
		      ep0->type,
#endif
		      ep0->ExternalID,
		      ep0->SystemID,
		      ep0->content);
#ifdef HAVE_LIBXML2
    assert(ep != NULL);
#endif
  }

  printf("done! (%d)\n", et->nb_entities);

  xmlFreeDoc(doc);
#endif
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

void
EntitiesTable::Dump() const
{
  assert(NOT_IMPLEMENTED);
#if 0
  xmlDtdPtr dtd = entityRepository->extSubset;
  assert(dtd != NULL);
  xmlEntitiesTablePtr et = static_cast<xmlEntitiesTablePtr>(dtd->entities);
  assert(et != NULL);

  for (int i = 0; i < et->nb_entities; i++) {
#ifdef HAVE_LIBXML2
    xmlEntityPtr ep0 = et->table[i];
#else
    xmlEntityPtr ep0 = &et->table[i];
#endif
    assert(ep0 != NULL);

    printf("{ \"%s\", \"", ep0->name);

    for (xmlChar* s = ep0->content; s != NULL && *s != '\0'; s++) {
      printf("\\x%02x", *s);
    }

    printf("\\x00\" },\n");
  }
#endif
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
