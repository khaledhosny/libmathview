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

#include "MathMLActionElement.hh"
#include "MathMLAlignMarkElement.hh"
#include "MathMLMarkNode.hh"
#include "MathMLTableCellElement.hh"
#include "MathMLTableElement.hh"
#include "MathMLTokenElement.hh"
#include "ValueConversion.hh"
#include "MathMLAttributeSignatures.hh"

MathMLTableCellElement::MathMLTableCellElement(const SmartPtr<class MathMLNamespaceContext>& context)
  : MathMLNormalizingContainerElement(context)
{
  rowIndex = 0;
  columnIndex = 0;
}

MathMLTableCellElement::~MathMLTableCellElement()
{ }

void
MathMLTableCellElement::setSpan(unsigned rSpan, unsigned cSpan)
{
  rowSpan = rSpan;
  columnSpan = cSpan;
}

void
MathMLTableCellElement::setPosition(unsigned i, unsigned j)
{
  rowIndex = i;
  columnIndex = j;
}

void
MathMLTableCellElement::setAlignment(TokenId ra, TokenId ca)
{
  rowAlign = ra;
  columnAlign = ca;
}

TokenId
MathMLTableCellElement::getRowAlign() const
{ return rowAlign; }

TokenId
MathMLTableCellElement::getColumnAlign() const
{ return columnAlign; }

void
MathMLTableCellElement::setDisplacement(const scaled& dx, const scaled& dy)
{
  dispX = dx;
  dispY = dy;
}

void
MathMLTableCellElement::getDisplacement(scaled& dx, scaled& dy) const
{
  dx = dispX;
  dy = dispY;
}

#if 0
void
MathMLTableCellElement::refine(AbstractRefinementContext& context)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      REFINE_ATTRIBUTE(context, MathML, TableCell, rowspan);
      REFINE_ATTRIBUTE(context, MathML, TableCell, columnspan);
      REFINE_ATTRIBUTE(context, MathML, TableCell, rowalign);
      REFINE_ATTRIBUTE(context, MathML, TableCell, columnalign);
      REFINE_ATTRIBUTE(context, MathML, TableCell, groupalign);
      MathMLNormalizingContainerElement::refine(context);
    }
}
#endif

#if 0
void
MathMLTableCellElement::SetupGroups(const SmartPtr<MathMLElement>& elem,
				    bool allowedGroup,
				    bool countGroups,
				    TableCell& status)
{
  // this static method is probably to be rewritten completely
  // as a set of virtual methods in the various Element classes
  assert(elem);
#if 0
  // if countGroups == TRUE  requires: normalization, alignment scopes
  // if countGroups == FALSE requires: normalization, alignment scopes, group alignment 

  if (elem->IsToken())
    {
      SmartPtr<MathMLTokenElement> token = smart_cast<MathMLTokenElement>(elem);
      assert(token);

      // remember that the first mn element, if present, always contains
      // the alignment for decimal point
      if (status.group != NULL && token->IsA() == T_MN &&
	  status.group->GetDecimalPoint() == NULL)
	status.group->SetDecimalPoint(token);

      for (Iterator< SmartPtr<MathMLTextNode> > p(token->GetContent()); p.More(); p.Next())
	{
	  if (p()->IsMark() && status.group != NULL)
	    {
	      SmartPtr<MathMLMarkNode> mark = smart_cast<MathMLMarkNode>(p());
	      assert(mark);
	      status.group->SetAlignmentMark(mark);
	      // only the first alignment mark (if present) in a token is ever taken into account
	      break;
	    }
	}
    }
  else if (elem->IsContainer())
    {
      SmartPtr<MathMLContainerElement> container = smart_cast<MathMLContainerElement>(elem);
      assert(container);

      Iterator< SmartPtr<MathMLElement> > p(container->GetContent());

      switch (container->IsA())
	{
	case T_MPADDED:
	case T_MSQRT:
	  if (status.group != NULL)
	    {
	      while (p.More())
		{
		  SetupGroups(p(), false, countGroups, status);
		  p.Next();
		}
	    }
	  break;
	case T_MSUB:
	case T_MSUP:
	case T_MSUBSUP:
	case T_MUNDER:
	case T_MOVER:
	case T_MUNDEROVER:
	case T_MMULTISCRIPTS:
	case T_MROOT:
	  if (status.group != NULL)
	    SetupGroups(p(), false, countGroups, status);
	  break;
	case T_MACTION:
	  {
	    SmartPtr<MathMLActionElement> action = smart_cast<MathMLActionElement>(container);
	    assert(action);
	    if (action->GetSelectedElement())
	      SetupGroups(action->GetSelectedElement(), allowedGroup, countGroups, status);
	  }
	  break;
	case T_MTD:
	  {
	    SmartPtr<MathMLTableCellElement> mtd = smart_cast<MathMLTableCellElement>(container);
	    assert(mtd);

	    if (status.group != NULL && !mtd->GetAlignmentScope())
	      SetupGroups(p(), false, countGroups, status);
	  }
	  break;
	case T_MTABLE:
	case T_MTR:
	case T_MLABELEDTR:
	default:
	  while (p.More())
	    {
	      SetupGroups(p(), allowedGroup, countGroups, status);
	      p.Next();
	    }
	  break;
	}
    } 
  else
    {
      switch (elem->IsA())
	{
	case TAG_MALIGNGROUP:
	  if (allowedGroup)
	    {
	      if (countGroups)
		{
		  status.nAlignGroup++;
		} 
	      else
		{
		  SmartPtr<MathMLAlignGroupElement> group = smart_cast<MathMLAlignGroupElement>(elem);
		  assert(group);

		  status.aGroup[status.iGroup].group = group;
		  status.group = group;
		  status.iGroup++;
		}
	    }
	  break;
	case TAG_MALIGNMARK:
	  if (status.group != NULL)
	    {
	      SmartPtr<MathMLAlignMarkElement> align = smart_cast<MathMLAlignMarkElement>(elem);
	      assert(align);
	      status.group->SetAlignmentMark(align);
	    }
	  break;
	case TAG_MSPACE:
	default:
	  break;
	}
    }
#endif
}

