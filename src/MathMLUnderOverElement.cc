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

#include "Globals.hh"
#include "ChildList.hh"
#include "scaledConv.hh"
#include "operatorAux.hh"
#include "traverseAux.hh"
#include "MathMLCharNode.hh"
#include "MathMLDummyElement.hh"
#include "RenderingEnvironment.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLUnderOverElement.hh"
#include "FormattingContext.hh"

MathMLUnderOverElement::MathMLUnderOverElement()
{
  underScript = overScript = 0;
}

#if defined(HAVE_GMETADOM)
MathMLUnderOverElement::MathMLUnderOverElement(const DOM::Element& node)
  : MathMLContainerElement(node)
{
}
#endif

MathMLUnderOverElement::~MathMLUnderOverElement()
{
}

const AttributeSignature*
MathMLUnderOverElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature underSig[] = {
    { ATTR_ACCENTUNDER, booleanParser, NULL, NULL },
    { ATTR_NOTVALID,    NULL,          NULL, NULL }
  };

  static AttributeSignature overSig[] = {
    { ATTR_ACCENT,      booleanParser, NULL, NULL },
    { ATTR_NOTVALID,    NULL,          NULL, NULL }
  };

  const AttributeSignature* signature = NULL;
  if (IsA() == TAG_MUNDER || IsA() == TAG_MUNDEROVER)
    signature = GetAttributeSignatureAux(id, underSig);
  if (signature == NULL && (IsA() == TAG_MOVER || IsA() == TAG_MUNDEROVER))
    signature = GetAttributeSignatureAux(id, overSig);    
  if (signature == NULL)
    signature = MathMLContainerElement::GetAttributeSignature(id);
  
  return signature;
}

void
MathMLUnderOverElement::SetBase(const SmartPtr<MathMLElement>& elem)
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
MathMLUnderOverElement::SetUnderScript(const SmartPtr<MathMLElement>& elem)
{
  if (elem != underScript)
    {
      if (underScript) underScript->Unlink();
      if (elem) elem->Link(this);
      underScript = elem;
      SetDirtyLayout();
    }
}

void
MathMLUnderOverElement::SetOverScript(const SmartPtr<MathMLElement>& elem)
{
  if (elem != overScript)
    {
      if (overScript) overScript->Unlink();
      if (elem) elem->Link(this);
      overScript = elem;
      SetDirtyLayout();
    }
}

void
MathMLUnderOverElement::Replace(const SmartPtr<MathMLElement>& oldElem, const SmartPtr<MathMLElement>& newElem)
{
  assert(oldElem);
  if (oldElem == base) SetBase(newElem);
  else if (oldElem == underScript) SetUnderScript(newElem);
  else if (oldElem == overScript) SetOverScript(newElem);
  else assert(0);
}

void
MathMLUnderOverElement::Normalize(const SmartPtr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  assert(IsA() == TAG_MUNDER || IsA() == TAG_MOVER || IsA() == TAG_MUNDEROVER);
	  ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
	  
	  if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(0)))
	    SetBase(e);
	  else if (!is_a<MathMLDummyElement>(GetBase()))
	    SetBase(MathMLDummyElement::create());

	  switch (IsA())
	    {
	    case TAG_MUNDER:
	      if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetUnderScript(e);
	      else if (!is_a<MathMLDummyElement>(GetUnderScript()))
		SetUnderScript(MathMLDummyElement::create());
	      SetOverScript(0);
	      break;
	    case TAG_MOVER:
	      SetUnderScript(0);
	      if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetOverScript(e);
	      else if (!is_a<MathMLDummyElement>(GetOverScript()))
		SetOverScript(MathMLDummyElement::create());
	      break;
	    case TAG_MUNDEROVER:
	      if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetUnderScript(e);
	      else if (!is_a<MathMLDummyElement>(GetUnderScript()))
		SetUnderScript(MathMLDummyElement::create());
	      if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(2)))
		SetOverScript(e);
	      else if (!is_a<MathMLDummyElement>(GetOverScript()))
		SetOverScript(MathMLDummyElement::create());
	      break;
	    default:
	      assert(0);
	    }
	}
#endif // HAVE_GMETADOM

      assert(base);
      base->Normalize(doc);
      if (underScript) underScript->Normalize(doc);
      if (overScript) overScript->Normalize(doc);

      ResetDirtyStructure();
    }
}

void
MathMLUnderOverElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
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
	      const Value* value = GetAttributeValue(ATTR_ACCENTUNDER, env, false);
	      if (value != NULL) accentUnder = value->ToBoolean();
	      else
		{
		  SmartPtr<MathMLOperatorElement> op = underScript->GetCoreOperator();
		  if (op)
		    {
		      underScript->Setup(env);
		      accentUnder = op->IsAccent();
		    }
		}
	    }

	  if (accentUnder) underSpacing = smallSpacing;
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
	      const Value* value = GetAttributeValue(ATTR_ACCENT, env, false);
	      if (value != NULL) accent = value->ToBoolean();
	      else
		{
		  SmartPtr<MathMLOperatorElement> op = overScript->GetCoreOperator();
		  if (op)
		    {
		      overScript->Setup(env);
		      accent = op->IsAccent();
		    }
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

      ResetDirtyAttribute();
    }
}

void
MathMLUnderOverElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
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
	  if (ctxt.GetLayoutType() != LAYOUT_AUTO)
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

	      SmartPtr<MathMLOperatorElement> baseOp  = findStretchyOperator(base, STRETCH_HORIZONTAL);
	      SmartPtr<MathMLOperatorElement> underOp = findStretchyOperator(underScript, STRETCH_HORIZONTAL);
	      SmartPtr<MathMLOperatorElement> overOp  = findStretchyOperator(overScript, STRETCH_HORIZONTAL);

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
	  SmartPtr<const MathMLCharNode> bChar = base->GetCharNode();

	  if (underScript)
	    {
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
		{
		  const BoundingBox& scriptBox = underScript->GetBoundingBox();

		  underShiftX = (baseBox.width - scriptBox.width) / 2;
		  underShiftY = baseBox.depth + underSpacing + scriptBox.height;
		  underClearance = ruleThickness;
		}
	    }

	  if (overScript)
	    {
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

      ResetDirtyLayout(ctxt);
    }
}

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
  if (Dirty())
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

SmartPtr<MathMLOperatorElement>
MathMLUnderOverElement::GetCoreOperator()
{
  if (base) return base->GetCoreOperator();
  else return 0;
}

void
MathMLUnderOverElement::SetDirtyAttribute()
{
  SetDirtyAttributeD();
}

void
MathMLUnderOverElement::SetFlagDown(Flags f)
{
  MathMLElement::SetFlagDown(f);
  if (base) base->SetFlagDown(f);
  if (underScript) underScript->SetFlagDown(f);
  if (overScript) overScript->SetFlagDown(f);
}

void
MathMLUnderOverElement::ResetFlagDown(Flags f)
{
  MathMLElement::ResetFlagDown(f);
  if (base) base->ResetFlagDown(f);
  if (underScript) underScript->ResetFlagDown(f);
  if (overScript) overScript->ResetFlagDown(f);
}
