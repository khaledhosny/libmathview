// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <assert.h>
#include <stddef.h>

#include "frameAux.hh"
#include "Globals.hh"
#include "operatorAux.hh"
#include "StringUnicode.hh"
#include "MathMLMarkNode.hh"
#include "ValueConversion.hh"
#include "MathMLTableElement.hh"
#include "MathMLTokenElement.hh"
#include "MathMLActionElement.hh"
#include "MathMLAlignMarkElement.hh"
#include "MathMLTableCellElement.hh"
#include "FormattingContext.hh"

MathMLTableCellElement::MathMLTableCellElement()
{
  Init();
}

#if defined(HAVE_GMETADOM)
MathMLTableCellElement::MathMLTableCellElement(const DOM::Element& node)
  : MathMLNormalizingContainerElement(node)
{
  Init();
}
#endif

void
MathMLTableCellElement::Init()
{
  minWidth = 0;
  rowIndex = 0;
  columnIndex = 0;
  cell = NULL;
}

MathMLTableCellElement::~MathMLTableCellElement()
{
}

const AttributeSignature*
MathMLTableCellElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_ROWSPAN,         unsignedIntegerParser,    new StringC("1"), NULL },
    { ATTR_COLUMNSPAN,      unsignedIntegerParser,    new StringC("1"), NULL },
    { ATTR_ROWALIGN,        rowAlignParser,           NULL,             NULL },
    { ATTR_COLUMNALIGN,     columnAlignParser,        NULL,             NULL },
    { ATTR_GROUPALIGN,      groupAlignListParser,     NULL,             NULL },
    { ATTR_NOTVALID,        NULL,                     NULL,             NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLElement::GetAttributeSignature(id);

  return signature;
}

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
      if (status.group != NULL && token->IsA() == TAG_MN &&
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
	case TAG_MPADDED:
	case TAG_MSQRT:
	  if (status.group != NULL)
	    {
	      while (p.More())
		{
		  SetupGroups(p(), false, countGroups, status);
		  p.Next();
		}
	    }
	  break;
	case TAG_MSUB:
	case TAG_MSUP:
	case TAG_MSUBSUP:
	case TAG_MUNDER:
	case TAG_MOVER:
	case TAG_MUNDEROVER:
	case TAG_MMULTISCRIPTS:
	case TAG_MROOT:
	  if (status.group != NULL)
	    SetupGroups(p(), false, countGroups, status);
	  break;
	case TAG_MACTION:
	  {
	    SmartPtr<MathMLActionElement> action = smart_cast<MathMLActionElement>(container);
	    assert(action);
	    if (action->GetSelectedElement())
	      SetupGroups(action->GetSelectedElement(), allowedGroup, countGroups, status);
	  }
	  break;
	case TAG_MTD:
	  {
	    SmartPtr<MathMLTableCellElement> mtd = smart_cast<MathMLTableCellElement>(container);
	    assert(mtd);

	    if (status.group != NULL && !mtd->GetAlignmentScope())
	      SetupGroups(p(), false, countGroups, status);
	  }
	  break;
	case TAG_MTABLE:
	case TAG_MTR:
	case TAG_MLABELEDTR:
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

  value = GetAttributeValue(ATTR_ROWSPAN, env);
  rowSpan = ToInteger(value);
  if (rowSpan <= 0)
    {
      Globals::logger(LOG_WARNING, "<mtd> has rowspan <= 0, defaulted to 1");
      rowSpan = 1;
    }

  value = GetAttributeValue(ATTR_COLUMNSPAN, env);
  columnSpan = ToInteger(value);
  if (columnSpan <= 0)
    {
      Globals::logger(LOG_WARNING, "<mtd> has columnspan <= 0, defaulted to 1");
      columnSpan = 1;
    }
}

void
MathMLTableCellElement::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      // if the <mtd> element is not used inside a table, 
      // the cell field is null
      if (cell)
	{
	  if (SmartPtr<Value> value = GetAttributeValue(ATTR_ROWALIGN, false))
	    cell->rowAlign = ToRowAlignId(value);

	  if (SmartPtr<Value> value = GetAttributeValue(ATTR_COLUMNALIGN, false))
	    cell->columnAlign = ToColumnAlignId(value);

	  if (SmartPtr<Value> value = GetAttributeValue(ATTR_GROUPALIGN, false))
	    for (unsigned k = 0; k < cell->nAlignGroup; k++)
	      {
		SmartPtr<Value> p = GetComponent(value, k);
		cell->aGroup[k].alignment = ToGroupAlignId(p);
	      }
	}
      
      MathMLNormalizingContainerElement::Setup(env);
      ResetDirtyAttribute();
    }
}

