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

#ifndef __gmetadom_Builder_hh__
#define __gmetadom_Builder_hh__

#include "gmetadom.hh"
#include "gmetadom_Model.hh"
#include "Builder.hh"
#include "String.hh"
#include "WeakPtr.hh"

class gmetadom_Builder : public Builder
{
protected:
  gmetadom_Builder(void) { }
  virtual ~gmetadom_Builder();

public:
  static SmartPtr<gmetadom_Builder> create(void);

  void setRootModelElement(const DOM::Element&);
  DOM::Element getRootModelElement(void) const { return root; }

  SmartPtr<Element> findElement(const DOM::Element& p) const { return linker.assoc(p); }
  DOM::Node findSelfOrAncestorModelNode(const SmartPtr<Element>&) const;
  SmartPtr<Element> findSelfOrAncestorElement(const DOM::Node&) const;

protected:
  // methods for accessing the linker
  SmartPtr<Element> linkerAssoc(const DOM::Element& el) const { return linker.assoc(el); }
  void linkerAdd(const DOM::Element& el, Element* elem) const { linker.add(el, elem); }
  void linkerRemove(Element* elem) const { linker.remove(elem); }

  void notifySubtreeModified(const DOM::Node&) const;
  void notifyAttributeChanged(const DOM::Node&, const String&) const;

  class DOMSubtreeModifiedListener : public DOM::EventListener
  {
  public:
    DOMSubtreeModifiedListener(const SmartPtr<gmetadom_Builder>& b) : builder(b) { };
    virtual ~DOMSubtreeModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    WeakPtr<gmetadom_Builder> builder;
  };

  class DOMAttrModifiedListener : public DOM::EventListener
  {
  public:
    DOMAttrModifiedListener(const SmartPtr<gmetadom_Builder>& b) : builder(b) { };
    virtual ~DOMAttrModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    WeakPtr<gmetadom_Builder> builder;
  };

private:
  DOMSubtreeModifiedListener* subtreeModifiedListener;
  DOMAttrModifiedListener* attrModifiedListener;
  mutable gmetadom_Model::Linker linker;
  DOM::Element root;
};

#endif // __gmetadom_Builder_hh__
