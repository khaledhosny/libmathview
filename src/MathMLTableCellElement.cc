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

#include "Iterator.hh"
#include "frameAux.hh"
#include "MathEngine.hh"
#include "operatorAux.hh"
#include "StringUnicode.hh"
#include "MathMLMarkNode.hh"
#include "ValueConversion.hh"
#include "MathMLTokenElement.hh"
#include "MathMLActionElement.hh"
#include "MathMLAlignMarkElement.hh"
#include "MathMLTableCellElement.hh"

MathMLTableCellElement::MathMLTableCellElement(mDOMNodeRef node): 
  MathMLNormalizingContainerElement(node, TAG_MTD)
{
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
MathMLTableCellElement::SetupGroups(MathMLElement* elem,
				    bool allowedGroup,
				    bool countGroups,
				    TableCell& status)
{
  assert(elem != NULL);

  // if countGroups == TRUE  requires: normalization, alignment scopes
  // if countGroups == FALSE requires: normalization, alignment scopes, group alignment 

  if (elem->IsToken()) {
    MathMLTokenElement* token = TO_TOKEN(elem);
    assert(token != NULL);

    // remember that the first mn element, if present, always contains
    // the alignment for decimal point
    if (status.group != NULL && token->IsA() == TAG_MN &&
	status.group->GetDecimalPoint() == NULL)
      status.group->SetDecimalPoint(token);

    for (Iterator<MathMLTextNode*> p(token->GetContent()); p.More(); p.Next()) {
      if (p()->IsMark() && status.group != NULL) {
	MathMLMarkNode* mark = TO_MARK(p());
	assert(mark != NULL);
	status.group->SetAlignmentMark(mark);
	// only the first alignment mark (if present) in a token is ever taken into account
	break;
      }
    }
  } else if (elem->IsContainer()) {
    MathMLContainerElement* container = TO_CONTAINER(elem);
    assert(container != NULL);

    Iterator<MathMLElement*> p(container->content);

    switch (container->IsA()) {
    case TAG_MPADDED:
    case TAG_MSQRT:
      if (status.group != NULL) {
	while (p.More()) {
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
	MathMLActionElement* action = TO_ACTION(container);
	assert(action != NULL);
	if (action->GetSelectedElement() != NULL)
	  SetupGroups(action->GetSelectedElement(), allowedGroup, countGroups, status);
      }
      break;
    case TAG_MTD:
      {
	MathMLTableCellElement* mtd = TO_TABLECELL(container);
	assert(mtd != NULL);

	if (status.group != NULL && !mtd->GetAlignmentScope())
	  SetupGroups(p(), false, countGroups, status);
      }
      break;
    case TAG_MTABLE:
    case TAG_MTR:
    case TAG_MLABELEDTR:
    default:
      while (p.More()) {
	SetupGroups(p(), allowedGroup, countGroups, status);
	p.Next();
      }
      break;
    }
  } else {
    switch (elem->IsA()) {
    case TAG_MALIGNGROUP:
      if (allowedGroup) {
	if (countGroups) {
	  status.nAlignGroup++;
	} else {
	  MathMLAlignGroupElement* group = dynamic_cast<MathMLAlignGroupElement*>(elem);
	  assert(group != NULL);

	  status.aGroup[status.iGroup].group = group;
	  status.group = group;
	  status.iGroup++;
	}
      }
      break;
    case TAG_MALIGNMARK:
      if (status.group != NULL) {
	MathMLAlignMarkElement* align = TO_ALIGN_MARK(elem);
	assert(align != NULL);
	status.group->SetAlignmentMark(align);
      }
      break;
    case TAG_MSPACE:
    default:
      break;
    }
  }
}

void
MathMLTableCellElement::SetupCellSpanning(RenderingEnvironment* env)
{
  const Value* value;

  value = GetAttributeValue(ATTR_ROWSPAN, env);
  rowSpan = value->ToInteger();
  delete value;

  value = GetAttributeValue(ATTR_COLUMNSPAN, env);
  columnSpan = value->ToInteger();
  delete value;
}

void
MathMLTableCellElement::Setup(RenderingEnvironment* env)
{
  assert(cell != NULL);

  const Value* value;

  value = GetAttributeValue(ATTR_ROWALIGN, NULL, false);
  if (value != NULL) cell->rowAlign = ToRowAlignId(value);
  delete value;

  value = GetAttributeValue(ATTR_COLUMNALIGN, NULL, false);
  if (value != NULL) cell->columnAlign = ToColumnAlignId(value);
  delete value;

  value = GetAttributeValue(ATTR_GROUPALIGN, NULL, false);
  if (value != NULL) {
    for (unsigned k = 0; k < cell->nAlignGroup; k++) {
      const Value* p = value->Get(k);
      cell->aGroup[k].alignment = ToGroupAlignId(p);
    }
  }
  delete value;

  MathMLNormalizingContainerElement::Setup(env);
}

void
MathMLTableCellElement::SetupCellPosition(unsigned i, unsigned j, unsigned nRows)
{
  rowIndex = i;
  columnIndex = j;

  if (i + rowSpan > nRows) {
    MathEngine::logger(LOG_WARNING, "`mtd' spans over the last row (truncated)");
    rowSpan = nRows - i;
  }
}

void
MathMLTableCellElement::SetupCell(TableCell* p)
{
  assert(p != NULL);
  cell = p;
}

void
MathMLTableCellElement::CalcGroupsExtent()
{
  assert(cell != NULL);
  SetPosition(0, 0);

  unsigned k;
  for (k = 0; k < cell->nAlignGroup; k++) {
    MathMLAlignGroupElement* group = cell->aGroup[k].group;
    assert(group != NULL);

    MathMLFrame* leftSibling = getFrameLeftSibling(group);
    MathMLFrame* rightSibling = getFrameRightSibling(group);

    if (k == 0) cell->aGroup[k].leftEdge = GetLeftEdge();
    else if (rightSibling != NULL) cell->aGroup[k].leftEdge = rightSibling->GetLeftEdge();
    else cell->aGroup[k].leftEdge = group->GetX();

    if (k > 0) {
      if (leftSibling != NULL) cell->aGroup[k - 1].rightEdge = leftSibling->GetRightEdge();
      else cell->aGroup[k - 1].rightEdge = group->GetX();
    }

    if (k == cell->nAlignGroup - 1) cell->aGroup[k].rightEdge = GetRightEdge();
  }

  scaled cellWidth = GetBoundingBox().width;

  scaled prevAlignPoint = 0;

  for (k = 0; k < cell->nAlignGroup; k++) {
    MathMLAlignGroupElement* group = cell->aGroup[k].group;
    assert(group != NULL);

    scaled alignPoint = 0;

    if (group->GetAlignmentMarkElement() != NULL ||
	group->GetAlignmentMarkNode() != NULL) {
      MathMLFrame* mark = NULL;
      MarkAlignType alignType = MARK_ALIGN_NOTVALID;

      if (group->GetAlignmentMarkElement() != NULL) {
	mark = group->GetAlignmentMarkElement();
	alignType = group->GetAlignmentMarkElement()->GetAlignmentEdge();
      } else {
	mark = group->GetAlignmentMarkNode();
	alignType = group->GetAlignmentMarkNode()->GetAlignmentEdge();
      }
      assert(mark != NULL);
      assert(alignType != MARK_ALIGN_NOTVALID);
      
      if (alignType == MARK_ALIGN_LEFT) {
	MathMLFrame* frame = getFrameRightSibling(mark);
	if (frame != NULL) alignPoint = frame->GetLeftEdge();
	else alignPoint = alignPoint = mark->GetX();
      } else {
	MathMLFrame* frame = getFrameLeftSibling(mark);
	if (frame != NULL) alignPoint = frame->GetRightEdge();
	else alignPoint = mark->GetX();
      }
    } else if (cell->aGroup[k].alignment == GROUP_ALIGN_DECIMALPOINT) {
      MathMLTokenElement* token = group->GetDecimalPoint();
      if (token != NULL) alignPoint = token->GetDecimalPointEdge();
      else alignPoint = cell->aGroup[k].rightEdge;
    } else {
      switch (cell->aGroup[k].alignment) {
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
MathMLTableCellElement::SetPosition(scaled x, scaled y)
{
  assert(cell != NULL);

  MathMLElement* elem = GetContent();
  assert(elem != NULL);

  const BoundingBox& elemBox = elem->GetBoundingBox();

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
    cellYOffset = box.descent - elemBox.descent;
    break;
  case ROW_ALIGN_CENTER:
    cellYOffset = (box.GetHeight() - elemBox.GetHeight()) / 2 +
      elemBox.ascent - box.ascent;
    break;
  case ROW_ALIGN_BASELINE:
    cellYOffset = 0;
    break;
  case ROW_ALIGN_AXIS:
    assert(IMPOSSIBLE);
    break;
  case ROW_ALIGN_TOP:
    cellYOffset = elemBox.ascent - box.ascent;
    break;
  default:
    cellYOffset = 0;
    break;
  }

  if (cell->nAlignGroup == 0)
    elem->SetPosition(x + cellXOffset, y + cellYOffset, cell->columnAlign);
  else
    elem->SetPosition(x + cellXOffset, y + cellYOffset);
}

bool
MathMLTableCellElement::IsStretchyOperator() const
{
  assert(content.GetSize() == 1);
  assert(content.GetFirst() != NULL);

  return isStretchyOperator(content.GetFirst());
}
