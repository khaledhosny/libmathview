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
#include <stddef.h>

#include "unidefs.h"
#include "stringAux.hh"
#include "Globals.hh"
#include "ChildList.hh"
#include "StringUnicode.hh"
#include "EntitiesTable.hh"
#include "MathMLCharNode.hh"
#include "MathMLRowElement.hh"
#include "MathMLDummyElement.hh"
#include "MathMLRadicalElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"

MathMLRadicalElement::MathMLRadicalElement()
{
  Init();
}

#if defined(HAVE_GMETADOM)
MathMLRadicalElement::MathMLRadicalElement(const DOM::Element& node)
  : MathMLContainerElement(node)
{
  Init();
}
#endif

MathMLRadicalElement::~MathMLRadicalElement()
{
  radical = 0;
  SetRadicand(0);
  SetIndex(0);
}

void
MathMLRadicalElement::Init()
{
  radical = 0;
  radicand = 0;
  index = 0;
}

void
MathMLRadicalElement::SetRadicand(const Ptr<MathMLElement>& elem)
{
  if (elem != radicand)
    {
      assert(!elem->GetParent());
      if (elem) elem->SetParent(this);
      if (radicand) radicand->SetParent(0);
      radicand = elem;
      SetDirtyLayout();
    }
}

void
MathMLRadicalElement::SetIndex(const Ptr<MathMLElement>& elem)
{
  if (elem != index)
    {
      assert(!elem->GetParent());
      if (elem) elem->SetParent(this);
      if (index) index->SetParent(0);
      index = elem;
      SetDirtyLayout();
    }
}

void
MathMLRadicalElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  if (IsA() == TAG_MSQRT)
	    {
	      ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
	      if (children.get_length() == 1)
		{
		  DOM::Node node = children.item(0);
		  assert(node.get_nodeType() == DOM::Node::ELEMENT_NODE);
		  SetRadicand(doc->getFormattingNode(node));
		}
	      else
		SetRadicand(MathMLRowElement::create(GetDOMElement()));

	      SetIndex(0);
	    }
	  else
	    {
	      assert(IsA() == TAG_MROOT);	      
	      ChildList children(GetDOMElement(), MATHML_NS_URI, "*");

	      switch (children.get_length())
		{
		case 0:
		  SetRadicand(MathMLDummyElement::create());
		  SetIndex(MathMLDummyElement::create());
		  break;
		case 1:
		  SetRadicand(doc->getFormattingNode(children.item(0)));
		  SetIndex(MathMLDummyElement::create());
		  break;
		default:
		  SetRadicand(doc->getFormattingNode(children.item(0)));
		  SetIndex(doc->getFormattingNode(children.item(1)));
		  break;
		}
	    }
	}
#endif

      if (!radical) radical = MathMLCharNode::create(U_SQRT);
      assert(radical);
      radical->SetParent(this);

      if (radicand) radicand->Normalize(doc);
      if (index) index->Normalize(doc);

      ResetDirtyStructure();
    }
}

void
MathMLRadicalElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      spacing       = env.ToScaledPoints(env.GetMathSpace(MATH_SPACE_MEDIUM));
      color         = env.GetColor();
      background    = env.GetBackgroundColor();
      lineThickness = env.GetRuleThickness();

      if (radical) radical->Setup(env);
      if (radicand) radicand->Setup(env);
      if (index)
	{
	  env.Push();
	  env.SetDisplayStyle(false);
	  env.AddScriptLevel(2);
	  index->Setup(env);
	  env.Drop();
	}

      ResetDirtyAttribute();
    }
}

void
MathMLRadicalElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      assert(radicand);
      radicand->DoLayout(ctxt);
      box = radicand->GetBoundingBox();

      assert(radical);
      radical->DoLayout(ctxt);
      radical->DoVerticalStretchyLayout(box.ascent + lineThickness, box.descent, 0, false);
      const BoundingBox& radBox = radical->GetBoundingBox();

      box.width += radBox.width;
      box.rBearing += radBox.width;
      box.ascent = scaledMax(box.ascent + spacing, radBox.ascent);
      box.descent = scaledMax(box.descent, radBox.descent);

      if (index)
	{
	  index->DoLayout(ctxt);
	  const BoundingBox& indexBox = index->GetBoundingBox();

	  box.width += indexBox.width;

	  if (box.GetHeight() / 2 < indexBox.GetHeight())
	    box.ascent += indexBox.GetHeight() - box.GetHeight() / 2;
	}

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLRadicalElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;

  assert(radical);
  const BoundingBox& radBox = radical->GetBoundingBox();

  if (index)
    {
      const BoundingBox& baseBox  = radicand->GetBoundingBox();
      const BoundingBox& indexBox = index->GetBoundingBox();

      index->SetPosition(x, y + (baseBox.GetHeight() / 2 - baseBox.ascent) - indexBox.descent);
      radical->SetPosition(x + indexBox.width, y);
      radicand->SetPosition(x + indexBox.width + radBox.width, y);
    } 
  else
    {
      radical->SetPosition(x, y - box.ascent + radBox.ascent);
      radicand->SetPosition(x + radBox.width, y);
    }
}

