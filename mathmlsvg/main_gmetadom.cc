// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "Logger.hh"

#include "Init.hh"
#include "Configuration.hh"
#include "gmetadom_MathView.hh"
#include "MathMLOperatorDictionary.hh"
#include "SVG_MathGraphicDevice.hh"
#include "SVG_StreamRenderingContext.hh"
#include "MathMLNamespaceContext.hh"

typedef gmetadom_MathView MathView;

#include <iostream>
#include "BoundingBoxAux.hh"

int
main(int argc, char* argv[])
{
  SmartPtr<AbstractLogger> logger = Logger::create();
  SmartPtr<Configuration> configuration = initConfiguration<MathView>(logger, getenv("GTKMATHVIEWCONF"));
  logger->setLogLevel(LOG_DEBUG);
  SmartPtr<SVG_MathGraphicDevice> mgd = SVG_MathGraphicDevice::create(logger, configuration);
  SmartPtr<MathMLOperatorDictionary> dictionary = initOperatorDictionary<MathView>(logger, configuration);
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
