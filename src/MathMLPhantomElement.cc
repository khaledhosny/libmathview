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

#include "MathMLPhantomElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLPhantomElement::MathMLPhantomElement(const SmartPtr<class MathMLView>& view)
  : MathMLNormalizingContainerElement(view)
{
}

MathMLPhantomElement::~MathMLPhantomElement()
{
}

bool
MathMLPhantomElement::IsSpaceLike() const
{
  assert(child);
  return child->IsSpaceLike();
}

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

AreaRef
MathMLPhantomElement::format(MathFormattingContext& ctxt)
{
  if (DirtyLayout())
    {
      ctxt.push(this);
      AreaRef res = child ? child->format(ctxt) : 0;
      
      if (res)
	res = ctxt.getDevice()->wrapper(ctxt, MathMLEmbellishment::formatEmbellishment(this, ctxt, ctxt.getDevice()->getFactory()->hide(res)));
      else
	res = 0;
      setArea(res);
      ctxt.pop();

      ResetDirtyLayout();
    }

  return getArea();
}

void
MathMLPhantomElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  if (GetChild()) GetChild()->SetPosition(x, y);
}

void
MathMLPhantomElement::Render(const DrawingArea&)
{
  ResetDirty();
}

SmartPtr<MathMLOperatorElement>
MathMLPhantomElement::GetCoreOperator()
{
  if (GetChild()) return GetChild()->GetCoreOperator();
  else return 0;
}
