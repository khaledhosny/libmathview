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

#include "Object.hh"
#include "SmartPtr.hh"
#include "BoundingBox.hh"

class View : public Object
{
protected:
  View(const SmartPtr<class Builder>&);
  virtual ~View();

public:
  static SmartPtr<View> create(const SmartPtr<class Builder>&);
  void initialize(const SmartPtr<class MathGraphicDevice>&,
		  const SmartPtr<class BoxGraphicDevice>&);

  bool frozen(void) const { return freezeCounter > 0; }
  bool freeze(void);
  bool thaw(void);

  void setOrigin(const scaled& x, const scaled& y) { x0 = x; y0 = y; }
  scaled getOriginX(void) const { return x0; }
  scaled getOriginY(void) const { return y0; }

  SmartPtr<class Builder> getBuilder(void) const;
  SmartPtr<class MathMLNamespaceContext> getMathMLNamespaceContext(void) const;
  SmartPtr<class BoxMLNamespaceContext> getBoxMLNamespaceContext(void) const;

  SmartPtr<class Element> getRootElement(void) const;
  void resetRootElement(void);
  SmartPtr<const class Area> getRootArea(void) const;
  SmartPtr<const class Area> getElementArea(const SmartPtr<class Element>&) const;
  bool getElementExtents(const SmartPtr<class Element>&, scaled&, scaled&, BoundingBox&) const;
  bool getElementAt(const scaled&, const scaled&, SmartPtr<class Element>&) const;
  bool getCharExtents(const SmartPtr<class Element>&, int, scaled&, scaled&, BoundingBox&) const;
  bool getCharAt(const scaled&, const scaled&, SmartPtr<class Element>&, int&) const;

  void render(class RenderingContext&) const;

  unsigned getDefaultFontSize(void) const { return defaultFontSize; }
  void setDefaultFontSize(unsigned);

private:
  mutable SmartPtr<class Element> rootElement;
  SmartPtr<class Builder> builder;
  SmartPtr<class MathMLNamespaceContext> mathmlContext;
  SmartPtr<class BoxMLNamespaceContext> boxmlContext;
  unsigned defaultFontSize;
  unsigned freezeCounter;
  scaled x0;
  scaled y0;
};

#endif // __View_hh__
