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
#include "operatorAux.hh"
#include "traverseAux.hh"
#include "MathMLRowElement.hh"
#include "MathMLBreakableRowElement.hh"
#include "MathMLSpaceElement.hh"
#include "MathMLOperatorElement.hh"
#include "FormattingContext.hh"
#include "MathFormattingContext.hh"
#include "MathGraphicDevice.hh"

MathMLRowElement::MathMLRowElement(const SmartPtr<class MathMLView>& view)
  : MathMLLinearContainerElement(view)
{
}

MathMLRowElement::~MathMLRowElement()
{
}

SmartPtr<MathMLRowElement>
MathMLRowElement::create(const SmartPtr<class MathMLView>& view)
{
#if defined(ENABLE_BREAKS)
  return MathMLBreakableRowElement::create(view);
#else
  return new MathMLRowElement(view);
#endif
}

void
MathMLRowElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      MathMLLinearContainerElement::Setup(env);
      ResetDirtyAttribute();
    }
}

void
MathMLRowElement::DoLayout(const class FormattingContext& ctxt)
{
  if (DirtyLayout(ctxt))
    {
      box.unset();
      for (std::vector< SmartPtr<MathMLElement> >::iterator elem = content.begin();
	   elem != content.end();
	   elem++)
	if (*elem)
	  {
	    (*elem)->DoLayout(ctxt);
	    box.append((*elem)->GetBoundingBox());
	  }

      DoStretchyLayout();
      DoEmbellishmentLayout(this, box);
      ResetDirtyLayout(ctxt);
    }
}

AreaRef
MathMLRowElement::format(MathFormattingContext& ctxt)
{
  if (DirtyLayout())
    {
      ctxt.push(this);

      bool stretchy = false;
      std::vector< SmartPtr<MathMLOperatorElement> > erow;
      erow.reserve(content.size());
      std::vector<AreaRef> row;
      row.reserve(content.size());
      for (std::vector< SmartPtr<MathMLElement> >::const_iterator elem = content.begin();
	   elem != content.end();
	   elem++)
	{
	  row.push_back((*elem)->format(ctxt));
	  SmartPtr<MathMLOperatorElement> coreOp = (*elem)->GetCoreOperatorTop();
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
		content[i]->SetDirtyLayout();
		row[i] = content[i]->format(ctxt);
	      }
	  ctxt.setStretchOperator(0);

	  res = ctxt.getDevice()->getFactory()->horizontalArray(row);
	}

      res = formatEmbellishment(this, ctxt, res);
      setArea(res);

      ctxt.pop();
      ResetDirtyLayout();
    }

  return getArea();
}

void
MathMLRowElement::DoStretchyLayout()
{
  unsigned nStretchy = 0; // # of stretchy operators in this line
  unsigned nOther    = 0; // # of non-stretchy elements in this line

  BoundingBox rowBox;
  BoundingBox opBox;

  for (std::vector< SmartPtr<MathMLElement> >::iterator elem = content.begin();
       elem != content.end();
       elem++)
    if (SmartPtr<MathMLOperatorElement> op = findStretchyOperator(*elem))
      {
	opBox.append(op->GetMinBoundingBox());
	nStretchy++;      
      } 
    else if (*elem)
      {
	rowBox.append((*elem)->GetBoundingBox());
	nOther++;
      }

  if (nStretchy > 0)
    {
      scaled toAscent  = (nOther == 0) ? opBox.height : rowBox.height;
      scaled toDescent = (nOther == 0) ? opBox.depth : rowBox.depth;

      for (std::vector< SmartPtr<MathMLElement> >::iterator elem = content.begin();
	   elem != content.end();
	   elem++)
	if (SmartPtr<MathMLOperatorElement> op = findStretchyOperator(*elem))
	  {
	    op->VerticalStretchTo(toAscent, toDescent);
	    (*elem)->DoLayout(FormattingContext(FormattingContext::LAYOUT_AUTO, 0));
	  }
    }
}

void
MathMLRowElement::SetPosition(const scaled& x0, const scaled& y0)
{
  scaled x = x0;
  scaled y = y0;

  position.x = x;
  position.y = y;
  SetEmbellishmentPosition(this, x, y);
  for (std::vector< SmartPtr<MathMLElement> >::iterator elem = content.begin();
       elem != content.end();
       elem++)
    if (*elem)
      {
	(*elem)->SetPosition(x, y);
	x += (*elem)->GetBoundingBox().width;
      }
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
MathMLRowElement::GetCoreOperator()
{
  SmartPtr<MathMLElement> candidate = 0;

  for (std::vector< SmartPtr<MathMLElement> >::const_iterator elem = content.begin();
       elem != content.end();
       elem++)
    if ((*elem) && !(*elem)->IsSpaceLike())
      {
	if (!candidate) candidate = *elem;
	else return 0;
      }

  if (candidate) return candidate->GetCoreOperator();
  else return 0;
}
