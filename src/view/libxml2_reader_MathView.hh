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

#ifndef __libxml2_reader_MathView_hh__
#define __libxml2_reader_MathView_hh__

#include <libxml/xmlreader.h>

#include "View.hh"

class GMV_FrontEnd_EXPORT libxml2_reader_MathView : public View
{
protected:
  libxml2_reader_MathView(const SmartPtr<class AbstractLogger>&);
  virtual ~libxml2_reader_MathView();

public:
  static SmartPtr<libxml2_reader_MathView> create(const SmartPtr<class AbstractLogger>&);

  virtual void unload(void);
  bool loadReader(xmlTextReaderPtr);

  bool notifyStructureChanged(void*) const;
  bool notifyAttributeChanged(void*, const xmlChar*) const;
  void* modelElementOfElement(const SmartPtr<class Element>&) const;
  SmartPtr<class Element> elementOfModelElement(void*) const;

  xmlDoc* getDocument(void) const { return currentDoc; }

  static bool loadConfiguration(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&, const String&);
  static bool loadOperatorDictionary(const SmartPtr<class AbstractLogger>&, const SmartPtr<class MathMLOperatorDictionary>&, const String&);

protected:
  xmlDoc* currentDoc;
};

#endif // __libxml2_reader_MathView_hh__
