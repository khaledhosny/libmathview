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
#include "StringUnicode.hh"
#include "AttributeParser.hh"
#include "MathMLActionElement.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"

MathMLActionElement::MathMLActionElement(void)
{
  selection = 0;
}

#if defined(HAVE_GMETADOM)
  MathMLActionElement::MathMLActionElement(const DOM::Element& node)
  : MathMLLinearContainerElement(node)
{
  selection = 0;
}
#endif

MathMLActionElement::~MathMLActionElement()
{
}

const AttributeSignature*
MathMLActionElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_ACTIONTYPE, NULL,          NULL,             NULL },
    { ATTR_SELECTION,  integerParser, new StringC("1"), NULL },
    { ATTR_NOTVALID,   NULL,          NULL,             NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLContainerElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLActionElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      const String* sValue = GetAttribute(ATTR_ACTIONTYPE, env, false);
      if (sValue != NULL) {
	if (!sValue->Equal("toggle"))
	  Globals::logger(LOG_WARNING, "action `%s' is not supported (ignored)", sValue->ToStaticC());
      } else
	Globals::logger(LOG_WARNING, "no action specified for `maction' element");

      const Value* value = GetAttributeValue(ATTR_SELECTION, env);
      if (value != NULL) SetSelectedIndex(value->ToInteger());

      if (Ptr<MathMLElement> elem = GetSelectedElement()) elem->Setup(env);
      //MathMLLinearContainerElement::Setup(env);

      ResetDirtyAttribute();
    }
}

void
MathMLActionElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      if (Ptr<MathMLElement> elem = GetSelectedElement())
	{
	  elem->DoLayout(ctxt);
	  box = elem->GetBoundingBox();
	}
      else
	box.Null();

      DoEmbellishmentLayout(this, box);

      ResetDirtyLayout(ctxt);
    }
}

void
MathMLActionElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (Ptr<MathMLElement> elem = GetSelectedElement()) elem->SetPosition(x, y);
}

void
MathMLActionElement::Render(const DrawingArea& area)
{
  if (Dirty())
    {
      if (Ptr<MathMLElement> elem = GetSelectedElement()) elem->Render(area);
      ResetDirty();
    }
}

#if 0
void
MathMLActionElement::SetDirty(const Rectangle* rect)
{
  if (Ptr<MathMLElement> elem = GetSelectedElement())
    {
      elem->SetDirty(rect);
      // dirty-children has to be called explicitly because if the child is already
      // dirty, then it does not invoke SetDirtyChildren by itself
      // (see MathMLFrame.hh)
      SetDirtyChildren();
      //dirty = elem->IsDirty();
    }
}
#endif

Ptr<MathMLElement>
MathMLActionElement::GetSelectedElement() const
{
  return (selection < content.size()) ? content[selection] : Ptr<MathMLElement>(0);
}

void
MathMLActionElement::SetSelectedIndex(unsigned i)
{
  if (content.size() > 0 && selection != (i - 1) % content.size())
    {
      selection = (i - 1) % content.size();
      if (Ptr<MathMLElement> elem = GetSelectedElement())
	{
	  elem->SetDirtyLayout();
	  if (elem->DirtyAttribute() || elem->DirtyAttributeP()) this->SetDirtyAttribute();
	}
      // has to set DirtyLayout itself because if the children hasn't been visited yet
      // then its Dirtylayout flag is still set and it won't be propagated up
      SetDirtyLayout();
    }
}

unsigned
MathMLActionElement::GetSelectedIndex() const
{
  return (content.size() > 0) ? selection + 1 : 0;
}

scaled
MathMLActionElement::GetLeftEdge() const
{
  if (Ptr<MathMLElement> elem = GetSelectedElement())
    return elem->GetLeftEdge();
  else
    return GetX();
}

scaled
MathMLActionElement::GetRightEdge() const
{
  if (Ptr<MathMLElement> elem = GetSelectedElement())
    return elem->GetRightEdge();
  else
    return GetX();
}

Ptr<MathMLElement>
MathMLActionElement::Inside(scaled x, scaled y)
{
  if (IsInside(x, y))
    {
      if (Ptr<MathMLElement> elem = GetSelectedElement())
	return elem->Inside(x, y);
      else
	return this;
    }
  else
    return 0;
}

#if 0
void
MathMLActionElement::SetSelected()
{
  if (IsSelected()) return;

  selected = 1;

  Ptr<MathMLElement> elem = GetSelectedElement();
  if (elem) elem->SetSelected();

  SetDirty();
}

void
MathMLActionElement::ResetSelected()
{
  if (!IsSelected()) return;

  SetDirty();

  Ptr<MathMLElement> elem = GetSelectedElement();
  if (elem) elem->ResetSelected();

  selected = 0;
}

void
MathMLActionElement::SetDirtyLayout(bool children)
{
  MathMLElement::SetDirtyLayout(children);
  if (children) {
    Ptr<MathMLElement> elem = GetSelectedElement();
    if (elem) elem->SetDirtyLayout(children);
  }
}
#endif

#if 0
void
MathMLActionElement::SetFlagDown(Flags f)
{
  MathMLElement::SetFlag(f);
  if (Ptr<MathMLElement> elem = GetSelectedElement()) elem->SetFlagDown(f);
}

void
MathMLActionElement::ResetFlagDown(Flags f)
{
  MathMLElement::ResetFlag(f);
  if (Ptr<MathMLElement> elem = GetSelectedElement()) elem->ResetFlagDown(f);
}
#endif

Ptr<MathMLOperatorElement>
MathMLActionElement::GetCoreOperator()
{
  if (Ptr<MathMLElement> elem = GetSelectedElement())
    return elem->GetCoreOperator();
  else
    return 0;
}
