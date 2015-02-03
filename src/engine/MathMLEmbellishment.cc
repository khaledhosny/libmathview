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

#include "AreaFactory.hh"
#include "MathMLEmbellishment.hh"
#include "MathMLFractionElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathGraphicDevice.hh"
#include "FormattingContext.hh"
#include "traverseAux.hh"

inline bool
isOperator(const SmartPtr<MathMLElement>& elem)
{
  if (elem && elem->getCoreOperatorTop())
    return true;
  return false;
}

AreaRef
MathMLEmbellishment::formatEmbellishment(const SmartPtr<MathMLElement>& elem,
					 const FormattingContext& context,
					 const AreaRef& area) const
{
  assert(elem);
  scaled leftPadding = scaled::zero();
  scaled rightPadding = scaled::zero();
  if (SmartPtr<MathMLOperatorElement> top = elem->getCoreOperatorTop())
    {
      // left/right paddings are calculated during formatting, so these
      // quantities should be requested only AFTER that the operator has
      // been formatted. Checking for top->dirtyLayout() is not
      // quite right, though, in the case the operator itself is the
      // top core operator (see MathMLOperatorElement), because the
      // dirtyLayout flag is reset only after the formatEmbellishment
      // method is called
      // assert(!top->dirtyLayout());
      leftPadding = top->getLeftPadding();
      rightPadding = top->getRightPadding();
    }
  else if (is_a<MathMLFractionElement>(elem) && context.getScriptLevel() <= 0)
    {
      // TeX math spacing rule (The TeXBook, p. 181) adds differnt spaces
      // between fractions (Inner in the table) and other nodes, but we ignore
      // operators here as they should be handled by the operator dictionary
      // above. The rest of the rules just add thin space in display and text
      // styles, so that is what we do here.
      scaled thinspace = context.MGD()->evaluate(context, Length(1, Length::THIN_SPACE), scaled::zero());
      if (!isOperator(findLeftSibling(elem)))
        leftPadding = thinspace;
      if (!isOperator(findRightSibling(elem)))
        rightPadding = thinspace;
    }

  if (leftPadding != scaled::zero() || rightPadding != scaled::zero())
    {
      std::vector<AreaRef> row;
      row.reserve(3);
      row.push_back(context.MGD()->getFactory()->horizontalSpace(leftPadding));
      row.push_back(area);
      row.push_back(context.MGD()->getFactory()->horizontalSpace(rightPadding));
      return context.MGD()->getFactory()->horizontalArray(row);
    }
  else
    return area;
}

