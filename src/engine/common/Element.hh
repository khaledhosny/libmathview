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

#ifndef __Element_hh__
#define __Element_hh__

#include <bitset>

#include "Node.hh"
#include "WeakPtr.hh"
#include "Area.hh"

class GMV_MathView_EXPORT Element : public Node
{
protected:
  Element(const SmartPtr<class NamespaceContext>&);
  virtual ~Element();

public:
  static bool hasParentLink(void) { return true; }
  static void setParent(Element* self, const SmartPtr<Element>& el) { self->setParent(el); }

  SmartPtr<class NamespaceContext> getNamespaceContext(void) const;
  SmartPtr<class AbstractLogger> getLogger(void) const;

  void setParent(const SmartPtr<Element>&);
  SmartPtr<Element> getParent(void) const { return static_cast<Element*>(parent); }
  template <typename T> SmartPtr<T> getParent(void) const { return smart_cast<T>(getParent()); }
  unsigned getDepth(void) const;

  void setAttribute(const SmartPtr<class Attribute>&);
  void removeAttribute(const class AttributeSignature&);
  SmartPtr<class Attribute> getAttribute(const class AttributeSignature&) const;
  SmartPtr<class Value> getAttributeValue(const class AttributeSignature&) const;
  SmartPtr<class Value> getAttributeValueNoDefault(const class AttributeSignature&) const;

  void setArea(const AreaRef& a) { area = a; }
  AreaRef getArea(void) const { return area; }

  virtual AreaRef format(class FormattingContext&);

  virtual void setDirtyStructure(void);
  void resetDirtyStructure(void) { resetFlag(FDirtyStructure); }
  bool dirtyStructure(void) const { return getFlag(FDirtyStructure); }
  virtual void setDirtyAttribute(void);
  virtual void setDirtyAttributeD(void);
  void resetDirtyAttribute(void)
  { resetFlag(FDirtyAttribute); resetFlag(FDirtyAttributeP); resetFlag(FDirtyAttributeD); }
  bool dirtyAttribute(void) const { return getFlag(FDirtyAttribute) || getFlag(FDirtyAttributeD); }
  bool dirtyAttributeP(void) const { return getFlag(FDirtyAttributeP); }
  bool dirtyAttributeD(void) const { return getFlag(FDirtyAttributeD); }
  virtual void setDirtyLayout(void);
  virtual void setDirtyLayoutD(void);
  void resetDirtyLayout(void) { resetFlag(FDirtyLayout); }
  bool dirtyLayout(void) const { return getFlag(FDirtyLayout); }

  enum Flags {
    FDirtyStructure,  // need to resynchronize with DOM
    FDirtyAttribute,  // an attribute was modified
    FDirtyAttributeP, // an attribute was modified in a descendant
    FDirtyAttributeD, // an attribute was modified and must set dirtyAttribute on all descendants
    FDirtyLayout,     // need to layout

    FUnusedFlag       // Just to know how many flags we use without having to count them
  };

  void setFlag(Flags f);// { flags.set(f); }
  void resetFlag(Flags f) { flags.reset(f); }
  void setFlagUp(Flags);
  void resetFlagUp(Flags);
  virtual void setFlagDown(Flags);
  virtual void resetFlagDown(Flags);
  bool getFlag(Flags f) const { return flags.test(f); }

private:
  WeakPtr<class NamespaceContext> context;
  WeakPtr<Element> parent;
  std::bitset<FUnusedFlag> flags;
  SmartPtr<class AttributeSet> attributes;
  AreaRef area;
};

#endif // __Element_hh__
