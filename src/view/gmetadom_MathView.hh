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

#include "View.hh"
#include "String.hh"
#include "gmetadom.hh"

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
  bool loadDocument(const DOM::Document&);
  bool loadRootElement(const DOM::Element&);

  bool notifyStructureChanged(const DOM::Element&) const;
  bool notifyStructureChanged(GdomeElement* el) const { return notifyStructureChanged(DOM::Element(el)); }
  bool notifyAttributeChanged(const DOM::Element&, const DOM::GdomeString&) const;
  bool notifyAttributeChanged(GdomeElement* el, GdomeDOMString* s) const
  { return notifyAttributeChanged(DOM::Element(el), DOM::GdomeString(s)); }
  DOM::Element modelElementOfElementAux(const SmartPtr<class Element>&) const;
  GdomeElement* modelElementOfElement(const SmartPtr<class Element>& elem) const
  { return reinterpret_cast<GdomeElement*>(modelElementOfElementAux(elem).gdome_object()); }
  SmartPtr<class Element> elementOfModelElement(const DOM::Element&) const;
  SmartPtr<class Element> elementOfModelElement(GdomeElement* el) const
  { return elementOfModelElement(DOM::Element(el)); }

  DOM::Document getDocument(void) const { return currentDoc; }

protected:
  DOM::Document currentDoc;
};

#endif // __gmetadom_MathView_hh__
