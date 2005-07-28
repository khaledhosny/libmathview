// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include "libxml/xmlreader.h"

#include "libxml2_reader_MathView.hh"
#include "libxml2_reader_Builder.hh"
#include "libxml2_reader_Setup.hh"

libxml2_reader_MathView::libxml2_reader_MathView(const SmartPtr<AbstractLogger>& logger)
  : View(logger)
{
  setBuilder(libxml2_reader_Builder::create());
}

libxml2_reader_MathView::~libxml2_reader_MathView()
{ }

SmartPtr<libxml2_reader_MathView>
libxml2_reader_MathView::create(const SmartPtr<AbstractLogger>& logger)
{ return new libxml2_reader_MathView(logger); }

void
libxml2_reader_MathView::unload()
{ }

bool
libxml2_reader_MathView::loadReader(xmlTextReaderPtr reader)
{
  if (SmartPtr<libxml2_reader_Builder> builder = smart_cast<libxml2_reader_Builder>(getBuilder()))
    {
      resetRootElement();
      builder->setReader(libxmlXmlReader::create(reader));
      return true;
    }

  unload();
  return false;
}

SmartPtr<Element>
libxml2_reader_MathView::elementOfModelElement(void*) const
{ return 0; }

void*
libxml2_reader_MathView::modelElementOfElement(const SmartPtr<Element>&) const
{ return 0; }

bool
libxml2_reader_MathView::notifyStructureChanged(void*) const
{ return false; }

bool
libxml2_reader_MathView::notifyAttributeChanged(void*, const xmlChar*) const
{ return false; }

bool
libxml2_reader_MathView::loadConfiguration(const SmartPtr<AbstractLogger>& logger,
					   const SmartPtr<Configuration>& configuration, const String& path)
{ return libxml2_reader_Setup::loadConfiguration(*logger, *configuration, path); }

bool
libxml2_reader_MathView::loadOperatorDictionary(const SmartPtr<AbstractLogger>& logger,
						const SmartPtr<MathMLOperatorDictionary>& dictionary, const String& path)
{ return libxml2_reader_Setup::loadOperatorDictionary(*logger, *dictionary, path); }

