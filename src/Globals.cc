// Copyright (C) 2000-2002, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Globals.hh"
#include "StringUnicode.hh"

#ifdef HAVE_LIBT1
#include <t1lib.h>
#endif

#include "config.dirs"

namespace Globals {

#if defined(HAVE_MINIDOM)
  EntitiesTable      entitiesTable;
#endif
  OperatorDictionary dictionary;
  Configuration      configuration;
  Logger             logger;

  static bool drawMissingCharacter = true;
  static bool done = false;

  void
  InitGlobalData(const char* confPath)
  {
    assert(!done);

    bool res = false;
    if (confPath != NULL) res = configuration.Load(confPath);
    if (!res) res = configuration.Load(PKGDATADIR"/math-engine-configuration.xml");
    if (!res) res = configuration.Load("config/math-engine-configuration.xml");
    if (!res) {
      logger(LOG_ERROR, "could not find configuration file");
      exit(-1);
    }

#if defined(HAVE_MINIDOM)
    // the entities table and the dictionary are global,
    // so they have to be configured once only
  
    //res = entitiesTable.Load("/usr/local/share/gtkmathview/entities-table.xml", false);
    //if (!res) res = entitiesTable.Load("config/entities-table.xml", true);
    entitiesTable.LoadInternalTable();
#endif

    if (!configuration.GetDictionaries().empty())
      for (std::vector<String*>::const_iterator dit = configuration.GetDictionaries().begin();
	   dit != configuration.GetDictionaries().end();
	   dit++)
	{
	  assert(*dit);
	  logger(LOG_DEBUG, "loading dictionary `%s'", (*dit)->ToStaticC());
	  if (!dictionary.Load((*dit)->ToStaticC()))
	    logger(LOG_WARNING, "could not load `%s'", (*dit)->ToStaticC());
	}
    else {
      bool res = dictionary.Load("config/dictionary.xml");
      if (!res) dictionary.Load(PKGDATADIR"/dictionary.xml");
    }

    if (getenv("T1LIB_CONFIG") == NULL && configuration.GetT1ConfigFiles().size() == 1)
      {
	StringC s("T1LIB_CONFIG=");
	assert(configuration.GetT1ConfigFiles()[0]);
	s.Append(*configuration.GetT1ConfigFiles()[0]);

	char *cs = strdup(s.ToStaticC());
	putenv(cs);
      }

    done = true;
  }

  void
  SetVerbosity(int level)
  {
    logger.SetLogLevel(level);
#ifdef HAVE_LIBT1
    T1_SetLogLevel(level + 1);
#endif // HAVE_LIBT1
  }

  int
  GetVerbosity()
  {
    return logger.GetLogLevel();
  }

  bool
  DrawMissingCharacter()
  {
    return drawMissingCharacter;
  }

  void
  DrawMissingCharacter(bool dmc)
  {
    drawMissingCharacter = dmc;
  }

}
