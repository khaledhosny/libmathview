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

#include "stringAux.hh"
#include "MathEngine.hh"
#include "EntitiesTable.hh"
#include "MathMLParseFile.hh"

#if 0
static mDOMEntityRef
getEntity(void* user_data, mDOMConstStringRef name)
{
  mDOMEntityRef entity = mdom_get_predefined_entity(name);
  if (entity == NULL) entity = MathEngine::entitiesTable.GetEntity(name);
  if (entity == NULL) entity = MathEngine::entitiesTable.GetErrorEntity();

  return entity;
}
#endif

GdomeDocument*
MathMLParseFile(const char* fileName, bool subst)
{
  assert(fileName != NULL);

  GdomeException exc;
  static GdomeDOMImplementation* di = NULL;

  if (di == NULL) di = gdome_di_mkref();
  assert(di != NULL);

  GdomeDocument* res;

  if (subst)
    res = gdome_di_createDocFromURIWithEntitiesTable(di, fileName, entitiesTable, GDOME_LOAD_PARSING, &exc);
  else
    res = gdome_di_createDocFromURI(di, fileName, GDOME_LOAD_PARSING, &exc);

  //gdome_di_unref(di, &exc);

  return res;
}
