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
#include "Globals.hh"
#include "MathMLDummyElement.hh"
#include "MathMLFormattingEngineFactory.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLUnderOverElement.hh"
#include "MathMLView.hh"
#include "ValueConversion.hh"
#include "scaledConv.hh"
#include "traverseAux.hh"
#include "MathGraphicDevice.hh"
#include "MathFormattingContext.hh"
#include "MathMLAttributeSignatures.hh"

MathMLUnderOverElement::MathMLUnderOverElement(const SmartPtr<class MathMLView>& view)
  : MathMLContainerElement(view)
{
  underScript = overScript = 0;
}

MathMLUnderOverElement::~MathMLUnderOverElement()
{
}

void
MathMLUnderOverElement::SetBase(const SmartPtr<MathMLElement>& elem)
{
  if (elem != base)
    {
      if (elem) elem->setParent(this);
      base = elem;
      setDirtyLayout();
    }
}

void
MathMLUnderOverElement::SetUnderScript(const SmartPtr<MathMLElement>& elem)
{
  if (elem != underScript)
    {
      if (elem) elem->setParent(this);
      underScript = elem;
      setDirtyLayout();
    }
}

void
MathMLUnderOverElement::SetOverScript(const SmartPtr<MathMLElement>& elem)
{
  if (elem != overScript)
    {
      if (elem) elem->setParent(this);
      overScript = elem;
      setDirtyLayout();
    }
}

#if 0
void
MathMLUnderOverElement::Replace(const SmartPtr<MathMLElement>& oldElem, const SmartPtr<MathMLElement>& newElem)
{
  assert(oldElem);
  if (oldElem == base) SetBase(newElem);
  else if (oldElem == underScript) SetUnderScript(newElem);
  else if (oldElem == overScript) SetOverScript(newElem);
  else assert(0);
}
#endif

void
MathMLUnderOverElement::construct()
{
  if (dirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (getDOMElement())
	{
	  assert(IsA() == T_MUNDER || IsA() == T_MOVER || IsA() == T_MUNDEROVER);
	  ChildList children(getDOMElement(), MATHML_NS_URI, "*");
	  
	  if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(0)))
	    SetBase(e);
	  else if (!is_a<MathMLDummyElement>(GetBase()))
	    SetBase(getFactory()->createDummyElement(getView()));

	  switch (IsA())
	    {
	    case T_MUNDER:
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		SetUnderScript(e);
	      else if (!is_a<MathMLDummyElement>(GetUnderScript()))
		SetUnderScript(getFactory()->createDummyElement(getView()));
	      SetOverScript(0);
	      break;
	    case T_MOVER:
	      SetUnderScript(0);
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		SetOverScript(e);
	      else if (!is_a<MathMLDummyElement>(GetOverScript()))
		SetOverScript(getFactory()->createDummyElement(getView()));
	      break;
	    case T_MUNDEROVER:
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(1)))
		SetUnderScript(e);
	      else if (!is_a<MathMLDummyElement>(GetUnderScript()))
		SetUnderScript(getFactory()->createDummyElement(getView()));
	      if (SmartPtr<MathMLElement> e = getFormattingNode(children.item(2)))
		SetOverScript(e);
	      else if (!is_a<MathMLDummyElement>(GetOverScript()))
		SetOverScript(getFactory()->createDummyElement(getView()));
	      break;
	    default:
	      assert(0);
	    }
	}
#endif // HAVE_GMETADOM

      assert(base);
      base->construct();
      if (underScript) underScript->construct();
      if (overScript) overScript->construct();

      resetDirtyStructure();
    }
}

void
MathMLUnderOverElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, UnderOver, accentunder);
      REFINE_ATTRIBUTE(context, MathML, UnderOver, accent);
      if (GetBase()) GetBase()->refine(context);
      if (GetUnderScript()) GetUnderScript()->refine(context);
      if (GetOverScript()) GetOverScript()->refine(context);
      MathMLContainerElement::refine(context);
    }
}