#if 0
void
MathMLRadicalElement::SetDirty(const Rectangle* rect)
{
  if (!IsDirty() && !HasDirtyChildren())
    {
      MathMLElement::SetDirty(rect);
      if (radicand) radicand->SetDirty(rect);
      if (index) index->SetDirty(rect);  
    }
}

void
MathMLRadicalElement::SetDirtyLayout(bool children)
{
  MathMLElement::SetDirtyLayout(children);
  if (children)
    {
      if (radicand) radicand->SetDirtyLayout(children);
      if (index) index->SetDirtyLayout(children);
    }
}

void
MathMLRadicalElement::SetSelected()
{
  if (IsSelected()) return;

  selected = 1;

  if (radicand) radicand->SetSelected();
  if (index) index->SetSelected();

  SetDirty();
}

void
MathMLRadicalElement::ResetSelected()
{
  if (!IsSelected()) return;
  
  SetDirty();

  if (radicand) radicand->ResetSelected();
  if (index) index->ResetSelected();

  selected = 0;
}
#endif

void
MathMLRadicalElement::SetFlagDown(Flags f)
{
  MathMLElement::SetFlagDown(f);
  if (radicand) radicand->SetFlagDown(f);
  if (index) index->SetFlagDown(f);
}

void
MathMLRadicalElement::ResetFlagDown(Flags f)
{
  MathMLElement::ResetFlagDown(f);
  if (radicand) radicand->ResetFlagDown(f);
  if (index) index->ResetFlagDown(f);
}

void
MathMLRadicalElement::Render(const DrawingArea& area)
{
  if (Dirty())
    {
      if (fGC[Selected()] == NULL) {
	GraphicsContextValues values;
	values.foreground = Selected() ? area.GetSelectionForeground() : color;
	values.background = Selected() ? area.GetSelectionBackground() : background;
	values.lineWidth = lineThickness;
	fGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND | GC_MASK_BACKGROUND | GC_MASK_LINE_WIDTH);
      }

      RenderBackground(area);

      assert(radicand);
      radicand->Render(area);
      if (index) index->Render(area);

      assert(radical);
      radical->Render(area);
      const BoundingBox& radBox = radical->GetBoundingBox();
      area.MoveTo(radical->GetX() + radBox.width, radical->GetY() - radBox.ascent + lineThickness / 2);
      area.DrawLineToDelta(fGC[Selected()], radicand->GetBoundingBox().width, 0);

      ResetDirty();
    }
}

scaled
MathMLRadicalElement::GetLeftEdge() const
{
  assert(radical);
  assert(radicand);
  scaled m = scaledMin(radicand->GetLeftEdge(), radical->GetLeftEdge());
  if (index) return scaledMin(m, index->GetLeftEdge());
  else return m;
}

scaled
MathMLRadicalElement::GetRightEdge() const
{
  assert(radical);
  assert(radicand);
  scaled m = scaledMax(radicand->GetRightEdge(), radical->GetRightEdge());
  if (index) return scaledMax(m, index->GetRightEdge());
  else return m;
}

void
MathMLRadicalElement::Replace(const Ptr<MathMLElement>& oldElem, const Ptr<MathMLElement>& newElem)
{
  assert(oldElem);
  if (oldElem == radicand) SetRadicand(newElem);
  else if (oldElem == index) SetIndex(newElem);
  else assert(0);
}

void
MathMLRadicalElement::ReleaseGCs()
{
  MathMLElement::ReleaseGCs();
  if (radicand) radicand->ReleaseGCs();
  if (index) index->ReleaseGCs();
}

Ptr<MathMLElement>
MathMLRadicalElement::Inside(scaled x, scaled y)
{
  if (!IsInside(x, y)) return 0;

  Ptr<MathMLElement> inside = 0;
  if (radicand && (inside = radicand->Inside(x, y))) return inside;
  if (index && (inside = index->Inside(x, y))) return inside;

  return this;
}
