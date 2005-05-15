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

#include <config.h>

#include <cassert>

#include "Logger.hh"

#include "Configuration.hh"
#include "gmetadom_MathView.hh"
#include "MathMLOperatorDictionary.hh"
#include "SVG_MathGraphicDevice.hh"
#include "SVG_StreamRenderingContext.hh"
#include "MathMLNamespaceContext.hh"

typedef gmetadom_MathView MathView;

static SmartPtr<Configuration>
initConfiguration(SmartPtr<AbstractLogger>& logger, const char* confPath)
{
  SmartPtr<Configuration> configuration = Configuration::create();

  bool res = false;
  if (confPath != NULL) res = MathView::loadConfiguration(logger, configuration, confPath);
  if (!res) res = MathView::loadConfiguration(logger, configuration, MathView::getDefaultConfigurationPath());
  if (!res) res = MathView::loadConfiguration(logger, configuration, "config/gtkmathview.conf.xml");
  if (!res)
    {
      logger->out(LOG_ERROR, "could not load configuration file");
      exit(-1);
    }

  return configuration;
}

static SmartPtr<MathMLOperatorDictionary>
initOperatorDictionary(const SmartPtr<AbstractLogger>& logger, const SmartPtr<Configuration> configuration)
{
  SmartPtr<MathMLOperatorDictionary> dictionary = MathMLOperatorDictionary::create();
  if (!configuration->getDictionaries().empty())
    for (std::vector<std::string>::const_iterator dit = configuration->getDictionaries().begin();
	 dit != configuration->getDictionaries().end();
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

#include <iostream>
#include "BoundingBoxAux.hh"

int
main(int argc, char* argv[])
{
  SmartPtr<AbstractLogger> logger = Logger::create();
  SmartPtr<Configuration> configuration = initConfiguration(logger, getenv("GTKMATHVIEWCONF"));
  logger->setLogLevel(LOG_DEBUG);
  SmartPtr<SVG_MathGraphicDevice> mgd = SVG_MathGraphicDevice::create(logger, configuration);
  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary(logger, configuration);
  SmartPtr<MathView> view = MathView::create();
  view->setLogger(logger);
  view->setOperatorDictionary(dictionary);
  view->setMathMLNamespaceContext(MathMLNamespaceContext::create(view, mgd));
  view->loadURI(argv[1]);
  const BoundingBox box = view->getBoundingBox();
  std::cerr << box << std::endl;
  SVG_StreamRenderingContext rc(logger, std::cout);
  rc.documentStart(box);
  view->render(rc, 0, box.height);
  rc.documentEnd();
  view->resetRootElement();
}