#if 0
void
MathMLUnderOverElement::Setup(RenderingEnvironment& env)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      bool displayStyle = env.GetDisplayStyle();
      background = env.GetBackgroundColor();

      ScriptSetup(env);

      scaled smallSpacing = ruleThickness;
      scaled bigSpacing   = 3 * ruleThickness;

      if (base) base->Setup(env);
      SmartPtr<MathMLOperatorElement> op = base ? base->GetCoreOperator() : SmartPtr<MathMLOperatorElement>(0);

      if (op)
	scriptize = !displayStyle && op->HasMovableLimits();
      else
	scriptize = false;

      env.Push();
      env.SetDisplayStyle(false);

      accentUnder = false;
      underSpacing = 0;
      if (underScript)
	{
	  if (!scriptize)
	    {
	      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(UnderOver, accentunder))
		accentUnder = ToBoolean(value);
	      else if (SmartPtr<MathMLOperatorElement> op = underScript->GetCoreOperator())
		{
		  underScript->Setup(env);
		  accentUnder = op->IsAccent();
		}
	    }

	  if (accentUnder)
	    underSpacing = smallSpacing;
	  else
	    {
	      env.AddScriptLevel(1);
	      underSpacing = displayStyle ? bigSpacing : smallSpacing;
	    }
	  underScript->Setup(env);
	}

      env.Drop();
      env.Push();
      env.SetDisplayStyle(false);

      accent = false;
      overSpacing = 0;
      if (overScript)
	{
	  if (!scriptize)
	    {
	      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(UnderOver, accent))
		accent = ToBoolean(value);
	      else if (SmartPtr<MathMLOperatorElement> op = overScript->GetCoreOperator())
		{
		  overScript->Setup(env);
		  accent = op->IsAccent();
		}
	    }

	  if (accent)
	    {
	      overSpacing = smallSpacing;
	    } 
	  else
	    {
	      env.AddScriptLevel(1);
	      overSpacing = displayStyle ? bigSpacing : smallSpacing;
	    }
	  overScript->Setup(env);
	}
  
      env.Drop();

      resetDirtyAttribute();
    }
}

