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

#ifndef __libxml2_reader_MathView_hh__
#define __libxml2_reader_MathView_hh__

#include <libxml/xmlreader.h>

#include "View.hh"

class GMV_EXPORT libxml2_reader_MathView : public View
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
