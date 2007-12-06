// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#include <config.h>

#include "BoxMLAtElement.hh"
#include "BoxMLAttributeSignatures.hh"
#include "FormattingContext.hh"
#include "BoxGraphicDevice.hh"
#include "ValueConversion.hh"

BoxMLAtElement::BoxMLAtElement(const SmartPtr<BoxMLNamespaceContext>& context)
  : BoxMLBinContainerElement(context)
{ }

BoxMLAtElement::~BoxMLAtElement()
{ }

SmartPtr<BoxMLAtElement>
BoxMLAtElement::create(const SmartPtr<BoxMLNamespaceContext>& context)
{ return new BoxMLAtElement(context); }

AreaRef
BoxMLAtElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      x = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, At, x)), ctxt.getSize());
      y = ctxt.BGD()->evaluate(ctxt, ToLength(GET_ATTRIBUTE_VALUE(BoxML, At, y)), ctxt.getSize());

      if (getChild())
	{
	  AreaRef res = getChild()->format(ctxt);
	  res = ctxt.BGD()->wrapper(ctxt, res);
	  setArea(res);
	}
      else
	setArea(0);

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
