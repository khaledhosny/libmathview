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

