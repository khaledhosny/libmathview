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

#ifndef __DOMView_hh__
#define __DOMView_hh__

#include "View.hh"
#include "gmetadom.hh"
#include "gmetadom_Builder.hh"

class DOMView : public View
{
protected:
  DOMView(void);
  virtual ~DOMView();

public:
  static SmartPtr<DOMView> create(void) { return new DOMView(); }

#if 0
  SmartPtr<class Linker> getLinker(void) const;
#endif

  DOM::Element findDOMElement(const SmartPtr<class Element>&) const;
  SmartPtr<class Element> findElement(const DOM::Element&) const;
  SmartPtr<class Element> findSelfOrAncestorElement(const DOM::Element&) const;

  virtual SmartPtr<class Element> getRootElement(void) const;

  void setRootDOMElement(const DOM::Element&);
  DOM::Element getRootDOMElement(void) const { return builder.getRootDOMElement(); }

  bool getDOMElementExtents(const DOM::Element&, scaled&, scaled&, BoundingBox&) const;
  bool getDOMElementAt(const scaled&, const scaled&, DOM::Element&) const;
  bool getCharExtents(const DOM::Element&, int, scaled&, scaled&, BoundingBox&) const;
  bool getCharAt(const scaled&, const scaled&, DOM::Element&, int&) const;

private:
#if 0
  class DOMSubtreeModifiedListener : public DOM::EventListener
  {
  public:
    DOMSubtreeModifiedListener(const SmartPtr<DOMView>& v) : view(v) { };
    virtual ~DOMSubtreeModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    SmartPtr<DOMView> view;
  };

  class DOMAttrModifiedListener : public DOM::EventListener
  {
  public:
    DOMAttrModifiedListener(const SmartPtr<DOMView>& v) : view(v) { };
    virtual ~DOMAttrModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    SmartPtr<DOMView> view;
  };

  SmartPtr<class Linker> linker;
  DOMSubtreeModifiedListener* subtreeModifiedListener;
  DOMAttrModifiedListener* attrModifiedListener;
#endif
  gmetadom_Builder builder;
  mutable SmartPtr<class Element> rootElement;
};

#endif // __DOMView_hh__
