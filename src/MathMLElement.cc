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

#include "Globals.hh"
#include "traverseAux.hh"
#include "DrawingArea.hh"
#include "MathMLElement.hh"
#include "MathMLDocument.hh"
#include "ValueConversion.hh"
#include "MathMLStyleElement.hh"
#include "MathMLAttributeList.hh"
#include "MathMLOperatorElement.hh"
#include "RenderingEnvironment.hh"
#include "FormattingContext.hh"
#include "AbstractRefinementContext.hh"
#include "Variant.hh"

#ifdef DEBUG
int MathMLElement::counter = 0;
#endif // DEBUG

MathMLElement::MathMLElement()
#if defined(HAVE_GMETADOM)
  : node(0)
#endif
{
  Init();
}

// MathMLElement: this is the base class for every MathML presentation element.
// It implements the basic skeleton of every such element, moreover it handles
// the attributes and provides some facility functions to access and parse
// attributes.
#if defined(HAVE_GMETADOM)
MathMLElement::MathMLElement(const DOM::Element& n)
  : node(n)
{
  Init();
}
#endif

void
MathMLElement::Init()
{
  SetDirtyStructure();
  SetDirtyAttribute();
  SetDirtyLayout();

  fGC[0] = fGC[1] = NULL;
  bGC[0] = bGC[1] = NULL;
  
  background = BLACK_COLOR;
}

MathMLElement::~MathMLElement()
{
  ReleaseGCs();
}

SmartPtr<Value>
MathMLElement::getAttributeValueNoDefault(const MathMLAttributeSignature& signature) const
{
  if (attributes)
    if (SmartPtr<Value> value = attributes->getValue(ATTRIBUTE_ID_OF_SIGNATURE(signature)))
      return value;

  return 0;
}

SmartPtr<Value>
MathMLElement::getAttributeValue(const MathMLAttributeSignature& signature) const
{
  if (attributes)
    if (SmartPtr<Value> value = getAttributeValueNoDefault(signature))
      return value;

  return signature.getDefaultValue();
}

void
MathMLElement::refineAttribute(const AbstractRefinementContext& context, const MathMLAttributeSignature& signature)
{
  if (!attributes) attributes = MathMLAttributeList::create();

  SmartPtr<MathMLAttribute> attr;

  if (signature.fromElement)
    if (DOM::Element elem = GetDOMElement())
      if (elem.hasAttribute(signature.name))
	attr = MathMLAttribute::create(signature, elem.getAttribute(signature.name));

  if (!attr && signature.fromContext)
    attr = context.get(signature);

  if (attr)
    {
      if (!attributes) attributes = MathMLAttributeList::create();
      if (attributes->set(attr)) SetDirtyLayout();
    }
  else
    if (attributes)
      if (attributes->remove(ATTRIBUTE_ID_OF_SIGNATURE(signature)))
	SetDirtyLayout();
}

bool
MathMLElement::IsSet(AttributeId id) const
{
#if defined(HAVE_GMETADOM)
  return node && node.hasAttribute(NameOfAttributeId(id));
#else // HAVE_GMETADOM
  return false;
#endif
}

void
MathMLElement::refine(AbstractRefinementContext& ctxt)
{
  // nothing to refine
  // reset flag
}

void
MathMLElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      background = env.GetBackgroundColor();
      ResetDirtyAttribute();
    }
}

void
MathMLElement::DoLayout(const FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt)) ResetDirtyLayout(ctxt);
}

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

bool
MathMLElement::IsSpaceLike() const
{
  return false;
}

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

bool
MathMLElement::HasLink() const
{
#if defined(HAVE_GMETADOM)
  DOM::Element p = GetDOMElement();
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

TagId
MathMLElement::IsA() const
{
  if (!node) return TAG_NOTVALID;

  std::string s_tag = nodeLocalName(node);
  TagId res = TagIdOfName(s_tag.c_str());

  return res;
}

void
MathMLElement::SetDirtyStructure()
{
  if (!DirtyStructure())
    {
      SetFlag(FDirtyStructure);
      SetFlagUp(FDirtyStructure);
    }
}

void
MathMLElement::SetDirtyAttribute()
{
  if (!DirtyAttribute())
    {
      SetFlag(FDirtyAttribute);
      SetFlagUp(FDirtyAttributeP);
    }
}

void
MathMLElement::SetDirtyAttributeD()
{
  if (!DirtyAttributeD())
    {
      SetFlagDown(FDirtyAttributeD);
      SetFlagUp(FDirtyAttributeP);
    }
}

void
MathMLElement::SetDirtyLayout()
{
  if (!DirtyLayout())
    {
      SetFlagDown(FDirtyLayout);
      SetFlagUp(FDirtyLayout);
    }
}

void
MathMLElement::SetFlag(Flags f)
{
  flags.set(f);
}

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
      SetFlagDown(FSelected);
      SetDirty();
    }
}

void
MathMLElement::ResetSelected()
{
  if (Selected())
    {
      ResetFlagDown(FSelected);
      SetDirty();
    }
}

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
MathMLElement::SetFlagDown(Flags f)
{
  SetFlag(f);
}

void
MathMLElement::ResetFlagDown(Flags f)
{
  ResetFlag(f);
}

void
MathMLElement::Unlink()
{
  // if the element is connected to another element, we remove it
  // from there first. This is to ensure that no node of the tree is shared
  if (SmartPtr<MathMLContainerElement> oldP = smart_cast<MathMLContainerElement>(GetParent()))
    {
      MathMLNode::SetParent(0); // this is to break the recursion!
      oldP->Remove(this);
    }
}

void
MathMLElement::Link(const SmartPtr<MathMLElement>& p)
{
  assert(p);
  assert(!GetParent());
  SetParent(p);
}

void
MathMLElement::SetParent(const SmartPtr<MathMLElement>& p)
{
  MathMLNode::SetParent(p);
  if (p)
    {
      if (DirtyStructure()) SetFlagUp(FDirtyStructure);
      if (DirtyAttribute()) SetFlagUp(FDirtyAttributeP);
      if (p->DirtyAttributeD()) SetFlagDown(FDirtyAttributeD);
      if (DirtyLayout()) SetFlagUp(FDirtyLayout);
      if (Dirty()) SetFlagUp(FDirty);
      if (p->DirtyLayout()) SetFlagDown(FDirtyLayout);
      if (p->Dirty()) SetFlagDown(FDirty);
      if (p->Selected()) SetFlagDown(FSelected);
    }
}
