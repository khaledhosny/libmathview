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

#include "minidom.h"
#include "stringAux.hh"
#include "MathEngine.hh"
#include "MathMLParseFile.hh"

static mDOMEntityRef
myVeryPrivateGetEntity(void* user_data, mDOMConstStringRef name)
{
  mDOMEntityRef entity = mdom_get_predefined_entity(name);
  if (entity == NULL) entity = MathEngine::entitiesTable.GetEntity(name);
  if (entity == NULL) {
    MathEngine::logger(LOG_WARNING, "cannot resolve entity reference `%s', a `?' will be used instead", name);
    entity = MathEngine::entitiesTable.GetErrorEntity();
  }

  return entity;
}

mDOMDocRef
MathMLParseFile(const char* filename, bool subst)
{
  return mdom_load(filename, subst, myVeryPrivateGetEntity);
}

