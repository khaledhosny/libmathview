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

#include <cassert>

#include "MathMLEncloseElement.hh"
#include "MathMLRadicalElement.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"
#include "ValueConversion.hh"
#include "MathMLAttributeSignatures.hh"

MathMLEncloseElement::MathMLEncloseElement(const SmartPtr<MathMLNamespaceContext>& context)
  : MathMLNormalizingContainerElement(context)
{ }

MathMLEncloseElement::~MathMLEncloseElement()
{ }

AreaRef
MathMLEncloseElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      AreaRef res;
      if (getChild())
	{
	  res = getChild()->format(ctxt);
	  SmartPtr<ValueSequence> type = ToSequence(GET_ATTRIBUTE_VALUE(MathML, Enclose, notation));
	  for (unsigned i = 0; i < type->getSize(); i++)
	    res = ctxt.MGD()->enclose(ctxt, res, ToString(type->getValue(i)));
	}
      else
	res = nullptr;

      setArea(res ? ctxt.MGD()->wrapper(ctxt, res) : nullptr);
      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}
