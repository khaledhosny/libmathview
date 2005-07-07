// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#ifndef __Init_hh__
#define __Init_hh__

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "MathMLOperatorDictionary.hh"

template <typename MathView>
SmartPtr<Configuration>
initConfiguration(SmartPtr<AbstractLogger>& logger, const char* confPath)
{
  SmartPtr<Configuration> configuration = Configuration::create();

  bool res = false;
  if (confPath != NULL) res = MathView::loadConfiguration(logger, configuration, confPath);
  if (!res) res = MathView::loadConfiguration(logger, configuration, MathView::getDefaultConfigurationPath());
  if (!res) res = MathView::loadConfiguration(logger, configuration, "config/gtkmathview.conf.xml");
  if (!res) logger->out(LOG_WARNING, "could not load configuration file");
  logger->setLogLevel(LogLevelId(configuration->getInt(logger, "logger/verbosity", 1)));
  String confVersion = configuration->getString(logger, "version", "<undefined>");
  if (confVersion != VERSION)
    logger->out(LOG_WARNING, "configuration file version (%s) differs from binary version (%s)", confVersion.c_str(), VERSION);
  return configuration;
}

template <typename MathView>
SmartPtr<MathMLOperatorDictionary>
initOperatorDictionary(const SmartPtr<AbstractLogger>& logger, const SmartPtr<Configuration> configuration)
{
  SmartPtr<MathMLOperatorDictionary> dictionary = MathMLOperatorDictionary::create();
  std::vector<String> paths = configuration->getStringList("dictionary/path");
  if (!paths.empty())
    for (std::vector<String>::const_iterator dit = paths.begin();
	 dit != paths.end();
	 dit++)
      {
	logger->out(LOG_DEBUG, "loading dictionary `%s'", (*dit).c_str());
	if (!MathView::loadOperatorDictionary(logger, dictionary, (*dit).c_str()))
	  logger->out(LOG_WARNING, "could not load `%s'", (*dit).c_str());
      }
  else
    {
      const bool res = MathView::loadOperatorDictionary(logger, dictionary, "config/dictionary.xml");
      if (!res) MathView::loadOperatorDictionary(logger, dictionary, MathView::getDefaultOperatorDictionaryPath());
    }

  return dictionary;
}

#endif // __Init_hh__
