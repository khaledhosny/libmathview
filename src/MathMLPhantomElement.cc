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

#include "MathMLPhantomElement.hh"
#include "MathMLOperatorElement.hh"

MathMLPhantomElement::MathMLPhantomElement()
{
}

#if defined(HAVE_GMETADOM)
MathMLPhantomElement::MathMLPhantomElement(const DOM::Element& node)
  : MathMLNormalizingContainerElement(node)
{
}
#endif

MathMLPhantomElement::~MathMLPhantomElement()
{
}

bool
MathMLPhantomElement::IsSpaceLike() const
{
  assert(child);
  return child->IsSpaceLike();
}

#if 0
void
MathMLPhantomElement::Normalize(const Ptr<MathMLDocument>& doc)
{
  if (DirtyStructure())
    {
      MathMLNormalizingContainerElement::Normalize(doc);
      if (Ptr<MathMLOperatorElement> coreOp = GetCoreOperator())
	{
	  Ptr<MathMLEmbellishedOperatorElement> eOp = coreOp->GetEmbellishment();
	  assert(eOp && eOp->GetParent() == this);
	  eOp->Lift(doc);
	}
      ResetDirtyStructure();
    }
}
#endif

void
MathMLPhantomElement::DoLayout(const FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      MathMLNormalizingContainerElement::DoLayout(ctxt);
      DoEmbellishmentLayout(this, box);
      ResetDirtyLayout(ctxt);
    }
}

void
MathMLPhantomElement::SetPosition(scaled x, scaled y)
{
  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (GetChild()) GetChild()->SetPosition(x, y);
}

void
MathMLPhantomElement::Render(const DrawingArea&)
{
  if (Dirty()) ResetDirty();
}

Ptr<MathMLOperatorElement>
MathMLPhantomElement::GetCoreOperator()
{
  if (GetChild()) return GetChild()->GetCoreOperator();
  else return 0;
}
