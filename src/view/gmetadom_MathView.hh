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

#ifndef __gmetadom_MathView_hh__
#define __gmetadom_MathView_hh__

#include <GdomeSmartDOM.hh>

#include "View.hh"
#include "String.hh"

class gmetadom_MathView : public View
{
protected:
  gmetadom_MathView(void);
  virtual ~gmetadom_MathView();

public:
  static SmartPtr<gmetadom_MathView> create(void) { return new gmetadom_MathView(); }

  void unload(void);
  bool loadURI(const String&);
  bool loadBuffer(const String&);
  bool loadDocument(const GdomeSmartDOM::Document&);
  bool loadRootElement(const GdomeSmartDOM::Element&);

  bool notifyStructureChanged(const GdomeSmartDOM::Element&) const;
  bool notifyStructureChanged(GdomeElement* el) const { return notifyStructureChanged(GdomeSmartDOM::Element(el)); }
  bool notifyAttributeChanged(const GdomeSmartDOM::Element&, const GdomeSmartDOM::GdomeString&) const;
  bool notifyAttributeChanged(GdomeElement* el, GdomeDOMString* s) const
  { return notifyAttributeChanged(GdomeSmartDOM::Element(el), GdomeSmartDOM::GdomeString(s)); }
  GdomeSmartDOM::Element modelElementOfElementAux(const SmartPtr<class Element>&) const;
  GdomeElement* modelElementOfElement(const SmartPtr<class Element>& elem) const
  { return reinterpret_cast<GdomeElement*>(modelElementOfElementAux(elem).gdome_object()); }
  SmartPtr<class Element> elementOfModelElement(const GdomeSmartDOM::Element&) const;
  SmartPtr<class Element> elementOfModelElement(GdomeElement*) const;

  GdomeSmartDOM::Document getDocument(void) const { return currentDoc; }

  virtual bool loadConfiguration(const SmartPtr<class Configuration>&, const String&) const;
  virtual bool loadOperatorDictionary(const SmartPtr<class MathMLOperatorDictionary>&, const String&) const;

protected:
  GdomeSmartDOM::Document currentDoc;
};

#endif // __gmetadom_MathView_hh__