void
MathMLTableCellElement::SetupCellSpanning(RenderingEnvironment& env)
{
  SmartPtr<Value> value;

  value = GET_ATTRIBUTE_VALUE(TableCell, rowspan);
  rowSpan = ToInteger(value);
  if (rowSpan <= 0)
    {
      getLogger()->out(LOG_WARNING, "<mtd> has rowspan <= 0, defaulted to 1");
      rowSpan = 1;
    }

  value = GET_ATTRIBUTE_VALUE(TableCell, columnspan);
  columnSpan = ToInteger(value);
  if (columnSpan <= 0)
    {
      getLogger()->out(LOG_WARNING, "<mtd> has columnspan <= 0, defaulted to 1");
      columnSpan = 1;
    }
}
#endif

#if 0
void
MathMLTableCellElement::Setup(RenderingEnvironment& env)
{
  if (dirtyAttribute() || dirtyAttributeP())
    {
      // if the <mtd> element is not used inside a table, 
      // the cell field is null
      if (cell)
	{
	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableCell, rowalign))
	    cell->rowAlign = ToTokenId(value);

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableCell, columnalign))
	    cell->columnAlign = ToTokenId(value);

	  if (SmartPtr<Value> value = GET_ATTRIBUTE_VALUE(TableCell, groupalign))
	    for (unsigned k = 0; k < cell->nAlignGroup; k++)
	      {
		SmartPtr<Value> p = GetComponent(value, k);
		cell->aGroup[k].alignment = ToTokenId(p);
	      }
	}
      
      MathMLNormalizingContainerElement::Setup(env);
      resetDirtyAttribute();
    }
}

void
MathMLTableCellElement::SetupCellPosition(unsigned i, unsigned j, unsigned nRows)
{
  rowIndex = i;
  columnIndex = j;

  if (i + rowSpan > nRows)
    {
      getLogger()->out(LOG_WARNING, "`mtd' spans over the last row (truncated)");
      rowSpan = nRows - i;
    }
}

void
MathMLTableCellElement::SetupCell(TableCell* p)
{
  assert(p);
  cell = p;
}

void
MathMLTableCellElement::DoLayout(const FormattingContext& ctxt)
{
  if (GetChild())
    {
      GetChild()->DoLayout(ctxt);
      box = GetChild()->GetBoundingBox();
    }
  else
    box.unset();

  if (ctxt.GetLayoutType() == FormattingContext::LAYOUT_MIN) minWidth = box.width;

  resetDirtyLayout(ctxt);
}

