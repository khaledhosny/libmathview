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

#include <bitset>

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "keyword.hh"
#include "MathMLFrame.hh"
#include "BoundingBox.hh"
#include "DrawingArea.hh"
#include "AttributeSignature.hh"
#include "FormattingContext.hh"

// MathMLElement: base class for every MathML Element
class MathMLElement: public MathMLFrame
{
protected:
  MathMLElement(void);
#if defined(HAVE_GMETADOM)
  MathMLElement(const DOM::Element&);
#endif
  virtual ~MathMLElement();
private:
  void Init(void);

public:
  virtual void SetParent(const Ptr<MathMLElement>&);

  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void Normalize(const Ptr<class MathMLDocument>&) = 0;
  virtual void Setup(class RenderingEnvironment&); // setup attributes
  virtual void DoLayout(const class FormattingContext&);
  virtual void RenderBackground(const DrawingArea&);
  virtual void Render(const DrawingArea&);
  virtual void ReleaseGCs(void);
  virtual Ptr<MathMLElement> Inside(scaled, scaled);

  const class GraphicsContext* GetForegroundGC(void) const { return fGC[Selected()]; }
  const class GraphicsContext* GetBackgroundGC(void) const { return bGC[Selected()]; }

  // attributes
  const String* GetDefaultAttribute(AttributeId) const;
  const Value*  GetDefaultAttributeValue(AttributeId) const;
  const String* GetAttribute(AttributeId, bool = true) const;
  const String* GetAttribute(AttributeId, const RenderingEnvironment&, bool = true) const;
  const Value*  GetAttributeValue(AttributeId, bool = true) const;
  const Value*  GetAttributeValue(AttributeId, const RenderingEnvironment&, bool = true) const;
  const Value*  ParseAttribute(AttributeId, const String*) const;
  static const Value* Resolve(const Value*, const RenderingEnvironment&, int = -1, int = -1);
  bool IsSet(AttributeId) const;

  // some queries
  TagId        	 IsA(void) const;
#if defined(HAVE_GMETADOM)
  const DOM::Element& GetDOMElement(void) const { return node; }
  static Ptr<MathMLElement> getRenderingInterface(const DOM::Element&);
#endif
  virtual bool 	 IsSpaceLike(void) const;
  virtual bool 	 IsInside(scaled, scaled) const;
  bool           HasLink(void) const;
  RGBValue     	 GetBackgroundColor(void) const { return background; }
  unsigned     	 GetDepth(void) const;
  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;
  virtual Ptr<class MathMLOperatorElement> GetCoreOperator(void);
  Ptr<class MathMLOperatorElement> GetCoreOperatorTop(void);

  bool DirtyBackground(void) const
  {
    return GetParent() && ((Selected() && !GetParent()->Selected()) ||
			   (background != GetParent()->background));
  }

  bool DirtyLayout(const class FormattingContext&) const { return DirtyLayout(); }
  void ResetDirtyLayout(const FormattingContext& ctxt)
  { if (ctxt.GetLayoutType() == LAYOUT_AUTO) ResetDirtyLayout(); }

protected:
  const AttributeSignature* GetAttributeSignatureAux(AttributeId,
						     AttributeSignature[]) const;

public:
  virtual void SetDirtyStructure(void);
  void ResetDirtyStructure(void) { ResetFlag(FDirtyStructure); }
  bool DirtyStructure(void) const { return GetFlag(FDirtyStructure); }
  virtual void SetDirtyAttribute(void);
  virtual void SetDirtyAttributeD(void);
  void ResetDirtyAttribute(void)
  { ResetFlag(FDirtyAttribute); ResetFlag(FDirtyAttributeP); ResetFlag(FDirtyAttributeD); }
  bool DirtyAttribute(void) const { return GetFlag(FDirtyAttribute) || GetFlag(FDirtyAttributeD); }
  bool DirtyAttributeP(void) const { return GetFlag(FDirtyAttributeP); }
  bool DirtyAttributeD(void) const { return GetFlag(FDirtyAttributeD); }
  virtual void SetDirtyLayout(void);
  void ResetDirtyLayout(void) { ResetFlag(FDirtyLayout); }
  bool DirtyLayout(void) const { return GetFlag(FDirtyLayout); }
  virtual void SetDirty(const Rectangle* = 0);
  void ResetDirty(void) { ResetFlag(FDirty); }
  bool Dirty(void) const { return GetFlag(FDirty); }
  virtual void SetSelected(void);
  void ResetSelected(void);
  bool Selected(void) const { return GetFlag(FSelected); }

public:
  enum Flags {
    FDirtyStructure,  // need to resynchronize with DOM
    FDirtyAttribute,  // an attribute was modified
    FDirtyAttributeP, // an attribute was modified in a descendant
    FDirtyAttributeD, // an attribute was modified and must set DirtyAttribute on all descendants
    FDirtyLayout,     // need to layout
    FDirty,           // need to render
    FDirtyP,          // need to render a descendant
    FSelected,        // selected subtree

    FUnusedFlag       // Just to know how many flags we use without having to count them
  };

  void SetFlag(Flags f);// { flags.set(f); }
  void ResetFlag(Flags f) { flags.reset(f); }
  void SetFlagUp(Flags);
  void ResetFlagUp(Flags);
  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);
  bool GetFlag(Flags f) const { return flags.test(f); }

private:
  std::bitset<FUnusedFlag> flags;

protected:
  const class GraphicsContext* fGC[2];
  const class GraphicsContext* bGC[2];

  RGBValue background; // background color

private:
#if defined(HAVE_GMETADOM)
  const DOM::Element node; // reference to the DOM node
#endif
};

#endif // MathMLElement_hh
