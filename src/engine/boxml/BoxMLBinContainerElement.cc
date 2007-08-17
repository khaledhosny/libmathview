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

#include "BoxMLBinContainerElement.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"

BoxMLBinContainerElement::BoxMLBinContainerElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLElement(context)
{ }

BoxMLBinContainerElement::~BoxMLBinContainerElement()
{ }

AreaRef
BoxMLBinContainerElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);
      if (SmartPtr<BoxMLElement> child = getChild())
	{
	  child->format(ctxt);
	  setMaxArea(ctxt.BGD()->wrapper(ctxt, child->getMaxArea()));
	  setArea(ctxt.BGD()->wrapper(ctxt, child->getArea()));
	}
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
