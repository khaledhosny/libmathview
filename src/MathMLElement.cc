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
#include <stdio.h>

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

// GetAttributeSignatureAux: this is an auxiliary function used to retrieve
// the signature of the attribute with id ID given an array of attribute
// signatures.
const AttributeSignature*
MathMLElement::GetAttributeSignatureAux(AttributeId id,
					AttributeSignature sig[]) const
{
  for (unsigned i = 0; sig[i].GetAttributeId() != ATTR_NOTVALID; i++)
    if (sig[i].GetAttributeId() == id) return &sig[i];

  return NULL;
}

// GetAttributeSignature: return the attribute signature of ID.
// Here are the attributes common to all (presentation) elements of MathML
const AttributeSignature*
MathMLElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_ID,         NULL, NULL, NULL },
    { ATTR_CLASS,      NULL, NULL, NULL },
    { ATTR_OTHER,      NULL, NULL, NULL },

    { ATTR_NOTVALID,   NULL, NULL, NULL }
  };

  return GetAttributeSignatureAux(id, sig);
}

String
MathMLElement::GetDefaultAttribute(AttributeId id) const
{
  const AttributeSignature* aSignature = GetAttributeSignature(id);
  assert(aSignature);
  return aSignature->GetDefaultValue();
}

SmartPtr<Value>
MathMLElement::GetDefaultAttributeValue(AttributeId id) const
{
  const AttributeSignature* aSignature = GetAttributeSignature(id);
  assert(aSignature);
  return aSignature->GetDefaultParsedValue();
}

String
MathMLElement::GetAttribute(AttributeId id, bool searchDefault) const
{
  // if this element is not connected with a DOM element
  // then it cannot have attributes. This may happen for
  // elements inferred with normalization
#if defined(HAVE_GMETADOM)
  if (node && node.hasAttribute(NameOfAttributeId(id)))
    return fromDOMString(node.getAttribute(NameOfAttributeId(id)));
#endif // HAVE_GMETADOM

  if (searchDefault) return GetDefaultAttribute(id);
  assert(false);
  return "";
}

String
MathMLElement::GetAttribute(AttributeId id,
			    const RenderingEnvironment& env,
			    bool searchDefault) const
{
  // if this element is not connected with a DOM element
  // then it cannot have attributes. This may happen for
  // elements inferred with normalization
#if defined(HAVE_GMETADOM)
  if (node && node.hasAttribute(NameOfAttributeId(id)))
    return fromDOMString(node.getAttribute(NameOfAttributeId(id)));
#endif // HAVE_GMETADOM

  if (const MathMLAttribute* attr = env.GetAttribute(id))
    return attr->GetValue();

  if (searchDefault) return GetDefaultAttribute(id);
  assert(false);
  return "";
}

SmartPtr<Value>
MathMLElement::GetAttributeValue(AttributeId id, bool searchDefault) const
{
  SmartPtr<Value> value;

  const AttributeSignature* aSignature = GetAttributeSignature(id);
  assert(aSignature);

#if defined(HAVE_GMETADOM)
  if (node && node.hasAttribute(NameOfAttributeId(id)))
    {
      AttributeParser parser = aSignature->GetParser();
      assert(parser);

      StringTokenizer st(fromDOMString(node.getAttribute(NameOfAttributeId(id))));
      value = parser(st);

      if (!value)
	Globals::logger(LOG_WARNING, "in element `%s' parsing error in attribute `%s'",
			NameOfTagId(IsA()), NameOfAttributeId(id));
    }
#endif // HAVE_GMETADOM

  if (!value && searchDefault) value = GetDefaultAttributeValue(id);

  return value;
}

SmartPtr<Value>
MathMLElement::GetAttributeValue(AttributeId id, 
				 const RenderingEnvironment& env,
				 bool searchDefault) const
{
  if (SmartPtr<Value> value = GetAttributeValue(id, false))
    return value;

  const AttributeSignature* aSignature = GetAttributeSignature(id);
  assert(aSignature);
  const MathMLAttribute* attr = env.GetAttribute(id);
  if (attr)
    if (SmartPtr<Value> value = attr->GetParsedValue(aSignature))
      return value;

  if (searchDefault)
    return GetDefaultAttributeValue(id);

  return 0;
}

SmartPtr<Value>
MathMLElement::Resolve(const SmartPtr<Value>& value,
		       const RenderingEnvironment& env,
		       int i, int j)
{
  assert(value);

  SmartPtr<Value> realValue = GetComponent(value, i, j);
  assert(realValue);

  if (IsKeyword(value))
    return Variant<UnitValue>::create(env.GetMathSpace(ToNamedSpaceId(value)));
  else
    return realValue;
}

bool
MathMLElement::IsSet(AttributeId id) const
{
#if defined(HAVE_GMETADOM)
  if (!node) return false;
  return node.hasAttribute(NameOfAttributeId(id));
#else // HAVE_GMETADOM
  return false;
#endif
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
//   if (IsA() == TAG_MSTYLE)
//     printf("=== %s %d %06x\n", NameOfTagId(IsA()), DirtyBackground(), background);

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

#if 0
void
MathMLElement::SetDirtyStructure()
{
  dirtyStructure = 1;
  
  SmartPtr<MathMLElement> parent = GetParent();
  while (parent)
    {
      parent->childWithDirtyStructure = 1;
      parent = parent->GetParent();
    }
}

void
MathMLElement::SetDirtyAttribute()
{
  dirtyAttribute = 1;

  SmartPtr<MathMLElement> parent = GetParent();
  while (parent)
    {
      parent->childWithDirtyAttribute = 1;
      parent = parent->GetParent();
    }
}

void
MathMLElement::SetDirtyChildren()
{
  if (HasDirtyChildren()) return;
  dirtyChildren = 1;
  for (SmartPtr<MathMLElement> elem = GetParent(); 
       elem && !elem->HasDirtyChildren(); 
       elem = elem->GetParent())
    elem->dirtyChildren = 1;
}

void
MathMLElement::SetDirtyLayout(bool)
{
  if (HasDirtyLayout()) return;
  dirtyLayout = 1;
  for (SmartPtr<MathMLElement> elem = GetParent(); 
       elem && !elem->dirtyLayout; 
       elem = elem->GetParent())
    elem->dirtyLayout = 1;
}

void
MathMLElement::SetSelected()
{
  if (IsSelected()) return;
  selected = 1;
  SetDirty();
}

void
MathMLElement::ResetSelected()
{
  if (!IsSelected()) return;
  SetDirty();
  selected = 0;
}
#endif

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
  if (true || !Dirty())
    {
//       if (rect)
// 	{
// 	  printf("%s setdirty ", NameOfTagId(IsA()));
// 	  GetRectangle().Dump();
// 	  rect->Dump();
// 	  printf(" overlap %d\n", GetRectangle().Overlaps(*rect));
// 	}
      
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
{ Link(0); }

void
MathMLElement::Link(const SmartPtr<MathMLElement>& p)
{
  // if the element is connected to another element, we remove it
  // from there first. This is to ensure that no node of the tree is shared
  if (SmartPtr<MathMLContainerElement> oldP = smart_cast<MathMLContainerElement>(GetParent()))
    {
      MathMLNode::SetParent(0); // this is to break the recursion!
      oldP->Remove(this);
    }
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
