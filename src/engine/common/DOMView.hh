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

class DOMView : public View
{
protected:
  DOMView(void);
  virtual ~DOMView();

public:
  static SmartPtr<DOMView> create(void) { return new DOMView(); }

  void setRoot(const DOM::Element&);
  DOM::Element getRoot(void) const { return root; }

  virtual SmartPtr<Element> getRootElement(void) const;

private:
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

  DOMSubtreeModifiedListener* subtreeModifiedListener;
  DOMAttrModifiedListener* attrModifiedListener;
  DOM::Element root;
};

#endif // __DOMView_hh__
