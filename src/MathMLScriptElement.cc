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

#include "ChildList.hh"
#include "FormattingContext.hh"
#include "MathMLDummyElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathMLScriptElement.hh"
#include "RenderingEnvironment.hh"
#include "ValueConversion.hh"
#include "traverseAux.hh"

MathMLScriptElement::MathMLScriptElement()
{
  subScript = superScript = 0;  
}

#if defined(HAVE_GMETADOM)
MathMLScriptElement::MathMLScriptElement(const DOM::Element& node)
  : MathMLContainerElement(node)
{
  subScript = superScript = 0;
}
#endif

MathMLScriptElement::~MathMLScriptElement()
{
}

const AttributeSignature*
MathMLScriptElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature subSig[] = {
    { ATTR_SUBSCRIPTSHIFT, numberUnitParser, NULL, NULL },
    { ATTR_NOTVALID,       NULL,             NULL, NULL }
  };

  static AttributeSignature supSig[] = {
    { ATTR_SUPERSCRIPTSHIFT, numberUnitParser, NULL, NULL },
    { ATTR_NOTVALID,         NULL,             NULL, NULL }
  };

  const AttributeSignature* signature = NULL;
  if (IsA() == TAG_MSUB || IsA() == TAG_MSUBSUP || IsA() == TAG_MMULTISCRIPTS)
    signature = GetAttributeSignatureAux(id, subSig);
  if ((IsA() == TAG_MSUP || IsA() == TAG_MSUBSUP || IsA() == TAG_MMULTISCRIPTS) && signature == NULL)
    signature = GetAttributeSignatureAux(id, supSig);

  if (signature == NULL) signature = MathMLContainerElement::GetAttributeSignature(id);

  return signature;
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
MathMLScriptElement::Normalize(const SmartPtr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  assert(IsA() == TAG_MSUB || IsA() == TAG_MSUP || IsA() == TAG_MSUBSUP);
	  ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
	  
	  if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(0)))
	    SetBase(e);
	  else if (!is_a<MathMLDummyElement>(GetBase()))
	    SetBase(MathMLDummyElement::create());

	  switch (IsA())
	    {
	    case TAG_MSUB:
	      if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetSubScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSubScript()))
		SetSubScript(MathMLDummyElement::create());
	      SetSuperScript(0);
	      break;
	    case TAG_MSUP:
	      SetSubScript(0);
	      if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetSuperScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSuperScript()))
		SetSuperScript(MathMLDummyElement::create());
	      break;
	    case TAG_MSUBSUP:
	      if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetSubScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSubScript()))
		SetSubScript(MathMLDummyElement::create());
	      if (SmartPtr<MathMLElement> e = doc->getFormattingNode(children.item(2)))
		SetSuperScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSuperScript()))
		SetSuperScript(MathMLDummyElement::create());
	      break;
	    default:
	      assert(false);
	    }
	}
#endif

      if (base) base->Normalize(doc);
      if (subScript) subScript->Normalize(doc);
      if (superScript) superScript->Normalize(doc);

      ResetDirtyStructure();
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

	  if (SmartPtr<Value> value = GetAttributeValue(ATTR_SUBSCRIPTSHIFT, env, false))
	    {
	      assert(IsNumberUnit(value));
	      UnitValue unitValue = ToNumberUnit(value);
	      assert(!unitValue.IsPercentage());
	      subMinShift = env.ToScaledPoints(unitValue);
	    }
	}

      if (superScript)
	{
	  superScript->Setup(env);

	  if (SmartPtr<Value> value = GetAttributeValue(ATTR_SUPERSCRIPTSHIFT, env, false))
	    {
	      assert(IsNumberUnit(value));
	      UnitValue unitValue = ToNumberUnit(value);
	      assert(!unitValue.IsPercentage());
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
  if (Dirty())
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

#if 0
void
MathMLScriptElement::SetDirty(const Rectangle* rect)
{
  if (!IsDirty() && !HasDirtyChildren())
    {
      MathMLElement::SetDirty(rect);
      if (base) base->SetDirty(rect);
      if (subScript) subScript->SetDirty(rect);
      if (superScript) superScript->SetDirty(rect);
    }
}

void
MathMLScriptElement::SetDirtyLayout(bool children)
{
  MathMLElement::SetDirtyLayout(children);
  if (children)
    {
      if (base) base->SetDirtyLayout(children);
      if (subScript) subScript->SetDirtyLayout(children);
      if (superScript) superScript->SetDirtyLayout(children);
    }
}
#endif

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
