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

#include <config.h>

#include <cassert>

#include "AbstractRefinementContext.hh"
#include "Globals.hh"
#include "AttributeList.hh"
#include "MathMLDOMLinker.hh"
#include "MathMLElement.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLStyleElement.hh"
#include "MathMLView.hh"
#include "ValueConversion.hh"
#include "Variant.hh"
#include "traverseAux.hh"

// MathMLElement: this is the base class for every MathML presentation element.
// It implements the basic skeleton of every such element, moreover it handles
// the attributes and provides some facility functions to access and parse
// attributes.
MathMLElement::MathMLElement(const SmartPtr<MathMLView>& v)
  : view(v)
{
  assert(view);

  setDirtyStructure();
  setDirtyAttribute();
  setDirtyLayout();

#if 0
  fGC[0] = fGC[1] = NULL;
  bGC[0] = bGC[1] = NULL;
  
  background = RGBColor::BLACK();
#endif
}

MathMLElement::~MathMLElement()
{
#if 0
  ReleaseGCs();
#endif
  setDOMElement(DOM::Element(0)); // it is important to invoke this method!
}

SmartPtr<Value>
MathMLElement::getAttributeValueNoDefault(const AttributeSignature& signature) const
{
  if (attributes)
    if (SmartPtr<Value> value = attributes->getValue(ATTRIBUTE_ID_OF_SIGNATURE(signature)))
      return value;

  return 0;
}

SmartPtr<Value>
MathMLElement::getAttributeValue(const AttributeSignature& signature) const
{
  if (attributes)
    if (SmartPtr<Value> value = getAttributeValueNoDefault(signature))
      return value;

  return signature.getDefaultValue();
}

void
MathMLElement::refineAttribute(const AbstractRefinementContext& context, const AttributeSignature& signature)
{
  if (!attributes) attributes = AttributeList::create();

  SmartPtr<Attribute> attr;

  if (signature.fromElement)
    if (DOM::Element elem = getDOMElement())
      if (elem.hasAttribute(signature.name))
	attr = Attribute::create(signature, elem.getAttribute(signature.name));

  if (!attr && signature.fromContext)
    attr = context.get(signature);

  if (attr)
    {
      if (!attributes) attributes = AttributeList::create();
      if (attributes->set(attr)) setDirtyLayout();
    }
  else
    if (attributes)
      if (attributes->remove(ATTRIBUTE_ID_OF_SIGNATURE(signature)))
	setDirtyLayout();
}

SmartPtr<MathMLView>
MathMLElement::getView() const
{
  return static_cast<MathMLView*>(view);
}

SmartPtr<MathMLViewContext>
MathMLElement::getViewContext() const
{
  return getView()->getContext();
}

SmartPtr<MathMLDOMLinker>
MathMLElement::getLinker() const
{
  return getViewContext()->linker;
}

SmartPtr<MathMLFormattingEngineFactory>
MathMLElement::getFactory() const
{
  return getViewContext()->engineFactory;
}

#if defined(HAVE_GMETADOM)
void
MathMLElement::setDOMElement(const DOM::Element& el)
{
  if (el != element)
    {
      if (element) getLinker()->remove(element);
      if (el) getLinker()->remove(el);
      element = el;
      if (element) getLinker()->add(element, this);
    }
}

SmartPtr<MathMLElement>
MathMLElement::getFormattingNode(const DOM::Element& elem) const
{
  assert(elem);

  if (SmartPtr<MathMLElement> el = getLinker()->get(elem))
    return el;

  SmartPtr<MathMLElement> res = getFactory()->createElement(getView(), nodeLocalName(elem));
  assert(res);
  res->setDOMElement(elem);
  return res;
}

#endif // HAVE_GMETADOM

bool
MathMLElement::IsSet(TokenId id) const
{
#if defined(HAVE_GMETADOM)
  return getDOMElement() && getDOMElement().hasAttribute(stringOfTokenId(id));
#else // HAVE_GMETADOM
  return false;
#endif
}

void
MathMLElement::construct(AbstractConstructionContext&)
{
  // nothing to construct
  resetDirtyStructure();
}

void
MathMLElement::construct()
{
  // nothing to construct
  resetDirtyStructure();
}

void
MathMLElement::refine(class AbstractRefinementContext&)
{
  // nothing to refine
  resetDirtyAttribute();
}

#if 0
void
MathMLElement::Setup(RenderingEnvironment& env)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      background = env.GetBackgroundColor();
      resetDirtyAttribute();
    }
}

void
MathMLElement::DoLayout(const FormattingContext& ctxt)
{
  if (dirtyLayout(ctxt)) resetDirtyLayout(ctxt);
}
#endif

AreaRef
MathMLElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      area = 0;
      resetDirtyLayout();
    }

  return getArea();
}

#if 0
void
MathMLElement::RenderBackground(const DrawingArea& area)
{
  // FIXME ???? true????
  if (true||bGC[Selected()] == NULL)
    {
      GraphicsContextValues values;
      values.background = values.foreground = Selected() ? area.GetSelectionBackground() : background;
      bGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
    }

  if (DirtyBackground()) area.Clear(bGC[Selected()], GetX(), GetY(), GetBoundingBox());
}

void
MathMLElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      RenderBackground(area);
      ResetDirty();
    }
}
#endif

bool
MathMLElement::IsSpaceLike() const
{
  return false;
}