void
MathMLTableCellElement::CalcGroupsExtent()
{
  assert(cell != NULL);
  SetPosition(0, 0);

  unsigned k;
  for (k = 0; k < cell->nAlignGroup; k++)
    {
      SmartPtr<MathMLAlignGroupElement> group = cell->aGroup[k].group;
      assert(group);

      SmartPtr<MathMLFrame> leftSibling = getLeftSibling(group);
      SmartPtr<MathMLFrame> rightSibling = getRightSibling(group);

      if (k == 0) cell->aGroup[k].leftEdge = GetLeftEdge();
      else if (rightSibling) cell->aGroup[k].leftEdge = rightSibling->GetLeftEdge();
      else cell->aGroup[k].leftEdge = group->GetX();

      if (k > 0) 
	{
	  if (leftSibling) cell->aGroup[k - 1].rightEdge = leftSibling->GetRightEdge();
	  else cell->aGroup[k - 1].rightEdge = group->GetX();
	}

      if (k == cell->nAlignGroup - 1) cell->aGroup[k].rightEdge = GetRightEdge();
    }

  scaled cellWidth = GetBoundingBox().width;

  scaled prevAlignPoint = 0;

  for (k = 0; k < cell->nAlignGroup; k++) 
    {
      SmartPtr<MathMLAlignGroupElement> group = cell->aGroup[k].group;
      assert(group);

      scaled alignPoint = 0;

      if (group->GetAlignmentMarkElement() ||
	  group->GetAlignmentMarkNode())
	{
	  SmartPtr<MathMLFrame> mark = 0;
	  TokenId alignType = T__NOTVALID;

	  if (group->GetAlignmentMarkElement())
	    {
	      mark = group->GetAlignmentMarkElement();
	      alignType = group->GetAlignmentMarkElement()->GetAlignmentEdge();
	    }
	  else
	    {
	      mark = group->GetAlignmentMarkNode();
	      alignType = group->GetAlignmentMarkNode()->GetAlignmentEdge();
	    }
	  assert(mark);
	  assert(alignType != T__NOTVALID);
      
	  if (alignType == T_LEFT)
	    {
	      SmartPtr<MathMLFrame> frame = getRightSibling(mark);
	      if (frame) alignPoint = frame->GetLeftEdge();
	      else alignPoint = alignPoint = mark->GetX();
	    } 
	  else
	    {
	      SmartPtr<MathMLFrame> frame = getLeftSibling(mark);
	      if (frame) alignPoint = frame->GetRightEdge();
	      else alignPoint = mark->GetX();
	    }
	} 
      else if (cell->aGroup[k].alignment == T_DECIMALPOINT)
	{
	  SmartPtr<MathMLTokenElement> token = group->GetDecimalPoint();
	  if (token) alignPoint = token->GetDecimalPointEdge();
	  else alignPoint = cell->aGroup[k].rightEdge;
	} 
      else
	{
	  switch (cell->aGroup[k].alignment)
	    {
	    case T_LEFT:
	      alignPoint = cell->aGroup[k].leftEdge;
	      break;
	    case T_RIGHT:
	      alignPoint = cell->aGroup[k].rightEdge;
	      break;
	    case T_CENTER:
	      alignPoint = (cell->aGroup[k].leftEdge + cell->aGroup[k].rightEdge) / 2;
	      break;
	    default:
	      assert(IMPOSSIBLE);
	      break;
	    }
	}

      if (k == 0) cell->aGroup[k].extent.left = alignPoint;
      else cell->aGroup[k].extent.left = alignPoint - group->GetX();

      if (k > 0) cell->aGroup[k - 1].extent.right = group->GetX() - prevAlignPoint;
      if (k == cell->nAlignGroup - 1) cell->aGroup[k].extent.right = cellWidth - alignPoint;
      prevAlignPoint = alignPoint;
    }
}

void
MathMLTableCellElement::SetPosition(const scaled& x, const scaled& y)
{
  if (GetChild())
    {
      if (cell)
	{
	  const BoundingBox& elemBox = child->GetBoundingBox();

	  position.x = x;
	  position.y = y;

	  scaled availableWidth  = box.width;

	  scaled cellXOffset = 0;
	  switch (cell->columnAlign) {
	  case T_RIGHT:
	    cellXOffset = availableWidth - elemBox.width;
	    break;
	  case T_CENTER:
	    cellXOffset = (availableWidth - elemBox.width) / 2;
	    break;
	  case T_LEFT:
	  default:
	    cellXOffset = 0;
	    break;
	  }

	  scaled cellYOffset = 0;
	  switch (cell->rowAlign) {
	  case T_BOTTOM:
	    cellYOffset = box.depth - elemBox.depth;
	    break;
	  case T_CENTER:
	    cellYOffset = (box.verticalExtent() - elemBox.verticalExtent()) / 2 +
	      elemBox.height - box.height;
	    break;
	  case T_BASELINE:
	    cellYOffset = 0;
	    break;
	  case T_AXIS:
	    assert(IMPOSSIBLE);
	    break;
	  case T_TOP:
	    cellYOffset = elemBox.height - box.height;
	    break;
	  default:
	    cellYOffset = 0;
	    break;
	  }

	  GetChild()->SetPosition(x + cellXOffset, y + cellYOffset);
	}
      else
	GetChild()->SetPosition(x, y);
    }
}
#endif

#if 0
bool
MathMLTableCellElement::IsStretchyOperator() const
{
#if 0
  if (GetChild()) return isStretchyOperator(GetChild());
  else return false;
#endif
  return false;
}
#endif

void
MathMLTableCellElement::setDirtyStructure()
{
#if 0
  SmartPtr<MathMLTableRowElement> row = getParent<MathMLTableRowElement>();
  assert(row);
  row->setDirtyStructure();
  MathMLNormalizingContainerElement::setDirtyStructure();
#endif
}

void
MathMLTableCellElement::setDirtyAttribute()
{
#if 0
  SmartPtr<MathMLTableRowElement> row = getParent<MathMLTableRowElement>();
  assert(row);
  row->setDirtyAttribute();
  MathMLNormalizingContainerElement::setDirtyAttribute();
#endif
}

void
MathMLTableCellElement::setDirtyLayout()
{
#if 0
  SmartPtr<MathMLTableRowElement> row = getParent<MathMLTableRowElement>();
  assert(row);
  row->setDirtyLayout();
  MathMLNormalizingContainerElement::setDirtyLayout();
#endif
}
