// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef MathMLElement_hh
#define MathMLElement_hh

#include <gdome.h>

#include "keyword.hh"
#include "MathMLFrame.hh"
#include "BoundingBox.hh"
#include "DrawingArea.hh"
#include "AttributeSignature.hh"

// MathMLElement: base class for every MathML Element
class MathMLElement: public MathMLFrame {
public:
  MathMLElement(GdomeElement* = NULL, TagId = TAG_NOTVALID);
  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Normalize(void);
  virtual void Setup(class RenderingEnvironment*); // setup attributes
  void         DoBoxedLayout(LayoutId id) { DoBoxedLayout(id, (id == LAYOUT_MAX) ? BREAK_NO : BREAK_GOOD); }
  void         DoBoxedLayout(LayoutId id, BreakId bid) { DoBoxedLayout(id, bid, 0); }
  void         DoBoxedLayout(LayoutId id, scaled w) { DoBoxedLayout(id, BREAK_GOOD, w); }
  virtual void DoBoxedLayout(LayoutId, BreakId, scaled);
  virtual void DoLayout(LayoutId, class Layout&);   // layout as breakable
  virtual void DoStretchyLayout(void);          // layout stretchy components
  virtual void SetPosition(scaled, scaled);
  virtual void SetPosition(scaled, scaled, ColumnAlignId);
  virtual void Freeze(void);
  virtual void RenderBackground(const DrawingArea&);
  virtual void Render(const DrawingArea&);
  virtual void ReleaseGCs(void);
  virtual void SetDirty(const Rectangle* = NULL);
  virtual MathMLElement* Inside(scaled, scaled);
  virtual bool IsElement(void) const;
  virtual ~MathMLElement();

  const class GraphicsContext* GetForegroundGC(void) const { return fGC[IsSelected()]; }
  const class GraphicsContext* GetBackgroundGC(void) const { return bGC[IsSelected()]; }

  // attributes
  const String* GetDefaultAttribute(AttributeId) const;
  const Value*  GetDefaultAttributeValue(AttributeId) const;
  const String* GetAttribute(AttributeId,
			     const RenderingEnvironment* = NULL,
			     bool = true) const;
  const Value*  GetAttributeValue(AttributeId,
				  const RenderingEnvironment* = NULL,
				  bool = true) const;
  const Value*  ParseAttribute(AttributeId, const String*) const;
  static const Value* Resolve(const Value*,
			      const RenderingEnvironment*,
			      int = -1, int = -1);
  bool IsSet(AttributeId) const;

  // bounding boxes
  const BoundingBox& GetMinBoundingBox(void) const { return minBox; }
  const BoundingBox& GetMaxBoundingBox(void) const { return maxBox; }
  virtual void   GetLinearBoundingBox(BoundingBox&) const;
  virtual void   RecalcBoundingBox(LayoutId, scaled = 0);

  // some queries
  TagId        	 IsA(void) const { return tag; }
  GdomeElement*  GetDOMNode(void) const { return node; }
  virtual bool 	 IsSpaceLike(void) const;
  virtual bool 	 IsExpanding(void) const;
  virtual bool 	 IsInside(scaled, scaled) const;
  bool         	 HasLayout(void) const { return layout != NULL; }
  bool         	 IsShaped(void) const { return shape != NULL; }
  bool           HasLink(void) const;
  RGBValue     	 GetBackgroundColor(void) const { return background; }
  unsigned     	 GetDepth(void) const;
  const Layout&  GetLayout(void) const;
  const Shape&   GetShape(void) const;
  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;

  bool HasDirtyLayout(void) const { return MathMLFrame::HasDirtyLayout(); }
  void ResetDirtyLayout(void) { MathMLFrame::ResetDirtyLayout(); }
  bool HasDirtyLayout(LayoutId, scaled) const;
  void ResetDirtyLayout(LayoutId, scaled);
  void ResetDirtyLayout(LayoutId);

#ifdef DEBUG
  static int GetCounter(void) { return counter; }
#endif // DEBUG

protected:
  void ResetLayout(void);
  const AttributeSignature* GetAttributeSignatureAux(AttributeId,
						     AttributeSignature[]) const;
  void ConfirmLayout(LayoutId);

  scaled      lastLayoutWidth;
  BoundingBox minBox;
  BoundingBox maxBox;
  class Layout* layout;
  const class Shape* shape;

  const class GraphicsContext* fGC[2];
  const class GraphicsContext* bGC[2];

  RGBValue background; // background color

private:
  GdomeElement* node; // reference to the DOM node
  TagId         tag;

#ifdef DEBUG
  static int counter;
#endif // DEBUG
};

typedef MathMLElement* MathMLElementPtr;

#define TO_ELEMENT(object) (dynamic_cast<MathMLElement*>(object))

inline void
MathMLElement::ConfirmLayout(LayoutId id)
{
  switch (id) {
  case LAYOUT_AUTO:
    break;
  case LAYOUT_MIN:
    minBox = box;
    break;
  case LAYOUT_MAX:
    maxBox = box;
    break;
  }
}

#endif // MathMLElement_hh