#if 0
bool
MathMLElement::IsInside(const scaled& x, const scaled& y) const
{
  return GetRectangle().IsInside(x, y);
}

SmartPtr<MathMLElement>
MathMLElement::Inside(const scaled& x, const scaled& y)
{
  return IsInside(x, y) ? this : 0;
}
#endif

unsigned
MathMLElement::GetDepth() const
{
  unsigned depth = 0;
  SmartPtr<const MathMLElement> p = this;
  
  while (p)
    {
      depth++;
      p = p->GetParent();
    }

  return depth;
}

#if 0
scaled
MathMLElement::GetLeftEdge() const
{
  return GetX();
}

scaled
MathMLElement::GetRightEdge() const
{
  return GetX();
}

void
MathMLElement::ReleaseGCs()
{
  fGC[0] = fGC[1] = NULL;
  bGC[0] = bGC[1] = NULL;
}
#endif

bool
MathMLElement::hasLink() const
{
#if defined(HAVE_GMETADOM)
  DOM::Element p = getDOMElement();
  return p && p.hasAttribute("href");
#else // HAVE_GMETADOM
  return false;
#endif
}

SmartPtr<MathMLOperatorElement>
MathMLElement::GetCoreOperator()
{
  return 0;
}

SmartPtr<MathMLOperatorElement>
MathMLElement::GetCoreOperatorTop()
{
  if (SmartPtr<MathMLOperatorElement> coreOp = GetCoreOperator())
    if (!GetParent() || GetParent()->GetCoreOperator() != coreOp)
      return coreOp;
  return 0;
}

TokenId
MathMLElement::IsA() const
{
  if (DOM::Element el = getDOMElement())
    return tokenIdOfString(std::string(nodeLocalName(el)).c_str());
  else
    return T__NOTVALID;
}

void
MathMLElement::setDirtyStructure()
{
  if (!dirtyStructure())
    {
      SetFlag(FDirtyStructure);
      SetFlagUp(FDirtyStructure);
    }
}

void
MathMLElement::setDirtyAttribute()
{
  if (!dirtyAttribute())
    {
      SetFlag(FDirtyAttribute);
      SetFlagUp(FDirtyAttributeP);
    }
}

void
MathMLElement::setDirtyAttributeD()
{
  if (!dirtyAttributeD())
    {
      setFlagDown(FDirtyAttributeD);
      SetFlagUp(FDirtyAttributeP);
    }
}

void
MathMLElement::setDirtyLayout()
{
  if (!dirtyLayout())
    {
      setFlagDown(FDirtyLayout);
      SetFlagUp(FDirtyLayout);
    }
}

void
MathMLElement::SetFlag(Flags f)
{
  flags.set(f);
}

#if 0
void
MathMLElement::SetDirty(const Rectangle* rect)
{
  // FIXME ???true???
  if (true || !Dirty())
    {
      
      if (!rect || GetRectangle().Overlaps(*rect))
	{
	  SetFlag(FDirty);
	  //SetFlagUp(FDirtyP);
	}
    }
}

void
MathMLElement::SetSelected()
{
  if (!Selected())
    {
      setFlagDown(FSelected);
      SetDirty();
    }
}

void
MathMLElement::ResetSelected()
{
  if (Selected())
    {
      resetFlagDown(FSelected);
      SetDirty();
    }
}
#endif

void
MathMLElement::SetFlagUp(Flags f)
{
  for (SmartPtr<MathMLElement> p = GetParent(); p && !p->GetFlag(f); p = p->GetParent())
    p->SetFlag(f);
}

void
MathMLElement::ResetFlagUp(Flags f)
{
  for (SmartPtr<MathMLElement> p = GetParent(); p && p->GetFlag(f); p = p->GetParent())
    p->ResetFlag(f);
}

void
MathMLElement::setFlagDown(Flags f)
{
  SetFlag(f);
}

void
MathMLElement::resetFlagDown(Flags f)
{
  ResetFlag(f);
}

#if 0
void
MathMLElement::Unlink()
{
  // if the element is connected to another element, we remove it
  // from there first. This is to ensure that no node of the tree is shared
  if (SmartPtr<MathMLContainerElement> oldP = smart_cast<MathMLContainerElement>(GetParent()))
    {
      MathMLNode::setParent(0); // this is to break the recursion!
      oldP->Remove(this);
    }
}

void
MathMLElement::Link(const SmartPtr<MathMLElement>& p)
{
  assert(p);
  assert(!GetParent());
  setParent(p);
}
#endif

// FIXME: when we'll have a real Node class we could move
// these operations in it and make setParent non-virtual
void
MathMLElement::setParent(const SmartPtr<MathMLElement>& p)
{
  MathMLNode::setParent(p);
  if (p)
    {
      if (dirtyStructure()) SetFlagUp(FDirtyStructure);
      if (dirtyAttribute()) SetFlagUp(FDirtyAttributeP);
      if (p->dirtyAttributeD()) setFlagDown(FDirtyAttributeD);
      if (dirtyLayout()) SetFlagUp(FDirtyLayout);
#if 0
      if (Dirty()) SetFlagUp(FDirty);
#endif
      if (p->dirtyLayout()) setFlagDown(FDirtyLayout);
#if 0
      if (p->Dirty()) setFlagDown(FDirty);
      if (p->Selected()) setFlagDown(FSelected);
#endif
    }
}
