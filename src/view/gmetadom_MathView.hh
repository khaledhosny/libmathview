// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __gmetadom_MathView_hh__
#define __gmetadom_MathView_hh__

#include <GdomeSmartDOM.hh>

#include "View.hh"
#include "String.hh"

class GMV_FrontEnd_EXPORT gmetadom_MathView : public View
{
protected:
  gmetadom_MathView(const SmartPtr<class AbstractLogger>&);
  virtual ~gmetadom_MathView();

public:
  static SmartPtr<gmetadom_MathView> create(const SmartPtr<class AbstractLogger>&);

  virtual void unload(void);
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

  static bool loadConfiguration(const SmartPtr<class AbstractLogger>&, const SmartPtr<class Configuration>&, const String&);
  static bool loadOperatorDictionary(const SmartPtr<class AbstractLogger>&, const SmartPtr<class MathMLOperatorDictionary>&, const String&);

protected:
  GdomeSmartDOM::Document currentDoc;
};

#endif // __gmetadom_MathView_hh__