void
MathMLUnderOverElement::DoLayout(const class FormattingContext& ctxt)
{
  if (dirtyLayout(ctxt))
    {
      assert(base);

      scaled overClearance = 0;
      scaled underClearance = 0;

      if (scriptize)
	{
	  base->DoLayout(ctxt);
	  if (overScript) overScript->DoLayout(ctxt);
	  if (underScript) underScript->DoLayout(ctxt);

	  const BoundingBox& baseBox = base->GetBoundingBox();
	  BoundingBox underBox;
	  BoundingBox overBox;
    
	  if (underScript) underBox = underScript->GetBoundingBox();
	  else underBox.unset();

	  if (overScript) overBox = overScript->GetBoundingBox();
	  else overBox.unset();

	  DoScriptLayout(baseBox, underBox, overBox, underShiftX, underShiftY, overShiftX, overShiftY);
	  underClearance = overClearance = 0;

	  baseShiftX = 0;
	} 
      else
	{    
	  if (ctxt.GetLayoutType() != FormattingContext::LAYOUT_AUTO)
	    {
	      base->DoLayout(ctxt);
	      if (underScript) underScript->DoLayout(ctxt);
	      if (overScript) overScript->DoLayout(ctxt);
	    } 
	  else
	    {
	      unsigned nOp    = 0;
	      unsigned nOther = 0;

	      scaled wOp      = 0;
	      scaled wOther   = 0;

	      SmartPtr<MathMLOperatorElement> baseOp  = findStretchyOperator(base);
	      SmartPtr<MathMLOperatorElement> underOp = findStretchyOperator(underScript);
	      SmartPtr<MathMLOperatorElement> overOp  = findStretchyOperator(overScript);

	      Globals::logger(LOG_DEBUG, "stretchy: %p %p %p",
			      static_cast<MathMLElement*>(baseOp),
			      static_cast<MathMLElement*>(underOp),
			      static_cast<MathMLElement*>(overOp));

	      if (!baseOp) base->DoLayout(ctxt);
	      if (underScript && !underOp) underScript->DoLayout(ctxt);
	      if (overScript && !overOp) overScript->DoLayout(ctxt);

	      if (!baseOp)
		{
		  wOther = base->GetBoundingBox().width;
		  nOther++;
		} 
	      else
		{
		  wOp = baseOp->GetMinBoundingBox().width;
		  nOp++;
		}

	      if (underScript)
		{
		  if (!underOp)
		    {
		      wOther = std::max(wOther, underScript->GetBoundingBox().width);
		      nOther++;
		    } 
		  else
		    {
		      wOp = std::max(wOp, underOp->GetMinBoundingBox().width);
		      nOp++;
		    }
		}

	      if (overScript)
		{
		  if (!overOp)
		    {
		      wOther = std::max(wOther, overScript->GetBoundingBox().width);
		      nOther++;
		    } 
		  else
		    {
		      wOp = std::max(wOp, overOp->GetMinBoundingBox().width);
		      nOp++;
		    }
		}

	      if (nOp > 0) {
		scaled w = (nOther == 0) ? wOp : wOther;

		if (baseOp) baseOp->HorizontalStretchTo(w);
		if (underOp) underOp->HorizontalStretchTo(w);
		if (overOp) overOp->HorizontalStretchTo(w);
	      }

	      if (baseOp) base->DoLayout(ctxt);
	      if (underScript && underOp) underScript->DoLayout(ctxt);
	      if (overScript && overOp) overScript->DoLayout(ctxt);
	    }

	  const BoundingBox& baseBox = base->GetBoundingBox();
	  //SmartPtr<const MathMLCharNode> bChar = base->GetCharNode();

	  if (underScript)
	    {
#if 0
	      SmartPtr<MathMLCharNode> cChar = underScript->GetCharNode();

	      if (accentUnder &&
		  bChar && cChar &&
		  isCombiningBelow(cChar->GetChar()) &&
		  bChar->CombineWith(cChar, underShiftX, underShiftY))
		{
		  Globals::logger(LOG_DEBUG, "this is the special handling for U+%04X used as accent under U+%04X",
				  cChar->GetChar(), bChar->GetChar());

		  underShiftY = -underShiftY;

#if defined(ENABLE_EXTENSIONS)
		  if (SmartPtr<MathMLOperatorElement> coreOp = underScript->GetCoreOperatorTop())
		    underShiftY += coreOp->GetTopPadding();
#endif
		} 
	      else
#endif
		{
		  const BoundingBox& scriptBox = underScript->GetBoundingBox();

		  underShiftX = (baseBox.width - scriptBox.width) / 2;
		  underShiftY = baseBox.depth + underSpacing + scriptBox.height;
		  underClearance = ruleThickness;
		}
	    }

	  if (overScript)
	    {
#if 0
	      SmartPtr<MathMLCharNode> cChar = overScript->GetCharNode();

	      if (accent &&
		  bChar && cChar &&
		  isCombiningAbove(cChar->GetChar()) &&
		  bChar->CombineWith(cChar, overShiftX, overShiftY))
		{
		  Globals::logger(LOG_DEBUG, "this is the special handling for U+%04X used as accent over U+%04X",
				  cChar->GetChar(), bChar->GetChar());

#if defined(ENABLE_EXTENSIONS)
		  if (SmartPtr<MathMLOperatorElement> coreOp = overScript->GetCoreOperatorTop())
		    {
		      Globals::logger(LOG_DEBUG, "the accent will get en extra spacing of %d", sp2ipx(coreOp->GetBottomPadding()));
		      overShiftY += coreOp->GetBottomPadding();
		    }
#endif
		} 
	      else
#endif
		{
		  const BoundingBox& scriptBox = overScript->GetBoundingBox();

		  overShiftX = (baseBox.width - scriptBox.width) / 2;
		  overShiftY = baseBox.height + overSpacing + scriptBox.depth;
		  overClearance = ruleThickness;
		}
	    }

	  baseShiftX = std::max(scaled(0), - std::min(overShiftX, underShiftX));
	}

      overShiftX += baseShiftX;
      underShiftX += baseShiftX;

      box = base->GetBoundingBox();
      box.width += baseShiftX;

      if (underScript)
	{
	  const BoundingBox& scriptBox = underScript->GetBoundingBox();

	  box.width = std::max(box.width, underShiftX + scriptBox.width);
	  box.height   = std::max(box.height, scriptBox.height - underShiftY);
	  box.depth  = std::max(box.depth, scriptBox.depth + underShiftY);
	  box.depth += underClearance;
	}

      if (overScript)
	{
	  const BoundingBox& scriptBox = overScript->GetBoundingBox();

	  box.width = std::max(box.width, overShiftX + scriptBox.width);
	  box.height   = std::max(box.height, scriptBox.height + overShiftY);
	  box.depth  = std::max(box.depth, scriptBox.depth - overShiftY);
	  box.height += overClearance;
	}

      DoEmbellishmentLayout(this, box);

      resetDirtyLayout(ctxt);
    }
}
#endif

