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
#include "ChildList.hh"
#include "RenderingEnvironment.hh"
#include "MathMLBinContainerElement.hh"
#include "FormattingContext.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLBinContainerElement::MathMLBinContainerElement(const SmartPtr<class MathMLView>& view)
  : MathMLContainerElement(view)
{
}

MathMLBinContainerElement::~MathMLBinContainerElement()
{
}

void
MathMLBinContainerElement::construct()
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      ChildList children(getDOMElement(), MATHML_NS_URI, "*");
      if (children.get_length() > 0)
	{
	  DOM::Node node = children.item(0);
	  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
	  SmartPtr<MathMLElement> elem = getFormattingNode(node);
	  assert(elem);
	  SetChild(elem);
	}
#endif // HAVE_GMETADOM

      if (child) child->construct();
      ResetDirtyStructure();
    }
}

void
MathMLBinContainerElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      if (child) child->refine(context);
      MathMLContainerElement::refine(context);
    }  
}

#if 0
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
	box.unset();

      ResetDirtyLayout(ctxt);
    }
}
#endif

AreaRef
MathMLBinContainerElement::format(MathFormattingContext& ctxt)
{
  if (DirtyLayout())
    {
      ctxt.push(this);
      AreaRef res = child ? child->format(ctxt) : 0;
      setArea(res ? ctxt.getDevice()->wrapper(ctxt, res) : 0);
      ctxt.pop();

      ResetDirtyLayout();
    }

  return getArea();
}

#if 0
void
MathMLBinContainerElement::SetPosition(const scaled& x, const scaled& y)
{
  MathMLContainerElement::SetPosition(x, y);
  if (child) child->SetPosition(x, y);
}

void
MathMLBinContainerElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      RenderBackground(area);
      if (child) child->Render(area);
      ResetDirty();
    }
}

SmartPtr<MathMLElement>
MathMLBinContainerElement::Inside(const scaled& x, const scaled& y)
{
  if (IsInside(x, y))
    {
      if (child)
	if (SmartPtr<MathMLElement> inside = child->Inside(x, y)) return inside;
      return this;
    }
  else
    return 0;
}

void
MathMLBinContainerElement::SetDirty(const Rectangle* rect)
{
//   dirtyBackground =
//     (GetParent()
//      && ((GetParent()->IsSelected() != IsSelected())
// 	 || (GetParent()->GetBackgroundColor() != GetBackgroundColor()))) ? 1 : 0;

  //if (IsDirty() || HasDirtyChildren()) return;
  if (!rect || GetRectangle().Overlaps(*rect))
    {
      SetFlag(FDirty);
      //SetFlagUp(FDirtyP);
      if (child) child->SetDirty(rect);
    }
}

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
#endif

void
MathMLBinContainerElement::Remove(const SmartPtr<MathMLElement>&)
{
  assert(0);
}

void
MathMLBinContainerElement::Replace(const SmartPtr<MathMLElement>& oldElem,
				   const SmartPtr<MathMLElement>& newElem)
{
  assert(oldElem == child);
  SetChild(newElem);
}

void
MathMLBinContainerElement::SetChild(const SmartPtr<MathMLElement>& elem)
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
