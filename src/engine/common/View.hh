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

#ifndef __View_hh__
#define __View_hh__

#include "View.hh"
#include "Element.hh"
#include "Rectangle.hh"

class View : public Object
{
protected:
  View(void);
  virtual ~View();

public:
  static SmartPtr<View> create(void) { return new View(); }

  bool frozen(void) const { return freezeCounter > 0; }
  virtual bool freeze(void);
  virtual bool thaw(void);

  virtual void setRoot(const DOM::Element&);
  SmartPtr<Element> getRoot(void) const { return root; }
  SmartPtr<Element> getElementAt(const scaled&, const scaled&) const;
  bool getElementExtents(const DOM::Element&, scaled&, scaled&, BoundingBox&) const;
  AreaRef getRootArea(void) const;

  void setOrigin(const scaled& x, const scaled& y) { x0 = x; y0 = y; }
  void setYOrigin(const scaled& y) { y0 = y; }
  scaled getYOrigin(void) const { return y0; }

  virtual BoundingBox getBoundingBox(void) const;
  virtual Rectangle getRectangle(void) const;
  virtual void render(class RenderingContext&) const;

protected:
  class DOMSubtreeModifiedListener : public DOM::EventListener
  {
  public:
    DOMSubtreeModifiedListener(const SmartPtr<View>& v) : view(v) { };
    virtual ~DOMSubtreeModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    SmartPtr<View> view;
  };

  class DOMAttrModifiedListener : public DOM::EventListener
  {
  public:
    DOMAttrModifiedListener(const SmartPtr<View>& v) : view(v) { };
    virtual ~DOMAttrModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    SmartPtr<View> view;
  };

  DOMSubtreeModifiedListener* subtreeModifiedListener;
  DOMAttrModifiedListener* attrModifiedListener;

private:
  unsigned freezeCounter;
  SmartPtr<Element> root;
  scaled x0;
  scaled y0;
};

#endif // __View_hh__
