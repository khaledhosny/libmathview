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

#include "Globals.hh"
#include "ChildList.hh"
#include "RenderingEnvironment.hh"
#include "MathMLBinContainerElement.hh"
#include "FormattingContext.hh"

MathMLBinContainerElement::MathMLBinContainerElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLBinContainerElement::MathMLBinContainerElement(const DOM::Element& node)
#endif
  : MathMLContainerElement(node)
{
}

MathMLBinContainerElement::~MathMLBinContainerElement()
{
}

void
MathMLBinContainerElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
      if (children.get_length() > 0)
	{
	  DOM::Node node = children.item(0);
	  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
	  Ptr<MathMLElement> elem = doc->getFormattingNode(node);
	  // it might be that we get a NULL. In that case it would probably make
	  // sense to create a dummy element, because we filtered MathML
	  // elements only
	  assert(elem);
	  SetChild(elem);
	}
#endif // HAVE_GMETADOM

      if (child) child->Normalize(doc);
      ResetDirtyStructure();
    }
}

void
MathMLBinContainerElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      background = env.GetBackgroundColor();
      if (child) child->Setup(env);
      ResetDirtyAttribute();
    }
}

void
MathMLBinContainerElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      if (child)
	{
	  child->DoLayout(ctxt);
	  box = child->GetBoundingBox();
	}
      else
	box.Null();

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLBinContainerElement::SetPosition(scaled x, scaled y)
{
  MathMLContainerElement::SetPosition(x, y);
  if (child) child->SetPosition(x, y);
}

void
MathMLBinContainerElement::Render(const DrawingArea& area)
{
  if (Dirty())
    {
      RenderBackground(area);
      if (child) child->Render(area);
      ResetDirty();
    }
}

Ptr<MathMLElement>
MathMLBinContainerElement::Inside(scaled x, scaled y)
{
  if (IsInside(x, y))
    {
      if (child)
	if (Ptr<MathMLElement> inside = child->Inside(x, y)) return inside;
      return this;
    }
  else
    return 0;
}

#if 0
void
MathMLBinContainerElement::SetDirtyLayout(bool children)
{
  MathMLElement::SetDirtyLayout(children);
  if (children && child) child->SetDirtyLayout(children);
}

void
MathMLBinContainerElement::SetDirty(const Rectangle* rect)
{
  dirtyBackground =
    (GetParent()
     && ((GetParent()->IsSelected() != IsSelected())
	 || (GetParent()->GetBackgroundColor() != GetBackgroundColor()))) ? 1 : 0;

  if (IsDirty() || HasDirtyChildren()) return;
  //if (rect != NULL && !GetRectangle().Overlaps(*rect)) return;

  //dirty = 1;
  //SetDirtyChildren();

  if (child) child->SetDirty(rect);
}

void
MathMLBinContainerElement::SetSelected()
{
  if (IsSelected()) return;

  selected = 1;
  if (child) child->SetSelected();

  SetDirty();
}

void
MathMLBinContainerElement::ResetSelected()
{
  if (!IsSelected()) return;

  SetDirty();

  if (child) child->ResetSelected();
  selected = 0;
}
#endif

scaled
MathMLBinContainerElement::GetLeftEdge() const
{
  if (child) return child->GetLeftEdge();
  else return 0;
}

scaled
MathMLBinContainerElement::GetRightEdge() const
{
  if (child) return child->GetRightEdge();
  else return 0;
}

void
MathMLBinContainerElement::ReleaseGCs()
{
  MathMLElement::ReleaseGCs();
  if (child) child->ReleaseGCs();
}

void
MathMLBinContainerElement::Remove(const Ptr<MathMLElement>&)
{
  assert(0);
}

void
MathMLBinContainerElement::Replace(const Ptr<MathMLElement>& oldElem,
				   const Ptr<MathMLElement>& newElem)
{
  assert(oldElem == child);
  SetChild(newElem);
}

void
MathMLBinContainerElement::SetChild(const Ptr<MathMLElement>& elem)
{
  if (elem != child)
    {
      if (child) child->Unlink();
      if (elem) elem->Link(this);
      child = elem;
      SetDirtyLayout();
    }
}

void
MathMLBinContainerElement::SetFlagDown(Flags f)
{
  MathMLElement::SetFlag(f);
  if (child) child->SetFlagDown(f);
}

void
MathMLBinContainerElement::ResetFlagDown(Flags f)
{
  MathMLElement::ResetFlag(f);
  if (child) child->ResetFlagDown(f);
}
