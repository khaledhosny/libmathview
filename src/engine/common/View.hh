// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __View_hh__
#define __View_hh__

#include "Object.hh"
#include "String.hh"
#include "SmartPtr.hh"
#include "BoundingBox.hh"
// Area.hh moved down here for Win32 build
#include "Area.hh"

class GMV_MathView_EXPORT View : public Object
{
protected:
  View(const SmartPtr<class AbstractLogger>&);
  virtual ~View();

public:
  virtual void unload(void) = 0;
  bool frozen(void) const { return freezeCounter > 0; }
  bool freeze(void);
  bool thaw(void);

  // configuration stuff
  static String getDefaultConfigurationPath(void);
  static String getDefaultOperatorDictionaryPath(void);

  SmartPtr<class AbstractLogger> getLogger(void) const;
  void setOperatorDictionary(const SmartPtr<class MathMLOperatorDictionary>&);
  SmartPtr<class MathMLOperatorDictionary> getOperatorDictionary(void) const;
  void setBuilder(const SmartPtr<class Builder>&);
  SmartPtr<class Builder> getBuilder(void) const;
  void setMathMLNamespaceContext(const SmartPtr<class MathMLNamespaceContext>&);
  SmartPtr<class MathMLNamespaceContext> getMathMLNamespaceContext(void) const;
#if GMV_ENABLE_BOXML
  void setBoxMLNamespaceContext(const SmartPtr<class BoxMLNamespaceContext>&);
  SmartPtr<class BoxMLNamespaceContext> getBoxMLNamespaceContext(void) const;
#endif // GMV_ENABLE_BOXML

  void setDirtyLayout(void) const;
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
  SmartPtr<const class Area> formatElement(const SmartPtr<class Element>&) const;

private:
  mutable SmartPtr<class Element> rootElement;
  SmartPtr<class AbstractLogger> logger;
  SmartPtr<class MathMLOperatorDictionary> dictionary;
  SmartPtr<class Builder> builder;
  SmartPtr<class MathMLNamespaceContext> mathmlContext;
#if GMV_ENABLE_BOXML
  SmartPtr<class BoxMLNamespaceContext> boxmlContext;
#endif // GMV_ENABLE_BOXML
  unsigned defaultFontSize;
  unsigned freezeCounter;
  scaled availableWidth;
};

#endif // __View_hh__
