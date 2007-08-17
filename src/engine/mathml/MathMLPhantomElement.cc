// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>

#include <cassert>

#include "AreaFactory.hh"
#include "MathMLPhantomElement.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLPhantomElement::MathMLPhantomElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLNormalizingContainerElement(context)
{ }

MathMLPhantomElement::~MathMLPhantomElement()
{ }

bool
MathMLPhantomElement::IsSpaceLike() const
{
  assert(getChild());
  return getChild()->IsSpaceLike();
}

AreaRef
MathMLPhantomElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      AreaRef res = getChild() ? getChild()->format(ctxt) : 0;
      
      if (res)
	res = ctxt.MGD()->wrapper(ctxt, MathMLEmbellishment::formatEmbellishment(this, ctxt, ctxt.MGD()->getFactory()->hide(res)));
      else
	res = 0;
      setArea(res);
      ctxt.pop();

      resetDirtyLayout();
    }

  return getArea();
}

SmartPtr<MathMLOperatorElement>
MathMLPhantomElement::getCoreOperator()
{
  return getChild() ? getChild()->getCoreOperator() : 0;
}
