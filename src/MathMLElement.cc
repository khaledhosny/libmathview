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

#include <config.h>
#include <assert.h>
#include <stdio.h>

#include "stringAux.hh"
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

const String*
MathMLElement::GetDefaultAttribute(AttributeId id) const
{
  const AttributeSignature* aSignature = GetAttributeSignature(id);
  assert(aSignature != NULL);
  return aSignature->GetDefaultValue();
}

const Value*
MathMLElement::GetDefaultAttributeValue(AttributeId id) const
{
  const AttributeSignature* aSignature = GetAttributeSignature(id);
  assert(aSignature != NULL);
  return aSignature->GetDefaultParsedValue();
}

const String*
MathMLElement::GetAttribute(AttributeId id, bool searchDefault) const
{
  const String* sValue = 0;

  // if this element is not connected with a DOM element
  // then it cannot have attributes. This may happen for
  // elements inferred with normalization
#if defined(HAVE_MINIDOM)
  if (node != 0)
    {
      mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING(NameOfAttributeId(id)));
      if (value != 0)
	{
	  sValue = allocString(value);
	  mdom_string_free(value);
	}
    }
#elif defined(HAVE_GMETADOM)
  if (node)
    {
      DOM::GdomeString value = node.getAttribute(NameOfAttributeId(id));
      if (!value.empty()) sValue = allocString(value);
    }
#endif // HAVE_GMETADOM

  if (sValue == 0 && searchDefault) sValue = GetDefaultAttribute(id);

  return sValue;
}

const String*
MathMLElement::GetAttribute(AttributeId id,
			    const RenderingEnvironment& env,
			    bool searchDefault) const
{
  const String* sValue = GetAttribute(id, false);

  if (sValue == 0)
    {
      const MathMLAttribute* attr = env.GetAttribute(id);
      if (attr != 0) sValue = attr->GetValue();
    }

  if (sValue == 0 && searchDefault) sValue = GetDefaultAttribute(id);

  return sValue;
}

const Value*
MathMLElement::GetAttributeValue(AttributeId id, bool searchDefault) const
{
  const Value* value = NULL;

  const AttributeSignature* aSignature = GetAttributeSignature(id);
  assert(aSignature != NULL);

  const String* sValue = NULL;

#if defined(HAVE_MINIDOM)
  if (node != 0)
    {
      mDOMStringRef value = mdom_node_get_attribute(node,
						    DOM_CONST_STRING(NameOfAttributeId(id)));
      if (value != 0) {
	sValue = allocString(value);
	mdom_string_free(value);
      }
    }
#elif defined(HAVE_GMETADOM)
  if (node)
    {
      DOM::GdomeString value = node.getAttribute(NameOfAttributeId(id));
      if (!value.empty()) sValue = allocString(value);
    }
#endif // HAVE_GMETADOM

  if (sValue != 0)
    {
      AttributeParser parser = aSignature->GetParser();
      assert(parser != 0);

      StringTokenizer st(*sValue);
      value = parser(st);

      if (value == 0)
	Globals::logger(LOG_WARNING, "in element `%s' parsing error in attribute `%s'",
			NameOfTagId(IsA()), NameOfAttributeId(id));

      delete sValue;
      sValue = 0;
    } 

  if (value == 0 && searchDefault) value = GetDefaultAttributeValue(id);

  return value;
}

const Value*
MathMLElement::GetAttributeValue(AttributeId id, 
				 const RenderingEnvironment& env,
				 bool searchDefault) const
{
  const Value* value = GetAttributeValue(id, false);

  if (value == 0)
    {
      const AttributeSignature* aSignature = GetAttributeSignature(id);
      assert(aSignature != 0);
      const MathMLAttribute* attr = env.GetAttribute(id);    
      if (attr != 0) value = attr->GetParsedValue(aSignature);
    }

  if (value == 0 && searchDefault) value = GetDefaultAttributeValue(id);

  return value;
}