void
MathMLTableCellElement::SetupCellPosition(unsigned i, unsigned j, unsigned nRows)
{
  rowIndex = i;
  columnIndex = j;

  if (i + rowSpan > nRows)
    {
      Globals::logger(LOG_WARNING, "`mtd' spans over the last row (truncated)");
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

  if (ctxt.GetLayoutType() == LAYOUT_MIN) minWidth = box.width;

  ResetDirtyLayout(ctxt);
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
	  MarkAlignType alignType = MARK_ALIGN_NOTVALID;

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
	  assert(alignType != MARK_ALIGN_NOTVALID);
      
	  if (alignType == MARK_ALIGN_LEFT)
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
      else if (cell->aGroup[k].alignment == GROUP_ALIGN_DECIMALPOINT)
	{
	  SmartPtr<MathMLTokenElement> token = group->GetDecimalPoint();
	  if (token) alignPoint = token->GetDecimalPointEdge();
	  else alignPoint = cell->aGroup[k].rightEdge;
	} 
      else
	{
	  switch (cell->aGroup[k].alignment)
	    {
	    case GROUP_ALIGN_LEFT:
	      alignPoint = cell->aGroup[k].leftEdge;
	      break;
	    case GROUP_ALIGN_RIGHT:
	      alignPoint = cell->aGroup[k].rightEdge;
	      break;
	    case GROUP_ALIGN_CENTER:
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

#if 0
  cout << "group " << k;
  cout << " right: " << sp2ipx(rightEdge);
  cout << " left: " << sp2ipx(leftEdge);
  cout << " align: " << sp2ipx(alignPoint);
  cout << endl;
#endif

#if 0
  for (unsigned k = 0; k < cell->nAlignGroup; k++) {
    cout << " [" << sp2ipx(cell->aGroup[k].extent.left) << ',' << sp2ipx(cell->aGroup[k].extent.right) << ']';
  }
  cout << endl;
#endif
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
	  case COLUMN_ALIGN_RIGHT:
	    cellXOffset = availableWidth - elemBox.width;
	    break;
	  case COLUMN_ALIGN_CENTER:
	    cellXOffset = (availableWidth - elemBox.width) / 2;
	    break;
	  case COLUMN_ALIGN_LEFT:
	  default:
	    cellXOffset = 0;
	    break;
	  }

	  scaled cellYOffset = 0;
	  switch (cell->rowAlign) {
	  case ROW_ALIGN_BOTTOM:
	    cellYOffset = box.depth - elemBox.depth;
	    break;
	  case ROW_ALIGN_CENTER:
	    cellYOffset = (box.verticalExtent() - elemBox.verticalExtent()) / 2 +
	      elemBox.height - box.height;
	    break;
	  case ROW_ALIGN_BASELINE:
	    cellYOffset = 0;
	    break;
	  case ROW_ALIGN_AXIS:
	    assert(IMPOSSIBLE);
	    break;
	  case ROW_ALIGN_TOP:
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

bool
MathMLTableCellElement::IsStretchyOperator() const
{
  if (GetChild()) return isStretchyOperator(GetChild());
  else return false;
}

void
MathMLTableCellElement::SetDirtyStructure()
{
  assert(GetParent());
  assert(is_a<MathMLTableRowElement>(GetParent()));
  SmartPtr<MathMLTableRowElement> row = smart_cast<MathMLTableRowElement>(GetParent());
  assert(row);
  row->SetDirtyStructure();
  MathMLNormalizingContainerElement::SetDirtyStructure();
}

void
MathMLTableCellElement::SetDirtyAttribute()
{
  assert(GetParent());
  assert(is_a<MathMLTableRowElement>(GetParent()));
  SmartPtr<MathMLTableRowElement> row = smart_cast<MathMLTableRowElement>(GetParent());
  assert(row);
  row->SetDirtyAttribute();
  MathMLNormalizingContainerElement::SetDirtyAttribute();
}

void
MathMLTableCellElement::SetDirtyLayout()
{
  assert(GetParent());
  assert(is_a<MathMLTableRowElement>(GetParent()));
  SmartPtr<MathMLTableRowElement> row = smart_cast<MathMLTableRowElement>(GetParent());
  assert(row);
  row->SetDirtyLayout();
  MathMLNormalizingContainerElement::SetDirtyLayout();
}
