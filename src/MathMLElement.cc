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

#include "Layout.hh"
#include "stringAux.hh"
#include "MathEngine.hh"
#include "DrawingArea.hh"
#include "MathMLElement.hh"
#include "MathMLDocument.hh"
#include "ValueConversion.hh"
#include "MathMLStyleElement.hh"
#include "MathMLAttributeList.hh"
#include "RenderingEnvironment.hh"

#ifdef DEBUG
int MathMLElement::counter = 0;
#endif // DEBUG

// MathMLElement: this is the base class for every MathML presentation element.
// It implements the basic skeleton of every such element, moreover it handles
// the attributes and provides some facility functions to access and parse
// attributes.
MathMLElement::MathMLElement(mDOMNodeRef n, TagId t)
{
  node = n;
  if (node != NULL) mdom_node_set_user_data(node, this);

  tag  = t;

  layout = NULL;
  shape  = NULL;

  fGC[0] = fGC[1] = NULL;
  bGC[0] = bGC[1] = NULL;

#ifdef DEBUG
  counter++;
#endif //DEBUG
}

MathMLElement::~MathMLElement()
{
  //MathEngine::logger(LOG_DEBUG, "destroying `%s' (DOM %p)", NameOfTagId(IsA()), node);
  if (node != NULL) mdom_node_set_user_data(node, NULL);

  delete layout;
  delete shape;

  ReleaseGCs();

#ifdef DEBUG
  counter--;
#endif // DEBUG
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

void
MathMLElement::Normalize()
{
  // nothing to normalize since nothing has been parsed
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
MathMLElement::GetAttribute(AttributeId id,
			    const RenderingEnvironment* env,
			    bool searchDefault) const
{
  const String* sValue = NULL;

  // if this element is not connected with a DOM element
  // then it cannot have attributes. This may happen for
  // elements inferred with normalization
  if (node != NULL) {
    mDOMStringRef value = mdom_node_get_attribute(node,
						  DOM_CONST_STRING(NameOfAttributeId(id)));
    if (value != NULL) {
      sValue = allocString(value);
      mdom_string_free(value);
    }
  }

  if (sValue == NULL && env != NULL) {
    const MathMLAttribute* attr = env->GetAttribute(id);
    if (attr != NULL) sValue = attr->GetValue();
  }

  if (sValue == NULL && searchDefault) sValue = GetDefaultAttribute(id);

  return sValue;
}

const Value*
MathMLElement::GetAttributeValue(AttributeId id,
				 const RenderingEnvironment* env,
				 bool searchDefault) const
{
  const Value* value = NULL;

  const AttributeSignature* aSignature = GetAttributeSignature(id);
  assert(aSignature != NULL);

  const String* sValue = NULL;

  if (node != NULL) {
    mDOMStringRef value = mdom_node_get_attribute(node,
						  DOM_CONST_STRING(NameOfAttributeId(id)));
    if (value != NULL) {
      sValue = allocString(value);
      mdom_string_free(value);
    }
  }

  if (sValue != NULL) {
    AttributeParser parser = aSignature->GetParser();
    assert(parser != NULL);

    StringTokenizer st(*sValue);
    value = parser(st);

    if (value == NULL) {
      MathEngine::logger(LOG_WARNING, "in element `%s' parsing error in attribute `%s'",
			 NameOfTagId(IsA()), NameOfAttributeId(id));
    }

    delete sValue;
    sValue = NULL;
  } else if (env != NULL) {
    const MathMLAttribute* attr = env->GetAttribute(id);    
    if (attr != NULL) value = attr->GetParsedValue(aSignature);
  }

  if (value == NULL && searchDefault) value = GetDefaultAttributeValue(id);

  return value;
}

const Value*
MathMLElement::Resolve(const Value* value,
		       const RenderingEnvironment* env,
		       int i, int j)
{
  assert(value != NULL);
  assert(env != NULL);

  const Value* realValue = value->Get(i, j);
  assert(realValue != NULL);

  if      (realValue->IsKeyword(KW_VERYVERYTHINMATHSPACE))
    realValue = new Value(env->GetMathSpace(MATH_SPACE_VERYVERYTHIN));
  else if (realValue->IsKeyword(KW_VERYTHINMATHSPACE))
    realValue = new Value(env->GetMathSpace(MATH_SPACE_VERYTHIN));
  else if (realValue->IsKeyword(KW_THINMATHSPACE))
    realValue = new Value(env->GetMathSpace(MATH_SPACE_THIN));
  else if (realValue->IsKeyword(KW_MEDIUMMATHSPACE))
    realValue = new Value(env->GetMathSpace(MATH_SPACE_MEDIUM));
  else if (realValue->IsKeyword(KW_THICKMATHSPACE))
    realValue = new Value(env->GetMathSpace(MATH_SPACE_THICK));
  else if (realValue->IsKeyword(KW_VERYTHICKMATHSPACE))
    realValue = new Value(env->GetMathSpace(MATH_SPACE_VERYTHICK));
  else if (realValue->IsKeyword(KW_VERYVERYTHICKMATHSPACE))
    realValue = new Value(env->GetMathSpace(MATH_SPACE_VERYVERYTHICK));
  else
    // the following cloning is necessary because values returned by
    // the resolving function must always be deleted (never cached)
    realValue = new Value(*realValue);

  return realValue;
}

bool
MathMLElement::IsSet(AttributeId id) const
{
  if (node == NULL) return false;

  mDOMStringRef value = mdom_node_get_attribute(node, DOM_CONST_STRING(NameOfAttributeId(id)));

  if (value != NULL) {
    mdom_string_free(value);
    return true;
  }

  return false;
}

void
MathMLElement::Setup(RenderingEnvironment*)
{
  // this function is defined to be empty but not pure-virtual
  // because some "space-like" elements such as <mspace>
  // <maligngroup>, <malignmark> effectively do nothing.
  // So we don't have to implement this function as empty
  // in every such element.
  // The same holds for Render below.
}

void
MathMLElement::ResetLayout()
{
  delete layout;
  layout = NULL;
}

bool
MathMLElement::HasDirtyLayout(LayoutId id, scaled w) const
{
  return true || HasDirtyLayout() || (id == LAYOUT_AUTO && !scaledEq(w, lastLayoutWidth));
}

void
MathMLElement::ResetDirtyLayout(LayoutId id, scaled w)
{
  if (id == LAYOUT_AUTO) {
    ResetDirtyLayout();
    lastLayoutWidth = w;
  }
}

void
MathMLElement::ResetDirtyLayout(LayoutId id)
{
  if (id == LAYOUT_AUTO) ResetDirtyLayout();
}

void
MathMLElement::DoBoxedLayout(LayoutId id, BreakId bid, scaled maxWidth)
{
  if (!HasDirtyLayout(id, maxWidth)) return;

  ResetLayout();

  layout = new Layout(maxWidth, IsBreakable() ? bid : BREAK_NO);
  DoLayout(id, *layout);
  layout->DoLayout(id);
  if (id == LAYOUT_AUTO) DoStretchyLayout();
  layout->GetBoundingBox(box, id);

  ConfirmLayout(id);

#if 0
  cout << '`' << NameOfTagId(IsA()) << "' (" << this << ") DoBoxedLayout " << box << endl;
#endif

  ResetDirtyLayout(id, maxWidth);
}

void
MathMLElement::DoLayout(LayoutId id, Layout&)
{
  // Well, there are some empty elements, such as <none/> or <prescripts/>
  // that do not have a layout, nonetheless they are unbreakable
  ResetDirtyLayout(id);
}

void
MathMLElement::RecalcBoundingBox(LayoutId id, scaled minWidth)
{
  if (HasLayout()) layout->GetBoundingBox(box, id);
  box.width = scaledMax(box.width, minWidth);
  ConfirmLayout(id);
}

void
MathMLElement::DoStretchyLayout()
{
}

void
MathMLElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;
  if (HasLayout()) layout->SetPosition(x, y);
}

void
MathMLElement::SetPosition(scaled x, scaled y, ColumnAlignId id)
{
  if (HasLayout()) {
      position.x = x;
      position.y = y;
      layout->SetPosition(x, y, id);
  } else
    SetPosition(x, y);
}

void
MathMLElement::RenderBackground(const DrawingArea& area)
{
  if (bGC[IsSelected()] == NULL) {
    GraphicsContextValues values;
    values.background = values.foreground = IsSelected() ? area.GetSelectionBackground() : background;
    bGC[IsSelected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND);
  }

  if (HasDirtyBackground()) {
#if 0
    printf("`%s' has dirty background : shape = ", NameOfTagId(IsA()));
    shape->Dump();
    printf("\n");
#endif
    assert(IsShaped());
    area.Clear(bGC[IsSelected()], GetShape());
  }
}

void
MathMLElement::Render(const DrawingArea& area)
{
  if (!IsDirty()) return;

  RenderBackground(area);

  ResetDirty();
}

void
MathMLElement::Freeze()
{
  assert(!IsBreakable() || HasLayout());

  if (shape != NULL) delete shape;
  
  Rectangle* rect = new Rectangle;
  if (!IsBreakable()) GetBoundingBox().ToRectangle(GetX(), GetY(), *rect);
  else {
    assert(HasLayout());
    BoundingBox box;
    layout->GetBoundingBox(box);
    box.ToRectangle(GetX(), GetY(), *rect);
  }
  shape = new Shape(rect);

#if 0
  printf("freezing: %s  ", NameOfTagId(IsA()));
  shape->Dump();
  printf("\n");
#endif

  ResetLayout();
}

void
MathMLElement::SetDirty(const Rectangle* rect)
{
  assert(IsShaped());

  dirtyBackground =
    (GetParent() != NULL && (GetParent()->IsSelected() != IsSelected())) ? 1 : 0;
#if 0
  if (GetParent() != NULL && (GetParent()->IsSelected() != IsSelected()))
    dirtyBackground = 1;
#endif

  if (IsDirty()) return;
  if (rect != NULL && !shape->Overlaps(*rect)) return;

  dirty = 1;
  SetDirtyChildren();
}

bool
MathMLElement::IsSpaceLike() const
{
  return false;
}

bool
MathMLElement::IsElement() const
{
  return true;
}

bool
MathMLElement::IsExpanding() const
{
  return false;
}

bool
MathMLElement::IsInside(scaled x, scaled y) const
{
  assert(IsShaped());
  return shape->IsInside(x, y);
}

void
MathMLElement::GetLinearBoundingBox(BoundingBox& b) const
{
  assert(!IsBreakable());
  b = box;
}

MathMLElement*
MathMLElement::Inside(scaled x, scaled y)
{
  return IsInside(x, y) ? this : NULL;
}

unsigned
MathMLElement::GetDepth() const
{
  unsigned depth = 0;
  const MathMLElement* p = this;
  
  while (p != NULL) {
    depth++;
    p = p->GetParent();
  }

  return depth;
}

const Layout&
MathMLElement::GetLayout() const
{
  assert(HasLayout());
  return *layout;
}

const Shape&
MathMLElement::GetShape() const
{
  assert(IsShaped());
  return *shape;
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
  mDOMNodeRef p = GetDOMNode();

  while (p != NULL && !mdom_node_has_attribute(p, DOM_CONST_STRING("href")))
    p = mdom_node_get_parent(p);

  return p != NULL;
}
