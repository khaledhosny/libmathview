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
#include "traverseAux.hh"
#include "MathMLDummyElement.hh"
#include "MathMLScriptElement.hh"
#include "MathMLOperatorElement.hh"
#include "RenderingEnvironment.hh"
#include "FormattingContext.hh"

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
MathMLScriptElement::SetBase(const Ptr<MathMLElement>& elem)
{
  if (elem != base)
    {
      if (base) base->SetParent(0);
      if (elem) elem->SetParent(this);
      base = elem;
      SetDirtyLayout();
    }
}

void
MathMLScriptElement::SetSubScript(const Ptr<MathMLElement>& elem)
{
  if (elem != subScript)
    {
      if (subScript) subScript->SetParent(0);
      if (elem) elem->SetParent(this);
      subScript = elem;
      SetDirtyLayout();
    }
}

void
MathMLScriptElement::SetSuperScript(const Ptr<MathMLElement>& elem)
{
  if (elem != superScript)
    {
      if (superScript) superScript->SetParent(0);
      if (elem) elem->SetParent(this);
      superScript = elem;
      SetDirtyLayout();
    }
}

void
MathMLScriptElement::Replace(const Ptr<MathMLElement>& oldElem, const Ptr<MathMLElement>& newElem)
{
  assert(oldElem);
  if (oldElem == base) SetBase(newElem);
  if (oldElem == subScript) SetSubScript(newElem);
  if (oldElem == superScript) SetSuperScript(newElem);
}

Ptr<MathMLElement>
MathMLScriptElement::Inside(scaled x, scaled y)
{
  if (!IsInside(x, y)) return 0;

  Ptr<MathMLElement> inside;
  assert(base);
  if (inside = base->Inside(x, y)) return inside;
  if (subScript && (inside = subScript->Inside(x, y))) return inside;
  if (superScript && (inside = superScript->Inside(x, y))) return inside;

  return this;
}

void
MathMLScriptElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (GetDOMElement())
	{
	  assert(IsA() == TAG_MSUB || IsA() == TAG_MSUP || IsA() == TAG_MSUBSUP);
	  ChildList children(GetDOMElement(), MATHML_NS_URI, "*");
	  
	  if (Ptr<MathMLElement> e = doc->getFormattingNode(children.item(0)))
	    SetBase(e);
	  else if (!is_a<MathMLDummyElement>(GetBase()))
	    SetBase(MathMLDummyElement::create());

	  switch (IsA())
	    {
	    case TAG_MSUB:
	      if (Ptr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetSubScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSubScript()))
		SetSubScript(MathMLDummyElement::create());
	      SetSuperScript(0);
	      break;
	    case TAG_MSUP:
	      SetSubScript(0);
	      if (Ptr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetSuperScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSuperScript()))
		SetSuperScript(MathMLDummyElement::create());
	      break;
	    case TAG_MSUBSUP:
	      if (Ptr<MathMLElement> e = doc->getFormattingNode(children.item(1)))
		SetSubScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSubScript()))
		SetSubScript(MathMLDummyElement::create());
	      if (Ptr<MathMLElement> e = doc->getFormattingNode(children.item(2)))
		SetSuperScript(e);
	      else if (!is_a<MathMLDummyElement>(GetSuperScript()))
		SetSuperScript(MathMLDummyElement::create());
	      break;
	    default:
	      assert(0);
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

      const Value* value = NULL;

      if (subScript)
	{
	  subScript->Setup(env);

	  value = GetAttributeValue(ATTR_SUBSCRIPTSHIFT, env, false);
	  if (value != NULL)
	    {
	      assert(value->IsNumberUnit());

	      UnitValue unitValue = value->ToNumberUnit();
	      assert(!unitValue.IsPercentage());

	      subMinShift = env.ToScaledPoints(unitValue);

	      delete value;
	    }
	}

      if (superScript)
	{
	  superScript->Setup(env);

	  value = GetAttributeValue(ATTR_SUPERSCRIPTSHIFT, env, false);
	  if (value != NULL)
	    {
	      assert(value->IsNumberUnit());
      
	      UnitValue unitValue = value->ToNumberUnit();
	      assert(!unitValue.IsPercentage());

	      superMinShift = env.ToScaledPoints(unitValue);

	      delete value;
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
      if (base) base->DoLayout(ctxt);
      if (subScript) subScript->DoLayout(ctxt);
      if (superScript) superScript->DoLayout(ctxt);

      Ptr<MathMLElement> rel = findRightmostChild(base);
      assert(rel);

      const BoundingBox& baseBox = base->GetBoundingBox();
      BoundingBox relBox = rel->GetBoundingBox();
      relBox.rBearing = baseBox.rBearing;
      relBox.width = baseBox.width;

      BoundingBox subScriptBox;
      BoundingBox superScriptBox;

      subScriptBox.Null();
      if (subScript) subScriptBox = subScript->GetBoundingBox();

      superScriptBox.Null();
      if (superScript) superScriptBox = superScript->GetBoundingBox();

      DoScriptLayout(relBox, subScriptBox, superScriptBox, subShiftX, subShiftY, superShiftX, superShiftY);

      box = baseBox;

      box.width = scaledMax(box.width,
			    scaledMax(superShiftX + superScriptBox.width,
				      subShiftX + subScriptBox.width));
      box.rBearing = scaledMax(box.rBearing,
			       scaledMax(superShiftX + superScriptBox.rBearing,
					 subShiftX + subScriptBox.rBearing));

      if (subScript)
	{
	  box.ascent   = scaledMax(box.ascent, subScriptBox.ascent - subShiftY);
	  box.descent  = scaledMax(box.descent, subScriptBox.descent + subShiftY);
	}

      if (superScript)
	{
	  box.ascent   = scaledMax(box.ascent, superScriptBox.ascent + superShiftY);
	  box.descent  = scaledMax(box.descent, superScriptBox.descent - superShiftY);
	}

      DoEmbellishmentLayout(this, box);

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLScriptElement::SetPosition(scaled x, scaled y)
{
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
  if (subScript) m = scaledMin(m, subScript->GetLeftEdge());
  if (superScript) m = scaledMin(m, superScript->GetLeftEdge());
  return m;
}

scaled
MathMLScriptElement::GetRightEdge() const
{
  assert(base);
  scaled m = base->GetRightEdge();
  if (subScript) m = scaledMax(m, subScript->GetRightEdge());
  if (superScript) m = scaledMax(m, superScript->GetRightEdge());
  return m;
}

Ptr<class MathMLOperatorElement>
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
