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

#ifndef __libxml2_MathView_hh__
#define __libxml2_MathView_hh__

#include <libxml/tree.h>

#include "View.hh"

class libxml2_MathView : public View
{
protected:
  libxml2_MathView(void);
  virtual ~libxml2_MathView();

public:
  static SmartPtr<libxml2_MathView> create(void) { return new libxml2_MathView(); }

  void unload(void);
  bool loadURI(const char*);
  bool loadBuffer(const char*);
  bool loadDocument(xmlDoc*);
  bool loadRootElement(xmlElement*);

  bool notifyStructureChanged(xmlElement*) const;
  bool notifyAttributeChanged(xmlElement*, const xmlChar*) const;
  xmlElement* modelElementOfElement(const SmartPtr<class Element>&) const;
  SmartPtr<class Element> elementOfModelElement(xmlElement*) const;

  xmlDoc* getDocument(void) const { return currentDoc; }

  virtual bool loadConfiguration(const SmartPtr<class Configuration>&, const String&) const;
  virtual bool loadOperatorDictionary(const SmartPtr<class MathMLOperatorDictionary>&, const String&) const;

protected:
  xmlDoc* currentDoc;
  bool docOwner;
};

#endif // __libxml2_MathView_hh__
