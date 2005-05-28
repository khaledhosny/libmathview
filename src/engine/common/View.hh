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

#include "Area.hh"
#include "Object.hh"
#include "String.hh"
#include "SmartPtr.hh"
#include "BoundingBox.hh"

class View : public Object
{
protected:
  View(void);
  virtual ~View();

public:
  virtual void unload(void) = 0;
  bool frozen(void) const { return freezeCounter > 0; }
  bool freeze(void);
  bool thaw(void);

  // configuration stuff
  static String getDefaultConfigurationPath(void);
  static String getDefaultOperatorDictionaryPath(void);

  void setLogger(const SmartPtr<class AbstractLogger>&);
  SmartPtr<class AbstractLogger> getLogger(void) const;
  void setOperatorDictionary(const SmartPtr<class MathMLOperatorDictionary>&);
  SmartPtr<class MathMLOperatorDictionary> getOperatorDictionary(void) const;
  void setBuilder(const SmartPtr<class Builder>&);
  SmartPtr<class Builder> getBuilder(void) const;
  void setMathMLNamespaceContext(const SmartPtr<class MathMLNamespaceContext>&);
  SmartPtr<class MathMLNamespaceContext> getMathMLNamespaceContext(void) const;
#if ENABLE_BOXML
  void setBoxMLNamespaceContext(const SmartPtr<class BoxMLNamespaceContext>&);
  SmartPtr<class BoxMLNamespaceContext> getBoxMLNamespaceContext(void) const;
#endif // ENABLE_BOXML

  SmartPtr<class Element> getRootElement(void) const;
  void resetRootElement(void);
  BoundingBox getBoundingBox(void) const;

  SmartPtr<class Element> getElementAt(const scaled&, const scaled&, Point* = 0, BoundingBox* = 0) const;
  bool getElementExtents(const SmartPtr<class Element>&, const SmartPtr<class Element>&, Point* = 0, BoundingBox* = 0) const;
  bool getElementExtents(const SmartPtr<class Element>&, Point* = 0, BoundingBox* = 0) const;
  bool getElementLength(const SmartPtr<class Element>&, CharIndex&) const;
  bool getElementOrigin(const SmartPtr<class Element>& elem, Point& p) const
  { return getElementExtents(elem, &p, 0); }
  bool getElementOrigin(const SmartPtr<class Element>& refElem, const SmartPtr<class Element>& elem, Point& p) const
  { return getElementExtents(refElem, elem, &p, 0); }
  bool getElementBoundingBox(const SmartPtr<class Element>& elem, BoundingBox& b) const
  { return getElementExtents(elem, 0, &b); }

  SmartPtr<class Element> getCharAt(const scaled&, const scaled&, CharIndex&, Point* = 0, BoundingBox* = 0) const;
  bool getCharExtents(const SmartPtr<class Element>&, const SmartPtr<class Element>&, CharIndex,
		      Point* = 0, BoundingBox* = 0) const;
  bool getCharExtents(const SmartPtr<class Element>&, CharIndex, Point* = 0, BoundingBox* = 0) const;
  bool getCharOrigin(const SmartPtr<class Element>& elem, CharIndex index, Point& p) const
  { return getCharExtents(elem, index, &p, 0); }
  bool getCharBoundingBox(const SmartPtr<class Element>& elem, CharIndex index, BoundingBox& b) const
  { return getCharExtents(elem, index, 0, &b); }

  void render(class RenderingContext&, const scaled&, const scaled&) const;

  unsigned getDefaultFontSize(void) const { return defaultFontSize; }
  void setDefaultFontSize(unsigned);

  scaled getAvailableWidth(void) const { return availableWidth; }
  void setAvailableWidth(const scaled&);

protected:
  SmartPtr<const class Area> getRootArea(void) const;
  void setDirtyLayout(void) const;
  SmartPtr<const class Area> formatElement(const SmartPtr<class Element>&) const;

private:
  mutable SmartPtr<class Element> rootElement;
  SmartPtr<class AbstractLogger> logger;
  SmartPtr<class MathMLOperatorDictionary> dictionary;
  SmartPtr<class Builder> builder;
  SmartPtr<class MathMLNamespaceContext> mathmlContext;
#if ENABLE_BOXML
  SmartPtr<class BoxMLNamespaceContext> boxmlContext;
#endif // ENABLE_BOXML
  unsigned defaultFontSize;
  unsigned freezeCounter;
  scaled availableWidth;
};

#endif // __View_hh__
