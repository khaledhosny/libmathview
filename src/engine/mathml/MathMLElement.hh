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

#ifndef __MathMLElement_hh__
#define __MathMLElement_hh__

#include <bitset>

#include "Area.hh"
#include "BoundingBox.hh"
#include "MathMLFrame.hh"
#include "SmartPtr.hh"
#include "token.hh"

#include "Element.hh"

// MathMLElement: base class for every MathML Element
class MathMLElement : public MathMLFrame
{
protected:
  MathMLElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLElement();

public:
  virtual void setParent(const SmartPtr<MathMLElement>&);
#if 0
  void Link(const SmartPtr<MathMLElement>&);
  void Unlink(void);
#endif

  SmartPtr<class MathMLView> getView(void) const;
#if defined(HAVE_GMETADOM)
  DOM::Element getDOMElement(void) const { return element; }
  void setDOMElement(const DOM::Element&);
  SmartPtr<MathMLElement> getFormattingNode(const DOM::Element&) const;
#endif // HAVE_GMETADOM

  virtual void construct(void);
  virtual void refine(class AbstractRefinementContext&);
  virtual AreaRef format(class MathFormattingContext&);

protected:
  SmartPtr<class MathMLViewContext> getViewContext(void) const;
  SmartPtr<class MathMLDOMLinker> getLinker(void) const;
  SmartPtr<class MathMLFormattingEngineFactory> getFactory(void) const;
  SmartPtr<class Value> getAttributeValue(const class AttributeSignature&) const;
  SmartPtr<class Value> getAttributeValueNoDefault(const class AttributeSignature&) const;
  void refineAttribute(const class AbstractRefinementContext&, const class AttributeSignature&);

  void setArea(const AreaRef& a) { area = a; }

public:
  bool IsSet(TokenId) const;

  // some queries
  TokenId      	 IsA(void) const;
  virtual bool 	 IsSpaceLike(void) const;
  bool           hasLink(void) const;

  unsigned     	 GetDepth(void) const;
  virtual SmartPtr<class MathMLOperatorElement> GetCoreOperator(void);
  SmartPtr<class MathMLOperatorElement> GetCoreOperatorTop(void);

  AreaRef getArea(void) const { return area; }

  virtual void setDirtyStructure(void);
  void resetDirtyStructure(void) { ResetFlag(FDirtyStructure); }
  bool dirtyStructure(void) const { return GetFlag(FDirtyStructure); }
  virtual void setDirtyAttribute(void);
  virtual void setDirtyAttributeD(void);
  void resetDirtyAttribute(void)
  { ResetFlag(FDirtyAttribute); ResetFlag(FDirtyAttributeP); ResetFlag(FDirtyAttributeD); }
  bool dirtyAttribute(void) const { return GetFlag(FDirtyAttribute) || GetFlag(FDirtyAttributeD); }
  bool dirtyAttributeP(void) const { return GetFlag(FDirtyAttributeP); }
  bool dirtyAttributeD(void) const { return GetFlag(FDirtyAttributeD); }
  virtual void setDirtyLayout(void);
  void resetDirtyLayout(void) { ResetFlag(FDirtyLayout); }
  bool dirtyLayout(void) const { return GetFlag(FDirtyLayout); }

public:
  enum Flags {
    FDirtyStructure,  // need to resynchronize with DOM
    FDirtyAttribute,  // an attribute was modified
    FDirtyAttributeP, // an attribute was modified in a descendant
    FDirtyAttributeD, // an attribute was modified and must set dirtyAttribute on all descendants
    FDirtyLayout,     // need to layout

    FUnusedFlag       // Just to know how many flags we use without having to count them
  };

  void SetFlag(Flags f);// { flags.set(f); }
  void ResetFlag(Flags f) { flags.reset(f); }
  void SetFlagUp(Flags);
  void ResetFlagUp(Flags);
  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);
  bool GetFlag(Flags f) const { return flags.test(f); }

private:
  std::bitset<FUnusedFlag> flags;
  SmartPtr<class AttributeList> attributes;

protected:
  WeakPtr<class MathMLView> view;
  AreaRef area;

#if defined(HAVE_GMETADOM)
  DOM::Element element;
#endif // HAVE_GMETADOM
};

#endif // __MathMLElement_hh__
