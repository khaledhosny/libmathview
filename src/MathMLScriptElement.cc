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

#include "ChildList.hh"
#include "FormattingContext.hh"
#include "MathMLDummyElement.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLScriptElement.hh"
#include "MathMLView.hh"
#include "RenderingEnvironment.hh"
#include "ValueConversion.hh"
#include "defs.h"
#include "traverseAux.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLScriptElement::MathMLScriptElement(const SmartPtr<class MathMLView>& view)
  : MathMLContainerElement(view)
{
  subScript = superScript = 0;  
}

MathMLScriptElement::~MathMLScriptElement()
{
}

void
MathMLScriptElement::SetBase(const SmartPtr<MathMLElement>& elem)
{
  if (elem != base)
    {
      if (base) base->Unlink();
      if (elem) elem->Link(this);
      base = elem;
      SetDirtyLayout();
    }
}

void
MathMLScriptElement::SetSubScript(const SmartPtr<MathMLElement>& elem)
{
  if (elem != subScript)
    {
      if (subScript) subScript->Unlink();
      if (elem) elem->Link(this);
      subScript = elem;
      SetDirtyLayout();
    }
}

void
MathMLScriptElement::SetSuperScript(const SmartPtr<MathMLElement>& elem)
{
  if (elem != superScript)
    {
      if (superScript) superScript->Unlink();
      if (elem) elem->Link(this);
      superScript = elem;
      SetDirtyLayout();
    }
}

void
MathMLScriptElement::Replace(const SmartPtr<MathMLElement>& oldElem, const SmartPtr<MathMLElement>& newElem)
{
  assert(oldElem);
  if (oldElem == base) SetBase(newElem);
  else if (oldElem == subScript) SetSubScript(newElem);
  else if (oldElem == superScript) SetSuperScript(newElem);
  else assert(false);
}

SmartPtr<MathMLElement>
MathMLScriptElement::Inside(const scaled& x, const scaled& y)
{
  if (!IsInside(x, y)) return 0;

  SmartPtr<MathMLElement> inside;
  assert(base);
  if (inside = base->Inside(x, y)) return inside;
  if (subScript && (inside = subScript->Inside(x, y))) return inside;
  if (superScript && (inside = superScript->Inside(x, y))) return inside;

  return this;
}

void
MathMLScriptElement::construct()
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  assert(IsA() == T_MSUB || IsA() == T_MSUP || IsA() == T_MSUBSUP);
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  
	  if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(0)))
	    SetBase(e);
	  else if (!is_a<MathMLDummyElement>(GetBase()))
	    SetBase(getFactory()->createDummyElement(getView()));

	  switch (IsA())
	    {
	    case T_MSUB:
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		SetSubScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSubScript()))
		SetSubScript(getFactory()->createDummyElement(getView()));
	      SetSuperScript(0);
	      break;
	    case T_MSUP:
	      SetSubScript(0);
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		SetSuperScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSuperScript()))
		SetSuperScript(getFactory()->createDummyElement(getView()));
	      break;
	    case T_MSUBSUP:
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		SetSubScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSubScript()))
		SetSubScript(getFactory()->createDummyElement(getView()));
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(2)))
		SetSuperScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSuperScript()))
		SetSuperScript(getFactory()->createDummyElement(getView()));
	      break;
	    default:
	      assert(false);
	    }
	}
#endif

      if (base) base->construct();
      if (subScript) subScript->construct();
      if (superScript) superScript->construct();

      ResetDirtyStructure();
    }
}

void
MathMLScriptElement::refine(AbstractRefinementContext& context)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, Script, subscriptshift);
      REFINE_ATTRIBUTE(context, Script, superscriptshift);
      if (GetBase()) GetBase()->refine(context);
      if (GetSubScript()) GetSubScript()->refine(context);
      if (GetSuperScript()) GetSuperScript()->refine(context);
      MathMLContainerElement::refine(context);
    }
}

void
MathMLScriptElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      MathMLElement::Setup(env);
      ScriptSetup(env);

      if (base) base->Setup(env);

      env.Push();
      env.AddScriptLevel(1);
      env.SetDisplayStyle(false);

      if (subScript)
	{
	  subScript->Setup(env);

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Script, subscriptshift))
	    {
	      assert(IsLength(value));
	      Length unitValue = ToLength(value);
	      assert(!unitValue.type == Length::PERCENTAGE_UNIT);
	      subMinShift = env.ToScaledPoints(unitValue);
	    }
	}

      if (superScript)
	{
	  superScript->Setup(env);

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Script, superscriptshift))
	    {
	      assert(IsLength(value));
	      Length unitValue = ToLength(value);
	      assert(!unitValue.type == Length::PERCENTAGE_UNIT);
	      superMinShift = env.ToScaledPoints(unitValue);
	    }
	}

      env.Drop();

      ResetDirtyAttribute();
    }
}

