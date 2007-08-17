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

#include "MathMLElement.hh"
#include "MathMLBoxMLAdapter.hh"
#include "BoxMLNamespaceContext.hh"
#include "MathMLNamespaceContext.hh"
#include "View.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "BoxGraphicDevice.hh"

MathMLBoxMLAdapter::MathMLBoxMLAdapter(const SmartPtr<MathMLNamespaceContext>& context)
  : MathMLElement(context)
{ }

MathMLBoxMLAdapter::~MathMLBoxMLAdapter()
{ }

SmartPtr<MathMLBoxMLAdapter>
MathMLBoxMLAdapter::create(const SmartPtr<MathMLNamespaceContext>& context)
{ return new MathMLBoxMLAdapter(context); }

AreaRef
MathMLBoxMLAdapter::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      ctxt.setMathMode(false);
      if (SmartPtr<BoxMLElement> child = getChild())
	setArea(ctxt.MGD()->wrapper(ctxt, child->format(ctxt)));
      else
	setArea(ctxt.MGD()->dummy(ctxt));
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