AreaRef
MathMLUnderOverElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      bool scriptize = false;
      bool accent = false;
      bool accentUnder = false;

      SmartPtr<MathMLOperatorElement> baseOp = base ? base->GetCoreOperator() : 0;
      SmartPtr<MathMLOperatorElement> underOp = underScript ? underScript->GetCoreOperator() : 0;
      SmartPtr<MathMLOperatorElement> overOp = overScript ? overScript->GetCoreOperator() : 0;

      if (!scriptize)
	{
	  if (underScript)
	    {
	      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, UnderOver, accentunder))
		accentUnder = ToBoolean(value);
	      else if (underOp)
		accentUnder = underOp->IsAccent();
	    }

	  if (overScript)
	    {
	      if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(MathML, UnderOver, accent))
		accent = ToBoolean(value);
	      else if (overOp)
		accent = overOp->IsAccent();
	    }
	}

      ctxt.push(this);
      ctxt.setDisplayStyle(false);
      if (!accentUnder) ctxt.addScriptLevel(1);
      AreaRef underArea = underScript ? underScript->format(ctxt) : 0;
      ctxt.pop();

      ctxt.push(this);
      ctxt.setDisplayStyle(false);
      if (!accent) ctxt.addScriptLevel(1);
      AreaRef overArea = overScript ? overScript->format(ctxt) : 0;
      ctxt.pop();

      ctxt.push(this);
      AreaRef baseArea = base ? base->format(ctxt) : 0;
      ctxt.pop();

      AreaRef res = ctxt.getDevice()->underOver(ctxt, baseArea,
						underArea, accentUnder,
						overArea, accent);
      setArea(ctxt.getDevice()->wrapper(ctxt, MathMLEmbellishment::formatEmbellishment(this, ctxt, res)));

      resetDirtyLayout();
    }

  return getArea();
}

#if 0
void
MathMLUnderOverElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;

  SetEmbellishmentPosition(this, x, y);

  if (base) base->SetPosition(x + baseShiftX, y);

  if (underScript)
    underScript->SetPosition(x + underShiftX, y + underShiftY);

  if (overScript)
    overScript->SetPosition(x + overShiftX, y - overShiftY);
}

void
MathMLUnderOverElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      RenderBackground(area);
      assert(base);
      base->Render(area);
      if (underScript) underScript->Render(area);
      if (overScript) overScript->Render(area);
      ResetDirty();
    }
}

SmartPtr<MathMLElement>
MathMLUnderOverElement::Inside(const scaled& x, const scaled& y)
{
  if (!IsInside(x, y)) return 0;

  SmartPtr<MathMLElement> inside;
  if (base && (inside = base->Inside(x, y))) return inside;
  if (underScript && (inside = underScript->Inside(x, y))) return inside;
  if (overScript && (inside = overScript->Inside(x, y))) return inside;

  return this;
}

void
MathMLUnderOverElement::ReleaseGCs()
{
  MathMLElement::ReleaseGCs();
  if (base) base->ReleaseGCs();
  if (underScript) underScript->ReleaseGCs();
  if (overScript) overScript->ReleaseGCs();
}

scaled
MathMLUnderOverElement::GetLeftEdge() const
{
  assert(base);
  scaled m = base->GetLeftEdge();
  if (underScript) m = std::min(m, underScript->GetLeftEdge());
  if (overScript) m = std::min(m, overScript->GetLeftEdge());
  return m;
}

scaled
MathMLUnderOverElement::GetRightEdge() const
{
  assert(base);
  scaled m = base->GetRightEdge();
  if (underScript) m = std::max(m, underScript->GetRightEdge());
  if (overScript) m = std::max(m, overScript->GetRightEdge());
  return m;
}
#endif

SmartPtr<MathMLOperatorElement>
MathMLUnderOverElement::GetCoreOperator()
{
  if (base) return base->GetCoreOperator();
  else return 0;
}

void
MathMLUnderOverElement::setDirtyAttribute()
{
  setDirtyAttributeD();
}

void
MathMLUnderOverElement::setFlagDown(Flags f)
{
  MathMLElement::setFlagDown(f);
  if (base) base->setFlagDown(f);
  if (underScript) underScript->setFlagDown(f);
  if (overScript) overScript->setFlagDown(f);
}

void
MathMLUnderOverElement::resetFlagDown(Flags f)
{
  MathMLElement::resetFlagDown(f);
  if (base) base->resetFlagDown(f);
  if (underScript) underScript->resetFlagDown(f);
  if (overScript) overScript->resetFlagDown(f);
}
