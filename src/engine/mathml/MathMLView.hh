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

#include "View.hh"
#include "MathMLElement.hh"
#include "Rectangle.hh"

class MathMLView : public Object
{
protected:
  MathMLView(const SmartPtr<class MathMLElementFactory>&,
	     const SmartPtr<class MathGraphicDevice>&);
  virtual ~MathMLView();

public:
  static SmartPtr<MathMLView> create(const SmartPtr<class MathMLElementFactory>& factory,
				     const SmartPtr<class MathGraphicDevice>& device)
  { return new MathMLView(factory, device); }

  bool frozen(void) const { return freezeCounter > 0; }
  virtual bool freeze(void);
  virtual bool thaw(void);

  void setRoot(const DOM::Element&);
  DOM::Element getRoot(void) const { return root; }
  SmartPtr<MathMLElement> getElementAt(const scaled&, const scaled&) const; // should return DOM::ELement
  bool getElementExtents(const DOM::Element&, scaled&, scaled&, BoundingBox&) const;
  AreaRef getRootArea(void) const;

  void setOrigin(const scaled& x, const scaled& y) { x0 = x; y0 = y; }
  void setYOrigin(const scaled& y) { y0 = y; }
  scaled getYOrigin(void) const { return y0; }

  void setDefaultFontSize(unsigned);
  unsigned getDefaultFontSize(void) const { return defaultFontSize; }

  virtual BoundingBox getBoundingBox(void) const;
  virtual Rectangle getRectangle(void) const;
  virtual void render(class RenderingContext&) const;

  SmartPtr<class Linker> getLinker(void) const;

protected:
  SmartPtr<MathMLElement> getRootElement(void) const;

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
  DOM::Element root;
  SmartPtr<class MathMLElementFactory> factory;
  SmartPtr<class MathGraphicDevice> device;
  scaled x0;
  scaled y0;
};

#endif // __MathMLView_hh__
