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

#include <functional>
#include <algorithm>

#include <assert.h>
#include <stddef.h>

#include "AreaFactory.hh"
#include "MathMLAdapters.hh"
#include "traverseAux.hh"
#include "MathMLRowElement.hh"
#include "MathMLSpaceElement.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLRowElement::MathMLRowElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLLinearContainerElement(context)
{ }

MathMLRowElement::~MathMLRowElement()
{ }

SmartPtr<MathMLRowElement>
MathMLRowElement::create(const SmartPtr<class MathMLNamespaceContext>& context)
{
  return new MathMLRowElement(context);
}

AreaRef
MathMLRowElement::format(FormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      bool stretchy = false;
      std::vector< SmartPtr<MathMLOperatorElement> > erow;
      erow.reserve(getSize());
      std::vector<AreaRef> row;
      row.reserve(getSize());
      for (const auto & elem : content)
	if (elem)
	  {
	    SmartPtr<MathMLOperatorElement> coreOp = elem->getCoreOperatorTop();
	    /* if we have an operator we must force reformatting cause we want to
	     * get the minimum size operator
	     */
	    if (coreOp) elem->setDirtyLayout();
	    if (AreaRef elemArea = elem->format(ctxt))
	      {
		row.push_back(elemArea);
		// WARNING: we can check for IsStretchy only *after* format because it is
		// at that time that the flags in the operator get set (see MathMLOperatorElement)
		if (coreOp && !coreOp->IsStretchy()) coreOp = nullptr;
		stretchy = stretchy || coreOp;
		erow.push_back(coreOp);
	      }
	  }

      AreaRef res;
      if (row.size() == 1) res = row[0];
      else res = ctxt.MGD()->getFactory()->horizontalArray(row);
      BoundingBox rowBox = res->box();

      if (stretchy)
	{
	  ctxt.setStretchToHeight(rowBox.height);
	  ctxt.setStretchToDepth(rowBox.depth);
	  for (std::vector< SmartPtr<MathMLOperatorElement> >::const_iterator op = erow.begin();
	       op != erow.end();
	       op++)
	    if (*op)
	      {
		const int i = op - erow.begin();
		ctxt.setStretchOperator(*op);
		(*op)->setDirtyLayout();
		row[i] = getChild(i)->format(ctxt);
	      }
	  ctxt.setStretchOperator(nullptr);

	  if (row.size() == 1) res = row[0];
	  else res = ctxt.MGD()->getFactory()->horizontalArray(row);
	}

      res = formatEmbellishment(this, ctxt, res);
      setArea(ctxt.MGD()->wrapper(ctxt, res));

      ctxt.pop();
      resetDirtyLayout();
    }

  return getArea();
}

bool
MathMLRowElement::IsSpaceLike() const
{
  return std::find_if(content.begin(), content.end(),
		      std::not1(IsSpaceLikePredicate())) == content.end();
}

TokenId
MathMLRowElement::GetOperatorForm(const SmartPtr<MathMLElement>& eOp) const
{
  assert(eOp);

  TokenId res = T_INFIX;

  unsigned rowLength = 0;
  unsigned position  = 0;
  for (const auto & elem : content)
    if (elem && !elem->IsSpaceLike())
      {
	if (elem == eOp) position = rowLength;
	rowLength++;
      }
    
  if (rowLength > 1) 
    {
      if (position == 0) res = T_PREFIX;
      else if (position == rowLength - 1) res = T_POSTFIX;
    }

  return res;
}

SmartPtr<class MathMLOperatorElement>
MathMLRowElement::getCoreOperator()
{
  SmartPtr<MathMLElement> candidate = nullptr;

  for (const auto & elem : content)
    if (elem && !elem->IsSpaceLike())
      {
      if (!candidate) candidate = elem;
      else return nullptr;
      }

  return candidate ? candidate->getCoreOperator() : nullptr;
}
