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

#include "MathMLRadicalElement.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLRadicalElement::MathMLRadicalElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLContainerElement(context)
{ }

MathMLRadicalElement::~MathMLRadicalElement()
{ }

AreaRef
MathMLRadicalElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      AreaRef baseArea = getBase()->format(ctxt);
      AreaRef indexArea;
      if (getIndex())
	{
	  ctxt.push(this);

	  ctxt.setDisplayStyle(false);
	  ctxt.addScriptLevel(2);
	  indexArea = getIndex()->format(ctxt);

	  ctxt.pop();
	}
      AreaRef res = ctxt.MGD()->radical(ctxt, baseArea, indexArea);

      setArea(ctxt.MGD()->wrapper(ctxt, res));

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

void
MathMLRadicalElement::setFlagDown(Flags f)
{
  MathMLContainerElement::setFlagDown(f);
  base.setFlagDown(f);
  index.setFlagDown(f);
}

void
MathMLRadicalElement::resetFlagDown(Flags f)
{
  MathMLContainerElement::resetFlagDown(f);
  base.resetFlagDown(f);
  index.resetFlagDown(f);
}

