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

#if defined(HAVE_MINIDOM)

#include <minidom.h>

#elif defined(HAVE_GMETADOM)

#include <assert.h>

#include "gmetadom.hh"
#include "EntitiesTable.hh"

#endif // HAVE_GMETADOM

#include "stringAux.hh"
#include "Globals.hh"
#include "MathMLParseFile.hh"

#if defined(HAVE_MINIDOM)
static mDOMEntityRef
myVeryPrivateGetEntity(void* user_data, mDOMConstStringRef name)
{
  mDOMEntityRef entity = mdom_get_predefined_entity(name);
  if (entity == NULL) entity = Globals::entitiesTable.GetEntity(name);
  if (entity == NULL) {
    Globals::logger(LOG_WARNING, "cannot resolve entity reference `%s', a `?' will be used instead", name);
    entity = Globals::entitiesTable.GetErrorEntity();
  }

  return entity;
}

mDOMDocRef
MathMLParseFile(const char* filename, bool subst)
{
  return mdom_load(filename, subst, myVeryPrivateGetEntity);
}

#elif defined(HAVE_GMETADOM)

DOM::Document
MathMLParseFile(const char* filename, bool subst)
{
  if (!subst) {
    DOM::DOMImplementation di;
    return di.createDocumentFromURI(filename);
  } else {
    GdomeDOMImplementation* di = gdome_di_mkref();
    assert(di != NULL);
    GdomeException exc = 0;
    GdomeDocument* doc = gdome_di_createDocFromURIWithEntitiesTable(di,
						    filename,
						    getMathMLEntities(),
						    GDOME_LOAD_PARSING | GDOME_LOAD_SUBSTITUTE_ENTITIES,
						    &exc);
    if (exc != 0) {
      gdome_di_unref(di, &exc);
      gdome_doc_unref(doc, &exc);
      return DOM::Document(0);
    }

    if (doc == 0) {
      // FIXME: this should be signalled as an exception, I think
      gdome_di_unref(di, &exc);
      return DOM::Document(0);
    }

    DOM::Document res(doc);
    gdome_di_unref(di, &exc);
    assert(exc == 0);
    gdome_doc_unref(doc, &exc);
    assert(exc == 0);
    
    return res;
  }
}

#endif // HAVE_GMETADOM
