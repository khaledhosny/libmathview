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

#include <functional>
#include <algorithm>

#include <assert.h>
#include <stddef.h>

#include "Adaptors.hh"
#include "ChildList.hh"
#include "traverseAux.hh"
#include "MathMLRowElement.hh"
#include "MathMLSpaceElement.hh"
#include "MathMLOperatorElement.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLRowElement::MathMLRowElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLLinearContainerElement(context)
{ }

MathMLRowElement::~MathMLRowElement()
{ }

SmartPtr<MathMLRowElement>
MathMLRowElement::create(const SmartPtr<class MathMLNamespaceContext>& context)
{
#if defined(ENABLE_BREAKS)
  return new MathMLRowElement(context);
#else
  return new MathMLRowElement(context);
#endif
}

AreaRef
MathMLRowElement::format(MathFormattingContext& ctxt)
{
  if (dirtyLayout())
    {
      ctxt.push(this);

      bool stretchy = false;
      std::vector< SmartPtr<MathMLOperatorElement> > erow;
      erow.reserve(getSize());
      std::vector<AreaRef> row;
      row.reserve(getSize());
      for (std::vector< SmartPtr<MathMLElement> >::const_iterator elem = content.begin();
	   elem != content.end();
	   elem++)
	if (*elem)
	  if (AreaRef elemArea = (*elem)->format(ctxt))
	    {
	      row.push_back(elemArea);
	      SmartPtr<MathMLOperatorElement> coreOp = (*elem)->getCoreOperatorTop();
	      // WARNING: we can check for IsStretchy only *after* format because it is
	      // at that time that the flags in the operator get set (see MathMLOperatorElement)
	      if (coreOp && !coreOp->IsStretchy()) coreOp = 0;
	      stretchy = stretchy || coreOp;
	      erow.push_back(coreOp);
	    }

      AreaRef res = ctxt.getDevice()->getFactory()->horizontalArray(row);
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
		getChild(i)->setDirtyLayout();
		row[i] = getChild(i)->format(ctxt);
	      }
	  ctxt.setStretchOperator(0);

	  res = ctxt.getDevice()->getFactory()->horizontalArray(row);
	}

      res = formatEmbellishment(this, ctxt, res);
      setArea(ctxt.getDevice()->wrapper(ctxt, res));

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
  for (std::vector< SmartPtr<MathMLElement> >::const_iterator elem = content.begin();
       elem != content.end();
       elem++)
    if (*elem && !(*elem)->IsSpaceLike())
      {
	if (*elem == eOp) position = rowLength;
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
  SmartPtr<MathMLElement> candidate = 0;

  for (std::vector< SmartPtr<MathMLElement> >::const_iterator elem = content.begin();
       elem != content.end();
       elem++)
    if ((*elem) && !(*elem)->IsSpaceLike())
      if (!candidate) candidate = *elem;
      else return 0;

  return candidate ? candidate->getCoreOperator() : 0;
}
