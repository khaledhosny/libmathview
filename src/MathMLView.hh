// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef __MathMLView_hh__
#define __MathMLView_hh__

#include "MathMLElement.hh"
#include "MathMLViewContext.hh"

class MathMLView : public Object
{
protected:
  MathMLView(const SmartPtr<MathMLViewContext>&);
  virtual ~MathMLView();

public:
  static SmartPtr<MathMLView> create(const SmartPtr<MathMLViewContext>& context)
  { return new MathMLView(context); }

  void Init(class DrawingArea*, class FontManager*);

  bool frozen(void) const { return freezeCounter > 0; }
  virtual bool freeze(void);
  virtual bool thaw(void);

  virtual void setRoot(const DOM::Element&);
  SmartPtr<MathMLElement> getRoot(void) const { return root; }
  SmartPtr<MathMLElement> getElementAt(const scaled&, const scaled&) const;

  void setDefaultFontSize(unsigned);
  unsigned getDefaultFontSize(void) const { return defaultFontSize; }

  virtual BoundingBox getBoundingBox(void) const;
  virtual Rectangle getRectangle(void) const;
  virtual void render(const Rectangle* = 0) const;

  SmartPtr<MathMLViewContext> getContext(void) const { return context; }

protected:
  void layout(void) const;

  class DOMSubtreeModifiedListener : public DOM::EventListener
  {
  public:
    DOMSubtreeModifiedListener(const SmartPtr<MathMLView>& v) : view(v) { };
    virtual ~DOMSubtreeModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    SmartPtr<MathMLView> view;
  };

  class DOMAttrModifiedListener : public DOM::EventListener
  {
  public:
    DOMAttrModifiedListener(const SmartPtr<MathMLView>& v) : view(v) { };
    virtual ~DOMAttrModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    SmartPtr<MathMLView> view;
  };

  DOMSubtreeModifiedListener* subtreeModifiedListener;
  DOMAttrModifiedListener* attrModifiedListener;

private:
  unsigned freezeCounter;
  unsigned defaultFontSize;
  SmartPtr<MathMLElement> root;
  SmartPtr<MathMLViewContext> context;

  class DrawingArea* area;
  class FontManager* fontManager;
};

#endif // __MathMLView_hh__
