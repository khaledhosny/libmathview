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

#include "RenderingEnvironment.hh"
#include "MathMLEncloseElement.hh"
#include "MathMLRadicalElement.hh"
#include "FormattingContext.hh"
#include "ValueConversion.hh"

MathMLEncloseElement::MathMLEncloseElement()
{
  normalized = false;
}

#if defined(HAVE_GMETADOM)
MathMLEncloseElement::MathMLEncloseElement(const DOM::Element& node)
  : MathMLNormalizingContainerElement(node)
{
  normalized = false;
}
#endif

MathMLEncloseElement::~MathMLEncloseElement()
{
}

const AttributeSignature*
MathMLEncloseElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_NOTATION, stringParser, "longdiv", NULL },
    { ATTR_NOTVALID, NULL,         NULL,      NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLEncloseElement::NormalizeRadicalElement(const SmartPtr<MathMLDocument>& doc)
{
  assert(GetChild());

  SmartPtr<MathMLRadicalElement> sqrt = smart_cast<MathMLRadicalElement>(MathMLRadicalElement::create());
  assert(sqrt);

  SmartPtr<MathMLElement> oldChild = GetChild();
  SetChild(0);
  sqrt->SetRadicand(oldChild);
  SetChild(sqrt);
  sqrt->Normalize(doc);
}

void
MathMLEncloseElement::Normalize(const SmartPtr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      if (normalized && GetDOMElement() &&
	  GetChild() && is_a<MathMLRadicalElement>(GetChild()) && !GetChild()->GetParent())
	{
	  // this must be an inferred msqrt element
	  SmartPtr<MathMLRadicalElement> inferredSqrt = smart_cast<MathMLRadicalElement>(GetChild());
	  assert(inferredSqrt);
	  SetChild(inferredSqrt->GetRadicand());
	}
#endif
      normalized = false;
      MathMLNormalizingContainerElement::Normalize(doc);
    }
}

void
MathMLEncloseElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      notation = ToString(GetAttributeValue(ATTR_NOTATION, env));

      spacing = env.ToScaledPoints(env.GetMathSpace(MATH_SPACE_MEDIUM));
      lineThickness = env.GetRuleThickness();
      color = env.GetColor();
      
      if (!normalized)
	{
	  if (notation == "radical") NormalizeRadicalElement(env.GetDocument());
	  normalized = true;
	}

      MathMLNormalizingContainerElement::Setup(env);

      ResetDirtyAttribute();
    }
}

void
MathMLEncloseElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      assert(child);

      MathMLNormalizingContainerElement::DoLayout(ctxt);
      box = child->GetBoundingBox();

      if (notation == "actuarial" || notation == "longdiv")
	{
	  box = child->GetBoundingBox();
	  box.height += spacing + lineThickness;
	  box.width += spacing + lineThickness;
	}

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLEncloseElement::SetPosition(const scaled& x, const scaled& y)
{
  assert(child);

  position.x = x;
  position.y = y;

  if (notation == "longdiv")
    child->SetPosition(x + spacing + lineThickness, y);
  else
    child->SetPosition(x, y);
}

void
MathMLEncloseElement::Render(const DrawingArea& area)
{
  if (Exposed(area))
    {
      MathMLNormalizingContainerElement::Render(area);

      if (fGC[Selected()] == NULL) {
	GraphicsContextValues values;
	values.foreground = Selected() ? area.GetSelectionForeground() : color;
	fGC[Selected()] = area.GetGC(values, GC_MASK_FOREGROUND);
      }

      if (notation == "longdiv") {
        area.MoveTo(GetX() + lineThickness / 2, GetY() + box.depth);
        area.DrawLineTo(fGC[Selected()], GetX() + lineThickness / 2, GetY() - box.height + lineThickness / 2);
        area.DrawLineTo(fGC[Selected()], GetX() + box.width, GetY() - box.height + lineThickness / 2);
      } else if (notation == "actuarial") {
        area.MoveTo(GetX(), GetY() - box.height + lineThickness / 2);
        area.DrawLineTo(fGC[Selected()], GetX() + box.width - lineThickness / 2, GetY() - box.height + lineThickness / 2);
        area.DrawLineTo(fGC[Selected()], GetX() + box.width - lineThickness / 2, GetY() + box.depth);
      } else if (notation == "overstrike") {
        area.MoveTo(GetX(), GetY() - box.height / 2);
        area.DrawLineTo(fGC[Selected()], GetX() + box.width, GetY() - box.height / 2);
      } else if (notation == "NESWslash") {
        area.MoveTo(GetX(), GetY() + box.depth);
        area.DrawLineTo(fGC[Selected()], GetX() + box.width, GetY() - box.height);
      } else if (notation == "NWSEslash") {
        area.MoveTo(GetX(), GetY() - box.height);
        area.DrawLineTo(fGC[Selected()], GetX() + box.width, GetY() + box.depth);
      }

      ResetDirty();
    }
}