const Value*
MathMLElement::Resolve(const Value* value,
		       const RenderingEnvironment& env,
		       int i, int j)
{
  assert(value != NULL);

  const Value* realValue = value->Get(i, j);
  assert(realValue != NULL);

  if      (realValue->IsKeyword(KW_VERYVERYTHINMATHSPACE))
    realValue = new Value(env.GetMathSpace(MATH_SPACE_VERYVERYTHIN));
  else if (realValue->IsKeyword(KW_VERYTHINMATHSPACE))
    realValue = new Value(env.GetMathSpace(MATH_SPACE_VERYTHIN));
  else if (realValue->IsKeyword(KW_THINMATHSPACE))
    realValue = new Value(env.GetMathSpace(MATH_SPACE_THIN));
  else if (realValue->IsKeyword(KW_MEDIUMMATHSPACE))
    realValue = new Value(env.GetMathSpace(MATH_SPACE_MEDIUM));
  else if (realValue->IsKeyword(KW_THICKMATHSPACE))
    realValue = new Value(env.GetMathSpace(MATH_SPACE_THICK));
  else if (realValue->IsKeyword(KW_VERYTHICKMATHSPACE))
    realValue = new Value(env.GetMathSpace(MATH_SPACE_VERYTHICK));
  else if (realValue->IsKeyword(KW_VERYVERYTHICKMATHSPACE))
    realValue = new Value(env.GetMathSpace(MATH_SPACE_VERYVERYTHICK));
  else
    // the following cloning is necessary because values returned by
    // the resolving function must always be deleted (never cached)
    realValue = new Value(*realValue);

  return realValue;
}

bool
MathMLElement::IsSet(AttributeId id) const
{
#if defined(HAVE_MINIDOM)
  if (node == NULL) return false;

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING(NameOfAttributeId(id)));

  if (value != NULL) {
    mdom_string_free(value);
    return true;
  }

  return false;
#elif defined(HAVE_GMETADOM)
  if (!node) return false;
  return node.hasAttribute(NameOfAttributeId(id));
#endif // HAVE_GMETADOM
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
  if (bGC[Selected()] == NULL)
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
  if (Dirty())
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
MathMLElement::IsInside(scaled x, scaled y) const
{
  return GetRectangle().IsInside(x, y);
}

Ptr<MathMLElement>
MathMLElement::Inside(scaled x, scaled y)
{
  return IsInside(x, y) ? this : 0;
}

unsigned
MathMLElement::GetDepth() const
{
  unsigned depth = 0;
  Ptr<const MathMLElement> p = this;
  
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
#if defined(HAVE_MINIDOM)
  mDOMNodeRef p = GetDOMElement();

  while (p != NULL && !mdom_node_has_attribute(p, DOM_CONST_STRING("href")))
    p = mdom_node_get_parent(p);

  return p != NULL;
#elif defined(HAVE_GMETADOM)
  DOM::Element p = GetDOMElement();

  while (p && !p.hasAttribute("href")) {
    DOM::Node parent = p.get_parentNode();
    p = parent;
  }

  return p;
#endif // HAVE_GMETADOM
}

Ptr<MathMLOperatorElement>
MathMLElement::GetCoreOperator()
{
  return 0;
}

Ptr<MathMLOperatorElement>
MathMLElement::GetCoreOperatorTop()
{
  if (Ptr<MathMLOperatorElement> coreOp = GetCoreOperator())
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
  
  Ptr<MathMLElement> parent = GetParent();
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

  Ptr<MathMLElement> parent = GetParent();
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
  for (Ptr<MathMLElement> elem = GetParent(); 
       elem && !elem->HasDirtyChildren(); 
       elem = elem->GetParent())
    elem->dirtyChildren = 1;
}

void
MathMLElement::SetDirtyLayout(bool)
{
  if (HasDirtyLayout()) return;
  dirtyLayout = 1;
  for (Ptr<MathMLElement> elem = GetParent(); 
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
  if (!Dirty())
    {
      if (!rect || GetRectangle().Overlaps(*rect))
	{
	  SetFlagDown(FDirty);
	  SetFlagUp(FDirtyP);
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
  for (Ptr<MathMLElement> p = GetParent(); p && !p->GetFlag(f); p = p->GetParent())
    p->SetFlag(f);
}

void
MathMLElement::ResetFlagUp(Flags f)
{
  for (Ptr<MathMLElement> p = GetParent(); p && p->GetFlag(f); p = p->GetParent())
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
MathMLElement::SetParent(const Ptr<MathMLElement>& p)
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