void
MathMLScriptElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      assert(base);
      if (base) base->DoLayout(ctxt);
      if (subScript) subScript->DoLayout(ctxt);
      if (superScript) superScript->DoLayout(ctxt);

      SmartPtr<MathMLElement> rel = findRightmostChild(base);
      assert(rel);

      const BoundingBox& baseBox = base->GetBoundingBox();
      BoundingBox relBox = rel->GetBoundingBox();
      relBox.width = baseBox.width;

      BoundingBox subScriptBox;
      BoundingBox superScriptBox;

      subScriptBox.unset();
      if (subScript) subScriptBox = subScript->GetBoundingBox();

      superScriptBox.unset();
      if (superScript) superScriptBox = superScript->GetBoundingBox();

      DoScriptLayout(relBox, subScriptBox, superScriptBox, subShiftX, subShiftY, superShiftX, superShiftY);

      box = baseBox;

      box.width = std::max(box.width,
			    std::max(superShiftX + superScriptBox.width,
				      subShiftX + subScriptBox.width));

      if (subScript)
	{
	  box.height = std::max(box.height, subScriptBox.height - subShiftY);
	  box.depth  = std::max(box.depth, subScriptBox.depth + subShiftY);
	}

      if (superScript)
	{
	  box.height = std::max(box.height, superScriptBox.height + superShiftY);
	  box.depth  = std::max(box.depth, superScriptBox.depth - superShiftY);
	}

      DoEmbellishmentLayout(this, box);

      ResetDirtyLayout(ctxt);
    }
}

AreaRef
MathMLScriptElement::format(MathFormattingContext& ctxt)
{
  if (DirtyLayout())
    {
      ctxt.push(this);

      assert(base);
      AreaRef baseArea = base->format(ctxt);

      ctxt.addScriptLevel(1);
      ctxt.setDisplayStyle(false);

      Length subScriptShift;
      AreaRef subScriptArea;
      if (subScript)
	{
	  subScriptArea = subScript->format(ctxt);

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Script, subscriptshift))
	    {
	      assert(IsLength(value));
	      subScriptShift = ToLength(value);
	    }
	}

      Length superScriptShift;
      AreaRef superScriptArea;
      if (superScript)
	{
	  superScriptArea = superScript->format(ctxt);

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(Script, superscriptshift))
	    {
	      assert(IsLength(value));
	      superScriptShift = ToLength(value);
	    }
	}

      AreaRef res = ctxt.getDevice()->script(ctxt, baseArea,
					     subScriptArea, subScriptShift,
					     superScriptArea, superScriptShift);
      res = formatEmbellishment(this, ctxt, res);
      setArea(ctxt.getDevice()->wrapper(ctxt, res));

      ctxt.pop();
      ResetDirtyLayout();
    }

  return getArea();
}

void
MathMLScriptElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;

  SetEmbellishmentPosition(this, x, y);

  if (base) base->SetPosition(x, y);

  if (subScript)
    subScript->SetPosition(x + subShiftX, y + subShiftY);

  if (superScript)
    superScript->SetPosition(x + superShiftX, y - superShiftY);
}

void
MathMLScriptElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      RenderBackground(area);
      assert(base);
      base->Render(area);
      if (subScript) subScript->Render(area);
      if (superScript) superScript->Render(area);
      ResetDirty();
    }
}

void
MathMLScriptElement::ReleaseGCs()
{
  MathMLElement::ReleaseGCs();
  assert(base);
  base->ReleaseGCs();
  if (subScript) subScript->ReleaseGCs();
  if (superScript) superScript->ReleaseGCs();
}

scaled
MathMLScriptElement::GetLeftEdge() const
{
  assert(base);
  scaled m = base->GetLeftEdge();
  if (subScript) m = std::min(m, subScript->GetLeftEdge());
  if (superScript) m = std::min(m, superScript->GetLeftEdge());
  return m;
}

scaled
MathMLScriptElement::GetRightEdge() const
{
  assert(base);
  scaled m = base->GetRightEdge();
  if (subScript) m = std::max(m, subScript->GetRightEdge());
  if (superScript) m = std::max(m, superScript->GetRightEdge());
  return m;
}

SmartPtr<class MathMLOperatorElement>
MathMLScriptElement::GetCoreOperator()
{
  if (base) return base->GetCoreOperator();
  else return 0;
}

void
MathMLScriptElement::SetFlagDown(Flags f)
{
  MathMLElement::SetFlagDown(f);
  if (base) base->SetFlagDown(f);
  if (subScript) subScript->SetFlagDown(f);
  if (superScript) superScript->SetFlagDown(f);
}

void
MathMLScriptElement::ResetFlagDown(Flags f)
{
  MathMLElement::ResetFlagDown(f);
  if (base) base->ResetFlagDown(f);
  if (subScript) subScript->ResetFlagDown(f);
  if (superScript) superScript->ResetFlagDown(f);
}
