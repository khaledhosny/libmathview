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
#include "token.hh"

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

    initTokens();

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
      for (std::vector<std::string>::const_iterator dit = configuration.GetDictionaries().begin();
	   dit != configuration.GetDictionaries().end();
	   dit++)
	{
	  logger(LOG_DEBUG, "loading dictionary `%s'", (*dit).c_str());
	  if (!dictionary.Load((*dit).c_str()))
	    logger(LOG_WARNING, "could not load `%s'", (*dit).c_str());
	}
    else {
      bool res = dictionary.Load("config/dictionary.xml");
      if (!res) dictionary.Load(PKGDATADIR"/dictionary.xml");
    }

    done = true;
  }

  void
  SetVerbosity(int level)
  {
    logger.SetLogLevel(level);
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
